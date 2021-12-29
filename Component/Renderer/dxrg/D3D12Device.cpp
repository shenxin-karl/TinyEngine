#include "D3D12Device.h"
#include "D3D12FrameQueue.h"
#include "D3Dx12.h"
#include <cassert>

namespace dxrg {

void D3D12Device::initialize(const DeviceDesc &desc) 
{
	_width = desc.width;
	_height = desc.height;
	_hwnd = static_cast<HWND>(desc.hwnd);
	_enableDebugLayer = desc.enableDebugLayer;

	if (desc.enableDebugLayer) {
		WRL::ComPtr<ID3D12Debug> debugController;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();
	}

	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&_pDxgiFactory)));
	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_pDevice));
	if (FAILED(hr)) {
		WRL::ComPtr<IDXGIAdapter> pWarpAdapter;
		ThrowIfFailed(_pDxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
		ThrowIfFailed(D3D12CreateDevice(
			pWarpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&_pDevice)
		));
	}

	_pFrameQueue = MakeShared<D3D12FrameQueue>();
	_pFrameQueue->initialize(_pDevice.Get());
	
	_rtvDescriptorSize = _pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	_dsvDescriptorSize = _pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	_cbvSrvUavDescriptorSize = _pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msaaQualityLevels;
	msaaQualityLevels.Format = _backBufferFormat;
	msaaQualityLevels.SampleCount = 4;
	msaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msaaQualityLevels.NumQualityLevels = 0;
	ThrowIfFailed(_pDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msaaQualityLevels,
		sizeof(msaaQualityLevels)
	));
	_4xMsaaQuality = msaaQualityLevels.NumQualityLevels;
	assert(_4xMsaaQuality > 0 && "unexpected MSAA quality level");

	createCommandObjects();
	createSwapChain();
	createRtvAndDsvDescriptorHeaps();
}

rg::RGSharePtr<rg::IFrameQueue> D3D12Device::getFrameQueue() const {
	return std::static_pointer_cast<IFrameQueue>(_pFrameQueue);
}

bool D3D12Device::getDebugLayerState() const {
	return _enableDebugLayer;
}

bool D3D12Device::setDebugLayerState(bool flag) {
	return false;
}

int D3D12Device::getSwapChainBufferCount() const {
	return kSwapChainBufferCount;
}

bool D3D12Device::get4xMsaaState() const {
	return _4xMsaaState;
}

void D3D12Device::set4xMsaaState(bool flag) {
	if (_4xMsaaState != flag) {
		_4xMsaaState = flag;
		onResize(_width, _height);
	}
}

void D3D12Device::onResize(int width, int height) {
	_pFrameQueue->waitForAllFrameResource();
	_width = width;
	_height = height;
	
	ThrowIfFailed(_pCommandList->Reset(_pCommandAlloc.Get(), nullptr));

	for (uint32 i = 0; i < kSwapChainBufferCount; ++i)
		_pSwapChainBuffer[i].Reset();
	_pDepthStencilBuffer.Reset();

	ThrowIfFailed(_pSwapChain->ResizeBuffers(
		kSwapChainBufferCount, 
		_width, 
		_height, 
		_backBufferFormat, 
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	));
	_currentBackBufferIndex = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(_pRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (uint32 i = 0; i < kSwapChainBufferCount; ++i) {
		ThrowIfFailed(_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&_pSwapChainBuffer[i])));
		_pDevice->CreateRenderTargetView(_pSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, _rtvDescriptorSize);
	}
}

void D3D12Device::createCommandObjects() {
	D3D12_COMMAND_QUEUE_DESC queueDesc;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(_pDevice->CreateCommandQueue(
		&queueDesc,
		IID_PPV_ARGS(&_pCommandQueue)
	));
	ThrowIfFailed(_pDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&_pCommandAlloc)
	));
	ThrowIfFailed(_pDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		_pCommandAlloc.Get(),
		nullptr,
		IID_PPV_ARGS(&_pCommandList)
	));
}

void D3D12Device::createSwapChain() {
	_pSwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = _width;
	sd.BufferDesc.Height = _height;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.Format = _backBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = get4xMsaaState() ? 4 : 1;
	sd.SampleDesc.Quality = get4xMsaaState() ? (_4xMsaaQuality - 1) : 0;
	sd.OutputWindow = _hwnd;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	ThrowIfFailed(_pDxgiFactory->CreateSwapChain(
		_pCommandQueue.Get(), 
		&sd, 
		&_pSwapChain
	));
}

void D3D12Device::createRtvAndDsvDescriptorHeaps() {
	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.NodeMask = 0;
	rtvDesc.NumDescriptors = kSwapChainBufferCount;
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	ThrowIfFailed(_pDevice->CreateDescriptorHeap(
		&rtvDesc,
		IID_PPV_ARGS(&_pRtvHeap)
	));

	D3D12_DESCRIPTOR_HEAP_DESC dsvDesc;
	dsvDesc.NodeMask = 0;
	dsvDesc.NumDescriptors = 1;
	dsvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	ThrowIfFailed(_pDevice->CreateDescriptorHeap(
		&dsvDesc,
		IID_PPV_ARGS(&_pDsvHeap)
	));
}

}