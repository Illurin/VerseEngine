---
title: RHIInstance
---

`RHIInstance`是容纳了 Verse RHI 所有方法的抽象父类，并且派生了`D3D12Instance`和`VulkanInstance`类，要使用 Direct3D 12 接口就使用具体的`D3D12Instance`类，要使用 Vulkan 接口就使用具体的`VulkanInstance`类。

使用`Init`函数来初始化`RHIInstance`，这要求一个`RHIInstanceInitInfo`结构体，它的原型如下：

```C++
struct RHIInstanceInitInfo {
    const char* applicationName;
    uint32_t applicationVersion;
};
```

使用`Destroy`函数来释放`RHIInstance`资源。

在 Debug 模式下，`RHIInstance`会自动为程序创建调试图形API所需要的接口，无需去手动设置图形API的调试。

`RHIInstance`提供的方法：

创建：

[CreateDevice](RHIDevice.md)

[CreateSwapchain](RHISwapchain.md)

[CreateCommandPool](RHICommandPool.md)

释放：

[DestroyDevice](RHIDevice.md)

[DestroySwapchain](RHISwapchain.md)

[DestroyCommandPool](RHICommandPool.md)

获取：

[GetQueue](RHIQueue.md)

[AllocateCommandBuffers](RHICommandBuffer.md)

命令：

