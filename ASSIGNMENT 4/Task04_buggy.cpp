// Task 4.1/4.2 - original defects, instrumented
#include <iostream>
#include <vector>
class Account { public: double rate() const { return 3.5; } ~Account() { std::cout << "~Account\n"; } };
class SeniorAccount : public Account {
    double* history = new double[100];
public:
    double rate() const { return 4.0; }
    ~SeniorAccount() { std::cout << "~SeniorAccount\n"; delete[] history; } };
int main() {
    Account* accounts[2] = { new Account, new SeniorAccount };
    for (Account* a : accounts) std::cout << a->rate() << "\n";   // defect 1: always 3.5
    for (Account* a : accounts) delete a;                          // defect 2: ~SeniorAccount never runs
    Account copy = SeniorAccount();                                // defect 3: slicing
    std::cout << "copy.rate()=" << copy.rate() << "\n";
}
