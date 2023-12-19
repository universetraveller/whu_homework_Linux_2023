#include "umalloc.c"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

// Test function
void run_tests() {
    // Test Case 1: Allocate and free memory
    printf("Test Case 1: Allocate and free memory\n");
    void* ptr1 = umalloc(100);
    assert(ptr1 != NULL);  // Allocation successful

    ufree(ptr1);
    printf("Test Case 1 passed\n\n");

    // Test Case 2: Allocate multiple blocks
    printf("Test Case 2: Allocate multiple blocks\n");
    void* ptr2 = umalloc(50);
    void* ptr3 = umalloc(200);

    assert(ptr2 != NULL && ptr3 != NULL);  // Allocation successful

    ufree(ptr2);
    ufree(ptr3);
    printf("Test Case 2 passed\n\n");

    // Test Case 3: Allocate more than available memory
    printf("Test Case 3: Allocate more than available memory\n");
    void* ptr4 = umalloc(SIZE_MAX);  // SIZE_MAX is the maximum value representable by size_t
    assert(ptr4 == NULL);  // Allocation should fail

    printf("Test Case 3 passed\n\n");

    // Test Case 4: Allocate and free in a loop
    printf("Test Case 4: Allocate and free in a loop\n");
    for (int i = 0; i < 1000; i++) {
        void* ptr = umalloc(10);
        assert(ptr != NULL);

        ufree(ptr);
    }
    printf("Test Case 4 passed\n\n");

    printf("Test Case 5: Reallocation test\n");

    void* ptr5 = umalloc(7500);
    void* ptr6 = umalloc(500);
    ufree(ptr5);
    ufree(ptr6);
    ptr5 = umalloc(8000);

    printf("Test Case 5 passed\n\n");

    printf("All tests passed!\n");
}

int main() {
    run_tests();
    return 0;
}

