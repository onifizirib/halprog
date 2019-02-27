#include <iostream>
#include <cmath>
#include <limits>

template<typename F, typename DF, typename T>
T newton_sq(T x0, F f, DF df)
{
    T x = x0 - f(x0)/df(x0);

    for(unsigned int itnum = 0; itnum < 40; itnum++)
    {
        if (std::abs(f(x)) < 10*std::numeric_limits<T>::epsilon())
        // x*x- num épp az f(x) értéke -> utóbbival általánosan fog működni, nem csak gyökkeresésre
        // ehhez  meghívásnál "vicces" szintaxist kell használni, [num] írandó [] helyett.
        // így a num változót kívülről használni tudja majd
        {
            std::cout << "itnum=" << itnum << std::endl;
            break;
        }
        else
        {
            x = x - f(x)/df(x);
        }        
    }
    return(x);
}

int main(int, char**) {
    //hívjuk meg double-re a nwt_sw-t:
    double num = 512.0;
    double guess = 10.0;    
    double gyok = newton_sq(guess,[num](double x){return x*x - num;},
                    [](double x){return 2.0*x;});
    //MEGJEGYZÉS:
    //minden lambdának saját típusa van -> nem lesz mindkettő F típusparaméterrel helyettesíthető
    //MO: newton_sq-nál két típustemplate bevezetése a két különböző függvényhez!
    std::cout.precision(16);
    std::cout << "szam: " << num << ", gyoke: " << gyok << std::endl;

    //hívjuk meg float-ra!
    float num1 = 512.0;
    float guess1 = 30.0;
    float gyok1 = newton_sq(guess,[num1](float x){return x*x - num1;},
                    [](float x){return 2.0*x;});
    std::cout << "szam: " << num1 << ", gyoke: " << gyok1 << std::endl;

    //összehasonlításképp wolfram alpha:
    std::cout << "WolframAlpha: 22.6274169979695207" << std::endl;
}
