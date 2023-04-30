#include "camera.hpp"

#include "render_device.hpp"

namespace engine {

    Camera::Camera() {
        auto aspect = static_cast<float>(RenderDevice::GetSwapchain()->GetImageExtent().width)
                    / static_cast<float>(RenderDevice::GetSwapchain()->GetImageExtent().height);
        SetLens(0.25f * math::Pi<float>(), aspect, 0.1f, 1000.0f);
    }

    void Camera::SetLens(float fovY, float aspect, float nearZ, float farZ) {
        this->fovY = fovY;
        this->aspect = aspect;
        this->nearZ = nearZ;
        this->farZ = farZ;
        
        projMatrix = math::Projection::Persp(fovY, aspect, nearZ, farZ);
    }

    void Camera::SetPosition(math::Vector3f position) {
        this->position = position;
        viewDirty = true;
    }

    void Camera::UpdataViewMatrix() {
        if (viewDirty) {
            look.normalize();
            right = look.cross(up).normalized();
            up = right.cross(look);

            viewMatrix << right.x(), right.y(), right.z(), -position.dot(right),
                          up   .x(), up   .y(), up   .z(), -position.dot(up),
                          -look.x(), -look.y(), -look.z(), -position.dot(-look),
                          0.0f     , 0.0f     , 0.0f     , 1.0f;

            viewDirty = false;
        }
    }

    void Camera::LookAt(math::Vector3f target, math::Vector3f worldUp) {
        look = math::Vector3f(target.x() - position.x(), target.y() - position.y(), target.z() - position.z()).normalized();
        right = look.cross(worldUp);
        up = right.cross(look);
        viewDirty = true;
    }

    void Camera::Walk(float distance) {
        position.x() += look.x() * distance;
        position.y() += look.y() * distance;
        position.z() += look.z() * distance;
        viewDirty = true;
    }

    void Camera::Strafe(float distance) {
        position.x() += right.x() * distance;
        position.y() += right.y() * distance;
        position.z() += right.z() * distance;
        viewDirty = true;
    }

    void Camera::Pitch(float angle) {
        math::Matrix3f rotation = math::AngleAxisf(angle, right).matrix();
        up = rotation * up;
        look = rotation * look;
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