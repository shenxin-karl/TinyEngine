#pragma once
#include "interface/IFrameResource.h"
#include "D3D12Common.h"

namespace dxrg {

class D3D12FrameResource : public IFrameResource {
public:
	virtual void addGraphPass(RGSharePtr<IGraphicsPass> pPass) override;
	virtual RGSharePtr<IGraphicsPass> getGraphPass(const std::string &name) const override;
	virtual RGUniquePtr<IConstantBuffer> createGPUConstantBuffer(uint32 size) override;
	virtual RGUniquePtr<IGPUTextureResource> createGPUTextureResource() override;
	virtual uint64 getFenceValue() const override;
	virtual void setFenceValue(uint64 fence) override;
	virtual ~D3D12FrameResource() override;
private:
	uint64 _fence = 0;
};

}