// Tasks 5.1, 5.2 + vector invalidation demo
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
class Cart {
    std::string items[10]; double prices[10]; int n = 0; double discount = 0; std::string owner;
public:
    explicit Cart(std::string owner = "") : owner(std::move(owner)) {}
    Cart& add(const std::string& item, double price) {
        if (n < 10) { items[n] = item; prices[n] = price; ++n; }
        return *this;
    }
    Cart& setDiscount(double discount) { this->discount = discount; return *this; }
    Cart& setOwner(const std::string& owner) { this->owner = owner; return *this; }  // member vs parameter
    double total() const {
        double t = 0; for (int i = 0; i < n; ++i) t += prices[i];
        return t * (100 - discount) / 100;
    }
    const std::string& getOwner() const { return owner; }
    const Cart* self() const { return this; }
};
void viaRef(Cart& r) { r.add("Milk", 50); }
int main() {
    std::cout << std::fixed << std::setprecision(2);
    Cart a("Asha"); a.add("Rice", 60).add("Dal", 120).setDiscount(10);   // (a) by name + chaining
    Cart* p = &a; p->add("Oil", 180);                                    // (b) pointer
    viaRef(a);                                                           // (c) reference
    Cart* h = new Cart("Heap"); h->add("Tea", 100).add("Sugar", 45);     // (d) new / delete
    std::cout << "a total = " << a.total() << "  (this == &a ? " << (a.self() == &a) << ")\n";
    std::cout << "heap total = " << h->total() << "\n";

    // Task 5.2: array of pointers, find highest total
    Cart* carts[4] = { &a, h, new Cart("C3"), new Cart("C4") };
    carts[2]->add("TV", 900); carts[3]->add("Soap", 30);
    Cart* best = carts[0];
    for (int i = 1; i < 4; ++i) if (carts[i]->total() > best->total()) best = carts[i];
    std::cout << "Highest: " << best->getOwner() << " = " << best->total() << "\n";
    Cart** q = carts; Cart* best2 = *q;                                   // pointer-arithmetic version
    for (Cart** e = carts + 4; q != e; ++q) if ((*q)->total() > best2->total()) best2 = *q;
    std::cout << "Highest (ptr arithmetic): " << best2->getOwner() << "\n";
    delete h; delete carts[2]; delete carts[3];

    // alternative: vector<Cart> - pointers invalidated on growth
    std::vector<Cart> v; v.reserve(1); v.emplace_back("V1");
    Cart* first = &v[0]; v.emplace_back("V2");
    std::cout << "pointer into vector still valid after growth? " << (first == &v[0] ? "yes" : "NO (dangling)") << "\n";
}
