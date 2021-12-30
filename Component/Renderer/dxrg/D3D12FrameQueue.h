#pragma once
#include "interface/IFrameQueue.h"
#include "D3D12Device.h"
#include <array>
#include <d3d12.h>

namespace dxrg {

using namespace rg;
class D3D12FrameResource;
class D3D12Fence;
class D3D12FrameQueue : public IFrameQueue {
	friend class D3D12Device;
public:
	void initialize(ID3D12Device *pDevice);
public:
	virtual RGSharePtr<IFrameResource> getCurrentFrameResource() const override;
	virtual RGSharePtr<IFrameFence> getFrameFence() const override;
	virtual void waitForAllFrameResource() override;
	virtual void beginFrame() override;
	virtual void endFrame() override;
	virtual ~D3D12FrameQueue() override;
private:
	std::array<RGSharePtr<D3D12FrameResource>, D3D12Device::kSwapChainBufferCount> _frameQueue;
	RGSharePtr<D3D12Fence> _pFrameFence;
	uint32 _currentFrameResourceIndex = 0;
};

}