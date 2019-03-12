#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

double vectormean(std::vector<double> const &v)
{
    double mean = std::accumulate(v.begin(), v.end(), 0.0) / v.size();
    return mean;
}

/* 
   két vektort szeretnék betuszkolni az accumulate-ba, amit szeritem nem lehet
   https://en.cppreference.com/w/cpp/header/numeric
   az inner product épp megfelel a célnak
   MEGJ.: füzetben van magyarázóábra
   UI: a dokumentáció és a magyarázóábra binOp sorszámai épp fel vannak cserélve ^^"
*/

std::vector<double> linreg(std::vector<double> const &x, std::vector<double> const &y)
{
    //hibakezelés, ha x és y nem egyforma hosszúak
    if (x.size() != y.size())
    {
        std::cout << "the two vectors must have the same size" << std::endl;
        std::vector<double> v{0.0};
        return v;
        /*
           egy hosszú vektorral térek vissza, bár lehet job lenne nulla hosszúval
           alapesetben 2 hosszúval tére vissza (b,m)
           azt nem tudom hogy lehet-e return után inicializálni v-t. egyelőre nem szereti.
        */ 
    }
    double x_mean = vectormean(x);
    double y_mean = vectormean(y);
    
    /*
        //ellenőrzésképp
        std::cout << "x_mean: " << x_mean << "\n";
        std::cout << "y_mean: " << y_mean << "\n";
    */

    auto binOp2 = [x_mean, y_mean](double x, double y)
    {
        return (x-x_mean)*(y-y_mean);
    };

    auto binOp1 = [](double x, double y)
    {
        return x+y;
    };
    // avagy lambda függvényeket függvényen belül is be lehet vezetni

    double szamlalo = std::inner_product(x.begin(), x.end(), y.begin(), 0.0, binOp1, binOp2);
    // y.end() nem kell az innerproduct-ba
    double nevezo =  std::inner_product(x.begin(), x.end(), x.begin(), 0.0, binOp1, binOp2);

    double meredekseg = szamlalo/nevezo;
    double eltolas = y_mean - meredekseg*x_mean;
    
    /*
        //ellenőrzésképp
        std::cout << "szamlalo: " << szamlalo << "\n";
        std::cout << "nevezo: " << nevezo << "\n";
    */
    
    std::vector<double> egyenes{meredekseg,eltolas};
    return egyenes;
    /*
        most vettem észre, hogy a feladat kiírás szerint array-jel kellett volna visszatérni.
        de így egy rövidebb vektorral vissza tudok térni hibajelzésképp!
    */ 
}

void kiirat(double const &a)
{
    std::cout << a << std::endl;
    return;
}

int main(int, char**) {
    std::cout << "A megoldás-vektor kiírása során az első elem a meredekség, a második a bias.\n";
    
    /* 
        első teszteset 2 pontra, avagy ezt elrontani kézzel sem lehet könnyen
        pontok: (1,3), (5,6)
        egyenes meredeksége: 0.75; eltolása: 2.25
    */
    std::vector<double> ketpont_x{1.0, 5.0};
    std::vector<double> ketpont_y{3.0, 6.0};
    std::vector<double> ketpont_lin = linreg(ketpont_x, ketpont_y);
    std::for_each(ketpont_lin.begin(), ketpont_lin.end(), &kiirat);
    /*
        ezért kellett az <algorithm>-t behúzni
        forrás https://stackoverflow.com/questions/10516495/calling-a-function-on-every-element-of-a-c-vector
        
        amúgy miért kellett a kiirat függvényt referencia szerint átadni? 
        az is lemásolódna alapból? hogy lehet lemásolni egy függvényt?
    */ 
    std::cout << "--------------" << std::endl;
    /* 
        második teszteset 5 pontra
        pontok: (0.0, 1.0)
                (3.0, 9.0)
                (2.8, 8.0)
                (1.0, 3.1)
                (2.0, 6.0)
        egyenes meredeksége: 2.645466; eltolása: 0.76398
        utolsó jegyek kerekítve, kézzel számolva (erre a debuggolás miatt volt szükség, lépésről lépésre kellett átnézni hogy mi történik)
        wolframAlpha-val ellenőrizve: https://www.wolframalpha.com/input/?i=linear+fit+%7B0,1+%7D,+%7B3,+9%7D,+%7B2.8,+8%7D,+%7B1,+3.1%7D,+%7B2,+6%7D
        Geogebrával ellenőrizve (utóbbi sajnos nem sok jegyet adott ki, linket emiatt nem is mellékelnék ha egyaltalán tudnék :D)
    */
    std::vector<double> otpont_x{0.0, 3.0, 2.8, 1.0, 2.0};
    std::vector<double> otpont_y{1.0, 9.0, 8.0, 3.1, 6.0};
    std::vector<double> otpont_lin = linreg(otpont_x, otpont_y);
    std::for_each(otpont_lin.begin(), otpont_lin.end(), &kiirat);
    
}
