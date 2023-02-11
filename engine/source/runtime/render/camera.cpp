#include "camera.hpp"

namespace engine {

	Camera::Camera(float aspect) : aspect(aspect) {
		SetLens(0.25f * math::Pi<float>(), aspect, 0.1f, 1000.0f);
	}

	void Camera::SetLens(float fovY, float aspect, float nearZ, float farZ) {
		this->fovY = fovY;
		this->aspect = aspect;
		this->nearZ = nearZ;
		this->farZ = farZ;

		projMatrix = math::Projection::Persp(fovY, aspect, nearZ, farZ);
	}

	void Camera::UpdataViewMatrix() {

	}

	void Camera::LookAt(math::Vector3f position, math::Vector3f target, math::Vector3f worldUp) {
		look = math::Vector3f(target.x() - position.x(), target.y() - position.y(), target.z() - position.z()).normalized();
		right = worldUp.cross(look);
		up = look.cross(right);
	}

	void Camera::Walk(float distance) {
		position.x() += look.x() * distance;
		position.y() += look.y() * distance;
		position.z() += look.z() * distance;
		viewDirty = true;
	}

	void Camera::Strafe(float distance) {
		position.x() -= right.x() * distance;
		position.y() -= right.y() * distance;
		position.z() -= right.z() * distance;
		viewDirty = true;
	}

	void Camera::Pitch(float angle) {
		math::Matrix3f rotation = math::AngleAxisf(angle, right).matrix();
		up = rotation * up;
		look = rotation * up;

		viewDirty = true;
	}

	void Camera::RotateY(float angle) {
		math::Matrix3f rotation = math::AngleAxisf(angle, math::Vector3f::UnitY()).matrix();
		up = rotation * up;
		right = rotation * right;
		look = rotation * look;

		viewDirty = true;
	}

}