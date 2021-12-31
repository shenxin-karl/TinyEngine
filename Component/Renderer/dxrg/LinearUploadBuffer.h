#pragma once
#include "D3D12Common.h"
#include <deque>
#include <memory>

namespace dxrg {

class LinearUploadBuffer {
	struct Allocation;
	struct Page;
	using PagePool = std::deque<RGSharePtr<Page>>;
public:
	constexpr static size_t kDefaultPageSize = 1024 * 1024 * 2;
	explicit LinearUploadBuffer(size_t pageSize = kDefaultPageSize);
	size_t getPageSize() const noexcept;
	Allocation allocate(size_t sizeInBytes, size_t alignment);
	void reset();
	~LinearUploadBuffer() = default;
private:
	RGSharePtr<Page> requestPage();
	RGSharePtr<Page>  _currentPage;
	PagePool		  _pagePool;			
	PagePool		  _availablePage;
	size_t			  _pageSize;
};

struct LinearUploadBuffer::Allocation {
	void *CPU;
	D3D12_GPU_VIRTUAL_ADDRESS GPU;
};

struct LinearUploadBuffer::Page {
	explicit Page(size_t pageSize);
	bool hasSpace(size_t sizeInBytes, size_t alignment) const;
	Allocation allocate(size_t sizeInBytes, size_t alignment);
	void reset();
private:
	WRL::ComPtr<ID3D12Resource>	 _resources;
	D3D12_GPU_VIRTUAL_ADDRESS	 _pGPU;
	void						*_pCPU;
	size_t						 _pageSize;
	size_t						 _offset;
};

}