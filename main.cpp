#include <iostream>
#include <vector>
#include <numeric>

double vectormean(std::vector<double> const &v)
{
    double mean = std::accumulate(v.begin(), v.end(), 0.0) / v.size();
    return mean;
}

// két vektort szeretnék betuszkolni az accumulate-ba, amit szeritem nem lehet
// https://en.cppreference.com/w/cpp/header/numeric
// az inner product épp megfelel a célnak
// MEGJ.: füzetben van magyarázóábra

std::vector<double> linreg(std::vector<double> const &x, std::vector<double> const &y)
{
    //hibakezelés, ha x és y nem egyforma hosszúak
    if (x.size() != y.size())
    {
        std::cout << "the two vectors must have the same size" << std::endl;
        std::vector<double> v{0.0};
        return v;
        // egy hosszú vektorral térek vissza, bár lehet job lenne nula hosszúval
        // alapesetben 2 hosszúval tére vissza (b,m)
        // azt nem tudom hogy lehet-e return után inicializálni v-t. egyelőre nem szereti.
    }
    double x_mean = vectormean(x);
    double y_mean = vectormean(y);

    auto binOp1 = [x_mean, y_mean](double x, double y)
    {
        return (x-x_mean)*(y-y_mean);
    };

    auto binOp2 = [](double x, double y)
    {
        return x+y;
    };
    // avagy lambda függvényeket függvényen belül is be lehet vezetni

    double b = std::inner_product(x.begin(), x.end(), y.begin(), y.end(), 0.0, binOp1, binOp2)
    / std::inner_product(x.begin(), x.end(), x.begin(), x.end(), 0.0, binOp1, binOp2);
    // fordító szerint inner_product nincs ^^"
}

int main(int, char**) {
    std::cout << "Hello, world!\n";
}
