#include "MemoryHeap.h"
#include "KernelMemory.h"

MemoryHeap_t kCreateHeap(uint64_t base, uint64_t size)
{
    // Initialize the heap
    MemoryHeap_t heap;
    heap.FirstSegment       = (MemorySegmentHeader_t*)base;
    heap.FirstFreeSegment   = (MemorySegmentHeader_t*)base;
    heap.Size               = size;

    // Initialize the first free segment
    heap.FirstFreeSegment->Size = size - sizeof(MemorySegmentHeader_t);
    heap.FirstFreeSegment->NextSegment = 0;
    heap.FirstFreeSegment->NextFreeSegment = 0;
    heap.FirstFreeSegment->PrevSegment = 0;
    heap.FirstFreeSegment->PrevFreeSegment = 0;
    heap.FirstFreeSegment->Free = TRUE;

    return heap;
}

// =========================================== //
//  Example Allocation of 0x100 bytes
//  at address 0x20000
//
//     -0x30-   ------0x100------
//  || ------ | ----------------- ||
//  || Header |   Usable memory   ||
//  || ------ | ----------------- ||
//
//  Result:
//      Usable memory starts at :  0x20030
//           Next segment is at :  0x20130
// ===========================================
void* kMemoryAllocWithAlignment(MemoryHeap_t* heap, uint64_t size, uint64_t alignment)
{
    // Calculate the remainder
    uint64_t remainder = size % alignment;
    size -= remainder;

    // Align the memory block if the remainder is not 0
    if (remainder > 0) size += alignment;

    // Iterate over every memory segment in the linked list
    // and attempt to find an empty one, otherwise return NULL.
    MemorySegmentHeader_t* current_segment = heap->FirstFreeSegment;

    while (TRUE)
    {
        // Check if the current segment is at least the required size
        if (current_segment->Size >= size)
        {
            // Check if there is space for another segment
            // after the current segment header and usable memory.
            if (current_segment->Size > size + sizeof(MemorySegmentHeader_t))
            {
                // If there is space left, create a new segment header to fill it
                MemorySegmentHeader_t* new_segment = (MemorySegmentHeader_t*)((uint64_t)current_segment + sizeof(MemorySegmentHeader_t) + size);
                new_segment->Free                   = TRUE;
                new_segment->Size                   = current_segment->Size - (sizeof(MemorySegmentHeader_t) + size);
                new_segment->NextSegment            = current_segment->NextSegment;
                new_segment->NextFreeSegment        = current_segment->NextFreeSegment;
                new_segment->PrevSegment            = current_segment;
                new_segment->PrevFreeSegment        = current_segment->PrevFreeSegment;

                current_segment->NextFreeSegment    = new_segment;
                current_segment->NextSegment        = new_segment;
                current_segment->Size               = size;
            }

            // If the current segment is also the heap's first free segment,
            // since now it's no longer free, update the heap's first free
            // segment to point to the next available free segment.
            if (current_segment == heap->FirstFreeSegment)
                heap->FirstFreeSegment = current_segment->NextFreeSegment;

            // Mark the current segment as no longer free
            current_segment->Free = FALSE;

            // If there was a previous free segment before this one,
            // then update its NextFreeSegment pointer to point to the
            // next available free segment.
            if (current_segment->PrevFreeSegment != NULL)
                current_segment->PrevFreeSegment->NextFreeSegment = current_segment->NextFreeSegment;

            // If there was a next free segment after this one,
            // then update its PrevFreeSegment pointer to point to the
            // next available free segment.
            if (current_segment->NextFreeSegment != NULL)
                current_segment->NextFreeSegment->PrevFreeSegment = current_segment->PrevFreeSegment;

            // Do the same check as above for the previous
            // segment and update its next free segment pointer.
            if (current_segment->PrevSegment != NULL)
                current_segment->PrevSegment->NextFreeSegment = current_segment->NextFreeSegment;

            // Do the same check as above for the next segment
            // and update its previous free segment pointer.
            if (current_segment->NextSegment != NULL);
                current_segment->NextSegment->PrevFreeSegment = current_segment->PrevFreeSegment;

            // Return the next available memory segment
            return current_segment + 1; // the compiler automatically adds sizeof(MemorySegmentHeader_t) bytes
        }

        // Special case when there is no free segments left to give out
        if (current_segment->NextFreeSegment == NULL)
        {
            return 0; // No free memory remaining
        }

        // Iterate to the next available free segment
        current_segment = current_segment->NextFreeSegment;
    }

    return NULL;
}

