Compile any file:  g++ -std=c++17 -Wall -Wextra TaskXX_name.cpp -o prog
ASan:  add -fsanitize=address -g   |  Benchmark (TaskB3_bench): add -O2
TaskB2_error_tests with ASan: add -DSKIP_BAD_ALLOC
Keep billing.hpp and billing_tests.hpp in the same folder (used by TaskB1_main, TaskB2_error_tests, TaskB4_extension).
TaskB1_error_remove_virtual.cpp is MEANT to fail to compile (compile-error demo).
