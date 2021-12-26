#pragma once
#include "IBindable.h"

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


class IPassResource : public IBindable {
public:
	virtual void clear(const ClearValueDesc &desc) const = 0;
};

}