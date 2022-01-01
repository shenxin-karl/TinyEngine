#pragma once
#include "IDevice.h"

namespace rg {

class IDrawable : public IObject {
public:
	virtual void setObjectConstantBuffer(RGSharePtr<IConstantBuffer> pCBBuffer) = 0;
	virtual void setGPUVertexBuffer(RGSharePtr<IGPUVertexBuffer> pGPUVB) = 0;
	virtual void setGPUIndexBuffer(RGSharePtr<IGPUIndexBuffer> pGPUIB) = 0;
	virtual void setInputLayout(RGSharePtr<IInputLayout> pInputLayout) = 0;
	virtual void setTopology(Topology type) = 0;
	virtual RGSharePtr<IConstantBuffer> getObjectConstantBuffer() const = 0;
	virtual RGSharePtr<IGPUVertexBuffer> getGPUVertexBuffer() const = 0;
	virtual RGSharePtr<IGPUIndexBuffer> getGPUIndexBuffer() const = 0;
	virtual RGSharePtr<IInputLayout> getInputLayout() const = 0;
	virtual Topology getTopology() const = 0;
};

}