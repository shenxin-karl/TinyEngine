#pragma once
#include <windows.h>
#include <exception>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include "D3Dx12.h"
#include "interface/RendererAllocator.h"
#undef  min
#undef  max

// direct x render graph
namespace dxrg {

using namespace rg;
namespace WRL = Microsoft::WRL;

inline void ThrowIfFailed(HRESULT hr) {
	if (FAILED(hr))
		throw std::exception("error");
}

#define USE_FUNCTION_GET_RIGHT_VALUE_PTR
template<typename T>
inline const T *_getRightValuePtr(const T &v) noexcept {
	return &v;
}

template<typename T>
inline auto RVPtr(T &&v) {
#ifdef USE_FUNCTION_GET_RIGHT_VALUE_PTR 
	return _getRightValuePtr(v);
#else
	return &v;
#endif
}

inline size_t alignmentUp256(size_t size) {
	constexpr size_t mask = ~0xff;
	return (size + 255) & mask;
}

}