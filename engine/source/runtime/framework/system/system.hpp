#pragma once

#include "runtime/framework/component/component.hpp"
#include "runtime/framework/ecs/ecs.hpp"

namespace engine {

	class ISystem {
	public:
		virtual ~ISystem() {}

		virtual void Start(ecs::registry&) = 0;
		virtual void Update(ecs::registry&) = 0;
	};

}