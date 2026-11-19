// B3: virtual vs switch vs variant vs dynamic_cast; return codes vs exceptions vs optional
// Build: g++ -std=c++17 -O2 bench_B3.cpp -o bench
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <optional>
#include <random>
#include <stdexcept>
#include <variant>
#include <vector>
#include <iomanip>

using Clock = std::chrono::steady_clock;
template <class F> double ms(F f) { auto t = Clock::now(); f(); return std::chrono::duration<double, std::milli>(Clock::now() - t).count(); }
template <class F> double bestOf(int reps, F f) { double b = 1e18; for (int i = 0; i < reps; ++i) b = std::min(b, ms(f)); return b; }
volatile double sink;

// ---------- (a) virtual hierarchy (lightweight: no std::string, so 10M objects fit in memory) ----------
struct VBill { virtual ~VBill() = default; virtual double total() const = 0; };
struct VOP : VBill { double c, t; VOP(double a, double b) : c(a), t(b) {} double total() const override { return c + t; } };
struct VIP : VBill { int d; double r, s; VIP(int a, double b, double c2) : d(a), r(b), s(c2) {} double total() const override { return d * r + s; } };
struct VDC : VBill { double p; explicit VDC(double a) : p(a) {} double total() const override { return p; } };
struct VER : VBill { double b, h, r; VER(double x, double y, double z) : b(x), h(y), r(z) {} double total() const override { return b + h * r; } };

// ---------- (b) one struct + type field + switch ----------
enum Kind : int { OP, IP, DC, ER };
struct SBill { Kind k; double a, b, c; };
inline double sTotal(const SBill& s) {
    switch (s.k) { case OP: return s.a + s.b; case IP: return s.a * s.b + s.c;
                   case DC: return s.a;       case ER: return s.a + s.b * s.c; }
    return 0;
}
// ---------- (c) std::variant ----------
struct XOP { double c, t;       double total() const { return c + t; } };
struct XIP { int d; double r, s; double total() const { return d * r + s; } };
struct XDC { double p;          double total() const { return p; } };
struct XER { double b, h, r;    double total() const { return b + h * r; } };
using VarBill = std::variant<XOP, XIP, XDC, XER>;

struct Spec { int k; double a, b, c; };
std::vector<Spec> makeSpecs(std::size_t n) {
    std::mt19937 g(12345); std::uniform_int_distribution<int> k(0, 3); std::uniform_real_distribution<double> v(1, 1000);
    std::vector<Spec> s(n); for (auto& e : s) e = {k(g), v(g), v(g), v(g)}; return s;
}

