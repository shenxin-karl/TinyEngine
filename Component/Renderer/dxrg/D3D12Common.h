#pragma once
#include <windows.h>
#include <exception>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>
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

}