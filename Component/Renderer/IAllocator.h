#pragma once
#include <cstddef>
#include <memory>
#include "IObject.h"

namespace re {

class IAllocator : IObject {
public:
	virtual void *allocate(std::size_t n) = 0;
	virtual void deallcate(void *p, std::size_t n) noexcept = 0;
	virtual ~IAllocator() = default;
};

}