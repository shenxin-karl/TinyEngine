#pragma once
#include "IDevice.h"
#include <vector>

namespace re {
class IFrameQueue : public IObject {
	virtual RESharePtr<IFrameResource> getCurrentFrameResource() const = 0;
	virtual RESharePtr<IFrameFence> getFrameFence() const = 0;
	virtual void beginFrame() = 0;
	virtual void endFrame() = 0;
	virtual ~IFrameQueue() = default;
};
}