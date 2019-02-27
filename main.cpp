#include <iostream>
#include <cmath>
#include <limits>

template<typename F, typename DF, typename T>
T newton_sq(T num, T x0, F f, DF df)
{
    T x = x0 - f(x0, num)/df(x0);

    for(unsigned int itnum = 0; itnum < 20; itnum++)
    {
        if (x*x - num < std::numeric_limits<T>::epsilon())
        {
            std::cout << "itnum=" << itnum << std::endl;
            break;
        }
        else
        {
            x = x - f(x, num)/df(x);
        }        
    }
    return(x);
}

int main(int, char**) {
    //hívjuk meg double-re a nwt_sw-t:
    double num = 512.0;
    double guess = 10.0;    
    double gyok = newton_sq(num, guess,[](double x, double num){return x*x - num;},
                    [](double x){return 2.0*x;});
    //MEGJEGYZÉS:
    //minden lambdának saját típusa van -> nem lesz mindkettő F típusparaméterrel helyettesíthető
    //MO: newton_sq-nál két típustemplate bevezetése a két különböző függvényhez!
    std::cout.precision(16);
    std::cout << "szam: " << num << ", gyoke: " << gyok << std::endl;

    //hívjuk meg float-ra!
    float num1 = 512.0;
    float guess1 = 30.0;
    float gyok1 = newton_sq(num, guess,[](float x, float num){return x*x - num;},
                    [](float x){return 2.0*x;});
    std::cout << "szam: " << num1 << ", gyoke: " << gyok1 << std::endl;
}
