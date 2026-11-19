// "throw e;" copies the object using e's STATIC type (slicing); "throw;" keeps the original.
#include <iostream>
#include <stdexcept>
struct ClaimError : std::runtime_error { using std::runtime_error::runtime_error; };

void viaThrow()  { try { throw ClaimError("exceeds cover"); }
                   catch (const std::runtime_error&) { throw; } }
void viaThrowE() { try { throw ClaimError("exceeds cover"); }
                   catch (const std::runtime_error& e) { throw e; } }  // sliced copy

template <class F> void run(const char* name, F f)
{
    try { f(); }
    catch (const ClaimError&)         { std::cout << name << ": caught ClaimError\n"; }
    catch (const std::runtime_error&) { std::cout << name << ": caught runtime_error (type LOST)\n"; }
}
int main()
{
    run("throw;   ", viaThrow);
    run("throw e; ", viaThrowE);
}
