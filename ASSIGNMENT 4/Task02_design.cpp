// Task 2.3 + alternative (composition) with sizeof
#include <iostream>
#include <string>
class Person {
protected:
    std::string name; int id; std::string phone;
public:
    Person(std::string n = "", int i = 0, std::string p = "") : name(std::move(n)), id(i), phone(std::move(p)) {}
    const std::string& getName() const { return name; }
};
class Student : public virtual Person {            // is-a Person
protected:
    std::string programme; double cgpa;
public:
    Student(std::string n, int i, std::string p, std::string prog, double c)
        : Person(n, i, p), programme(std::move(prog)), cgpa(c) {}
};
class Staff : public virtual Person {              // is-a Person
protected:
    std::string dept; double salary;
public:
    Staff(std::string n, int i, std::string p, std::string d, double s)
        : Person(n, i, p), dept(std::move(d)), salary(s) {}
};
class TeachingAssistant : public Student, public Staff {   // hybrid (diamond)
public:
    TeachingAssistant(std::string n, int i, std::string p, std::string prog, double c, std::string d, double s)
        : Person(n, i, p), Student(n, i, p, prog, c), Staff(n, i, p, d, s) {}
    void show() const { std::cout << "TA name (unambiguous): " << name << "\n"; }
};
class ResearchScholar : public Student {           // multilevel w.r.t. Person
    double fellowship;
public:
    ResearchScholar(std::string n, int i, std::string p, std::string prog, double c, double f)
        : Person(n, i, p), Student(n, i, p, prog, c), fellowship(f) {}
};

// ---- alternative: composition (has-a), non-virtual Person copies ----
class StudentN : public Person { std::string programme; double cgpa;
public: StudentN(std::string n, int i, std::string p, std::string pr, double c) : Person(n,i,p), programme(pr), cgpa(c) {} };
class StaffN : public Person { std::string dept; double salary;
public: StaffN(std::string n, int i, std::string p, std::string d, double s) : Person(n,i,p), dept(d), salary(s) {} };
class TA_Composed {
public:
    StudentN s; StaffN f;
    TA_Composed(std::string n, int i, std::string p, std::string pr, double c, std::string d, double sal)
        : s(n,i,p,pr,c), f(n,i,p,d,sal) {}
};

int main() {
    TeachingAssistant ta("Kavya", 7, "99999", "MCA", 8.5, "CSE", 20000);
    ta.show();
    ResearchScholar rs("Arun", 8, "88888", "PhD", 9.1, 30000);
    TA_Composed tc("Kavya", 7, "99999", "MCA", 8.5, "CSE", 20000);
    std::cout << "sizeof(TeachingAssistant, virtual diamond) = " << sizeof(TeachingAssistant) << "\n";
    std::cout << "sizeof(TA_Composed, two Person copies)     = " << sizeof(TA_Composed) << "\n";
    std::cout << "composed name via student: " << tc.s.getName() << " | via staff: " << tc.f.getName() << " (must choose)\n";
    (void)rs;
}
