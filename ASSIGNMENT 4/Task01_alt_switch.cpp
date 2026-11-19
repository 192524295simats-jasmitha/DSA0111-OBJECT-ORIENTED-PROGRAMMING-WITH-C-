// Alternative: one Staff class + type field + switch
#include <iostream>
#include <string>
enum class Type { Professor, LabAssistant, Guest };
struct Staff {
    Type type; std::string name; int id;
    double basic = 0, research = 0, hours = 0, rate = 0;   // many unused per object
    double pay() const {
        switch (type) {
            case Type::Professor:    return basic + basic * 0.42 + research;
            case Type::LabAssistant: return basic + basic * 0.42;
            case Type::Guest:        return hours * rate;
        }
        return 0;
    }
};
int main() {
    Staff a{Type::Professor, "Asha", 1, 50000, 8000};
    Staff b{Type::LabAssistant, "Ravi", 2, 20000};
    Staff c{Type::Guest, "Meena", 3, 0, 0, 40, 1500};
    std::cout << a.pay() << " " << b.pay() << " " << c.pay() << "\n";
    std::cout << "sizeof(Staff single class) = " << sizeof(Staff) << "\n";
}
