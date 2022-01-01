#pragma once
#include "IObject.h"
#include "IDevice.h"

namespace rg {

class IBindable : public IObject {
public:
	virtual void bind(RGSharePtr<IGraphicsPass> pGraphPass) const = 0;
	virtual void unbind(RGSharePtr<IGraphicsPass> pGraphPass) const = 0;
};

}