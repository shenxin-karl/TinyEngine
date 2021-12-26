#pragma once
#include <memory>
#include <cstddef>
#include "IAllocator.h"

namespace re {

class IFrameFence;
class IPassRenderable;
class IFrameResource;
class IGraphPass;
class IPassConstantBuffer;
class IDrawable;
class IShaderProgram;
class IConstantBuffer;
class ITextureResource;
class IOBjectTranstfromBuffer;
class IGPUIndexBuffer;
class IGPUVertexBuffer;
class IInputLayout;
class IFrameQueue;
enum class Topology;
enum class DxgiFormat;

using uint64 = std::uint64_t;
using uint32 = std::uint32_t;

struct IndexBufferDesc {
	void *pData = nullptr;
	uint64 size = 0;
};

struct VertexBufferDesc {
	void *pData = nullptr;
	uint64 size = 0;
};

struct InputElementDesc {
	const char *pSemanticName;
	uint32	   semanticIndex;
	DxgiFormat format;
	uint32	   inputSlot;
	uint32	   alignedByteOffset;
	uint32	   instanceDataStepRate;
};

struct InputLayoutDesc {
	const InputElementDesc *pElement;
	uint32 numElement;
};

struct ScissorRect {
	float topLeftX;
	float topLeftY;
	float width;
	float height;
	float minDepth;
	float maxDepth;
};

template<typename T>
using RESharePtr = std::shared_ptr<T>;

template<typename T>
using REUniquePtr = std::unique_ptr<T>;

template<typename T>
using REWeakPtr = std::weak_ptr<T>;

class IDevice : public IObject {
public:
	static IDevice *instance();

	virtual void initialize() = 0;
	virtual void getFrameResourceSize() const = 0;
	virtual RESharePtr<IFrameQueue> getFrameQueue() const = 0;
	virtual REUniquePtr<IGPUIndexBuffer> createIndexBuffer(const IndexBufferDesc &desc) const = 0;
	virtual REUniquePtr<IGPUVertexBuffer> createVertexBuffer(const VertexBufferDesc &desc) const = 0;
	virtual REUniquePtr<IInputLayout> createInputLayout(const InputLayoutDesc &desc) const = 0;
	virtual ~IDevice() = default;
private:

	virtual IAllocator *getAllocator() const = 0;

	template<typename T>
	T *allocate(std::size_t n) {
		return static_cast<T *>(getAllocator()->allocate(n));
	}

	template<typename T>
	void deallocate(T *ptr, size_t n) {
		getAllocator()->deallcate(ptr, n);
	}
};

template<typename T, typename... Args>
RESharePtr<T> MakeShared(Args&&... args) {
	return std::allocate_shared<T>(*IDevice::instance()->getAllocator());
}

template<typename T, typename... Args>
REUniquePtr<T> MakeUnique(Args&&... args) {
	auto *ptr = IDevice::instance()->allocate(1);
	new(ptr) T(std::forward<Args>(args)...);
	return REUniquePtr<T>(ptr);
}

}