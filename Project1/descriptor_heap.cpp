#include "descriptor_heap.h"
#include <wincrypt.h>



HRESULT CreateDescriptorHeap(
	[in] const D3D12_DESCRIPTOR_HEAP_DESC* pDescriptorHeapDesc,
	REFIID									riid,
	[out] void								ppvHeap
);
//
 rtvHeapDesc = {};