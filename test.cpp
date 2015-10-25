#include "Redis_methods.cpp"

int main() {
    Redis_API test;
    std::cout << "Type command, please." << std::endl;
    test.command();
    return 0;
}