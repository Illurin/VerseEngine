<template><div><p><code v-pre>RHIInstance</code>是容纳了 Verse RHI 所有方法的抽象父类，并且派生了<code v-pre>D3D12Instance</code>和<code v-pre>VulkanInstance</code>类，要使用 Direct3D 12 接口就使用具体的<code v-pre>D3D12Instance</code>类，要使用 Vulkan 接口就使用具体的<code v-pre>VulkanInstance</code>类。</p>
<p>使用<code v-pre>Init</code>函数来初始化<code v-pre>RHIInstance</code>，这要求一个<code v-pre>RHIInstanceInitInfo</code>结构体，它的原型如下：</p>
<div class="language-C++ line-numbers-mode" data-ext="C++"><pre v-pre class="language-C++"><code>struct RHIInstanceInitInfo {
    const char* applicationName;
    uint32_t applicationVersion;
};
</code></pre><div class="line-numbers" aria-hidden="true"><div class="line-number"></div><div class="line-number"></div><div class="line-number"></div><div class="line-number"></div></div></div><p>使用<code v-pre>Destroy</code>函数来释放<code v-pre>RHIInstance</code>资源。</p>
<p>在 Debug 模式下，<code v-pre>RHIInstance</code>会自动为程序创建调试图形API所需要的接口，无需去手动设置图形API的调试。</p>
<p><code v-pre>RHIInstance</code>提供的方法：</p>
<p>创建：</p>
<p><RouterLink to="/zh/docs/rhi/RHIDevice.html">CreateDevice</RouterLink></p>
<p><RouterLink to="/zh/docs/rhi/RHISwapchain.html">CreateSwapchain</RouterLink></p>
<p><RouterLink to="/zh/docs/rhi/RHICommandPool.html">CreateCommandPool</RouterLink></p>
<p>释放：</p>
<p><RouterLink to="/zh/docs/rhi/RHIDevice.html">DestroyDevice</RouterLink></p>
<p><RouterLink to="/zh/docs/rhi/RHISwapchain.html">DestroySwapchain</RouterLink></p>
<p><RouterLink to="/zh/docs/rhi/RHICommandPool.html">DestroyCommandPool</RouterLink></p>
<p>获取：</p>
<p><RouterLink to="/zh/docs/rhi/RHIQueue.html">GetQueue</RouterLink></p>
<p><RouterLink to="/zh/docs/rhi/RHICommandBuffer.html">AllocateCommandBuffers</RouterLink></p>
<p>命令：</p>
</div></template>


