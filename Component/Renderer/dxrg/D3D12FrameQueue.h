#pragma once
#include "interface/IFrameQueue.h"
#include "D3D12Device.h"
#include <array>
#include <d3d12.h>

namespace dxrg {

using namespace rg;
class D3DFrameResource;
class D3D12Fence;
class D3D12FrameQueue : public IFrameQueue {
	friend class D3D12Device;
public:
	void initialize(ID3D12Device *pDevice);
public:
	virtual RESharePtr<IFrameResource> getCurrentFrameResource() const override;
	virtual RESharePtr<IFrameFence> getFrameFence() const override;
	virtual void beginFrame() override;
	virtual void endFrame() override;
	virtual ~D3D12FrameQueue() override;
private:
	std::array<RESharePtr<D3DFrameResource>, D3D12Device::kSwapChainBufferCount> _frameQueue;
	RESharePtr<D3D12Fence> _pFrameFence;
	uint32 _currentFrameResourceIndex = 0;
};

}