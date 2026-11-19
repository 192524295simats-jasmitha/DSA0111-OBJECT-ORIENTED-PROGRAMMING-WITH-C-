// Shows the ambiguity when virtual is removed (does NOT compile on purpose)
#include <string>
struct Person { std::string name; };
struct Student : Person {};
struct Staff : Person {};
struct TA : Student, Staff {};
int main() { TA t; t.name = "x"; }   // error: request for member 'name' is ambiguous
