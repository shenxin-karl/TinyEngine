#pragma once
#include "Allocator/Allocator.hpp"

namespace rg {

template<typename T>
using RGUniquePtr = alloc::UniquePtr<T, alloc::AllocatorType::Renderer>;

template<typename T>
using RGSharePtr = alloc::SharePtr<T, alloc::AllocatorType::Renderer>;

template<typename T>
using RGWeakPtr = alloc::WeakPtr<T, alloc::AllocatorType::Renderer>;

template<typename T, typename... Args>
RGUniquePtr<T> MakeUnique(Args&&... args) {
	return alloc::MakeUnique<T, alloc::AllocatorType::Renderer>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
RGSharePtr<T> MakeShared(Args&&... args) {
	return alloc::MakeShared<T, alloc::AllocatorType::Renderer>(std::forward<Args>(args)...);
}

template<typename T>
using RGAllocator = alloc::Allocator<T, alloc::AllocatorType::Renderer>;

}