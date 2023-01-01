---
title: RHI API 手册
---

Verse RHI 是专为 Verse Engine 设计的图形硬件接口，目的是提供高性能且灵活的与 GPU 底层交互的方式，其满足现代图形API的贴合底层，充分利用硬件性能的特点。

由于 Verse RHI 仅考虑现代图形API特点，所以只接入了 Vulkan 和 Direct3D 12 ，其中仅 Vulkan 支持跨平台调用，我们也更多地推荐使用 Vulkan 完成绘制任务。

Verse RHI 的设计理念是简洁高效，采用类似 Vulkan 的创建接口->使用接口->释放接口的模式，所有接口的命名方式为`RHI*`，所有用于创建接口的结构体的命名方式为`RHI*Info`，并且支持利用自身引用来简化书写。

从 [RHIInstance](RHIInstance.md) 开始了解有关 Verse RHI 的使用。