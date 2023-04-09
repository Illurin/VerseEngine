#include "control_system.hpp"

#include "runtime/system/time.hpp"
#include "runtime/system/platform/input_system.hpp"

namespace engine {

	void ControlSystem::Start(ecs::registry& registry) {

	}

	void ControlSystem::Update(ecs::registry& registry) {
		auto view = registry.view<CameraComponent, const ControllerComponent>();

		view.each([&](CameraComponent& camera, const ControllerComponent& controller) {
			auto movement = controller.moveSpeed * static_cast<float>(Time::GetDeltaTime());

			if (input::GetKey(input::KeyCode::D)) { camera.camera.Strafe(movement);  camera.dirty = true; }
			if (input::GetKey(input::KeyCode::A)) { camera.camera.Strafe(-movement); camera.dirty = true; }
			if (input::GetKey(input::KeyCode::W)) { camera.camera.Walk(movement);    camera.dirty = true; }
			if (input::GetKey(input::KeyCode::S)) { camera.camera.Walk(-movement);   camera.dirty = true; }

			auto rotation = controller.rotateSpeed * static_cast<float>(Time::GetDeltaTime());

			if (input::GetKey(input::KeyCode::RightArrow)) { camera.camera.RotateY(-rotation);  camera.dirty = true; }
			if (input::GetKey(input::KeyCode::LeftArrow))  { camera.camera.RotateY(rotation); camera.dirty = true; }
			if (input::GetKey(input::KeyCode::UpArrow))    { camera.camera.Pitch(rotation);    camera.dirty = true; }
			if (input::GetKey(input::KeyCode::DownArrow))  { camera.camera.Pitch(-rotation);   camera.dirty = true; }
		});
	}

}