#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmockery/cmockery.h>

extern void test_simple(void** state);

int main(void) {
    const UnitTest tests[] = {
        unit_test(test_simple),
    };
    return run_tests(tests);
}
