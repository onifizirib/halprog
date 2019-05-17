#include "distr.h"

const double euler_gamma = 0.57721566490153286060651209008240243104215933593992;
 //form https://hu.wikipedia.org/wiki/Euler%E2%80%93Mascheroni-%C3%A1lland%C3%B3

struct statistics
{
    //egy gráf vagy elméleti modell alapján tölthető fel
    int nodeNum;        //number of nodes
    int linkNum;        //numebr of links
    double avgPath;     //average shortest path
    double avgClus;     //average clustering coefficient
    double avgDegr;     //average degree
    distr degreeDist;   //degree distribution

//konstruktorok
    statistics()
    {
        nodeNum=0, linkNum=0, avgPath=0.0, avgClus=0.0, avgDegr=0.0;
        //distr-nek van default konstruktora
    }
    statistics(statistics const& cpy) = default;
    /* TODO
    statistics(statistics && mv)
    */

//expected statistisc to ER graph with parameter nodenum and linkprob
   statistics(int const& nodenum, double const& linkprob)
   {
       if(nodenum < 2)
       {
           std::cout << "zero or one noded estimation is given! (dividing with zero or log0 is performed)\n";
           statistics();
       }
       else
       {       
           nodeNum = nodenum;
           double dlN = double(nodenum);

           linkNum = linkprob*dlN*(dlN-1.0)/2.0;
           avgDegr = dlN*linkprob;
            //igazából pontosan (dlN-1)linkprob
           avgPath = (log(dlN)-euler_gamma)/log(avgDegr) + 0.5;
           avgClus = linkprob;

           degreeDist = distr(0.0, dlN, nodenum);
           std::random_device rd;
           std::minstd_rand rng(rd());
           std::binomial_distribution<int> binom(nodenum-1, linkprob);
           for(int i=0; i<10000; i++)
           {
               degreeDist.hist[binom(rng)]++;
           }
           std::for_each(degreeDist.hist.begin(), degreeDist.hist.end(), [](double& value){value /= 10000.0; }); 
       }      
   }

//expected statistisc to WS graph with parameter nodenum, "distance of farest neighbour to connect" q, and rewiring probability beta
   statistics(int const& nodenum, int q, double const& beta)
   {
       if(nodenum < 2)
       {
           std::cout << "zero or one noded estimation is given! (dividing with zero or log0 is performed)\n";
           statistics();
       }
       else
       {
           int thres = floor((double)nodenum/2.0);
           if (q > thres) {q = thres;}

           nodeNum = nodenum;
           double dlN = double(nodenum);
           double dlq = double(q);

           linkNum = nodeNum*q;
           avgDegr = 2.0*dlq;
           if (beta < 1e-15)
           {
               //std::cout << "beta~0" << std::endl;
               avgPath = dlN/(4.0*dlq);
               avgClus = (3.0*dlq-3.0)/(4.0*dlq-2.0);
           }
           else if (1.0-beta < 1e-15)
           {
               //std::cout << "beta~1" << std::endl;
               avgPath = log(dlN)/log(2.0*dlq);
               avgClus = (2.0*dlq)/(dlN-1.0);
           }
           else
           {
               std::cout << "beta value is not 0 nor 1, thus some quantities cannot be predicted.\n";
               avgPath = -1.0;
               avgClus = -1.0;
           }

           degreeDist = distr();
           //no prediction available for this quantity
        }
   }

//expected statistisc to BA graph with parameter timesteps t, number of links a new node comes with m
   statistics(int const& m, int const& t)
   {
       nodeNum = t;
       double dlN = (double)nodeNum;
       double dlm = (double)m;
       linkNum = m*t;
       avgDegr = 2.0*dlm;
       if (m == 1)
       {
           avgClus = 0.0;
            // tree-like graph therefore with zero clustering coeff
       }
       else
       {
           avgClus = (pow(dlm*(dlm+1.0),2.0) / (4*(dlm-1.0))) * (log((dlm+1.0)/dlm) - (1/(dlm+1.0))) * (pow(log(dlN),2.0) / dlN);
       }
       /*
        avgClus = dlm*std::pow((log(dlN)),2.0)/(8.0*dlN);
        pontatlan volt az a közelítés, amit levezetve találtam a complex network kurzus anyagában,
        eszerint a cikk szerint http://mars.if.pw.edu.pl/~jholyst/data/PRE46126.pdf a mean-filed approximation:
        [(m^2*(m+1)*2) / (4*(m-1))] * [ln((m+1)/m) - 1/(m-1)] * [(lnt^2) / (t)]
       */

       degreeDist = distr(dlm, dlm-1.0+(double)t, t-1); 
       //legalábbis gráf esetére ez a tartomány biztos, így az elméleti legenerálást is ide korlátozom
       for(int i=0; i<t-1; i++)
       {
           degreeDist.hist[i] = 2.0*std::pow(dlm, 2.0)*std::pow((double)i+dlm, -3.0);
       }
       double sum = std::accumulate(degreeDist.hist.begin(), degreeDist.hist.end(), 0.0);
       std::for_each(degreeDist.hist.begin(), degreeDist.hist.end(), [&](double& value){value /= sum; });
       //és így normálva lett az hist elemeinek összege egyre (amit egy gráf produkálhat arra biztosan fennáll ez a tulajdonság)

       avgPath = (log(dlN) - log(dlm/2.0) - 1.0 - euler_gamma) / ((log(log(dlN))) + log(dlm+2.0)) + 1.5;
       /*
        with this non-tunable model the scalefree-exponent is 3, therefore the <l> expected value is
        (lnN - ln(m/2) - 1 - euler_gamma) / (lnlnN + ln(m+2)) + 3/2
        according to this paper: http://www.if.pw.edu.pl/~jholyst/data/PhysRevE_70_056110.pdf
       */
   }

//mennyiségek számolása a kész gráf alapján
   template<typename T>
   statistics(graph<T> const& graf)
   {    
        nodeNum = graf.getNodeNum();
        linkNum = graf.getLinkNum();
        avgDegr = graf.avgDegree();
        avgPath = avgPathLen(graf);
        avgClus = avgClusCoeff(graf);
        degreeDist = distr(0.0, (double)nodeNum, nodeNum);
        degreeDist.feltolt(graf);
   }  

//metódusok
   template<typename T>
   void update(graph<T> const& graf)
   {
        this = statistics(graf);
   }
};

//kiírás
std::ostream& operator<< (std::ostream& o, statistics const& stat)
{
    o << stat.nodeNum << std::endl;
    o << stat.linkNum << std::endl;
    o << stat.avgDegr << std::endl;
    o << stat.avgPath << std::endl;
    o << stat.avgClus << std::endl;

    o << stat.degreeDist.mini << " ";
    o << stat.degreeDist.maxi << " ";
    o << stat.degreeDist.binNum << " ";
    o << stat.degreeDist.binsize << std::endl;

	int n = stat.degreeDist.hist.size();
	if( n > 0 )
	{
		for(int i=0; i<n-1; ++i)
		{
			o << stat.degreeDist.hist[i] << " ";
		}
		o << stat.degreeDist.hist[n-1] << std::endl;
	}
	return o;
}
