#pragma once
#include <cstddef>
#include <memory>
#include "IObject.h"

namespace rg {

class IAllocator : IObject {
public:
	struct Deallocate {
		
	};

	IAllocator *instance();
public:
	virtual void *allocate(std::size_t n) = 0;
	virtual void deallcate(void *p, std::size_t n) noexcept = 0;
	virtual ~IAllocator() = default;
};

template<typename T>
using RGSharePtr = std::shared_ptr<T>;

template<typename T>
using RGUniquePtr = std::unique_ptr<T>;

template<typename T>
using RGWeakPtr = std::weak_ptr<T>;

template<typename T, typename... Args>
RGSharePtr<T> MakeShared(Args&&... args) {
	return std::allocate_shared<T>(*IDevice::instance()->getAllocator());
}

template<typename T, typename... Args>
RGUniquePtr<T> MakeUnique(Args&&... args) {
	auto *ptr = IDevice::instance()->allocate(1);
	new(ptr) T(std::forward<Args>(args)...);
	return RGUniquePtr<T>(ptr);
}

}