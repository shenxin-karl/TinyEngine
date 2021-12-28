#pragma once
#include "interface/IFrameFence.h"
#include "D3D12Common.h"

namespace dxrg {

using namespace rg;
class D3D12Fence : public IFrameFence {
public:
	void initialize(ID3D12Device *pDevice);
	virtual uint64 getCompletedValue() const override;
	virtual void waitForCompletion(uint32 fence) const override;
	virtual ~D3D12Fence() override = default;
private:
	WRL::ComPtr<ID3D12Fence> _pFence;
};

}