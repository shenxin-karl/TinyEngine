#pragma once
#include <memory>
#include "interface/RendererAllocator.h"
#include "interface/InterfaceCommon.h"
#include "IObject.h"

namespace rg {

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
public:
	static IDevice *instance() noexcept;
	static void D3DInitialize();
private:
	static inline RGUniquePtr<IDevice> _pDevice;
};

}