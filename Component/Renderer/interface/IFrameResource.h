#pragma once
#include "IDevice.h"
#include <string>

namespace rg {

class IConstantBuffer;
class IGPUTextureResource;
class IFrameResource : public IObject {
public:
	virtual void addGraphPass(RESharePtr<IGraphicsPass> pPass) = 0;
	virtual RESharePtr<IGraphicsPass> getGraphPass(const std::string &name) const = 0;
	virtual REUniquePtr<IConstantBuffer> createGPUConstantBuffer(uint32 size) = 0;
	virtual REUniquePtr<IGPUTextureResource> createGPUTextureResource() = 0;
	virtual uint64 getFenceValue() const = 0;
	virtual void addFenceValue() = 0;
	virtual ~IFrameResource() = default;
};

}