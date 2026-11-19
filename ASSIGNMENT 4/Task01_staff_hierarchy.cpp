// Task 1.2 - repaired hierarchy
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Staff {                       // abstract base: common data + interface
protected:
    std::string name;
    int id;
public:
    Staff(std::string n, int i) : name(std::move(n)), id(i) {}
    virtual ~Staff() = default;
    virtual double pay() const = 0;
    virtual std::string role() const = 0;
    const std::string& getName() const { return name; }
    int getId() const { return id; }
};

class SalariedStaff : public Staff {   // shared basic+DA rule written ONCE
protected:
    double basic;
    static constexpr double DA_RATE = 0.42;   // named constant, no magic number
public:
    SalariedStaff(std::string n, int i, double b) : Staff(std::move(n), i), basic(b) {}
    double pay() const override { return basic + basic * DA_RATE; }
};

class Professor : public SalariedStaff {
    double research;
public:
    Professor(std::string n, int i, double b, double r) : SalariedStaff(std::move(n), i, b), research(r) {}
    double pay() const override { return SalariedStaff::pay() + research; }
    std::string role() const override { return "Professor"; }
};

class LabAssistant : public SalariedStaff {
public:
    using SalariedStaff::SalariedStaff;
    std::string role() const override { return "LabAssistant"; }
};

class GuestFaculty : public Staff {
    double hours, rate;
public:
    GuestFaculty(std::string n, int i, double h, double r) : Staff(std::move(n), i), hours(h), rate(r) {}
    double pay() const override { return hours * rate; }
    std::string role() const override { return "GuestFaculty"; }
};

int main() {
    std::vector<std::unique_ptr<Staff>> staff;
    staff.push_back(std::make_unique<Professor>("Asha", 1, 50000, 8000));
    staff.push_back(std::make_unique<LabAssistant>("Ravi", 2, 20000));
    staff.push_back(std::make_unique<GuestFaculty>("Meena", 3, 40, 1500));
    double total = 0;
    for (const auto& s : staff) {            // one loop for ALL staff
        std::cout << s->role() << " " << s->getName() << " pay=" << s->pay() << "\n";
        total += s->pay();
    }
    std::cout << "Total payroll = " << total << "\n";
}
