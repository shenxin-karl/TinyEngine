#pragma once
#include "IDevice.h"

namespace re {

enum class PassResourceType {
	PRT_RenderTarget,
	PRT_Depth,
	PRT_Stencil,
};

struct ClearValueDesc {
	union  {
		float colors[4];
		float depth;
		uint32 stencil;
	};
	ScissorRect rect;
};


class IPassResource : public IObject {
public:
	virtual void bind() const = 0;
	virtual void unbind() const = 0;
	virtual void clear(const ClearValueDesc &desc) const = 0;
};

}