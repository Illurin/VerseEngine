#pragma once

#include "projection.hpp"

namespace engine {
	namespace math {

		template<typename T>
		T Pi() {
			return static_cast<T>(EIGEN_PI);
		}

	}
}