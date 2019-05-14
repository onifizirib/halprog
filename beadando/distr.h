#include "graph.h"

struct distr
{
    //balsól zárt, jobbról nyílt intervallumokra osztja fel a megadott mini-maxi interval-t.
    std::vector<double> hist;
    double mini;
    double maxi;
    double binsize;
    int binNum;

    //konstruktorok
    distr()
    {
        mini = 0.0, maxi = 0.0, binsize = 0.0;
        binNum = 0;
    }
    distr( distr const& cpy) = default;
    distr( distr && mv)
    {
        std::swap(hist, mv.hist);
        mini = 0.0, maxi = 0.0, binsize = 0.0;
        binNum = 0;
        std::swap(mini, mv.mini);
        std::swap(maxi, mv.maxi);
        std::swap(binsize, mv.binsize);
        std::swap(binNum, mv.binNum);
    }
    distr(double const& Min, double const& Max, double const& Binsize)
    {
        mini = Min;
        maxi = Max;
        double interval = maxi-mini;
        binNum = (int)std::round(interval/Binsize);
        binsize = interval/(double)binNum;
        hist.resize(binNum);
    }
    distr(double const& Min, double const& Max, int const& BinNum)
    {
        mini = Min;
        maxi = Max;
        binNum = BinNum;
        binsize = (maxi-mini)/(double)binNum;
        hist.resize(binNum);
    }

    //operator=
    distr& operator=(distr const& cpy)
    {
        if(&cpy == this) {return *this;}

        hist = cpy.hist;
        mini = cpy.mini;
        maxi = cpy.maxi;
        binNum = cpy.binNum;
        binsize = cpy.binsize;        
        return *this;
    }
    distr& operator=(distr && mv)
    {
        if(&mv == this) {return *this;}

        hist = std::move(mv.hist);
        mini = std::move(mv.mini);
        maxi = std::move(mv.maxi);	
        binNum = std::move(mv.binNum);
        binsize = std::move(mv.binsize);	
		//skalár másolódni fog -> kinullázni érdemes
		mv.mini = 0.0;
        mv.maxi = 0.0;
        mv.binNum = 0;
        mv.binsize = 0.0;
        return *this;
    }

    //adatfeltöltés
    template<typename T>
    void feltolt(graph<T> const& graf)
    {
        std::vector<std::vector<T>> adjList = graf.getAdjList();
        double nodeNum = (double)graf.getNodeNum();
        std::vector<int> ranks;
        std::for_each(adjList.begin(), adjList.end(), [&](std::vector<T>& v) mutable {ranks.push_back(v.size()-1);});
        std::for_each(ranks.begin(), ranks.end(), [](int& rank) mutable {std::cout << rank << ", ";});
        std::cout<<std::endl;
        int hanyadik = 0;
        std::for_each(ranks.begin(), ranks.end(), [&](int& rank) mutable
                        {
                            for(int i=0; i<binNum; i++)
                            {                                   
                                if(mini+((double)i*binsize) <= rank && rank < mini+(((double)i+1.0)*binsize))
                                {
                                    std::cout << mini+((double)i*binsize) << "-" << mini+(((double)i+1.0)*binsize) << ", rank: " << rank <<"\n";
                                    hist[i]++;
                                    break;
                                }
                            }
                            return;
                        });
        std::for_each(hist.begin(), hist.end(), [&](double& data) mutable {data /= nodeNum;});
        return;
    }    
};

std::ostream& operator<< (std::ostream& o, distr const& d)
{
    std::vector<double> data = d.hist;
    int binnum = d.binNum;
    double Min = d.mini;
    double Max = d.maxi;
    double binsize = d.binsize;
    for (int i=0; i<binnum; i++)
        {
            o << Min+(i*binsize) <<"-" << Min+((i+1)*binsize) << ": " << data[i] << std::endl;
        }    
    return o;
}
