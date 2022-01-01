#include "D3D12FrameQueue.h"
#include "D3D12Fence.h"
#include "D3D12FrameResource.h"

namespace dxrg {

void D3D12FrameQueue::initialize(ID3D12Device *pDevice) {
	_pFrameFence = MakeShared<D3D12Fence>();
	_pFrameFence->initialize(pDevice);
	for (uint32 i = 0; i < _frameQueue.size(); ++i)
		_frameQueue[i] = MakeShared<D3D12FrameResource>();
}

RGSharePtr<IFrameResource> D3D12FrameQueue::getCurrentFrameResource() const {
	return std::static_pointer_cast<IFrameResource>(
		_frameQueue[_currentFrameResourceIndex]
	);
}

RGSharePtr<IFrameFence> D3D12FrameQueue::getFrameFence() const {
	return _pFrameFence;
}

void D3D12FrameQueue::waitForAllFrameResource() {
	for (auto &pFrameResource : _frameQueue)
		_pFrameFence->waitForCompletion(pFrameResource->getFenceValue());
	_currentFrameResourceIndex = 0;
}

void D3D12FrameQueue::beginFrame() {
	auto fence = getCurrentFrameResource()->getFenceValue();
	_pFrameFence->waitForCompletion(fence);
}

void D3D12FrameQueue::endFrame() {
	getCurrentFrameResource()->setFenceValue(_pFrameFence->incrementFenceValue());
	_currentFrameResourceIndex = (_currentFrameResourceIndex + 1) % D3D12Device::kSwapChainBufferCount;
}

D3D12FrameQueue::~D3D12FrameQueue() {
	waitForAllFrameResource();
}

}