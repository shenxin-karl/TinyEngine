#pragma once
#include "D3D12Common.h"
#include <deque>
#include <memory>

namespace dxrg {

class UploadBuffer {
	struct Allocation;
	struct Page;
	using PagePool = std::deque<std::shared_ptr<Page>>;
	std::shared_ptr<Page> requestPage();
public:
	constexpr static size_t kDefaultPageSize = 1024 * 1024 * 2;
	explicit UploadBuffer(size_t pageSize = kDefaultPageSize);
	size_t getPageSize() const noexcept;
	Allocation allocate(size_t sizeInBytes, size_t alignment);
	void reset();
	~UploadBuffer();
private:
	std::shared_ptr<Page> _currentPage;
	PagePool			  _pagePool;			
	PagePool			  _availablePage;
	size_t				  _pageSize;
};

struct UploadBuffer::Allocation {
	void *CPU;
	D3D12_GPU_VIRTUAL_ADDRESS GPU;
};

struct UploadBuffer::Page {
	explicit Page(size_t sizeInBytes);
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