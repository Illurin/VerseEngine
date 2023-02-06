#pragma once

#include "rhi_enum.hpp"

#include "runtime/system/file_system/path.hpp"
#include "runtime/system/platform/platform_info.hpp"

#define RHI_DEFINE_HANDLE(object) class object##_T; typedef object##_T* object;

namespace engine {
	namespace rhi {

		/* -------------------- handle -------------------- */

		RHI_DEFINE_HANDLE(Instance)
		RHI_DEFINE_HANDLE(Device)
		RHI_DEFINE_HANDLE(Queue)
		RHI_DEFINE_HANDLE(Swapchain)
		RHI_DEFINE_HANDLE(CommandPool)
		RHI_DEFINE_HANDLE(CommandBuffer)
		RHI_DEFINE_HANDLE(Buffer)
		RHI_DEFINE_HANDLE(Image)
		RHI_DEFINE_HANDLE(Sampler)
		RHI_DEFINE_HANDLE(DescriptorPool)
		RHI_DEFINE_HANDLE(DescriptorSet)
		RHI_DEFINE_HANDLE(DescriptorSetLayout)
		RHI_DEFINE_HANDLE(PipelineLayout)
		RHI_DEFINE_HANDLE(RenderPass)
		RHI_DEFINE_HANDLE(ShaderModule)
		RHI_DEFINE_HANDLE(Pipeline)
		RHI_DEFINE_HANDLE(Framebuffer)
		RHI_DEFINE_HANDLE(Fence)

		/* -------------------- info --------------------*/

		struct Extent2D {
			Extent2D& SetWidth(uint32_t width) { this->width = width; return *this; }
			Extent2D& SetHeight(uint32_t height) { this->height = height; return *this; }

			uint32_t width;
			uint32_t height;
		};

		struct Extent3D {
			Extent3D& SetWidth(uint32_t width) { this->width = width; return *this; }
			Extent3D& SetHeight(uint32_t height) { this->height = height; return *this; }
			Extent3D& SetDepth(uint32_t depth) { this->depth = depth; return *this; }

			uint32_t width;
			uint32_t height;
			uint32_t depth;
		};

		struct Offset2D {
			Offset2D& SetX(int32_t x) { this->x = x; return *this; }
			Offset2D& SetY(int32_t y) { this->y = y; return *this; }

			int32_t x;
			int32_t y;
		};

		struct Offset3D {
			Offset3D& SetX(int32_t x) { this->x = x; return *this; }
			Offset3D& SetY(int32_t y) { this->y = y; return *this; }
			Offset3D& SetZ(int32_t z) { this->z = z; return *this; }

			int32_t x;
			int32_t y;
			int32_t z;
		};

		struct Rect2D {
			Rect2D& SetOffset(Offset2D offset) { this->offset = offset; return *this; }
			Rect2D& SetExtent(Extent2D extent) { this->extent = extent; return *this; }

			Offset2D offset;
			Extent2D extent;
		};

		struct Rect3D {
			Rect3D& SetOffset(Offset3D offset) { this->offset = offset; return *this; }
			Rect3D& SetExtent(Extent3D extent) { this->extent = extent; return *this; }

			Offset3D offset;
			Extent3D extent;
		};

		struct Viewport {
			Viewport& SetX(float x) { this->x = x; return *this; }
			Viewport& SetY(float y) { this->y = y; return *this; }
			Viewport& SetWidth(float width) { this->width = width; return *this; }
			Viewport& SetHeight(float height) { this->height = height; return *this; }
			Viewport& SetMinDepth(float minDepth) { this->minDepth = minDepth; return *this; }
			Viewport& SetMaxDepth(float maxDepth) { this->maxDepth = maxDepth; return *this; }
			
			float x;
			float y;
			float width;
			float height;
			float minDepth;
			float maxDepth;
		};

		struct ClearColorValue {
			ClearColorValue& SetColorValue(std::array<float, 4> colorValue) { this->colorValue = colorValue; return *this; }

			std::array<float, 4> colorValue;
		};

		struct ClearDepthStencilValue {
			ClearDepthStencilValue& SetDepthValue(float depthValue) { this->depthValue = depthValue; return *this; }
			ClearDepthStencilValue& SetStencilValue(uint32_t stencilValue) { this->stencilValue = stencilValue; return *this; }

			float depthValue;
			uint32_t stencilValue;
		};

		struct ComponentMapping {
			ComponentMapping& SetR(ComponentSwizzle r) { this->r = r; return *this; }
			ComponentMapping& SetG(ComponentSwizzle g) { this->g = g; return *this; }
			ComponentMapping& SetB(ComponentSwizzle b) { this->b = b; return *this; }
			ComponentMapping& SetA(ComponentSwizzle a) { this->a = a; return *this; }

			ComponentSwizzle r;
			ComponentSwizzle g;
			ComponentSwizzle b;
			ComponentSwizzle a;
		};

