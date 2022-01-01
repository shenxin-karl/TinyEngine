#pragma once
#include "D3D12Common.h"
#include <cstdint>
#include <mutex>
#include <memory>
#include <set>
#include <vector>
#include <map>
#include <queue>

namespace dxrg {

class DescriptorAllocation {

};

class DescriptorAllocatorPage : public std::enable_shared_from_this<DescriptorAllocatorPage> {
public:
	DescriptorAllocatorPage(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32 numDescriptors);
	auto getHeapType() const noexcept;
	bool hasSpace(uint32 numDescriptors) const noexcept;
	uint32 numFreeHandles() const noexcept;
	DescriptorAllocation allocate(uint32 numDescripotrs);
	void free(DescriptorAllocation &&handle, uint64 frameNumber);
	void releaseStaleDescriptor(uint64 frameNumber);
protected:
	uint32 computeOffset(D3D12_CPU_DESCRIPTOR_HANDLE handle);
	void addNewBlock(uint32 offset, uint32 numDescriptors);
	void freeBlock(uint32 offset, uint32 numDescriptors);
private:
	struct FreeBlockInfo;
	using OffsetType = uint32;
	using SizeType = uint32;
	using FreeListByOffset = std::map<OffsetType, FreeBlockInfo>;
	using FreeListBySize = std::multimap<SizeType, FreeListByOffset::iterator>;
	struct FreeBlockInfo {
		FreeBlockInfo(SizeType size);
	public:
		SizeType				  _size;
		FreeListBySize::iterator  _freeListBySizeIt;
	};

	struct StaleDescriptorInfo {
		StaleDescriptorInfo(OffsetType offset, SizeType size, uint64 frameNumber);
	public:
		OffsetType  _offset;
		SizeType	_size;
		uint64		_frameNumber;
	};

	using StaleDescriptorQueue = std::queue<StaleDescriptorInfo>;

private:
	// available free list
	FreeListByOffset	  _freeListByOffset;
	FreeListBySize		  _freeListBySize;
	StaleDescriptorQueue  _staleDescriptorQueue;
	
	WRL::ComPtr<ID3D12DescriptorHeap> _descriptorHeap;
	D3D12_DESCRIPTOR_HEAP_TYPE		  _heapType;
	CD3DX12_CPU_DESCRIPTOR_HANDLE	  _baseDescriptor;
	std::mutex _allocationMutex;
	uint32	   _descriptorHandleIncrementSize;
	uint32     _numDescriptorInHeap;
	uint32     _numFreeHandles;
};



class DescriptorAllocator {
public:
	DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32 numDescriptorPreHeap = 256);
	DescriptorAllocator(const DescriptorAllocator &) = delete;
	DescriptorAllocation allocate(uint32 numDescriptors = 1);
	void releaseStaleDescriptors(uint64 frameNumber);
	virtual ~DescriptorAllocator();
private:
	using DescriptorHeapPool = std::vector<RGSharePtr<DescriptorAllocatorPage>>;
	RGSharePtr<DescriptorAllocatorPage> createAllocatorPage();
private:
	D3D12_DESCRIPTOR_HEAP_TYPE	_heapType;
	uint32						_numDescriptorsPreHeap;
	DescriptorHeapPool			_heapPool;
	std::set<size_t>			_availableHeaps;
	std::mutex					_allocationMutex;
};

}