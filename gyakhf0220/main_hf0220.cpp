#include <iostream>
#include <cmath>
#include <limits>

void solve(double a, double b, double c)
{
    std::cout << "a=" << a << " b=" << b << " c=" << c << std::endl;

    if (a==0.0)
    {
        std::cout << "nem masodfoku kifejezes" << std::endl;
        return;
    }

    double discr = b*b- 4.0*a*c;

    if (discr < 0.0)
    {
        std::cout << "nincs valos megoldas" << std::endl;
        return;
    }
    else
    {
        if (discr == 0)
        {
            double x = -b/(2*a);

            std::cout << "egy valos megoldas:" << std::endl;
            std::cout << x << std::endl;

            return;
        }
        else
        {
            discr = sqrt(discr);
            double x1, x2;
            x1 = (-b + discr)/(2*a);
            x2 = x1 - discr/a;

            std::cout << "ket valos megoldas:" << std::endl;
            std::cout << x1 << " es " << x2 <<std::endl;

            return;
        }
    }
}

//--------------------------------------------------------

double f_newton(double num, double x)
{
    return x*x-num;
}

double df_newton(double x)
{
    return 2*x;
}

double sqrt_newton_rekurziv(double num, double x0, unsigned int itnum)
{
    // next guess is x:
    double x = x0 - f_newton(num, x0)/df_newton(x0);

    if (x*x == num)
    {
        // ez tippre sosem fog teljesülni, egy |x*x-num| < eps jobb lenne
        // ehelyett a végtelen ciklust max iterációszámmal fogom elkerülni
        std::cout << "sqrt founded, sqrt=" << x << std::endl;
    }
    else if (itnum > 40)
    {
        std::cout << "iteration number exceeded limit" <<std::endl;
        std::cout << "sqrt=" << x << std::endl;
    }
    else
    {
        // next iteration:
        x = sqrt_newton_rekurziv(num, x, itnum+1);
    }

    return x;
}


double sqrt_newton(double num, double x0)
{
    double x = x0 - f_newton(num,x0)/df_newton(x0);
    double diffx = 10.0;
    // diffx legyen elég nagy

    for(unsigned int itnum = 0; itnum < 40; itnum++)
    {
        if (diffx < std::numeric_limits<double>::epsilon())
        {
            std::cout << "itnum=" << itnum << std::endl;
            break;
        }
        else
        {
            double x_new = x - f_newton(num,x)/df_newton(x);
            diffx = std::abs(x_new - x);
            x = x_new;
            //std::cout << itnum << ": gyok=" << x << std::endl;
        }
    }
    return x;
}

//--------------------------------------------------------

double fv(double x)
{
    return(6*cos(x)/(x));
}

double integrate(int n, double a, double b)
{
/*
    composite Simpson 3/8 rule
    source: https://en.wikipedia.org/wiki/Simpson%27s_rule
    uses cubic interpolation (on 4 points) -> n%3 must be 0
*/
    std::cout << "a=" << a << ", b=" << b << ", n=" << n << std::endl;

    //alapvető hibakezelések
    if(b < a)
    {
        double dummy = a;
        a = b;
        b = dummy;
    }

    if (n%3 != 0)
    {
        std::cout << "number of intervals must be divisible by 3" << std::endl;
        return(0.0);
        // jó lenne egy lehetetlen értékkel visszatérni ebben az esetben..
    } else if (n <= 0)
    {
        std::cout << "number of intervals must be positive" << std::endl;
        return(0.0);
    }

    double stepsize = (b-a)/n;
    double sum = 0.0;

    for(int i = 1; i < n; i++)
    {
        if (i%3 == 0)
        {
            sum += 2.0*fv(a+i*stepsize);
        }
        else
        {
            sum += 3.0*fv(a+i*stepsize);
        }
    }
    sum += fv(a) +fv(b);
    sum *= (3.0/8.0)*stepsize;
    return sum;
}

//--------------------------------------------------------

int main(int, char**) {

    solve(1.0, 2.1, 0.2);
    solve(0.0, 5.7, 3.0);
    solve(-3.4, 0.1, -5.9);
    solve(3.0, 6.0, 3.0);
    std::cout << "--------------" << std::endl;

    std::cout.precision(16);
    double num = 612.0;
    std::cout << "number=" << num << std::endl;
    double gyok = sqrt_newton_rekurziv(num, 10.0, 0);
    std::cout << "now without recursion:" << std::endl;
    gyok = sqrt_newton(num, 10.0);
    std::cout << "gyok: " << gyok << std::endl;
    std::cout << "--------------" << std::endl;

    double lowerlim = 1.0;
    double upperim = 10.0;
    double ter;
    // wolframalpha as reference:
    //https://www.wolframalpha.com/input/?i=integrate+(6cosx%2Fx)+x%3D%5B1,10%5D
    double wa = -2.2971621354325410;
    for (int n = 6; n < 91; n=n+9)
    {
        ter = integrate(n, lowerlim, upperim);
        std::cout << "integral hibaja=" << wa-ter << std::endl;
    }

    return 0;
}
