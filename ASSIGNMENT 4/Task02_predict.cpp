#include <iostream>
#ifdef NOVIRT
#define V
#else
#define V virtual
#endif
struct Person { Person() { std::cout << "Person "; } };
struct Student : V Person { Student() { std::cout << "Student "; } };
struct Staff : V Person { Staff() { std::cout << "Staff "; } };
struct TeachingAssistant : Student, Staff {
    TeachingAssistant() { std::cout << "TA "; }
};
int main() { TeachingAssistant ta; std::cout << '\n'; }
