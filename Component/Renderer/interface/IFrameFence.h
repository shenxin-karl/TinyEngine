#pragma once
#include "IDevice.h"

namespace rg {

class IFrameFence : public IObject {
	virtual uint64 getCompletedValue() const = 0;
	virtual uint64 getCurrentFenceValue() const = 0;
	virtual uint64 incrementFenceValue() = 0;
	virtual void waitForCompletion(uint32 fence) const = 0;
};

}