// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/corelibc/stdlib.h>
#include <openenclave/debugmalloc.h>
#include <stdio.h>

// Include the trusted debugmalloc header that is generated
// during the build. This file is generated by calling the
// sdk tool oeedger8r against the debugmalloc.edl file.
#include "debugmalloc_t.h"

// This is the function that the host calls. It prints
// a message in the enclave before calling back out to
// the host to print a message from there too.
void enclave_hello()
{
    void *buffer1, *buffer2, *buffer3;
    uint64_t count;
    char* report;

    oe_assert(oe_debug_malloc_tracking_start() == OE_OK);
    buffer1 = oe_malloc(4096);
    oe_assert(oe_debug_malloc_tracking_stop() == OE_OK);
    if (oe_debug_malloc_tracking_report(&count, &report) == OE_OK)
    {
        printf("There are %d un-freed objects reported:\n%s\n", count, report);
        oe_free(report);
    }

    oe_assert(oe_debug_malloc_tracking_start() == OE_OK);
    buffer2 = oe_malloc(4096);
    oe_assert(oe_debug_malloc_tracking_stop() == OE_OK);

    oe_assert(oe_debug_malloc_tracking_start() == OE_OK);
    buffer3 = oe_malloc(4096);
    oe_assert(oe_debug_malloc_tracking_stop() == OE_OK);
    if (oe_debug_malloc_tracking_report(&count, &report) == OE_OK)
    {
        printf("There are %d un-freed objects reported:\n%s\n", count, report);
        oe_free(report);
    }

    oe_free(buffer1);
    oe_free(buffer2);
    oe_free(buffer3);

    // Call back into the host
    oe_result_t result = host_hello();
    if (result != OE_OK)
    {
        fprintf(
            stderr,
            "Call to host_hello failed: result=%u (%s)\n",
            result,
            oe_result_str(result));
    }
}
