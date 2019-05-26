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
    auto binOp2 = [] (double x, double y)
    {
        return std::array<double,2> {x, y};
    };
    auto binOp1 = [] (std::array<double,2> A, std::array<double,2> B)
    {
        return std::array<double,2> {A[0]+B[0], A[1]+B[1]};
    };
    std::array<double,2> means = std::inner_product(x.begin(), x.end(), y.begin(), std::array<double,2> {0.0, 0.0}, binOp1, binOp2);
    
    double numOfSample = (double)x.size();    
    means[0] /= numOfSample;
    means[1] /= numOfSample;
    
    const double x_mean = means[0];
    const double y_mean = means[1];
       
//meredekség és eltolás számítása  
    auto binOp3 = [& x_mean, & y_mean](double x, double y)
    {
        return std::array<double,2> {(x-x_mean)*(y-y_mean), std::pow((x-x_mean),2.0)};
    };
    std::array<double,2> tort = std::inner_product(x.begin(), x.end(), y.begin(), std::array<double,2> {0.0, 0.0}, binOp1, binOp3);
    double meredekseg = tort[0]/tort[1];
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
