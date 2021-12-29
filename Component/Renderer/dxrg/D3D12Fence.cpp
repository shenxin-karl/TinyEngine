#include "D3D12Fence.h"

namespace dxrg {

void D3D12Fence::initialize(ID3D12Device *pDevice) {
	ThrowIfFailed(pDevice->CreateFence(
		0,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&_pFence)
	));
}

uint64 D3D12Fence::getCompletedValue() const {
	return _pFence->GetCompletedValue();
}

uint64 D3D12Fence::getCurrentFenceValue() const {
	return _fenceValue;
}

uint64 D3D12Fence::incrementFenceValue() {
	return ++_fenceValue;
}

void D3D12Fence::waitForCompletion(uint32 fence) const {
	if (fence != 0 && getCompletedValue() < fence) {
		HANDLE event = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
		if (event != nullptr) {
			ThrowIfFailed(_pFence->SetEventOnCompletion(fence, event));
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
}

}