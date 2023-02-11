#pragma once

#include "Eigen/Dense"

namespace engine {
	namespace math {

		using namespace Eigen;

		class Projection {
		public:
			template<typename T>
			static Matrix<T, 4, 4> Ortho(T left, T right, T bottom, T top, T nearZ, T farZ) {
				auto result = Matrix<T, 4, 4>::Identity();
				result(0, 0) = static_cast<T>(2) / (right - left);
				result(1, 1) = static_cast<T>(2) / (top - bottom);
				result(2, 2) = -static_cast<T>(1) / (farZ - nearZ);
				result(3, 0) = -(right + left) / (right - left);
				result(3, 1) = -(top + bottom) / (top - bottom);
				result(3, 2) = -nearZ / (farZ - nearZ);
				return result;
			}

			template<typename T>
			static Matrix<T, 4, 4> Persp(T fovY, T aspect, T nearZ, T farZ) {
				const T tanHalfFovY = tan(fovY / static_cast<T>(2));
				
				Matrix<T, 4, 4> result = Matrix<T, 4, 4>::Zero();
				result(0, 0) = static_cast<T>(1) / (aspect * tanHalfFovY);
				result(1, 1) = static_cast<T>(1) / (tanHalfFovY);
				result(2, 2) = farZ / (nearZ - farZ);
				result(2, 3) = - static_cast<T>(1);
				result(3, 2) = - (farZ * nearZ) / (farZ - nearZ);
				return result;
			}
		};

	}
}