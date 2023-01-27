#include "vulkan_util.hpp"

bool VulkanUtil::MemoryTypeFromProperties(
	vk::PhysicalDeviceMemoryProperties memProp,
	uint32_t typeBits, vk::MemoryPropertyFlags requirementMask,
	uint32_t& typeIndex) {
	for (uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; i++) {
		if ((typeBits & (1 << i)) && (memProp.memoryTypes[i].propertyFlags & requirementMask) == requirementMask) {
			typeIndex = i;
			return true;
		}
	}
	return false;
}

void VulkanUtil::CreateBuffer(
	vk::PhysicalDevice physicalDevice,
	vk::Device device,
	vk::DeviceSize size,
	vk::BufferUsageFlags usage,
	vk::Buffer& buffer,
	vk::DeviceMemory& memory) {
	auto bufferInfo = vk::BufferCreateInfo()
		.setSize(size)
		.setUsage(usage);

	buffer = device.createBuffer(bufferInfo);

	if (!buffer) {
		throw std::runtime_error("Create buffer failed");
	}

	auto memReqs = device.getBufferMemoryRequirements(buffer);
	auto memoryInfo = vk::MemoryAllocateInfo().setAllocationSize(memReqs.size);
	MemoryTypeFromProperties(physicalDevice.getMemoryProperties(), memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal, memoryInfo.memoryTypeIndex);

	memory = device.allocateMemory(memoryInfo);

	if (!memory) {
		throw std::runtime_error("Allocate memory failed");
	}

	device.bindBufferMemory(buffer, memory, 0);
}

void VulkanUtil::CreateImage(
	vk::PhysicalDevice physicalDevice,
	vk::Device device,
	uint32_t width,
	uint32_t height,
	uint32_t mipLevels,
	vk::Format format,
	vk::ImageUsageFlags usage,
	vk::Image& image,
	vk::DeviceMemory& memory) {
	auto imageInfo = vk::ImageCreateInfo()
		.setArrayLayers(1)
		.setExtent(vk::Extent3D(width, height, 1))
		.setFormat(format)
		.setImageType(vk::ImageType::e2D)
		.setInitialLayout(vk::ImageLayout::eUndefined)
		.setMipLevels(mipLevels)
		.setSamples(vk::SampleCountFlagBits::e1)
		.setUsage(usage)
		.setTiling(vk::ImageTiling::eOptimal);

	image = device.createImage(imageInfo);

	if (!image) {
		throw std::runtime_error("Create image failed");
	}

	auto memReqs = device.getImageMemoryRequirements(image);
	auto memoryInfo = vk::MemoryAllocateInfo().setAllocationSize(memReqs.size);
	MemoryTypeFromProperties(physicalDevice.getMemoryProperties(), memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal, memoryInfo.memoryTypeIndex);

	memory = device.allocateMemory(memoryInfo);

	if (!memory) {
		throw std::runtime_error("Allocate memory failed");
	}

	device.bindImageMemory(image, memory, 0);
}

vk::ShaderModule VulkanUtil::CreateShaderModule(vk::Device device, const char* path) {
	std::ifstream loadFile(path, std::ios::ate | std::ios::binary);

	if (!loadFile.is_open()) {
		throw std::runtime_error("Cannot open the shader file");
	}

	size_t fileSize = (size_t)loadFile.tellg();

	std::vector<char> buffer(fileSize);
	loadFile.seekg(0);
	loadFile.read(buffer.data(), fileSize);
	loadFile.close();

	auto info = vk::ShaderModuleCreateInfo()
		.setCodeSize(buffer.size())
		.setPCode(reinterpret_cast<const uint32_t*>(buffer.data()));

	auto shaderModule = device.createShaderModule(info);

	if (!shaderModule) {
		throw std::runtime_error("Create shader module failed");
	}

	return shaderModule;
}

vk::CommandBuffer VulkanUtil::BeginSingleTimeCommand(vk::Device device, vk::CommandPool cmdPool) {
	auto cmdAllocInfo = vk::CommandBufferAllocateInfo()
		.setCommandBufferCount(1)
		.setCommandPool(cmdPool)
		.setLevel(vk::CommandBufferLevel::ePrimary);

	vk::CommandBuffer cmd;
	device.allocateCommandBuffers(&cmdAllocInfo, &cmd);

	if (!cmd) {
		throw std::runtime_error("Begin single time command failed");
	}

	auto beginInfo = vk::CommandBufferBeginInfo()
		.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

	cmd.begin(&beginInfo);

	return cmd;
}

void VulkanUtil::EndSingleTimeCommand(vk::Device device, vk::CommandPool cmdPool, vk::CommandBuffer cmd, vk::Queue queue) {
	cmd.end();

	auto submitInfo = vk::SubmitInfo()
		.setCommandBufferCount(1)
		.setPCommandBuffers(&cmd);

	queue.submit(1, &submitInfo, vk::Fence());
	queue.waitIdle();

	device.freeCommandBuffers(cmdPool, 1, &cmd);
}

bool VulkanUtil::CheckValidationLayerSupport(const std::vector<const char*>& validationLayers) {
	uint32_t layerCount = 0;
	vk::enumerateInstanceLayerProperties(&layerCount, static_cast<vk::LayerProperties*>(nullptr));

	if (layerCount == 0)
		return false;

	std::vector<vk::LayerProperties> availableLayers(layerCount);
	vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}
	return true;
}