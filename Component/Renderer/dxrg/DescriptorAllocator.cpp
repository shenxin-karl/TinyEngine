#include "DescriptorAllocator.h"
#include "D3D12Device.h"

namespace dxrg {

DescriptorAllocatorPage::DescriptorAllocatorPage(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32 numDescriptors)
: _heapType(type), _numDescriptorInHeap(numDescriptors) 
{
	auto *pDevice = D3D12Device::instance()->getD3DDevice();
	D3D12_DESCRIPTOR_HEAP_DESC haepDesc = {};
	haepDesc.Type = _heapType;
	haepDesc.NodeMask = 0;
	haepDesc.NumDescriptors = _numDescriptorInHeap;
	haepDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	ThrowIfFailed(pDevice->CreateDescriptorHeap(
		&haepDesc,
		IID_PPV_ARGS(&_descriptorHeap)
	));

	addNewBlock(0, _numFreeHandles);
}

auto DescriptorAllocatorPage::getHeapType() const noexcept {
	return _heapType;
}

bool DescriptorAllocatorPage::hasSpace(uint32 numDescriptors) const noexcept {
	return _freeListBySize.lower_bound(numDescriptors) != _freeListBySize.end();
}

uint32 DescriptorAllocatorPage::numFreeHandles() const noexcept {
	return _numFreeHandles;
}

void DescriptorAllocatorPage::addNewBlock(uint32 offset, uint32 numDescriptors) {
	auto offsetIt = _freeListByOffset.emplace(offset);
	auto sizeIt = _freeListBySize.emplace(numDescriptors);
	offsetIt.first->second._freeListBySizeIt = sizeIt;
}

DescriptorAllocator::DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32 numDescriptorPreHeap)
: _heapType(type), _numDescriptorsPreHeap(numDescriptorPreHeap) {
}

DescriptorAllocation DescriptorAllocator::allocate(uint32 numDescriptors) {
	std::lock_guard lock(_allocationMutex);
	DescriptorAllocation allocation;
	for (auto iter = _availableHeaps.begin(); iter != _availableHeaps.end(); ++iter) {
		auto pAllocatorPage = _heapPool[*iter];
		allocation = pAllocatorPage->allocate(numDescriptors);
		if (pAllocatorPage->numFreeHandles() == 0)
			iter = _availableHeaps.erase(iter);

		if (allocation.isNull()) {
			_numDescriptorsPreHeap = std::max(_numDescriptorsPreHeap, numDescriptors);
			auto pNewPage = createAllocatorPage();
			allocation = pNewPage->allocate(numDescriptors);
		} else {
			break;
		}
	}
	return allocation;
}

RGSharePtr<DescriptorAllocatorPage> DescriptorAllocator::createAllocatorPage() {
	auto pNewPage = MakeShared<DescriptorAllocatorPage>(_heapType, _numDescriptorsPreHeap);
	_heapPool.emplace_back(pNewPage);
	_availableHeaps.insert(_heapPool.size() - 1);
	return pNewPage;
}



}