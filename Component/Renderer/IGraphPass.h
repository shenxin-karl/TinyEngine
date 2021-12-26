#pragma once
#include "IDevice.h"
#include "IPassResource.h"
#include <string>


namespace re {

struct GraphPassDesc {
	PassResourceType type;
	DxgiFormat format;
	union {
		float      colors[4];
		uint32	   stencil;
		float	   depth;
	};
};

class IGraphPass : public IObject {
public:
	virtual std::string getName() const = 0;
	virtual void source(PassResourceType type, const std::string &name) = 0;
	virtual void sink(const GraphPassDesc &desc, const std::string &name) = 0;
	virtual REUniquePtr<IPassRenderable> createRenderable() const = 0;
	virtual RESharePtr<IConstantBuffer> getPassConstantBuffer() const = 0;
	virtual REWeakPtr<IPassResource> getSink(PassResourceType type, const std::string &name) const = 0;
	virtual bool isFinished() const = 0;
	virtual bool isRunning() const = 0;
	virtual ~IGraphPass() = default;
};

}