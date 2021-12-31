#pragma once
#include <cstdlib>
#include <memory>

namespace alloc {

enum class AllocatorType {
	Default,
	Renderer
};

template<typename T, AllocatorType AllocType = AllocatorType::Default>
struct Allocator {
public:
	constexpr Allocator() = default;

	using value_type = T;
	template<typename U, AllocatorType AT>
	constexpr Allocator(const Allocator<U, AT> & other) noexcept {}

	template<typename U>
	struct rebind {
		using other = Allocator<U, AllocType>;
	};

	[[nodiscard]] T *allocate(std::size_t n) {
		return reinterpret_cast<T *>(std::malloc(n * sizeof(T)));
	}

	void deallocate(T *p, std::size_t n) noexcept {
		std::free(p);
	}

	static Allocator &instance() noexcept {
		static Allocator alloc;
		return alloc;
	}
};

template<typename T, AllocatorType AT>
using SharePtr = std::shared_ptr<T>;

template<typename T, AllocatorType AT = AllocatorType::Default>
struct Deleter {
	void operator()(T *ptr) const noexcept {
		Allocator<T, AT>::instance().deallocate(ptr, 1);
	}
};

template<typename T, AllocatorType AT>
using UniquePtr = std::unique_ptr<T, Deleter<T, AT>>;

template<typename T, AllocatorType AT>
using WeakPtr = std::weak_ptr<T>;

template<typename T, AllocatorType AT, typename... Args> 
requires std::is_constructible_v<T, Args...>
SharePtr<T, AT> MakeShared(Args&&... args) {
	return std::allocate_shared<T>(
		Allocator<T, AT>::instance(),
		std::forward<Args>(args)...
	);
}

template<typename T, AllocatorType AT, typename... Args> 
requires std::is_constructible_v<T, Args...>
UniquePtr<T, AT> MakeUnique(Args&&... args) {
	auto *ptr = Allocator<T, AT>::instance().allocate(1);
	new(ptr) T(std::forward<Args>(args)...);
	return UniquePtr<T, AT>(ptr);
}


}