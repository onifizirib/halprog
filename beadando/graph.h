#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cmath>
#include <random>
//#include <chrono>


template<typename T>
//T lehet szám, vagy karakter például, vagy a csúcs neve (str)
class graph
{
    std::vector<std::vector<T>> adjList;
    int nodeNum;
    int linkNum;

    public:

//konstruktorok
    graph()
    {
        nodeNum = 0;
        linkNum = 0;
    }

    graph(graph<T> const&) = default;
    graph(graph<T> && mv)
    {
        std::swap(adjList, mv.adjList);
        //ez hozzáfér egyaltalán az adjList-ekhez??
        //honnan szedtem én ezt a swap-ot a mátrix osztályban?
        nodeNum = 0;
        linkNum = 0;
        std::swap(nodeNum, mv.nodeNum);
        std::swap(linkNum, mv.linkNum);
    }
    graph(int const& N)
    {
        //nevesítést csak T<-int írok, másra nem lesz szükség, ugyanis azt listában adnák át
        nodeNum = N;
        linkNum = 0;
        adjList.resize(N);        
        int i = 0;
        std::for_each(adjList.begin(), adjList.end(), [&](std::vector<T> v)
                        {
                            v.push_back((T)i);
                            i++;
                            std::cout<<"v-hez adott elem: "<< v[0] << ";   v.size()="<<v.size()<<std::endl;
                            return;
                            // ha [=](std::vector<T> v){...} a szintaxis, akkor nem tudja i-t növelni
                        });
        std::cout << "konstruálás végén i = " <<i<<std::endl;    
    }

//állapot lekérdezések
    int getNodeNum() const
    {
        return nodeNum;
        //kell-e const a visszatérési értékhez? elvégre maga az attribútum nem const 
    }
    int getLinkNum() const
    {
        return linkNum;
    }
    std::vector<std::vector<T>> getAdjList() const
    {
        return adjList;
    }
    // adjList-en végigiteráláshoz, hogy ne kelljen lemásolni az adatot:
    auto begin()   		{return adjList.begin();}
    auto cbegin() const {return adjList.cbegin();}
    auto end()      	{return adjList.end();}
    auto cend()   const	{return adjList.cend();}
    /*
    Q: hogy ne tudja módosítani a felhasználó az adjList-et, nem kéne csak cbegin-t és cend-et írnom?
    */

//műveletek
    void addNode(T const& node)
    {
        std::vector<T> v;
        for (int i = 0; i<nodeNum; i++)
        {
            v = adjList[i];
            if(std::find(v.begin(), v.end(), node) != v.end() && !v.empty())
            {
                std::cout << "this node is already in the graph.\n";
                return;
            }
        }

        adjList.push_back(std::vector<T> {node});
        nodeNum++;
        return;
    }

    void addLink(T const& node1, T const& node2)
    {
        if (node1 == node2)
        {
            std::cout<<"egyszeru graf nem tartalmazhat hurokelt!\n";
            return;
        }

        if (nodeNum <2)
        {
            std::cout<<"empty or single noded simple graph cannot contain a link.\n";
            return;
        }
        //várjunk csak. a find() az egy elemet keres meg egy vektorban. hogy kéne ezt vector<vector> - ra átírni elegánsan?
        int N1 = -1;
        int N2 = -1;
        for(int i=0; i<nodeNum; i++)
        {
            std::vector<T> v = adjList[i];
            if(v[0] == node1)
            {
                if (std::find(v.begin(), v.end(), node2) == v.end() && !v.empty())
                {
                    N1 = i;
                }
            }
            if(v[0] == node2)
            {
                if (std::find(v.begin(), v.end(), node1) == v.end() && !v.empty())
                {
                    N2 = i;
                }
            }
        }
        
        if(N1 != -1 && N2 != -1)
        {
            linkNum++;
            adjList[N1].push_back(node2);
            adjList[N2].push_back(node1);
            //hát, ennél lehet hogy célravezetőbb, ha csak int-ekből lehet gráfot csinálni, és egyből tudom hogy hanyadik elemeket akarom összehuzalozni egy-egy push_back-kel...
        }
        else
        {
            std::cout<<"sikertelen elhozzaadas.\n";
        }
        
        return;
    }

};

//kiírás
template<typename T>
std::ostream& operator<< (std::ostream& o, graph<T> const& g)
{
    int size = 0;
    o << g.getNodeNum() << ", " << g.getLinkNum() << std::endl;
    std::for_each(g.cbegin(), g.cend(), [&](std::vector<T> v)
        {
            size = v.size();
            if (size != 0)
            {
                for(int i=0; i<size-1; i++)
                {
                    o << v[i] << ", ";
                }
                o << v[size-1] << std::endl;
            }
        });
    return o;
}




//-----------------------------------------------------------------------------------------------------------
template<typename T>
struct distr
{
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

    //adatfeltöltés
    void feltolt(graph<T> const& graf)
    {
        std::vector<std::vector<T>> adjList = graf.getAdjList();
        double nodeNum = (double)graf.getNodeNum();
        std::vector<int> ranks;
        std::for_each(adjList.begin(), adjList.end(), [=](std::vector<T> v){ranks.push_back(v.size()-1);});
        std::for_each(ranks.begin(), ranks.end(), [=](int rank)
                        {
                            for(int i=0; i<binNum-1; i++)
                            {
                                if(mini+((double)i*binsize)<rank && rank<mini+(((double)i+1.0)*binsize)
                                {
                                    hist[i]++;
                                    break;
                                }
                            }
                            return;
                        });
        std::for_each(hist.begin(), hist.end(), [=](double data){data /= nodeNum;});
        return;
    }    
};