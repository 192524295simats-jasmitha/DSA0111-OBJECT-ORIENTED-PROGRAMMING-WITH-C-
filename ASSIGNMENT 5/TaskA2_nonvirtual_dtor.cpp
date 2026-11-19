#include <iostream>
#include <string>
struct Bill {
    ~Bill() { std::cout << "~Bill "; }          // NOT virtual
    virtual double total() const = 0;
    void show() const { std::cout << total() << ' '; }
};
struct OPBill : Bill {
    std::string note = std::string(100, 'x');   // heap memory so the leak is visible
    ~OPBill() { std::cout << "~OPBill "; }
    double total() const override { return 500; }
};
int main()
{
    Bill* b = new OPBill;
    b->show();
    delete b;                                   // UB: only ~Bill runs
    std::cout << '\n';
}
