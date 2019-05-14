#include "distr.h"
#include <random>

struct ER_exp
{
    //ez nem tárol gráfot, így egy új structot írtam hozzá..
    int N;                  //number of nodes
    double p;                  //linking probability
    double M_exp;           //expected number of links
    double k_exp;           //expected average degree
    double l_exp;           //expected average shorest path
    double c_exp;           //expected average clustering coeff
    distr degreeDist_exp;   //expected degree distribution
    

    ER_exp()
    {
        N=0, p=0.0, M_exp=0.0, k_exp=0.0, l_exp=0.0, c_exp=0.0;
        //distr-nek van default konstruktora
    }
    ER_exp(ER_exp const& cpy) = default;
    /* TODO
    ER_exp(ER_exp && mv)
    */
   ER_exp(int const& nodenum, double const& linkprob)
   {
       N = nodenum;
       double dlN = double(nodenum);
       p = linkprob;

       M_exp = p*dlN*(dlN-1.0)/2.0;
       k_exp = dlN*p;
       l_exp = log(dlN)/log(k_exp);
       c_exp = p;

       degreeDist_exp = distr(0.0, dlN, N);
       std::random_device rd;
       std::minstd_rand rng(rd());
       std::binomial_distribution<int> binom(N-1, p);
       for(int i=0; i<10000; i++)
       {
           degreeDist_exp.hist[binom(rng)]++;
       }
       std::for_each(degreeDist_exp.hist.begin(), degreeDist_exp.hist.end(), [](double& value){value /= 10000.0; });       
   }
};


struct ER
{
//core graph
    graph<int> graf;
//parameters for building it:
    int N;              //number of nodes
    double p;           //linking probability
//quantities to test
    double M;           //number of links
    double k;           //average degree
    double l;           //average shorest path
    double c;           //average clustering coefficient
    distr degreeDist;   //degree distribution    

//------------------------------------------------
//konstruktorok:
    ER()
    {
        N=0, p=0.0, M=0.0, k=0.0, l=0.0, c=0.0;
        //distr-nek és graph-nak van default konstruktora
    }
    ER(ER const& cpy) = default;
    //elég ide a default? tudja ő majd, hogy a graph cpy konstruktorát hívja meg majd?
    ER(ER && mv)
    //JELENLEG NEM MŰKÖDIK JÓL 
    {
        //std::swap(graf, mv.graf);
        //std::swap(degreeDist, mv.degreeDist);
        //Q: hogy kéne overload-olni az std::swap-ot graph-ra, főleg hogy a graph attribútumaihoz csak const get()-tel férek hozzá?
        N=0, p=0.0, M=0.0, k=0.0, l=0.0, c=0.0;
        std::swap(N, mv.N);
        std::swap(p, mv.p);
        std::swap(M, mv.M);
        std::swap(k, mv.k);
        std::swap(l, mv.l);
        std::swap(c, mv.c);        
    }
    ER(int const& nodenum, double const& linkprob)
    {
        N = nodenum;        
        graf = graph<int>(N);
        p = linkprob;

        //feltöltés élekkel
        std::random_device rd{};
        std::minstd_rand rng(rd());
        std::uniform_real_distribution<double> unif(0, 1);
        double rndNum;
        int iternum = 0;
        for(int i=0; i<N; i++)
        {
            for(int j=N-1; j>i; j--)
            {
                iternum++;
                if(i!=j)
                {
                    rndNum = unif(rng);
                    if (rndNum < p)
                    {
                        std::cout << "unif random number: "<<rndNum<<" < "<< p <<"\n";
                        graf.addLink(i,j);
                    }
                }
            }
        }
        std::cout<<"iternum: "<<iternum<<std::endl;

        //mennyiségek számolása a kész gráf alapján
        M = (double)graf.getLinkNum();
        k = graf.avgDegree();
        l = avgPathLen(graf);
        c = avgClusCoeff(graf);
        degreeDist = distr(0.0,(double)N,N);
        degreeDist.feltolt(graf);
    }
};