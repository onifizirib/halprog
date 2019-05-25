#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <variant>

struct NoSolution{};

std::variant<NoSolution, std::array<double,2>> linreg(std::vector<double> const &x, std::vector<double> const &y)
{  
//hibakezelés, ha x és y nem egyforma hosszúak
    if (x.size() != y.size())
    {
        std::cout << "the two vectors must have the same size" << std::endl;
        return NoSolution{};
    }
    
//mintaátlagok számítása
    double x_mean = 0.0;
    double y_mean = 0.0;    
    auto binOp0 = [&x_mean, &y_mean](double x, double y)
    {
       x_mean += x;
       y_mean += y;
       return x;
    };
    std::vector<double> dummy = x;    
    std::transform(x.begin(), x.end(), y.begin(), dummy.begin(), binOp0);
    //mivel x és y const -> nem írhatom őket, hiába írnám vissza ugyanazt
    //de nem akarom véletlen elrontani az adatot, így maradjon inkább const
    
    double numOfSample = (double)x.size();    
    x_mean /= numOfSample;
    y_mean /= numOfSample;
       
//meredekség és eltolás számítása  
    double nevezo = 0.0;
    auto binOp2 = [x_mean, y_mean, &nevezo](double x, double y)
    {
        nevezo += std::pow((x-x_mean), 2.0);
        return (x-x_mean)*(y-y_mean);
    };
    auto binOp1 = [](double x, double y)
    {
        return x+y;
    };
    double szamlalo = std::inner_product(x.begin(), x.end(), y.begin(), 0.0, binOp1, binOp2);
    double meredekseg = szamlalo/nevezo;
    double eltolas = y_mean - meredekseg*x_mean;
    
    return std::array<double,2> {meredekseg,eltolas};
}

bool egyenlo(double const& a, double const& b)
{
    if(a-b > 1e-6){return false;}
    return true;
}

int main(int, char**) {
    /* 
        első teszteset 2 pontra, avagy ezt elrontani kézzel sem lehet könnyen
        pontok: (1,3), (5,6)
        egyenes meredeksége: 0.75; eltolása: 2.25
    */
    std::vector<double> ketpont_x{1.0, 5.0};
    std::vector<double> ketpont_y{3.0, 6.0, 3.0};    
    try
    {
        std::array<double,2> ketpont_lin = std::get<std::array<double,2>>(linreg(ketpont_x, ketpont_y));
        std::array<double,2> ref {0.75,2.25};
        if( !egyenlo(ref[0], ketpont_lin[0]) ) {std::cout << "2pont -> meredekseg nem jo!" << std::endl;}
        if( !egyenlo(ref[1], ketpont_lin[1]) ) {std::cout << "2pont -> eltolas nem jo!" << std::endl;}
    }
    catch (const std::bad_variant_access&) {std::cout << "2pont -> nem illesztheto egyenes!\n"; }
      
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
        Sajnos csak kevés jegyig tudtam kiszámolni, így a hibahatárt nem túl kicsire kellett állítani (1e-6), hogy átmenjen ez a teszteset.
    */
    std::vector<double> otpont_x{0.0, 3.0, 2.8, 1.0, 2.0};
    std::vector<double> otpont_y{1.0, 9.0, 8.0, 3.1, 6.0};
    try
    {
    std::array<double,2> otpont_lin = std::get<std::array<double,2>>(linreg(otpont_x, otpont_y));
    std::array<double,2> ref = {2.645466,0.76398};
    if( !egyenlo(ref[0], otpont_lin[0]) ) {std::cout << "5pont -> meredekseg nem jo!" << std::endl;}
    if( !egyenlo(ref[1], otpont_lin[1]) ) {std::cout << "5pont -> eltolas nem jo!" << std::endl;}
    }
    catch(const std::bad_variant_access&) {std::cout << "5pont -> nem illesztheto egyenes!\n"; }
    
    std::cout << "Linreg teszt vege\n";
    return 0;
}