		struct InstanceCreateInfo {
			InstanceCreateInfo& SetPApiName(const char* pApiName) { this->pApiName = pApiName; return *this; }
			InstanceCreateInfo& SetPApplicationName(const char* pApplicationName) { this->pApplicationName = pApplicationName; return *this; }
			InstanceCreateInfo& SetApplicationVersion(uint32_t applicationVersion) { this->applicationVersion = applicationVersion; return *this; }

			const char* pApiName;
			const char* pApplicationName;
			uint32_t applicationVersion;
		};

		struct PhysicalDeviceInfo {
			PhysicalDeviceInfo& SetId(uint32_t id) { this->id = id; return *this; }
			PhysicalDeviceInfo& SetVendorId(uint32_t vendorID) { this->vendorId = vendorID; return *this; }
			PhysicalDeviceInfo& SetDeviceId(uint32_t deviceID) { this->deviceId = deviceID; return *this; }
			PhysicalDeviceInfo& SetDeviceDescription(std::string deviceDescription) { this->deviceDescription = deviceDescription; return *this; }

			uint32_t id;
			uint32_t vendorId;
			uint32_t deviceId;
			std::string deviceDescription;
		};

		struct QueueCreateInfo {
			QueueCreateInfo& SetQueueType(QueueType queueType) { this->queueType = queueType; return *this; }

			QueueType queueType;
		};

		struct DeviceCreateInfo {
			DeviceCreateInfo& SetPhysicalDeviceId(uint32_t physicalDeviceId) { this->physicalDeviceId = physicalDeviceId; return *this; }
			DeviceCreateInfo& SetQueueCreateInfoCount(uint32_t queueCreateInfoCount) { this->queueCreateInfoCount = queueCreateInfoCount; return *this; }
			DeviceCreateInfo& SetPQueueCreateInfos(QueueCreateInfo* pQueueCreateInfos) { this->pQueueCreateInfos = pQueueCreateInfos; return *this; }
			DeviceCreateInfo& SetQueueCreateInfos(std::vector<QueueCreateInfo>& queueCreateInfos) { queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()); pQueueCreateInfos = queueCreateInfos.data(); return *this; }

			uint32_t physicalDeviceId;
			uint32_t queueCreateInfoCount;
			QueueCreateInfo* pQueueCreateInfos;
		};

		struct SwapchainCreateInfo {
			SwapchainCreateInfo& SetFormat(Format format) { this->format = format; return *this; }
			SwapchainCreateInfo& SetFrameCount(uint32_t frameCount) { this->frameCount = frameCount; return *this; }
			SwapchainCreateInfo& SetImageExtent(Extent2D imageExtent) { this->imageExtent = imageExtent; return *this; }
			SwapchainCreateInfo& SetAlphaMode(AlphaMode alphaMode) { this->alphaMode = alphaMode; return *this; }
			SwapchainCreateInfo& SetImageUsage(ImageUsage imageUsage) { this->imageUsage = imageUsage; return *this; }
			SwapchainCreateInfo& SetPlatformInfo(PlatformInfo platformInfo) { this->platformInfo = platformInfo; return *this; }
			SwapchainCreateInfo& SetQueue(Queue queue) { this->queue = queue; return *this; }

			Format format;
			uint32_t frameCount;
			Extent2D imageExtent;
			AlphaMode alphaMode;
			ImageUsage imageUsage;
			PlatformInfo platformInfo;
			Queue queue;
		};

		struct CommandPoolCreateInfo {
			CommandPoolCreateInfo& SetQueue(Queue queue) { this->queue = queue; return *this; }

			Queue queue;
		};

		struct BufferCreateInfo {
			BufferCreateInfo& SetSize(uint64_t size) { this->size = size; return *this; }
			BufferCreateInfo& SetUsage(BufferUsage usage) { this->usage = usage; return *this; }

			uint64_t size;
			BufferUsage usage;
		};

		struct ImageCreateInfo {
			ImageCreateInfo& SetFormat(Format format) { this->format = format; return *this; }
			ImageCreateInfo& SetImageType(ImageType imageType) { this->imageType = imageType; return *this; }
			ImageCreateInfo& SetExtent(Extent3D extent) { this->extent = extent; return *this; }
			ImageCreateInfo& SetUsage(ImageUsage usage) { this->usage = usage; return *this; }
			ImageCreateInfo& SetInitialLayout(ImageLayout initialLayout) { this->initialLayout = initialLayout; return *this; }
			ImageCreateInfo& SetMipLevels(uint32_t mipLevels) { this->mipLevels = mipLevels; return *this; }
			ImageCreateInfo& SetArrayLayers(uint32_t arrayLayers) { this->arrayLayers = arrayLayers; return *this; }
			ImageCreateInfo& SetSampleCount(SampleCount sampleCount) { this->sampleCount = sampleCount; return *this; }

