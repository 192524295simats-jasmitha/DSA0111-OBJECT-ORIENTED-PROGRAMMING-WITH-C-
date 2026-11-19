// Task 4.3 - repaired program
#include <iostream>
#include <memory>
#include <vector>
class Account {
public:
    virtual double rate() const { return 3.5; }
    virtual ~Account() { std::cout << "~Account\n"; }
};
class SeniorAccount : public Account {
    std::vector<double> history = std::vector<double>(100);   // RAII, no manual delete
public:
    double rate() const override { return 4.0; }
    ~SeniorAccount() override { std::cout << "~SeniorAccount\n"; }
};
int main() {
    std::vector<std::unique_ptr<Account>> accounts;            // pointers, never copies
    accounts.push_back(std::make_unique<Account>());
    accounts.push_back(std::make_unique<SeniorAccount>());
    for (const auto& a : accounts) std::cout << a->rate() << "\n";
}   // unique_ptr deletes through the virtual destructor
