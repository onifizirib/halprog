#include <iostream>
#include <cmath>
#include <limits>

template<typename T, typename F, typename DF>
T newton_sq(T x0, F f, DF df)
{
    T x = x0;
	T diffx = 1.0;
	T x_new = x0;

    for(unsigned int itnum = 0; itnum < 40; itnum++)
    {
        if (diffx < std::numeric_limits<T>::epsilon()*std::abs(x))
		/* előző leállási feltételem az abs(x*x-num) < epsilon volt, ehhez megjegyzések: 
		   x*x-num épp az f(x) értéke -> utóbbival általánosan fog működni, nem csak gyökkeresésre
		   (lévén mindig zérushelyet kell keressünk)
           ehhez  meghívásnál "vicces" szintaxist kell használni, [num] írandó [] helyett.
           így a num változót kívülről használni tudja majd
           ha az összes kívül létezőt akarjuk használni, [=] írható
		*/
        {
            std::cout << "itnum=" << itnum << std::endl;
            break;
        }
        else
        {
            x_new = x - f(x)/df(x);
			diffx = std::abs(x-x_new);
			x = x_new;
        }        
    }
    return x;
}

int main(int, char**) {
    //hívjuk meg double-re a nwt_sq-t:
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
    float gyok1 = newton_sq(guess1,[num1](float x){return x*x - num1;},
                    [](float x){return 2.0*x;});
    std::cout << "szam: " << num1 << ", gyoke: " << gyok1 << std::endl;

    //összehasonlításképp wolfram alpha:
    std::cout << "WolframAlpha: 22.6274169979695207" << std::endl;
}
