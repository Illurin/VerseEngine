#pragma once

#include "runtime/math/math.hpp"

namespace engine {

	class Camera {
	public:
		explicit Camera(float aspect);

		math::Vector3f GetPosition() const { return position; }
		math::Matrix4f GetViewMatrix() const { return viewMatrix; }
		math::Matrix4f GetProjMatrix() const { return projMatrix; }

		void SetLens(float fovY, float aspect, float nearZ, float farZ);

		void UpdataViewMatrix();

		void LookAt(math::Vector3f position, math::Vector3f target, math::Vector3f worldUp);
		void Walk(float distance);
		void Strafe(float distance);
		void Pitch(float angle);
		void RotateY(float angle);

	private:
		math::Vector3f position{ 0.0f, 0.0f, 0.0f };
		math::Vector3f right{ 1.0f, 0.0f, 0.0f };
		math::Vector3f up{ 0.0f, 1.0f, 0.0f };
		math::Vector3f look{ 0.0f, 0.0f, -1.0f };

		math::Matrix4f viewMatrix;
		math::Matrix4f projMatrix;

		float nearZ{ 0.0f };
		float farZ{ 0.0f };

		float aspect{ 0.0f };
		float fovY{ 0.0f };

		bool viewDirty{ false };
	};

}