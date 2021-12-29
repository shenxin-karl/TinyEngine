#include "interface/IDevice.h"
#include "D3D12Common.h"

namespace dxrg {

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class D3D12FrameQueue;
class D3D12Device : public IDevice {
public:
	virtual void initialize(const DeviceDesc &desc) override;
	virtual RGSharePtr<IFrameQueue> getFrameQueue() const override;
	virtual RGUniquePtr<IGPUIndexBuffer> createIndexBuffer(const IndexBufferDesc &desc) const override;
	virtual RGUniquePtr<IGPUVertexBuffer> createVertexBuffer(const VertexBufferDesc &desc) const override;
	virtual RGUniquePtr<IInputLayout> createInputLayout(const InputLayoutDesc &desc) const override;
	virtual bool getDebugLayerState() const override;
	virtual bool setDebugLayerState(bool flag) override;
	virtual int getSwapChainBufferCount() const override;
	virtual bool get4xMsaaState() const override;
	virtual void set4xMsaaState(bool flag) override;
	virtual void onResize(int width, int height) override;
public:
	constexpr uint32 static kSwapChainBufferCount = 2;
private:
	void createCommandObjects();
	void createSwapChain();
	void createRtvAndDsvDescriptorHeaps();
private:
	WRL::ComPtr<ID3D12Device>				_pDevice;
	WRL::ComPtr<IDXGIFactory4>				_pDxgiFactory;
	WRL::ComPtr<IDXGISwapChain>				_pSwapChain;
	WRL::ComPtr<ID3D12Resource>				_pSwapChainBuffer[kSwapChainBufferCount];
	WRL::ComPtr<ID3D12Resource>				_pDepthStencilBuffer;
	WRL::ComPtr<ID3D12CommandAllocator>		_pCommandAlloc;
	WRL::ComPtr<ID3D12CommandQueue>			_pCommandQueue;
	WRL::ComPtr<ID3D12GraphicsCommandList>  _pCommandList;
	WRL::ComPtr<ID3D12DescriptorHeap>		_pRtvHeap;
	WRL::ComPtr<ID3D12DescriptorHeap>		_pDsvHeap;
	RGSharePtr<D3D12FrameQueue>				_pFrameQueue;
	UINT		_rtvDescriptorSize = 0;
	UINT		_dsvDescriptorSize = 0;
	UINT		_cbvSrvUavDescriptorSize = 0;
	UINT		_4xMsaaQuality = 0;
	uint32		_currentBackBufferIndex = 0;
	DXGI_FORMAT _backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT _depthStencilForamt = DXGI_FORMAT_D24_UNORM_S8_UINT;
	int			_width = 0;
	int			_height = 0;
	HWND		_hwnd = nullptr;
	bool		_4xMsaaState = false;
	bool		_enableDebugLayer = false;
};




}