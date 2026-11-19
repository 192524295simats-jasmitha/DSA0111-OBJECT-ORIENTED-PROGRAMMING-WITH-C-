// switch vs virtual measurement + sizeof
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>
struct VB { virtual ~VB() = default; virtual double charge(int d) const = 0; };
struct VA : VB { double r; VA(double r) : r(r) {} double charge(int d) const override { return d * r; } };
struct VC : VB { double r; VC(double r) : r(r) {} double charge(int d) const override { return d * r * 1.1; } };
struct SW { int t; double r, fuel = 0, kwh = 0, bat = 0; int seats = 0;
    double charge(int d) const { switch (t) { case 0: return d * r; default: return d * r * 1.1; } } };
int main() {
    const int N = 2000000; std::vector<std::unique_ptr<VB>> a; std::vector<SW> b;
    for (int i = 0; i < N; ++i) { if (i % 2) a.push_back(std::make_unique<VA>(10)); else a.push_back(std::make_unique<VC>(10)); b.push_back({i % 2, 10}); }
    auto t0 = std::chrono::steady_clock::now(); double s1 = 0; for (auto& x : a) s1 += x->charge(3);
    auto t1 = std::chrono::steady_clock::now(); double s2 = 0; for (auto& x : b) s2 += x.charge(3);
    auto t2 = std::chrono::steady_clock::now();
    auto ms = [](auto d) { return std::chrono::duration<double, std::milli>(d).count(); };
    std::cout << "virtual: " << ms(t1 - t0) << " ms  switch: " << ms(t2 - t1) << " ms  (sums " << s1 << "," << s2 << ")\n";
    std::cout << "sizeof(VA)=" << sizeof(VA) << " sizeof(SW)=" << sizeof(SW) << "\n";
}
