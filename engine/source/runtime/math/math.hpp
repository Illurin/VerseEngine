#pragma once

#include "Eigen/Dense"

namespace engine {
	namespace math {

		using namespace Eigen;

		template<typename T>
		T Pi() {
			return static_cast<T>(EIGEN_PI);
		}

	}
}