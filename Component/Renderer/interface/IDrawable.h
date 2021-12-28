#pragma once
#include "IDevice.h"

namespace rg {

class IDrawable : public IObject {
public:
	virtual void setObjectConstantBuffer(RESharePtr<IConstantBuffer> pCBBuffer) = 0;
	virtual void setGPUVertexBuffer(RESharePtr<IGPUVertexBuffer> pGPUVB) = 0;
	virtual void setGPUIndexBuffer(RESharePtr<IGPUIndexBuffer> pGPUIB) = 0;
	virtual void setInputLayout(RESharePtr<IInputLayout> pInputLayout) = 0;
	virtual void setTopology(Topology type) = 0;
	virtual RESharePtr<IConstantBuffer> getObjectConstantBuffer() const = 0;
	virtual RESharePtr<IGPUVertexBuffer> getGPUVertexBuffer() const = 0;
	virtual RESharePtr<IGPUIndexBuffer> getGPUIndexBuffer() const = 0;
	virtual RESharePtr<IInputLayout> getInputLayout() const = 0;
	virtual Topology getTopology() const = 0;
};

}