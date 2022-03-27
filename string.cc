#include <string>
#include <iostream>

int main(void) {
    char x[] = "xxx";
    std::string str = std::string(x);
    std::cout << str << std::endl;
}
