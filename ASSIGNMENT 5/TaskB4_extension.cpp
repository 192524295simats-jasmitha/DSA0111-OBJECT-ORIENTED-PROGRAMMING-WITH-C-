// B4.1: a NEW bill type and a NEW payer, added WITHOUT editing any existing class or header.
#include "billing_tests.hpp"
using namespace hosp;

class DialysisBill : public Bill {                      // new bill type
    int sessions; double perSession;
public:
    DialysisBill(std::string p, int s, double r) : Bill(std::move(p)), sessions(s), perSession(r)
    { if (s < 0) throw InvalidChargeError("negative sessions"); requireNonNegative(r, "session rate"); }
    double total() const override { return sessions * perSession; }
    std::string type() const override { return "DL"; }
};

class CorporateTieUp : public Payer {                   // new payer: pays 100% up to a per-bill cap
    double cap;
public:
    explicit CorporateTieUp(double c) : cap(c) {}
    double amountCovered(const Bill& b) const override { return std::min(cap, b.total()); }
    std::string name() const override { return "Corporate"; }
};

int main()
{
    std::cout << "--- earlier (regression) tests ---\n";
    int fails = runCoreTests(false);
    std::cout << (fails == 0 ? "all earlier tests still PASS\n" : "REGRESSION!\n");

    std::cout << "--- new bill + new payer ---\n";
    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<DialysisBill>("Lata", 4, 1800));          // 7200
    bills.push_back(std::make_unique<OutPatientBill>("Ravi", 500, 1200));      // 1700
    CorporateTieUp corp(5000);
    for (auto& b : bills) {
        b->print();
        std::cout << "   covered=" << corp.amountCovered(*b) << " patientPays=" << corp.patientPays(*b) << '\n';
    }
    bool ok = near(corp.amountCovered(*bills[0]), 5000) && near(corp.amountCovered(*bills[1]), 1700)
              && near(dayTotal(bills), 8900);
    std::cout << "extension checks " << (ok ? "PASS" : "FAIL") << '\n';

    std::cout << "--- thrown and caught exceptions during the run (for ASan B4.2) ---\n";
    try { DialysisBill bad("Z", -1, 100); }
    catch (const BillingError& e) { std::cout << "caught: " << e.what() << '\n'; }
    try {
        auto p = std::make_unique<OutPatientBill>("Q", 100, 100);
        Insurance i(150, 10); i.amountCovered(*p);     // 200 > 150 -> ClaimError
    } catch (const ClaimError& e) { std::cout << "caught: " << e.what() << '\n'; }
    return (fails == 0 && ok) ? 0 : 1;
}
