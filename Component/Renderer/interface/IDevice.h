#pragma once
#include <memory>
#include <cstddef>
#include "IAllocator.h"

namespace rg {

class IFrameFence;
class IPassRenderable;
class IFrameResource;
class IGraphicsPass;
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
class ISamplerResource;
enum class Topology;
enum class DxgiFormat;

using uint64 = std::uint64_t;
using uint32 = std::uint32_t;
using uint16 = std::uint16_t;
using uint8  = std::uint8_t;

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

struct DeviceDesc {
	int width;
	int height;
	bool enableDebugLayer;
	union {
		void *hwnd;		// window hwnd
	};
};

class IDevice : public IObject {
public:
	virtual void initialize(const DeviceDesc &desc) = 0;
	virtual RGSharePtr<IFrameQueue> getFrameQueue() const = 0;
	virtual RGUniquePtr<IGPUIndexBuffer> createIndexBuffer(const IndexBufferDesc &desc) const = 0;
	virtual RGUniquePtr<IGPUVertexBuffer> createVertexBuffer(const VertexBufferDesc &desc) const = 0;
	virtual RGUniquePtr<IInputLayout> createInputLayout(const InputLayoutDesc &desc) const = 0;
	virtual ~IDevice() = default;
public:	// option
	virtual bool getDebugLayerState() const = 0;
	virtual bool setDebugLayerState(bool flag) = 0;
	virtual int getSwapChainBufferCount() const = 0;
	virtual bool get4xMsaaState() const = 0;
	virtual void set4xMsaaState(bool flag) = 0;
public:	// event
	virtual void onResize(int width, int height) = 0;
public:	// static member function
	static IDevice *instance();
	
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



}