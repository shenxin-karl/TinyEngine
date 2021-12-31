#include "D3D12FrameResource.h"

namespace dxrg {

uint64 D3D12FrameResource::getFenceValue() const {
	return _fence;
}

void D3D12FrameResource::setFenceValue(uint64 fence) {
	_fence = fence;
}

}