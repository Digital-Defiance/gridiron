// TODO: make this test gridiron!

#include <gridiron/controls/controls.hpp>

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
    std::cout << GridIron::Controls::Page("gridiron-demo/testapp.html") << std::endl;

    return 0;
}
