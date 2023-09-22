#pragma once
#include "Platform.h"
#include <memory>
#include <iostream>

#define BIT(x) (1 << x)

namespace Nuim {
	template <typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T>
	using Ref = std::shared_ptr<T>;
}