#include <iostream>
#include <vector>

typedef std::vector<std::pair<std::string, int>> pairlist_t;

namespace first {
    int x = 1;
}

using namespace first;

void happyBirthday () {
    std::cout << "POOPY" << "\n";
}

int main() {
    // type conversion - implicit, automatic. explicit, precede value with new data type
    // namespaces are for no conflict
    // typedef - makes long types smaller
    std::cout << "Hello world" << std::endl;
    std::cout << x << std::endl;
    // string methods
    // .length()
    
    // functions
    happyBirthday();

    // overloaded functions are just different versions of the same function.
    return 0;
}