			Format format;
			ImageType imageType;
			Extent3D extent;
			ImageUsage usage;
			ImageLayout initialLayout;
			uint32_t mipLevels;
			uint32_t arrayLayers;
			SampleCount sampleCount;
		};

		struct BufferViewInfo {
			BufferViewInfo& SetBuffer(Buffer buffer) { this->buffer = buffer; return *this; }
			BufferViewInfo& SetFormat(Format format) { this->format = format; return *this; }
			BufferViewInfo& SetRange(uint64_t range) { this->range = range; return *this; }
			BufferViewInfo& SetOffset(uint64_t offset) { this->offset = offset; return *this; }

			Buffer buffer;
			Format format;
			uint64_t range;
			uint64_t offset;
		};

		struct ImageViewInfo {
			ImageViewInfo& SetImage(Image image) { this->image = image; return *this; }
			ImageViewInfo& SetViewType(ImageViewType viewType) { this->viewType = viewType; return *this; }
			ImageViewInfo& SetFormat(Format format) { this->format = format; return *this; }
			ImageViewInfo& SetComponentMapping(ComponentMapping componentMapping) { this->componentMapping = componentMapping; return *this; }
			ImageViewInfo& SetBaseMipLevel(uint32_t baseMipLevel) { this->baseMipLevel = baseMipLevel; return *this; }
			ImageViewInfo& SetMipLevelCount(uint32_t mipLevelCount) { this->mipLevelCount = mipLevelCount; return *this; }
			ImageViewInfo& SetBaseArrayLayer(uint32_t baseArrayLayer) { this->baseArrayLayer = baseArrayLayer; return *this; }
			ImageViewInfo& SetArrayLayerCount(uint32_t arrayLayerCount) { this->arrayLayerCount = arrayLayerCount; return *this; }

			Image image;
			ImageViewType viewType;
			Format format;
			ComponentMapping componentMapping;
			uint32_t baseMipLevel;
			uint32_t mipLevelCount;
			uint32_t baseArrayLayer;
			uint32_t arrayLayerCount;
		};

		struct SamplerCreateInfo {
			SamplerCreateInfo& SetMinFilter(Filter minFilter) { this->minFilter = minFilter; return *this; }
			SamplerCreateInfo& SetMagFilter(Filter magFilter) { this->magFilter = magFilter; return *this; }
			SamplerCreateInfo& SetMipFilter(Filter mipFilter) { this->mipFilter = mipFilter; return *this; }
			SamplerCreateInfo& SetAddressModeU(SamplerAddressMode addressModeU) { this->addressModeU = addressModeU; return *this; }
			SamplerCreateInfo& SetAddressModeV(SamplerAddressMode addressModeV) { this->addressModeV = addressModeV; return *this; }
			SamplerCreateInfo& SetAddressModeW(SamplerAddressMode addressModeW) { this->addressModeW = addressModeW; return *this; }
			SamplerCreateInfo& SetMipLodBias(float mipLodBias) { this->mipLodBias = mipLodBias; return *this; }
			SamplerCreateInfo& SetMinLod(float minLod) { this->minLod = minLod; return *this; }
			SamplerCreateInfo& SetMaxLod(float maxLod) { this->maxLod = maxLod; return *this; }
			SamplerCreateInfo& SetAnisotropyEnable(bool anisotropyEnable) { this->anisotropyEnable = anisotropyEnable; return *this; }
			SamplerCreateInfo& SetMaxAnisotropy(float maxAnisotropy) { this->maxAnisotropy = maxAnisotropy; return *this; }
			SamplerCreateInfo& SetCompareEnable(bool compareEnable) { this->compareEnable = compareEnable; return *this; }
			SamplerCreateInfo& SetCompareOp(CompareOp compareOp) { this->compareOp = compareOp; return *this; }
			SamplerCreateInfo& SetBorderColor(BorderColor borderColor) { this->borderColor = borderColor; return *this; }

			Filter minFilter;
			Filter magFilter;
			Filter mipFilter;
			SamplerAddressMode addressModeU;
			SamplerAddressMode addressModeV;
			SamplerAddressMode addressModeW;
			float mipLodBias;
			float minLod;
			float maxLod;
			bool anisotropyEnable;
			float maxAnisotropy;
			bool compareEnable;
			CompareOp compareOp;
			BorderColor borderColor;
		};

		struct DescriptorPoolSize {
			DescriptorPoolSize& SetType(DescriptorType type) { this->type = type; return *this; }
			DescriptorPoolSize& SetSize(uint32_t size) { this->size = size; return *this; }

			DescriptorType type;
			uint32_t size;
		};

