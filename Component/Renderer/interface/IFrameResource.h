#pragma once
#include "IDevice.h"
#include <string>

namespace rg {

class IConstantBuffer;
class IGPUTextureResource;
class IFrameResource : public IObject {
public:
	virtual void addGraphPass(RGSharePtr<IGraphicsPass> pPass) = 0;
	virtual RGSharePtr<IGraphicsPass> getGraphPass(const std::string &name) const = 0;
	virtual RGUniquePtr<IConstantBuffer> createGPUConstantBuffer(uint32 size) = 0;
	virtual RGUniquePtr<IGPUTextureResource> createGPUTextureResource() = 0;
	virtual uint64 getFenceValue() const = 0;
	virtual void setFenceValue(uint64 fence) = 0;
	virtual ~IFrameResource() = default;
};

}