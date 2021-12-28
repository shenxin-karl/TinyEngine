#pragma once
#include "IObject.h"
#include "IDevice.h"

namespace rg {

class IBindable : public IObject {
public:
	virtual void bind(RESharePtr<IGraphicsPass> pGraphPass) const = 0;
	virtual void unbind(RESharePtr<IGraphicsPass> pGraphPass) const = 0;
};

}