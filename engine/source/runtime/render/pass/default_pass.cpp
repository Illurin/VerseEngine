#include "default_pass.hpp"

namespace engine {

	DefaultPass::DefaultPass() {
		
	}

	DefaultPass::~DefaultPass() {
		renderPass->Destroy();
		pipeline->Destroy();
		framebuffer->Destroy();
	}

	void DefaultPass::PrepareRenderPass() {

	}

}