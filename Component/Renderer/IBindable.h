#pragma once
#include "IObject.h"
#include "IDevice.h"

namespace re {

class IBindable : public IObject {
public:
	virtual void bind(RESharePtr<IGraphPass> pGraphPass) const = 0;
	virtual void unbind(RESharePtr<IGraphPass> pGraphPass) const = 0;
};

}