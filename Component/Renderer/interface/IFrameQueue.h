#pragma once
#include "IDevice.h"
#include <vector>

namespace rg {

class IFrameQueue : public IObject {
public:
	virtual RGSharePtr<IFrameResource> getCurrentFrameResource() const = 0;
	virtual RGSharePtr<IFrameFence> getFrameFence() const = 0;
	virtual void waitForAllFrameResource() = 0;
	virtual void beginFrame() = 0;
	virtual void endFrame() = 0;
	virtual ~IFrameQueue() = default;
};

}