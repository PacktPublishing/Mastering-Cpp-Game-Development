/*
    Copyright (c) 2016 Michael "Mickey" MacDonald.

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software
    is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "MemoryMgr.h"
#include <algorithm> 

// This is an example of an exported variable
//EXPORT int nMemoryMgr=0;

// This is an example of an exported function.
//EXPORT int fnMemoryMgr(void)
//{
//}

// This is the constructor of a class that has been exported.
// see MemoryMgr.h for the class definition
CMemoryMgr::CMemoryMgr(unsigned int dataSize, 
					   unsigned int heapSize, 
	                   unsigned int memoryAlignmentSize) :
	m_dataSize(dataSize), m_heapSize(heapSize)
{
	DeallocateAll();

	auto maxHeaderSize = std::max((unsigned int)sizeof(Block),m_dataSize);

	m_memoryAlignment = (maxHeaderSize % memoryAlignmentSize) ?
					(memoryAlignmentSize - maxHeaderSize % memoryAlignmentSize) : (0);

	m_blockSize = maxHeaderSize + m_memoryAlignment;

	m_blocksPerHeap = (m_heapSize - sizeof(Heap)) / m_blockSize;
}

CMemoryMgr::~CMemoryMgr()
{
	DeallocateAll();
}

void* CMemoryMgr::Allocate()
{
	// free list empty, create new page
	if (!m_freeBlockList)
	{
		//Create new Heap
		Heap* newHeap = reinterpret_cast<Heap*>(new char[m_heapSize]);
		++m_numOfBlocks;
		m_numOfBlocks += m_blocksPerHeap;
		m_numOfBlocksFree += m_blocksPerHeap;

		OverWriteHeap(newHeap);

		// page list not empty, link new page
		if (m_heapList)
		{
			newHeap->next = m_heapList;
		}

		// push new page
		m_heapList = newHeap;

		// link new free blocks
		Block* currentBlock = newHeap->block();
		for (unsigned i = 0; i < m_blocksPerHeap - 1; ++i)
		{
			currentBlock->next = GetNextBlock(currentBlock);
			currentBlock = GetNextBlock(currentBlock);
		}
		currentBlock->next = nullptr; // last block

								   // push new blocks
		m_freeBlockList = newHeap->block();
	}

	// pop free block
	Block* freeBlock = m_freeBlockList;
	m_freeBlockList = m_freeBlockList->next;
	--m_numOfBlocksFree;
	OverWriteAllocatedBlock(freeBlock);
	return freeBlock;
}

void CMemoryMgr::Deallocate(void* pointerToMemory)
{
	//Get memory block
	Block* block = reinterpret_cast<Block*>(pointerToMemory);

	OverWriteBlock(block);

	//Put memory block back in free list
	block->next = m_freeBlockList;
	m_freeBlockList = block;
	++m_numOfBlocksFree;

}

void CMemoryMgr::DeallocateAll()
{
	Heap* tempIter = m_heapList;
	while (tempIter)
	{
		Heap* currentHeap = tempIter;
		tempIter = tempIter->next;
		delete[] reinterpret_cast<char*>(currentHeap);
	}

	//Reset
	m_numOfHeaps = 0;
	m_numOfBlocks = 0;
	m_numOfBlocksFree = 0;
	m_heapList = nullptr;
	m_freeBlockList = nullptr;
}


void CMemoryMgr::OverWriteHeap(Heap* heapPointer)
{
	heapPointer->next = nullptr;

	Block* currentBlock = heapPointer->block();
	for (unsigned i = 0; i < m_blocksPerHeap; ++i)
	{
		OverWriteBlock(currentBlock);
		currentBlock = GetNextBlock(currentBlock);
	}
}

void CMemoryMgr::OverWriteBlock(Block* blockPointer)
{
	std::memset(blockPointer, FREE_MEMORY_PATTERN, m_blockSize - m_memoryAlignment);

	//For memory alignment
	std::memset(reinterpret_cast<char *>(blockPointer) + m_blockSize - m_memoryAlignment,
				ALIGNMENT_PATTERN,
				m_memoryAlignment);
}

void CMemoryMgr::OverWriteAllocatedBlock(Block* blockPointer)
{
	std::memset(blockPointer,ALLOCATION_PATTERN, m_blockSize - m_memoryAlignment);

	//For memory alignment
	std::memset(reinterpret_cast<char *>(blockPointer) + m_blockSize - m_memoryAlignment,
				ALIGNMENT_PATTERN,
				m_memoryAlignment);
}

Block* CMemoryMgr::GetNextBlock(Block* block)
{
	return  reinterpret_cast<Block*> (reinterpret_cast<char*>(block) + m_blockSize);
}
