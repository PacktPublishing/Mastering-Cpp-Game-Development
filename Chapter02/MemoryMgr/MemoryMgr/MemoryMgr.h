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
struct Block
{
	Block* next;
};

struct Heap
{
	Heap* next;

	//Return first block
	Block* block()
	{
		return reinterpret_cast<Block*>(this + 1);
	}
};

// This class is exported from the MemoryMgr.dll
class CMemoryMgr {
public:

	CMemoryMgr(unsigned int dataSize, unsigned int heapSize, unsigned int memoryAlignmentSize);
	~CMemoryMgr();

	void*  Allocate();
	void Deallocate(void* pointerToMemory);
	void DeallocateAll();

private:
	
	static const unsigned char ALLOCATION_PATTERN = 0xBEEF;
	static const unsigned char ALIGNMENT_PATTERN = 0xBADD;
	static const unsigned char FREE_MEMORY_PATTERN = 0xF00D;

	Block* GetNextBlock(Block* block);

	void OverWriteHeap(Heap* heapPointer);
	void OverWriteBlock(Block* blockPointer);
	void OverWriteAllocatedBlock(Block* blockPointer);
	
	Heap* m_heapList = nullptr;
	Block* m_freeBlockList = nullptr;

	unsigned int m_dataSize;
	unsigned int m_heapSize;
	unsigned int m_memoryAlignment;
	unsigned int m_blockSize;
	unsigned int m_blocksPerHeap;

	unsigned int m_numOfHeaps;
	unsigned int m_numOfBlocks;
	unsigned int m_numOfBlocksFree;
};


