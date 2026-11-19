#include <cassert>
#include <iomanip>
#include <memory>
#include <vector>
#include "Vehicle.h"
#include "ElectricBus.h"
class Fleet {
    std::vector<std::unique_ptr<Vehicle>> v;
    Vehicle* find(const std::string& r) { for (auto& x : v) if (x->reg() == r) return x.get(); return nullptr; }
public:
    void add(std::unique_ptr<Vehicle> p) { v.push_back(std::move(p)); }
    bool rent(const std::string& r) { Vehicle* p = find(r); if (!p || p->isRented()) return false; p->setRented(true); return true; }
    double giveBack(const std::string& r, int days, int late, double used) {
        Vehicle* p = find(r); if (!p || !p->isRented()) return -1;
        p->setRented(false); return p->charge(days, late, used);
    }
    void list() const { for (auto& x : v) x->describe(); }
};
int main() {
    std::cout << std::fixed << std::setprecision(2);
    Fleet f; Date due{2027, 3, 1};
    f.add(std::make_unique<Bicycle>("BI-01", 100));
    f.add(std::make_unique<TwoWheeler>("TW-01", 300, 8, due));
    f.add(std::make_unique<Car>("CA-01", 1500, 30, due, 5));
    f.add(std::make_unique<ElectricCar>("EC-01", 2000, 80, due, 5, 60));
    f.list();
    // tests
    assert(f.rent("BI-01")); assert(!f.rent("BI-01")); assert(!f.rent("XX-99"));
    assert(f.giveBack("BI-01", 2, 0, 0) == 200.0);                 // no late
    assert(f.giveBack("BI-01", 2, 0, 0) == -1);                    // not rented
    f.rent("TW-01"); std::cout << "TwoWheeler 2d, 1 late, 3L : " << f.giveBack("TW-01", 2, 1, 3) << "\n";
    f.rent("CA-01"); std::cout << "Car 3d, 2 late, 10L        : " << f.giveBack("CA-01", 3, 2, 10) << "\n";
    f.rent("EC-01"); std::cout << "ElectricCar 1d, 1 late, 20kWh: " << f.giveBack("EC-01", 1, 1, 20) << "\n";
    f.add(std::make_unique<ElectricBus>("EB-01", 5000, 100, due, 40));   // extension
    f.rent("EB-01"); std::cout << "ElectricBus 2d, 1 late, 50kWh: " << f.giveBack("EB-01", 2, 1, 50) << "\n";
    f.list();
    std::cout << "All tests passed\n";
}
