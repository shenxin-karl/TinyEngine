#pragma once
#include "D3D12Common.h"
#include "interface/IDevice.h"

namespace dxrg {

class UploadBuffer {
public:
	UploadBuffer(const void *pData, uint64 size, bool isConstantBuffer);
	UploadBuffer(const UploadBuffer &) = delete;
	UploadBuffer(UploadBuffer &&other) noexcept;
	uint64 getSize() const noexcept;
	void copyData(const void *pData, uint64 size);
	auto getGPUAddress() const noexcept;
	~UploadBuffer();
	friend void swap(UploadBuffer &lhs, UploadBuffer &rhs) noexcept;
private:
	void   *_pCPU;
	size_t  _size;
	WRL::ComPtr<ID3D12Resource> _pUploadBuffer;
};

}