		struct DescriptorPoolCreateInfo {
			DescriptorPoolCreateInfo& SetMaxSetCount(uint32_t maxSetCount) { this->maxSetCount = maxSetCount; return *this; }
			DescriptorPoolCreateInfo& SetPoolSizeCount(uint32_t poolSizeCount) { this->poolSizeCount = poolSizeCount; return *this; }
			DescriptorPoolCreateInfo& SetPPoolSizes(DescriptorPoolSize* pPoolSizes) { this->pPoolSizes = pPoolSizes; return *this; }
			DescriptorPoolCreateInfo& SetPoolSizes(std::vector<DescriptorPoolSize>& poolSizes) { poolSizeCount = static_cast<uint32_t>(poolSizes.size()); pPoolSizes = poolSizes.data(); return *this; }

			uint32_t maxSetCount;
			uint32_t poolSizeCount;
			DescriptorPoolSize* pPoolSizes;
		};

		struct DescriptorSetAllocateInfo {
			DescriptorSetAllocateInfo& SetDescriptorSetCount(uint32_t descriptorSetCount) { this->descriptorSetCount = descriptorSetCount; return *this; }
			DescriptorSetAllocateInfo& SetPDescriptorSetLayouts(DescriptorSetLayout* pDescriptorSetLayouts) { this->pDescriptorSetLayouts = pDescriptorSetLayouts; return *this; }
			DescriptorSetAllocateInfo& SetDescriptorSetLayouts(std::vector<DescriptorSetLayout>& descriptorSetLayouts) { descriptorSetCount = static_cast<uint32_t>(descriptorSetLayouts.size()); pDescriptorSetLayouts = descriptorSetLayouts.data(); return *this; }

			uint32_t descriptorSetCount;
			DescriptorSetLayout* pDescriptorSetLayouts;
		};

		struct DescriptorSetLayoutBinding {
			DescriptorSetLayoutBinding& SetBinding(uint32_t binding) { this->binding = binding; return *this; }
			DescriptorSetLayoutBinding& SetDescriptorType(DescriptorType descriptorType) { this->descriptorType = descriptorType; return *this; }
			DescriptorSetLayoutBinding& SetPStaticSampler(Sampler* pStaticSampler) { this->pStaticSampler = pStaticSampler; return *this; }
			DescriptorSetLayoutBinding& SetShaderStage(ShaderStage shaderStage) { this->shaderStage = shaderStage; return *this; }

			uint32_t binding;
			DescriptorType descriptorType;
			Sampler* pStaticSampler;
			ShaderStage shaderStage;
		};

		struct DescriptorSetLayoutCreateInfo {
			DescriptorSetLayoutCreateInfo& SetBindingCount(uint32_t bindingCount) { this->bindingCount = bindingCount; return *this; }
			DescriptorSetLayoutCreateInfo& SetPBindings(DescriptorSetLayoutBinding* pBindings) { this->pBindings = pBindings; return *this; }
			DescriptorSetLayoutCreateInfo& SetBindings(std::vector<DescriptorSetLayoutBinding>& bindings) { bindingCount = static_cast<uint32_t>(bindings.size()); pBindings = bindings.data(); return *this; }

			uint32_t bindingCount;
			DescriptorSetLayoutBinding* pBindings;
		};

		struct PipelineLayoutCreateInfo {
			PipelineLayoutCreateInfo& SetDescriptorSetLayoutCount(uint32_t descriptorSetLayoutCount) { this->descriptorSetLayoutCount = descriptorSetLayoutCount; return *this; }
			PipelineLayoutCreateInfo& SetPDescriptorSetLayouts(DescriptorSetLayout* pDescriptorSetLayouts) { this->pDescriptorSetLayouts = pDescriptorSetLayouts; return *this; }

			uint32_t descriptorSetLayoutCount;
			DescriptorSetLayout* pDescriptorSetLayouts;
		};

		struct AttachmentDescription {
			AttachmentDescription& SetFormat(Format format) { this->format = format; return *this; }
			AttachmentDescription& SetSampleCount(SampleCount sampleCount) { this->sampleCount = sampleCount; return *this; }
			AttachmentDescription& SetLoadOp(AttachmentLoadOp loadOp) { this->loadOp = loadOp; return *this; }
			AttachmentDescription& SetStoreOp(AttachmentStoreOp storeOp) { this->storeOp = storeOp; return *this; }
			AttachmentDescription& SetStencilLoadOp(AttachmentLoadOp stencilLoadOp) { this->stencilLoadOp = stencilLoadOp; return *this; }
			AttachmentDescription& SetStencilStoreOp(AttachmentStoreOp stencilStoreOp) { this->stencilStoreOp = stencilStoreOp; return *this; }
			AttachmentDescription& SetInitialLayout(ImageLayout initialLayout) { this->initialLayout = initialLayout; return *this; }
			AttachmentDescription& SetPassLayout(ImageLayout passLayout) { this->passLayout = passLayout; return *this; }
			AttachmentDescription& SetFinalLayout(ImageLayout finalLayout) { this->finalLayout = finalLayout; return *this; }

