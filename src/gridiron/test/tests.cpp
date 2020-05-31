// TODO: make this test gridiron!

#include <gridiron/gridiron.hpp>
#include <iostream>

namespace GridIron {

    class Test {
    public:
        Test() {

        }
    };

    void runTests() {
    }

}

int main() {
    std::cout << GridIron::Page("gridiron-demo/testapp.html") << std::endl;

    return 0;
}
