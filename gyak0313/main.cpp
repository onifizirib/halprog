#include <iostream>
#include "vector2.h"

int main(int, char**) {
    Vector2<double> v{3.0, 4.0};
    std::cout << "vector 2d v: "<< v << std::endl;
    v = v*3.0;
    std::cout << "jobbról szorzás 3-mal: " << v << std::endl;
    v = 2.0*v;
    std::cout << "balról szorzás 2-vel: " << v << std::endl;

    //még nincs megírva a balról szorzás!
    std::cout << "így a vektor hossza: " <<len(v) << std::endl;

    Vector2<double> u{0.0, 2.0};
    std::cout << "vector 2d u: "<< u << std::endl;
    std::cout << "u és v vektor összege: " <<u+v << std::endl;
    std::cout << "u és v vektor különbsége: " <<u-v << std::endl;
    std::cout << "összegvektor hossznégyzete: " << sqlen(u+v) << std::endl;
    
    std::cout << "u skalárszorzata önmagával: " << dot(u,u) << std::endl;
    std::cout << "u és v skalárszorzata: " << dot(u,v) << std::endl;
    
    double angle = 4.0*atan(1); //nincs beépített pi érték
    std::cout << "u vektor elforgatása " << angle << " radiánnal: " << u.rotate(angle) << std::endl;
    
    //std::cout << u.norm() << std::endl;
    //ez szomorú, errorként feldobja, hogy "no matching function for call to 'len'", valamint "note: template argument deduction/substitution failed". 
    //megírom kívül a norm()-ot.
    std::cout << "u vektor " << u << " normálása: " << norm(u) << std::endl;
    std::cout << "v vektor " << v << " normálása: " << norm(v) << std::endl;
    
    Vector2<double> w{1.0, 1.0};
    std::cout << "w vektor: " << w << std::endl;
    u += w;
    std::cout << "u += w parancs után u: " << u << std::endl;
    w -= u;
    std::cout << "w -= u parancs után w: " << w << std::endl;
    u *= 1.5;
    std::cout << "u *= 1.5 parancs után u: " << u << std::endl;
    u /= 0.5;
    std::cout << "u /= 0.5 parancs után u: " << u << std::endl;
    
}
