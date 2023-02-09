#include "camera.hpp"

namespace engine {

	Camera::Camera(float aspect) : aspect(aspect) {
		SetLens(0.25f * math::Pi<float>(), aspect, 0.1f, 1000.0f);
	}

	void Camera::SetLens(float fovY, float aspect, float nearZ, float farZ) {

	}

	void Camera::UpdataViewMatrix() {

	}

	void Camera::LookAt(math::Vector3f pos, math::Vector3f target, math::Vector3f worldUp) {

	}

	void Camera::Walk(float distance) {

	}

	void Camera::Strafe(float distance) {

	}

	void Camera::Pitch(float angle) {

	}

	void Camera::RotateY(float angle) {

	}

}