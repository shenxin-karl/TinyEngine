#pragma once
#include "IBindable.h"

namespace rg {

class IConstantBuffer : public IBindable {
public:
	virtual uint32 getSize() const = 0;
	virtual uint32 getCapacity() const = 0;
	virtual void copyData(void *pData, uint32 size) = 0;
};

}