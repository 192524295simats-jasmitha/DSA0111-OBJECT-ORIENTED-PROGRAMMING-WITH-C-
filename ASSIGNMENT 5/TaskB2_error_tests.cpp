// B2: deliberately engineered error conditions
#include "billing.hpp"
#include <new>
#include <cstdlib>

using namespace hosp;

struct Tracer {                                   // proves destructors run during unwinding
    std::string n;
    explicit Tracer(std::string s) : n(std::move(s)) { std::cout << "  +" << n << '\n'; }
    ~Tracer() { std::cout << "  ~" << n << '\n'; }
};

std::map<std::string, int> patientRecords{{"Ravi", 1}, {"Meena", 2}};
int lookup(const std::string& name)
{
    auto it = patientRecords.find(name);
    if (it == patientRecords.end()) throw MissingPatientError("no record for " + name);
    return it->second;
}

template <class F> void test(const char* title, F f)
{
    std::cout << "[" << title << "]\n";
    Tracer t(std::string("scope-of-") + title);
    try { f(); std::cout << "  no error\n"; }
    catch (const InvalidChargeError& e)   { std::cout << "  InvalidChargeError : " << e.what() << '\n'; }
    catch (const ClaimError& e)           { std::cout << "  ClaimError         : " << e.what() << '\n'; }
    catch (const UnknownPackageError& e)  { std::cout << "  UnknownPackageError: " << e.what() << '\n'; }
    catch (const MissingPatientError& e)  { std::cout << "  MissingPatientError: " << e.what() << '\n'; }
    catch (const BillingError& e)         { std::cout << "  BillingError       : " << e.what() << '\n'; }
    catch (const std::invalid_argument& e){ std::cout << "  std::invalid_argument: " << e.what() << '\n'; }
    catch (const std::bad_alloc& e)       { std::cout << "  std::bad_alloc     : " << e.what() << '\n'; }
}

int main()
{
    test("negative-charge",  [] { OutPatientBill b("X", -500, 100); (void)b; });
    test("claim-above-cover",[] { OutPatientBill b("X", 60000, 1000); Insurance i(50000, 20); i.amountCovered(b); });
    test("unknown-package",  [] { DayCareBill b("X", 100); GovernmentScheme g({{"OP", 1000}}); g.amountCovered(b); });
    test("missing-patient",  [] { lookup("Zoya"); });
    test("text-not-number",  [] { (void)std::stod("abc"); });                    // std::invalid_argument
#ifndef SKIP_BAD_ALLOC   // ASan aborts on impossible allocations, so the ASan build defines this
    test("bad_alloc",        [] { volatile std::size_t n = (std::size_t)1 << 50; double* p = new double[n]; delete[] p; });
#endif

    // Top level catch(...) : program keeps running, all objects destroyed
    std::cout << "[top-level catch(...)]\n";
    try {
        Tracer a("A"); Tracer b("B");
        throw 3.14;                              // a type nobody handles specifically
    } catch (...) {
        std::cout << "  caught by catch(...), program continues\n";
    }
    std::cout << "program still running\n";
}
