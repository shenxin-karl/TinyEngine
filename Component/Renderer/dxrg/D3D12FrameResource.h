#pragma once
#include "interface/IFrameResource.h"
#include "D3D12Common.h"

namespace dxrg {

class D3D12FrameResource : public IFrameResource {
public:
	virtual void addGraphPass(RESharePtr<IGraphicsPass> pPass) override;
	virtual RESharePtr<IGraphicsPass> getGraphPass(const std::string &name) const override;
	virtual REUniquePtr<IConstantBuffer> createGPUConstantBuffer(uint32 size) override;
	virtual REUniquePtr<IGPUTextureResource> createGPUTextureResource() override;
	virtual uint64 getFenceValue() const override;
	virtual void addFenceValue() override;
	virtual ~D3D12FrameResource() override;
private:
	uint64 _fence = 0;
};

}