void* kMemoryAlloc(MemoryHeap_t* heap, uint64_t size)
{
    return kMemoryAllocWithAlignment(heap, size, 8);
}

void* kClearMemoryAllocWithAlignment(MemoryHeap_t* heap, uint64_t size, uint64_t alignment)
{
    // Calculate the padding
    uint64_t remainder = size % alignment;
    size -= remainder;

    // Align the memory block if the remainder is not 0
    if (remainder > 0) size += alignment;

    void* ptr = kMemoryAllocWithAlignment(heap, size, alignment);
    kSetMemory(ptr, 0, size);

    return ptr;
}

void* kClearMemoryAlloc(MemoryHeap_t* heap, uint64_t size)
{
    return kClearMemoryAllocWithAlignment(heap, size, 8);
}

// Combines the two adjacent free blocks into one to avoid fragmentation.
void _kCombineFreeSegments(MemorySegmentHeader_t* left, MemorySegmentHeader_t* right)
{
    // Check to make sure both pointers are valid
    if (left == NULL || right == NULL)
        return;

    if (left < right)
    {
        // Increment the left block's size to consume the right block
        left->Size += right->Size + sizeof(MemorySegmentHeader_t);

        // Adjust the next segment pointers
        left->NextSegment       = right->NextSegment;
        left->NextFreeSegment   = right->NextFreeSegment;

        // Fix the next segment's pointers to point to the
        // first (left) block rather than the second one (right).
        right->NextSegment->PrevSegment         = left;
        right->NextSegment->PrevFreeSegment     = left;
        right->NextFreeSegment->PrevFreeSegment = left;
    }
    else
    {
        // Follow the same procedure as the above if block
        // but reverse left and right as the right block's
        // address is lower than the left one's.

        right->Size += left->Size + sizeof(MemorySegmentHeader_t);
        right->NextSegment       = left->NextSegment;
        right->NextFreeSegment   = left->NextFreeSegment;

        left->NextSegment->PrevSegment          = right;
        left->NextSegment->PrevFreeSegment      = right;
        left->NextFreeSegment->PrevFreeSegment  = right;
    }
}

void kFreeMemory(MemoryHeap_t* heap, void* ptr)
{
    // Get the memory segment header address
    // of the block we are trying to free.
    MemorySegmentHeader_t* current_segment = (MemorySegmentHeader_t*)((uint64_t)ptr - sizeof(MemorySegmentHeader_t));

    // Mark the current segment as free
    current_segment->Free;

    // If the heap's first free segment has a higher address than the
    // current memory segment, then replace it with the current segment.
    if (current_segment < heap->FirstFreeSegment)
        heap->FirstFreeSegment = current_segment;

    // If the next free memory segment exists, then check
    // if its previous free memory segment's address is lower than
    // the current segment, and if so, adjust it to point to the current segment.
    if (current_segment->NextFreeSegment != NULL)
        if (current_segment->NextFreeSegment->PrevFreeSegment < current_segment)
            current_segment->NextFreeSegment->PrevFreeSegment = current_segment;

    // If the previous free memory segment exists, then check
    // if its next free memory segment's address is higher than
    // the current segment, and if so, adjust it to point to the current segment.
    if (current_segment->PrevFreeSegment != NULL)
        if (current_segment->PrevFreeSegment->NextFreeSegment > current_segment)
            current_segment->PrevFreeSegment->NextFreeSegment = current_segment;

    // If the next segment exists, adjust its previous
    // segment pointer to point to the current segment.
    if (current_segment->NextSegment != NULL)
    {
        current_segment->NextSegment->PrevSegment = current_segment;

        // To avoid fragmentation, if the next memory
        // block is free, combine it with the current segment.
        if (current_segment->NextSegment->Free)
            _kCombineFreeSegments(current_segment, current_segment->NextSegment);
    }

    // If the previous segment exists, adjust its next
    // segment pointer to point to the current segment.
    if (current_segment->PrevSegment != NULL)
    {
        current_segment->PrevSegment->NextSegment = current_segment;

        // To avoid fragmentation, if the previous memory
        // block is free, combine it with the current segment.
        if (current_segment->PrevSegment->Free)
            _kCombineFreeSegments(current_segment, current_segment->PrevSegment);
    }
}
