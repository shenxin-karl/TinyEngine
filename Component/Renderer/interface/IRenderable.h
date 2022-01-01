#pragma once
#include "IDevice.h"

namespace rg {

class IRenderable : public IObject {
	friend class IGraphicsPass;
public:
	virtual void setConstantBuffer(RGSharePtr<IConstantBuffer> pCB) = 0;
	virtual void setSamplerResource(int index, RGSharePtr<ISamplerResource> pSamTex) = 0;
	virtual void appendSamplerResource(RGSharePtr<ISamplerResource> pSamTex) const = 0;
	virtual int getSamplerResource() const = 0;
	virtual RGSharePtr<ISamplerResource> getSamplerResource(int index) const = 0;
	virtual RGSharePtr<IConstantBuffer> getConstantBuffer() const = 0;
private:
	virtual RGSharePtr<IShaderProgram> getShaderProgram() const = 0;
	virtual void setShaderProgram(RGSharePtr<IShaderProgram> pShader) = 0;
};

}