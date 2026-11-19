// Alternative: Student owns Address* (Rule of Three required)
#include <iostream>
#include <string>
class Address { std::string city; int pin;
public: Address(const std::string& c, int p) : city(c), pin(p) {}
    std::string str() const { return city + "-" + std::to_string(pin); } };
class Student {
    std::string name; Address* home;
public:
    Student(const std::string& n, const std::string& c, int p) : name(n), home(new Address(c, p)) {}
    Student(const Student& o) : name(o.name), home(new Address(*o.home)) {}          // deep copy
    Student& operator=(const Student& o) { if (this != &o) { Address* t = new Address(*o.home); delete home; home = t; name = o.name; } return *this; }
    ~Student() { delete home; }
    void print() const { std::cout << name << " " << home->str() << "\n"; }
};
int main() { Student a("Kavya", "Salem", 636001); Student b = a; b.print(); }
