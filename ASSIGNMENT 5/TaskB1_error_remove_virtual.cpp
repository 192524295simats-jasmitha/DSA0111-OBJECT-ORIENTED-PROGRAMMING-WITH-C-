// B1.3 (Complete-the-code variant): 'virtual' removed from a PURE declaration -> compile error
struct Bill { double total() const = 0; };
int main() {}
