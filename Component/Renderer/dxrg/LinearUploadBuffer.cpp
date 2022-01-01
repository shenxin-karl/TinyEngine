#include "LinearUploadBuffer.h"
#include "D3D12Device.h"

namespace dxrg {

RGSharePtr<LinearUploadBuffer::Page> LinearUploadBuffer::requestPage() {
	RGSharePtr<LinearUploadBuffer::Page> page;
	if (!_availablePage.empty()) {
		page = _availablePage.front();
		_availablePage.pop_front();
	} else {
		page = MakeShared<Page>();
		_pagePool.push_back(page);
	}
	return page;
}

LinearUploadBuffer::LinearUploadBuffer(size_t pageSize /*= kDefaultPageSize*/) : _pageSize(pageSize) {
}

size_t LinearUploadBuffer::getPageSize() const noexcept {
	return _pageSize;
}

LinearUploadBuffer::Allocation LinearUploadBuffer::allocate(size_t sizeInBytes, size_t alignment) {
	if (sizeInBytes > _pageSize)
		throw std::bad_alloc();

	if (_currentPage == nullptr || !_currentPage->hasSpace(sizeInBytes, alignment))
		_currentPage = requestPage();
	return _currentPage->allocate(sizeInBytes, alignment);
}

void LinearUploadBuffer::reset() {
	_currentPage = nullptr;
	_availablePage = _pagePool;
	for (auto &page : _availablePage)
		page->reset();
}


LinearUploadBuffer::Page::Page(size_t pageSize) 
: _pageSize(pageSize), _pGPU(0), _pCPU(nullptr), _offset(0) 
{
	auto *pDevice = D3D12Device::instance()->getD3DDevice();
	ThrowIfFailed(pDevice->CreateCommittedResource(
		RVPtr(CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD)),
		D3D12_HEAP_FLAG_NONE,
		RVPtr(CD3DX12_RESOURCE_DESC::Buffer(_pageSize)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_resources)
	));

	_pGPU = _resources->GetGPUVirtualAddress();
	_resources->Map(0, nullptr, &_pCPU);
}

bool LinearUploadBuffer::Page::hasSpace(size_t sizeInBytes, size_t alignment) const {
	size_t aligneSize = sizeInBytes + (sizeInBytes % alignment);
	size_t aligneOffset = _offset + (_offset % alignment);
	return (aligneOffset + aligneSize) <= _pageSize;
}

LinearUploadBuffer::Allocation LinearUploadBuffer::Page::allocate(size_t sizeInBytes, size_t alignment) {
	if (!hasSpace(sizeInBytes, alignment))
		throw std::bad_alloc();

	char *pStart = static_cast<char *>(_pCPU);
	Allocation ret = {
		pStart + _offset,
		_pGPU  + _offset
	};

	sizeInBytes += (sizeInBytes % alignment);
	_offset = _offset + (_offset % alignment);
	_offset += sizeInBytes;
	return ret;
}


void LinearUploadBuffer::Page::reset() {
	_resources->Unmap(0, nullptr);
	_pCPU = nullptr;
	_pGPU = D3D12_GPU_VIRTUAL_ADDRESS(0);
}


}