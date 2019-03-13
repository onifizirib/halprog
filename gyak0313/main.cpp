#include <iostream>
#include "vector2.h"

int main(int, char**) {
    std::cout << "Hello, world!\n";
    Vector2<double> v{3.0, 4.0};
    std::cout << v << std::endl;
    v = v*3.0;
    std::cout << v << std::endl;
    //v = 3*v;
    //std::cout << v << std::endl;
    //még nincs megírva a balról szorzás!
    std::cout << len(v) << std::endl;

    Vector2<double> u{0.0, 2.0};
    std::cout << u+v << std::endl;
    std::cout << u.norm() << std::endl;
    //ez szomorú, errorként feldobja, hogy "no matching function for call to 'len'".
}