void benchPolymorphism()
{
    std::cout << "\n=== B3.1 correctness on 10 bills ===\n";
    auto sp = makeSpecs(10);
    {
        std::vector<std::unique_ptr<VBill>> v; std::vector<SBill> s; double tv = 0, ts = 0, tx = 0;
        for (auto& e : sp) {
            switch (e.k) {
                case 0: v.push_back(std::make_unique<VOP>(e.a, e.b)); break;
                case 1: v.push_back(std::make_unique<VIP>((int)e.a, e.b, e.c)); break;
                case 2: v.push_back(std::make_unique<VDC>(e.a)); break;
                default: v.push_back(std::make_unique<VER>(e.a, e.b, e.c)); }
            s.push_back({(Kind)e.k, e.k == 1 ? (double)(int)e.a : e.a, e.b, e.c});
        }
        std::vector<VarBill> xv;
        for (auto& e : sp) switch (e.k) { case 0: xv.emplace_back(XOP{e.a, e.b}); break; case 1: xv.emplace_back(XIP{(int)e.a, e.b, e.c}); break;
                                           case 2: xv.emplace_back(XDC{e.a}); break; default: xv.emplace_back(XER{e.a, e.b, e.c}); }
        for (auto& p : v) tv += p->total();
        for (auto& b : s) ts += sTotal(b);
        for (auto& b : xv) tx += std::visit([](const auto& x) { return x.total(); }, b);
        std::cout << std::fixed << std::setprecision(4) << "virtual=" << tv << " switch=" << ts << " variant=" << tx
                  << ((std::fabs(tv - ts) < 1e-6 && std::fabs(tv - tx) < 1e-6) ? "  -> AGREE\n" : "  -> MISMATCH\n");
    }
    std::cout << "\n=== B3.2/alt: time to total n bills (ms, best of reps, -O2) ===\n"
              << std::left << std::setw(12) << "n" << std::setw(12) << "virtual" << std::setw(12) << "switch"
              << std::setw(12) << "variant" << std::setw(14) << "dynamic_cast" << "\n";
    for (std::size_t n : {1000ul, 1000000ul, 10000000ul}) {
        auto specs = makeSpecs(n);
        std::vector<std::unique_ptr<VBill>> v; v.reserve(n);
        std::vector<SBill> s; s.reserve(n); std::vector<VarBill> x; x.reserve(n);
        for (auto& e : specs) {
            switch (e.k) {
                case 0: v.push_back(std::make_unique<VOP>(e.a, e.b)); break;
                case 1: v.push_back(std::make_unique<VIP>((int)e.a, e.b, e.c)); break;
                case 2: v.push_back(std::make_unique<VDC>(e.a)); break;
                default: v.push_back(std::make_unique<VER>(e.a, e.b, e.c)); }
            s.push_back({(Kind)e.k, e.a, e.b, e.c});
            switch (e.k) { case 0: x.emplace_back(XOP{e.a, e.b}); break; case 1: x.emplace_back(XIP{(int)e.a, e.b, e.c}); break;
                           case 2: x.emplace_back(XDC{e.a}); break; default: x.emplace_back(XER{e.a, e.b, e.c}); }
        }
        specs.clear(); specs.shrink_to_fit();
        int reps = n >= 10000000 ? 3 : (n >= 1000000 ? 5 : 2000);
        double tv = bestOf(reps, [&] { double t = 0; for (auto& p : v) t += p->total(); sink = t; });
        double ts = bestOf(reps, [&] { double t = 0; for (auto& b : s) t += sTotal(b); sink = t; });
        double tx = bestOf(reps, [&] { double t = 0; for (auto& b : x) t += std::visit([](const auto& q) { return q.total(); }, b); sink = t; });
        double td = bestOf(reps, [&] { double t = 0; for (auto& p : v) {
                    if (auto* a = dynamic_cast<VOP*>(p.get())) t += a->c + a->t;
                    else if (auto* b = dynamic_cast<VIP*>(p.get())) t += b->d * b->r + b->s;
                    else if (auto* c = dynamic_cast<VDC*>(p.get())) t += c->p;
                    else if (auto* d = dynamic_cast<VER*>(p.get())) t += d->b + d->h * d->r; } sink = t; });
        std::cout << std::left << std::setw(12) << n << std::fixed << std::setprecision(4) << std::setw(12) << tv << std::setw(12) << ts
                  << std::setw(12) << tx << std::setw(14) << td << "\n";
    }
    std::cout << "\nsizeof: VBill-derived max=" << sizeof(VIP) << " (incl. 8-byte vptr)   SBill=" << sizeof(SBill)
              << "   variant=" << sizeof(VarBill) << "   XIP=" << sizeof(XIP) << "\n";
}

// ---------- B3.3 error reporting cost ----------
enum class Status { Ok, Negative, TooLarge };
struct ValErr : std::runtime_error { using std::runtime_error::runtime_error; };
Status validateRC(double a) { if (a < 0) return Status::Negative; if (a > 1e6) return Status::TooLarge; return Status::Ok; }
void validateEX(double a) { if (a < 0) throw ValErr("negative"); if (a > 1e6) throw ValErr("too large"); }
std::optional<double> validateOpt(double a) { if (a < 0 || a > 1e6) return std::nullopt; return a; }

void benchErrors()
{
    std::cout << "\n=== B3.3 validating 1,000,000 bills (ms, best of 3, -O2) ===\n"
              << std::left << std::setw(10) << "invalid" << std::setw(14) << "return-code" << std::setw(14) << "exception" << std::setw(14) << "optional" << "\n";
    const std::size_t N = 1000000;
    for (int pct : {0, 1, 50}) {
        std::mt19937 g(7); std::uniform_int_distribution<int> d(0, 99);
        std::vector<double> data(N); for (auto& x : data) x = d(g) < pct ? -1.0 : 100.0;
        std::size_t bad1 = 0, bad2 = 0, bad3 = 0;
        double t1 = bestOf(3, [&] { bad1 = 0; for (double x : data) if (validateRC(x) != Status::Ok) ++bad1; });
        double t2 = bestOf(3, [&] { bad2 = 0; for (double x : data) { try { validateEX(x); } catch (const ValErr&) { ++bad2; } } });
        double t3 = bestOf(3, [&] { bad3 = 0; for (double x : data) if (!validateOpt(x)) ++bad3; });
        sink = (double)(bad1 + bad2 + bad3);
        std::cout << std::left << std::setw(10) << (std::to_string(pct) + "%") << std::fixed << std::setprecision(3)
                  << std::setw(14) << t1 << std::setw(14) << t2 << std::setw(14) << t3
                  << " (rejected " << bad1 << "/" << bad2 << "/" << bad3 << ")\n";
    }
}
int main() { benchPolymorphism(); benchErrors(); }
