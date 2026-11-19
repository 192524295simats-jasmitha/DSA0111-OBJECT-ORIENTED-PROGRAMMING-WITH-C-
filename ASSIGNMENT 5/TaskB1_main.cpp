#include "billing_tests.hpp"
int main()
{
    int f = hosp::runCoreTests();
    std::cout << (f == 0 ? "ALL CORE TESTS PASSED\n" : "SOME TESTS FAILED\n");
    return f;
}
