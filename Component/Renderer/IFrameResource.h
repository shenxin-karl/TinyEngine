#pragma once
#include "IDevice.h"
#include <string>

namespace re {

class IConstantBuffer;
class IGPUTextureResource;
class IFrameResource : public IObject {
	virtual void addGraphPass(RESharePtr<IGraphPass> pPass) = 0;
	virtual RESharePtr<IGraphPass> getGraphPass(const std::string &name) const = 0;
	virtual REUniquePtr<IConstantBuffer> createGPUConstantBuffer(uint32 size) = 0;
	virtual REUniquePtr<IGPUTextureResource> createGPUTextureResource() = 0;
	virtual ~IFrameResource() = default;
};

}