#pragma once
#include "IDevice.h"
#include "IPassResource.h"
#include <string>


namespace rg {

struct GraphicsPassDesc {
	PassResourceType type;
	DxgiFormat format;
	union {
		float      colors[4];
		uint32	   stencil;
		float	   depth;
	};
};

class IGraphicsPass : public IObject {
public:
	virtual std::string getName() const = 0;
	virtual void source(PassResourceType type, const std::string &name) = 0;
	virtual void sink(const GraphicsPassDesc &desc, const std::string &name) = 0;
	virtual RGUniquePtr<IPassRenderable> createRenderable() const = 0;
	virtual RGSharePtr<IConstantBuffer> getPassConstantBuffer() const = 0;
	virtual RGWeakPtr<IPassResource> getSink(PassResourceType type, const std::string &name) const = 0;
	virtual bool isFinished() const = 0;
	virtual bool isRunning() const = 0;
	virtual ~IGraphicsPass() = default;
};

}