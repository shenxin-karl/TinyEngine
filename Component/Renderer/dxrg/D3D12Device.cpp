#include "D3D12Device.h"
#include "D3D12FrameQueue.h"

namespace dxrg {

void D3D12Device::initialize() {
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&_pDxgiFactory)));
	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_pDevice));
	if (FAILED(hr)) {
		WRL::ComPtr<IDXGIAdapter> pWarpAdapter;
		ThrowIfFailed(_pDxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
		ThrowIfFailed(D3D12CreateDevice(
			pWarpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&_pDevice)
		));
	}

	_pFrameQueue = MakeUnique<D3D12FrameQueue>();
	_pFrameQueue->initialize(_pDevice.Get());


}

}