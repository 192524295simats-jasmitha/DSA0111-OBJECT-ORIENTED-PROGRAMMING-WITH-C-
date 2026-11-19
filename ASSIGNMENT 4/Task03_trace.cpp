// Tasks 3.1-3.4 (compile with -DNOVIRT for experiment 3.4)
#include <iostream>
#include <string>
#ifdef NOVIRT
#define V
#define PINIT
#else
#define V virtual
#define PINIT Person(n, city),
#endif
struct Address { std::string city;
    Address(std::string c) : city(std::move(c)) { std::cout << "Address "; }
    ~Address() { std::cout << "~Address "; } };
struct Course { std::string title;
    Course(std::string t) : title(std::move(t)) { std::cout << "Course "; }
    ~Course() { std::cout << "~Course "; } };
struct Person { Address addr; std::string name;
    Person(std::string n = "", std::string city = "") : addr(city), name(n) { std::cout << "Person "; }
    ~Person() { std::cout << "~Person "; } };
struct Student : V Person { Course course;
    Student(std::string n, std::string city, std::string c) : Person(n, city), course(c) { std::cout << "Student "; }
    ~Student() { std::cout << "~Student "; } };
struct Staff : V Person { std::string dept;
    Staff(std::string n, std::string city, std::string d) : Person(n, city), dept(d) { std::cout << "Staff "; }
    ~Staff() { std::cout << "~Staff "; } };
struct TeachingAssistant : Student, Staff {
    TeachingAssistant(std::string n, std::string city, std::string c, std::string d)
        : PINIT Student(n, city, c), Staff(n, city, d) { std::cout << "TA "; }
    ~TeachingAssistant() { std::cout << "~TA "; } };

int main() {
    std::cout << "Student:  "; { Student s("A", "Salem", "C++"); std::cout << "| "; } std::cout << "\n";
    std::cout << "Staff:    "; { Staff f("B", "Salem", "CSE"); std::cout << "| "; } std::cout << "\n";
    std::cout << "TA:       "; { TeachingAssistant t("C", "Salem", "C++", "CSE"); std::cout << "| "; } std::cout << "\n";
    std::cout << "sizeof(TeachingAssistant) = " << sizeof(TeachingAssistant) << "\n";
}
