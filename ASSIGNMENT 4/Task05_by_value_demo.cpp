// What if add() returned Cart (by value)? Each call returns a COPY.
#include <iostream>
struct Cart { int n = 0;
    Cart add() { ++n; return *this; }   // returns copy
};
int main() { Cart c; c.add().add().add(); std::cout << "n = " << c.n << " (expected 3 with Cart&)\n"; }
