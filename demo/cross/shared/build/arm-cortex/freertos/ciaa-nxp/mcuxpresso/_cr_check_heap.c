/*
 * _cr_check_heap.c
 *
 * Copyright 2010-2014, 2018-2019 NXP
 *
 * The function __check_heap_overflow() is called from _sbrk()
 * to provide a check for the heap growing too large.
 *
 * The implementation provided below copes with two different
 * models of heap location in memory.
 *
 * The standard check allows for the application specifying the
 * end of the heap via the "_pvHeapLimit" symbol (which would
 * normally be set up, if required, in the linker script).
 *
 * Thus it first of all confirms whether "_pvHeapLimit" is
 * defined, and if it is checks whether the heap would now
 * overflow this limit.

 * Secondly, there is an optional check enabled only if the
 * the symbol "ONE_REGION_MODEL_HEAPCHECK" is defined. This
 * allows for a simple "1 region memory model" in which the
 * heap grows up from the bottom of a block of memory and
 * the stack grows down from the top of the same block.
 *
 * In checking for this, it first of all confirms that the
 * base of the stack sits below the heap, and if so it checks
 * the end of heap against the current stack pointer. This
 * hence avoids errors in the case where the heap has been
 * located entirely above the stack.
 *
 * This 1 region check is disabled by default though as
 * the assumptions it makes about memory layout are quite
 * simplistic, and can 'broken' in many real applications,
 * for example where an RTOS is being used which provides
 * a separate stack for each task.
 */

#ifdef ONE_REGION_MODEL_HEAPCHECK
register void * stack_ptr asm ("sp");
extern char _pvHeapStart; // Pointer to start of heap (from linker script)
#endif

// Optional pointer to end of heap (from linker script)
extern char _pvHeapLimit __attribute__((weak));

unsigned __check_heap_overflow (void * new_end_of_heap) {

	// Check to see if a HeapLimit is defined
	if (&_pvHeapLimit)
	{
		// If so, then check to see if have hit the limit
		if (new_end_of_heap >= (void *)&_pvHeapLimit)
		{
			return 1; // Heap has overflowed
		}
	}

#ifdef ONE_REGION_MODEL_HEAPCHECK
	// 1 region model - check that heap is below stack
	else if ((void *)&_pvHeapStart < stack_ptr)
	{
		// If so, then check to see if end of heap
		// would overflow into stack
		if (new_end_of_heap >= stack_ptr)
		{
			return 1; // Heap has overflowed
		}
	}
#endif

	return 0; // Heap still OK
}
