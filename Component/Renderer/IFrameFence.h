#pragma once
#include "IDevice.h"

namespace re {

class IFrameFence : public IObject {
	virtual uint64 getCompletedValue() const = 0;
	virtual void onCompletion(uint32 fence) const = 0;
};

}