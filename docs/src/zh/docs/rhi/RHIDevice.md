---
title: RHIDevice
---

`RHIDevice`是对于物理设备的抽象，也是 Verse RHI 中大部分接口的基础，在确保你完成了对所有派生接口的操作之前，请不要释放`RHIDevice`。

`CreateDevice`函数原型：

```C++
RHIDevice* RHIInstance::CreateDevice(const RHIDeviceCreateInfo&) const;
```

`DestroyDevice`函数原型：

```C++
void DestroyDevice(RHIDevice*) const;
```

`RHIDeviceCreateInfo`结构体原型：

```C++
struct RHIDeviceCreateInfo {
    uint32_t physicalDeviceID;
    RHIDeviceFeature deviceFeature;
    uint32_t queueCreateInfoCount;
    RHIQueueCreateInfo* pQueueCreateInfo;
};
```

`RHIPhysicalDeviceInfo`结构体原型：

```C++
struct RHIPhysicalDeviceInfo {
    uint32_t deviceID;
    const char* deviceName;
    uint32_t apiVersion;
};
```