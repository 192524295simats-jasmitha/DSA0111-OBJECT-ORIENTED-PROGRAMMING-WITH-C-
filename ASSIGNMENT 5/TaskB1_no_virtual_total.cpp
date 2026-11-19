// B1.3: total() is NOT virtual (hiding instead of overriding) -> base-pointer calls use Bill::total
#include <iostream>
#include <memory>
#include <vector>
struct Bill {
    virtual ~Bill() = default;
    double total() const { return 0; }                // not virtual
    virtual double virt() const { return 0; }          // for contrast
};
struct OP : Bill {
    double total() const { return 1700; }
    double virt() const override { return 1700; }
};
int main()
{
    std::vector<std::unique_ptr<Bill>> v; v.push_back(std::make_unique<OP>());
    std::cout << "via Bill* non-virtual total(): " << v[0]->total() << '\n';
    std::cout << "via Bill* virtual virt()      : " << v[0]->virt() << '\n';
    OP op; std::cout << "via OP object total()         : " << op.total() << '\n';
}
