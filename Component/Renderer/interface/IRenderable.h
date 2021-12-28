#pragma once
#include "IDevice.h"

namespace rg {

class IRenderable : public IObject {
	friend class IGraphicsPass;
public:
	virtual void setConstantBuffer(RESharePtr<IConstantBuffer> pCB) = 0;
	virtual void setSamplerResource(int index, RESharePtr<ISamplerResource> pSamTex) = 0;
	virtual void appendSamplerResource(RESharePtr<ISamplerResource> pSamTex) const = 0;
	virtual int getSamplerResource() const = 0;
	virtual RESharePtr<ISamplerResource> getSamplerResource(int index) const = 0;
	virtual RESharePtr<IConstantBuffer> getConstantBuffer() const = 0;
private:
	virtual RESharePtr<IShaderProgram> getShaderProgram() const = 0;
	virtual void setShaderProgram(RESharePtr<IShaderProgram> pShader) = 0;
};

}