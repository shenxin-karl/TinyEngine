#include "UploadBuffer.h"
#include "D3D12Device.h"

namespace dxrg {

std::shared_ptr<dxrg::UploadBuffer::Page> UploadBuffer::requestPage() {
	std::shared_ptr<Page> page;
	if (!_availablePage.empty()) {
		page = _availablePage.front();
		_availablePage.pop_front();
	} else {
		page = std::make_shared<Page>(_pageSize);
		_pagePool.push_back(page);
	}
	return page;
}

UploadBuffer::UploadBuffer(size_t pageSize /*= kDefaultPageSize*/) : _pageSize(pageSize) {
}

size_t UploadBuffer::getPageSize() const noexcept {
	return _pageSize;
}

UploadBuffer::Allocation UploadBuffer::allocate(size_t sizeInBytes, size_t alignment) {
	if (sizeInBytes > _pageSize)
		throw std::bad_alloc();

	if (_currentPage == nullptr || !_currentPage->hasSpace(sizeInBytes, alignment))
		_currentPage = requestPage();
	return _currentPage->allocate(sizeInBytes, alignment);
}

void UploadBuffer::reset() {
	_currentPage = nullptr;
	_availablePage = _pagePool;
	for (auto &page : _availablePage)
		page->reset();
}


UploadBuffer::Page::Page(size_t pageSize) 
: _pageSize(pageSize), _pGPU(0), _pCPU(nullptr), _offset(0) 
{
	//D3D12Device::instance()->
}

}