			Format format;
			SampleCount sampleCount;
			AttachmentLoadOp loadOp;
			AttachmentStoreOp storeOp;
			AttachmentLoadOp stencilLoadOp;
			AttachmentStoreOp stencilStoreOp;
			ImageLayout initialLayout;
			ImageLayout passLayout;
			ImageLayout finalLayout;
		};

		struct RenderPassCreateInfo {
			RenderPassCreateInfo& SetPipelineType(PipelineType pipelineType) { this->pipelineType = pipelineType; return *this; }
			RenderPassCreateInfo& SetColorAttachmentCount(uint32_t colorAttachmentCount) { this->colorAttachmentCount = colorAttachmentCount; return *this; }
			RenderPassCreateInfo& SetPColorAttachments(AttachmentDescription* pColorAttachments) { this->pColorAttachments = pColorAttachments; return *this; }
			RenderPassCreateInfo& SetColorAttachments(std::vector<AttachmentDescription>& colorAttachments) { colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size()); pColorAttachments = colorAttachments.data(); return *this; }
			RenderPassCreateInfo& SetPDepthStencilAttachment(AttachmentDescription* pDepthStencilAttachment) { this->pDepthStencilAttachment = pDepthStencilAttachment; return *this; }

			PipelineType pipelineType;
			uint32_t colorAttachmentCount;
			AttachmentDescription* pColorAttachments;
			AttachmentDescription* pDepthStencilAttachment;
		};

		struct ShaderModuleCreateInfo {
			ShaderModuleCreateInfo& SetSourceCodeSize(uint32_t sourceCodeSize) { this->sourceCodeSize = sourceCodeSize; return *this; }
			ShaderModuleCreateInfo& SetPSourceCode(uint8_t* pSourceCode) { this->pSourceCode = pSourceCode; return *this; }
			ShaderModuleCreateInfo& SetSourceCode(std::vector<uint8_t>& sourceCode) { sourceCodeSize = static_cast<uint32_t>(sourceCode.size()); pSourceCode = sourceCode.data(); return *this; }
			ShaderModuleCreateInfo& SetPEntryPoint(const char* pEntryPoint) { this->pEntryPoint = pEntryPoint; return *this; }
			ShaderModuleCreateInfo& SetShaderStage(ShaderStage shaderStage) { this->shaderStage = shaderStage; return *this; }

			uint32_t sourceCodeSize;
			uint8_t* pSourceCode;
			const char* pEntryPoint;
			ShaderStage shaderStage;
		};
		
		struct PipelineShaderStageInfo {
			PipelineShaderStageInfo& SetShaderModuleCount(uint32_t shaderModuleCount) { this->shaderModuleCount = shaderModuleCount; return *this; }
			PipelineShaderStageInfo& SetPShaderModules(ShaderModule* pShaderModules) { this->pShaderModules = pShaderModules; return *this; }
			PipelineShaderStageInfo& SetShaderModules(std::vector<ShaderModule>& shaderModules) { shaderModuleCount = static_cast<uint32_t>(shaderModules.size()); pShaderModules = shaderModules.data(); return *this; }

			uint32_t shaderModuleCount;
			ShaderModule* pShaderModules;
		};

		struct VertexBindingInfo {
			VertexBindingInfo& SetInputRate(rhi::VertexInputRate inputRate) { this->inputRate = inputRate; return *this; }
			VertexBindingInfo& SetStride(uint32_t stride) { this->stride = stride; return *this; }

			rhi::VertexInputRate inputRate;
			uint32_t stride;
		};

		struct VertexAttributeInfo {
			VertexAttributeInfo& SetBinding(uint32_t binding) { this->binding = binding; return *this; }
			VertexAttributeInfo& SetSemantic(const char* semantic) { this->semantic = semantic; return *this; }
			VertexAttributeInfo& SetFormat(rhi::Format format) { this->format = format; return *this; }
			VertexAttributeInfo& SetOffset(uint32_t offset) { this->offset = offset; return *this; }

			uint32_t binding;
			const char* semantic;
			rhi::Format format;
			uint32_t offset;
		};

		struct PipelineVertexInputInfo {
			PipelineVertexInputInfo& SetBindingCount(uint32_t bindingCount) { this->bindingCount = bindingCount; return *this; }
			PipelineVertexInputInfo& SetPBindings(VertexBindingInfo* pBindings) { this->pBindings = pBindings; return *this; }
			PipelineVertexInputInfo& SetBindings(std::vector<VertexBindingInfo>& bindings) { bindingCount = static_cast<uint32_t>(bindings.size()); pBindings = bindings.data(); return *this; }
			PipelineVertexInputInfo& SetAttributeCount(uint32_t attributeCount) { this->attributeCount = attributeCount; return *this; }
			PipelineVertexInputInfo& SetPAttributes(VertexAttributeInfo* pAttributes) { this->pAttributes = pAttributes; return *this; }
			PipelineVertexInputInfo& SetAttributes(std::vector<VertexAttributeInfo>& attributes) { attributeCount = static_cast<uint32_t>(attributes.size()); pAttributes = attributes.data(); return *this; }

			uint32_t bindingCount;
			VertexBindingInfo* pBindings;
			uint32_t attributeCount;
			VertexAttributeInfo* pAttributes;
		};

		struct PipelineInputAssemblyInfo {
			PipelineInputAssemblyInfo& SetTopology(PrimitiveTopology topology) { this->topology = topology; return *this; }
			PipelineInputAssemblyInfo& SetPrimitivieRestart(bool primitivieRestart) { this->primitivieRestart = primitivieRestart; return *this; }

			PrimitiveTopology topology;
			bool primitivieRestart;
		};

		struct PipelineTessellationInfo {
			PipelineTessellationInfo& SetPatchControlPoints(uint32_t patchControlPoints) { this->patchControlPoints = patchControlPoints; return *this; }

			uint32_t patchControlPoints;
		};

		struct PipelineRasterizationInfo {
			PipelineRasterizationInfo& SetCullMode(CullMode cullMode) { this->cullMode = cullMode; return *this; }
			PipelineRasterizationInfo& SetPolygonMode(PolygonMode polygonMode) { this->polygonMode = polygonMode; return *this; }
			PipelineRasterizationInfo& SetFrontFace(FrontFace frontFace) { this->frontFace = frontFace; return *this; }
			PipelineRasterizationInfo& SetSmoothLine(bool smoothLine) { this->smoothLine = smoothLine; return *this; }
			PipelineRasterizationInfo& SetDepthBias(float depthBias) { this->depthBias = depthBias; return *this; }
			PipelineRasterizationInfo& SetSlopeScaledDepthBias(float slopeScaledDepthBias) { this->slopeScaledDepthBias = slopeScaledDepthBias; return *this; }
			PipelineRasterizationInfo& SetDepthClamp(float depthClamp) { this->depthClamp = depthClamp; return *this; }

			CullMode cullMode;
			PolygonMode polygonMode;
			FrontFace frontFace;
			bool smoothLine;
			float depthBias;
			float slopeScaledDepthBias;
			float depthClamp;
		};

		struct PipelineViewportInfo {
			PipelineViewportInfo& SetScissorCount(uint32_t scissorCount) { this->scissorCount = scissorCount; return *this; }
			PipelineViewportInfo& SetPScissors(Rect2D* pScissors) { this->pScissors = pScissors; return *this; }
			PipelineViewportInfo& SetScissors(std::vector<Rect2D>& scissors) { scissorCount = static_cast<uint32_t>(scissors.size()); pScissors = scissors.data(); return *this; }
			PipelineViewportInfo& SetViewportCount(uint32_t viewportCount) { this->viewportCount = viewportCount; return *this; }
			PipelineViewportInfo& SetPViewports(Viewport* pViewports) { this->pViewports = pViewports; return *this; }
			PipelineViewportInfo& SetViewports(std::vector<Viewport>& viewports) { viewportCount = static_cast<uint32_t>(viewports.size()); pViewports = viewports.data(); return *this; }

			uint32_t scissorCount;
			Rect2D* pScissors;
			uint32_t viewportCount;
			Viewport* pViewports;
		};

		struct DepthStencilOpState {
			DepthStencilOpState& SetCompareOp(CompareOp compareOp) { this->compareOp = compareOp; }
			DepthStencilOpState& SetPassOp(StencilOp passOp) { this->passOp = passOp; }
			DepthStencilOpState& SetFailOp(StencilOp failOp) { this->failOp = failOp; }
			DepthStencilOpState& SetDepthFailOp(StencilOp depthFailOp) { this->depthFailOp = depthFailOp; }

			CompareOp compareOp;
			StencilOp passOp;
			StencilOp failOp;
			StencilOp depthFailOp;
		};

		struct PipelineDepthStencilInfo {
			PipelineDepthStencilInfo& SetDepthTestEnable(bool depthTestEnable) { this->depthTestEnable = depthTestEnable; return *this; }
			PipelineDepthStencilInfo& SetDepthWriteEnable(bool depthWriteEnable) { this->depthWriteEnable = depthWriteEnable; return *this; }
			PipelineDepthStencilInfo& SetDepthBoundsTestEnable(bool depthBoundsTestEnable) { this->depthBoundsTestEnable = depthBoundsTestEnable; return *this; }
			PipelineDepthStencilInfo& SetMinDepthBounds(float minDepthBounds) { this->minDepthBounds = minDepthBounds; return *this; }
			PipelineDepthStencilInfo& SetMaxDepthBounds(float maxDepthBounds) { this->maxDepthBounds = maxDepthBounds; return *this; }
			PipelineDepthStencilInfo& SetDepthCompareOp(CompareOp depthCompareOp) { this->depthCompareOp = depthCompareOp; return *this; }
			PipelineDepthStencilInfo& SetStencilTestEnable(bool stencilTestEnable) { this->stencilTestEnable = stencilTestEnable; return *this; }
			PipelineDepthStencilInfo& SetFrontOp(DepthStencilOpState frontOp) { this->frontOp = frontOp; return *this; }
			PipelineDepthStencilInfo& SetBackOp(DepthStencilOpState backOp) { this->backOp = backOp; return *this; }
			PipelineDepthStencilInfo& SetStencilCompareMask(uint8_t stencilCompareMask) { this->stencilCompareMask = stencilCompareMask; return *this; }
			PipelineDepthStencilInfo& SetStencilWriteMask(uint8_t stencilWriteMask) { this->stencilWriteMask = stencilWriteMask; return *this; }

			bool depthTestEnable;
			bool depthWriteEnable;
			bool depthBoundsTestEnable;
			float minDepthBounds;
			float maxDepthBounds;
			CompareOp depthCompareOp;
			bool stencilTestEnable;
			DepthStencilOpState frontOp;
			DepthStencilOpState backOp;
			uint8_t stencilCompareMask;
			uint8_t stencilWriteMask;
		};

		struct ColorBlendAttachmentInfo {
			ColorBlendAttachmentInfo& SetBlendEnable(bool blendEnable) { this->blendEnable = blendEnable; return *this; }
			ColorBlendAttachmentInfo& SetColorWriteMask(ColorComponent colorWriteMask) { this->colorWriteMask = colorWriteMask; return *this; }
			ColorBlendAttachmentInfo& SetColorBlendOp(BlendOp colorBlendOp) { this->colorBlendOp = colorBlendOp; return *this; }
			ColorBlendAttachmentInfo& SetSrcColorBlendFactor(BlendFactor srcColorBlendFactor) { this->srcColorBlendFactor = srcColorBlendFactor; return *this; }
			ColorBlendAttachmentInfo& SetDstColorBlendFactor(BlendFactor dstColorBlendFactor) { this->dstColorBlendFactor = dstColorBlendFactor; return *this; }
			ColorBlendAttachmentInfo& SetAlphaBlendOp(BlendOp alphaBlendOp) { this->alphaBlendOp = alphaBlendOp; return *this; }
			ColorBlendAttachmentInfo& SetSrcAlphaBlendFactor(BlendFactor srcAlphaBlendFactor) { this->srcAlphaBlendFactor = srcAlphaBlendFactor; return *this; }
			ColorBlendAttachmentInfo& SetDstAlphaBlendFactor(BlendFactor dstAlphaBlendFactor) { this->dstAlphaBlendFactor = dstAlphaBlendFactor; return *this; }

			bool blendEnable;
			ColorComponent colorWriteMask;
			BlendOp colorBlendOp;
			BlendFactor srcColorBlendFactor;
			BlendFactor dstColorBlendFactor;
			BlendOp alphaBlendOp;
			BlendFactor srcAlphaBlendFactor;
			BlendFactor dstAlphaBlendFactor;
		};

		struct PipelineColorBlendInfo {
			PipelineColorBlendInfo& SetAttachmentCount(uint32_t attachmentCount) { this->attachmentCount = attachmentCount; return *this; }
			PipelineColorBlendInfo& SetPAttachments(ColorBlendAttachmentInfo* pAttachments) { this->pAttachments = pAttachments; return *this; }
			PipelineColorBlendInfo& SetAttachments(std::vector<ColorBlendAttachmentInfo>& attachments) { attachmentCount = static_cast<uint32_t>(attachments.size()); pAttachments = attachments.data(); return *this; }

			uint32_t attachmentCount;
			ColorBlendAttachmentInfo* pAttachments;
		};

		struct PipelineMultisampleInfo {
			PipelineMultisampleInfo& SetAlphaToCoverageEnable(bool alphaToCoverageEnable) { this->alphaToCoverageEnable = alphaToCoverageEnable; return *this; }
			PipelineMultisampleInfo& SetSampleShadingEnable(bool sampleShadingEnable) { this->sampleShadingEnable = sampleShadingEnable; return *this; }
			PipelineMultisampleInfo& SetMinSampleShading(float minSampleShading) { this->minSampleShading = minSampleShading; return *this; }
			PipelineMultisampleInfo& SetRasterizationSamples(SampleCount rasterizationSamples) { this->rasterizationSamples = rasterizationSamples; return *this; }

			bool alphaToCoverageEnable;
			bool sampleShadingEnable;
			float minSampleShading;
			SampleCount rasterizationSamples;
		};

		struct PipelineLayoutInfo {

		};

		struct GraphicsPipelineCreateInfo {
			GraphicsPipelineCreateInfo& SetShaderStageInfo(PipelineShaderStageInfo shaderStageInfo) { this->shaderStageInfo = shaderStageInfo; return *this; }
			GraphicsPipelineCreateInfo& SetVertexInputInfo(PipelineVertexInputInfo vertexInputInfo) { this->vertexInputInfo = vertexInputInfo; return *this; }
			GraphicsPipelineCreateInfo& SetInputAssemblyInfo(PipelineInputAssemblyInfo inputAssemblyInfo) { this->inputAssemblyInfo = inputAssemblyInfo; return *this; }
			GraphicsPipelineCreateInfo& SetTessellationInfo(PipelineTessellationInfo tessellationInfo) { this->tessellationInfo = tessellationInfo; return *this; }
			GraphicsPipelineCreateInfo& SetRasterizationInfo(PipelineRasterizationInfo rasterizationInfo) { this->rasterizationInfo = rasterizationInfo; return *this; }
			GraphicsPipelineCreateInfo& SetViewportInfo(PipelineViewportInfo viewportInfo) { this->viewportInfo = viewportInfo; return *this; }
			GraphicsPipelineCreateInfo& SetDepthStencilInfo(PipelineDepthStencilInfo depthStencilInfo) { this->depthStencilInfo = depthStencilInfo; return *this; }
			GraphicsPipelineCreateInfo& SetColorBlendInfo(PipelineColorBlendInfo colorBlendInfo) { this->colorBlendInfo = colorBlendInfo; return *this; }
			GraphicsPipelineCreateInfo& SetMultisampleInfo(PipelineMultisampleInfo multisampleInfo) { this->multisampleInfo = multisampleInfo; return *this; }
			GraphicsPipelineCreateInfo& SetPipelineLayout(PipelineLayout pipelineLayout) { this->pipelineLayout = pipelineLayout; return *this; }
			GraphicsPipelineCreateInfo& SetRenderPass(RenderPass renderPass) { this->renderPass = renderPass; return *this; }

			PipelineShaderStageInfo shaderStageInfo;
			PipelineVertexInputInfo vertexInputInfo;
			PipelineInputAssemblyInfo inputAssemblyInfo;
			PipelineTessellationInfo tessellationInfo;
			PipelineRasterizationInfo rasterizationInfo;
			PipelineViewportInfo viewportInfo;
			PipelineDepthStencilInfo depthStencilInfo;
			PipelineColorBlendInfo colorBlendInfo;
			PipelineMultisampleInfo multisampleInfo;
			PipelineLayout pipelineLayout;
			RenderPass renderPass;
		};

		struct FramebufferCreateInfo {
			FramebufferCreateInfo& SetColorAttachmentCount(uint32_t colorAttachmentCount) { this->colorAttachmentCount = colorAttachmentCount; return *this; }
			FramebufferCreateInfo& SetPColorAttachments(ImageViewInfo* pColorAttachments) { this->pColorAttachments = pColorAttachments; return *this; }
			FramebufferCreateInfo& SetColorAttachments(std::vector<ImageViewInfo>& colorAttachments) { colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size()); pColorAttachments = colorAttachments.data(); return *this; }
			FramebufferCreateInfo& SetPDepthStencilAttachments(ImageViewInfo* pDepthStencilAttachment) { this->pDepthStencilAttachment = pDepthStencilAttachment; return *this; }
			FramebufferCreateInfo& SetWidth(uint32_t width) { this->width = width; return *this; }
			FramebufferCreateInfo& SetHeight(uint32_t height) { this->height = height; return *this; }
			FramebufferCreateInfo& SetLayers(uint32_t layers) { this->layers = layers; return *this; }
			FramebufferCreateInfo& SetRenderPass(RenderPass renderPass) { this->renderPass = renderPass; return *this; }
			 
			uint32_t colorAttachmentCount;
			ImageViewInfo* pColorAttachments;
			ImageViewInfo* pDepthStencilAttachment;
			uint32_t width;
			uint32_t height;
			uint32_t layers;
			RenderPass renderPass;
		};

		struct RenderPassBeginInfo {
			RenderPassBeginInfo& SetRenderPass(RenderPass renderPass) { this->renderPass = renderPass; return *this; }
			RenderPassBeginInfo& SetFramebuffer(Framebuffer framebuffer) { this->framebuffer = framebuffer; return *this; }
			RenderPassBeginInfo& SetPClearColorValues(ClearColorValue* pClearColorValues) { this->pClearColorValues = pClearColorValues; return *this; }
			RenderPassBeginInfo& SetClearDepthStencilValue(ClearDepthStencilValue clearDepthStencilValue) { this->clearDepthStencilValue = clearDepthStencilValue; return *this; }

			RenderPass renderPass;
			Framebuffer framebuffer;
			ClearColorValue* pClearColorValues;
			ClearDepthStencilValue clearDepthStencilValue;
		};

		struct FenceCreateInfo {};

		struct CommandBufferBeginInfo {};

	}
}