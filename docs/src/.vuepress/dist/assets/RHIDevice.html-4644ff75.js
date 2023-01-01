import{_ as e,V as i,W as n,$ as c}from"./framework-6ade477a.js";const d={},a=c(`<p><code>RHIDevice</code>是对于物理设备的抽象，也是 Verse RHI 中大部分接口的基础，在确保你完成了对所有派生接口的操作之前，请不要释放<code>RHIDevice</code>。</p><p><code>CreateDevice</code>函数原型：</p><div class="language-C++ line-numbers-mode" data-ext="C++"><pre class="language-C++"><code>RHIDevice* RHIInstance::CreateDevice(const RHIDeviceCreateInfo&amp;) const;
</code></pre><div class="line-numbers" aria-hidden="true"><div class="line-number"></div></div></div><p><code>DestroyDevice</code>函数原型：</p><div class="language-C++ line-numbers-mode" data-ext="C++"><pre class="language-C++"><code>void DestroyDevice(RHIDevice*) const;
</code></pre><div class="line-numbers" aria-hidden="true"><div class="line-number"></div></div></div><p><code>RHIDeviceCreateInfo</code>结构体原型：</p><div class="language-C++ line-numbers-mode" data-ext="C++"><pre class="language-C++"><code>struct RHIDeviceCreateInfo {
    uint32_t physicalDeviceID;
    RHIDeviceFeature deviceFeature;
    uint32_t queueCreateInfoCount;
    RHIQueueCreateInfo* pQueueCreateInfo;
};
</code></pre><div class="line-numbers" aria-hidden="true"><div class="line-number"></div><div class="line-number"></div><div class="line-number"></div><div class="line-number"></div><div class="line-number"></div><div class="line-number"></div></div></div><p><code>RHIPhysicalDeviceInfo</code>结构体原型：</p><div class="language-C++ line-numbers-mode" data-ext="C++"><pre class="language-C++"><code>struct RHIPhysicalDeviceInfo {
    uint32_t deviceID;
    const char* deviceName;
    uint32_t apiVersion;
};
</code></pre><div class="line-numbers" aria-hidden="true"><div class="line-number"></div><div class="line-number"></div><div class="line-number"></div><div class="line-number"></div><div class="line-number"></div></div></div>`,9),s=[a];function l(r,v){return i(),n("div",null,s)}const o=e(d,[["render",l],["__file","RHIDevice.html.vue"]]);export{o as default};
