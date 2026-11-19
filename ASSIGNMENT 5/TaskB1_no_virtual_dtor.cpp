// B1.4: remove virtual destructor from the real hierarchy; run with -fsanitize=address
#include <iostream>
#include <string>
struct Bill {
    std::string patient{"Ravi with a name long enough to be heap allocated........"};
    ~Bill() {}                                     // NOT virtual
    virtual double total() const = 0;
};
struct IP : Bill {
    std::string ward = std::string(200, 'w');
    double total() const override { return 1; }
};
int main() { Bill* b = new IP; std::cout << b->total() << '\n'; delete b; }
