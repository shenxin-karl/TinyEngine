#include "UploadBuffer.h"
#include "D3D12Device.h"
#include <cassert>

namespace dxrg {

UploadBuffer::UploadBuffer(const void *pData, uint64 size, bool isConstantBuffer) {
	_size = size;
	if (isConstantBuffer)
		_size = alignmentUp256(size);

	auto *pDevice = D3D12Device::instance()->getD3DDevice();
	ThrowIfFailed(pDevice->CreateCommittedResource(
		RVPtr(CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD)),
		D3D12_HEAP_FLAG_NONE,
		RVPtr(CD3DX12_RESOURCE_DESC::Buffer(_size)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_pUploadBuffer)
	));
	_pUploadBuffer->Map(0, nullptr, &_pCPU);
	copyData(pData, size);
}

UploadBuffer::UploadBuffer(UploadBuffer &&other) noexcept
: _pCPU(other._pCPU), _size(other._size), _pUploadBuffer(other._pUploadBuffer) 
{
	other._pCPU = nullptr;
	other._size = 0;
	other._pUploadBuffer = nullptr;
}

uint64 UploadBuffer::getSize() const noexcept {
	return _size;
}

void UploadBuffer::copyData(const void *pData, uint64 size) {
	assert(size <= _size);
	memcpy(_pCPU, pData, size);
}

auto UploadBuffer::getGPUAddress() const noexcept {
	return _pUploadBuffer->GetGPUVirtualAddress();
}

UploadBuffer::~UploadBuffer() {
	_pUploadBuffer->Unmap(0, nullptr);
	_pCPU = nullptr;
	_size = 0;
}

void swap(UploadBuffer &lhs, UploadBuffer &rhs) noexcept {
	using std::swap;
	swap(lhs._pCPU, rhs._pCPU);
	swap(lhs._size, rhs._size);
	swap(lhs._pUploadBuffer, rhs._pUploadBuffer);
}


}