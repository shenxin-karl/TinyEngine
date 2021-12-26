#pragma once
#include "IDevice.h"
#include "IPassResource.h"
#include <string>


namespace re {

class IGraphPass : public IObject {
public:
	virtual std::string getName() const = 0;
	virtual void source(PassResourceType type, const std::string &name) = 0;
	virtual void sink(PassResourceType type, const std::string &name) = 0;
	virtual RESharePtr<IShaderProgram> getShaderProgrma(const std::string &name) = 0;
	virtual REWeakPtr<IPassResource> getSink(PassResourceType type, const std::string &name) const = 0;
	virtual bool isFinished() const = 0;
	virtual bool isRunning() const = 0;
	virtual ~IGraphPass() = default;
};

}