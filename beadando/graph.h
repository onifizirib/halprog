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
        std::for_each(adjList.begin(), adjList.end(), [&](std::vector<T>& v) mutable
                    {
                        v.push_back((T)i);
                        i++;
                        return;
                        // ha [=](std::vector<T> v){...} a szintaxis, akkor nem tudja i-t növelni
                    });
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
        //vector<vector> nulladik "oszlopában" elem keresése, index visszaadása, ha megtaláltatott és nincs abban a sorban egy spec. másik elem
        int N1 = -1;
        int N2 = -1;
        std::vector<T> v;
        for(int i=0; i<nodeNum; i++)
        {
            v = adjList[i];
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

    // ezt azt hiszem nem igazán használom sehol..
    bool doesContain(T const& node) const
    {
        bool contain = false;
        std::for_each(adjList.begin(), adjList.end(), [&](std::vector<T> const& v)
                    {
                        if (v[0] == node) 
                        {
                            contain = true;
                            break;
                        }
                    });
        return contain;
    }

    int indexOfNode(T const& node) const
    {
        if(nodeNum==0)
        {
            return -1;
        }
        for (int i=0; i<nodeNum; i++)
        {
            if(adjList[i][0] == node) {return i;}
        }
        return -1;
    }

};

//kiírás
template<typename T>
std::ostream& operator<< (std::ostream& o, graph<T> const& g)
{
    int size = 0;
    o << g.getNodeNum() << ", " << g.getLinkNum() << std::endl;
    std::for_each(g.cbegin(), g.cend(), [&](std::vector<T> const& v) mutable
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

//egy elem szomszédlistája
template<typename T>
std::vector<T> findNeighbors(T const& node, graph<T> const& g)
{
    std::vector<T> neigh;
    int ind = g.indexOfNode(node);
    if (ind != -1)
    {
        neigh = g.getAdjList()[ind];
        neigh.erase(neigh.begin());
        return neigh;
    }
    std::cout << "the graph does not contain the given node.\n";
    return neigh;
}

//két szomszédlista közös elemei
template<typename T>
std::vector<T> commonNodes(std::vector<T> const& v1, std::vector<T> const& v2)
{
    std::vector<T> common;
    if (v1.empty() || v2.empty())
    {
        std::cout << "empty vector given as input.\n";
        return common;
    }
    std::for_each(v1.begin(), v1.end(), [&](T const& node) mutable
                    {
                        if (std::find(v2.begin(), v2.end(), node) != v2.end())
                        {
                            common.push_back(node);
                        }
                    });
    return common;
}

// clustering coefficient
template<typename T>
double clusCoeff(T const& node, graph<T> const& g)
{
    int lbn = 0;    //links between nodes (x2)
    std::vector<T> neigh = findNeighbors(node, g);
    double neiSize = double(neigh.size());
    // per definition, if neiSize < 2 then clusCoeff = 0.
    if (neiSize < 2) {return 0.0;}

    std::vector<T> neinei;
    std::for_each(neigh.begin(), neigh.end(), [&](T const& neiNode) mutable
    {
        neinei = findNeighbors(neiNode, g);
        lbn += commonNodes(neigh, neinei).size();
    });   
    
    return (double(lbn)) / (neiSize*(neiSize-1.0));
}

// average clustering coefficient for the whole graph
template<typename T>
double avgClusCoeff(graph<T> const& g)
{
    double avgC = 0.0;
    int nodenum = g.getNodeNum();
    if (nodenum == 0)
    {
        std::cout << "the graph has zero nodes.\n";
        return 0.0;
    }
    T node;
    for(int i=0; i<nodenum; i++)
    {
        T node = g.getAdjList()[i][0];
        avgC += clusCoeff(node, g);
    }
    return avgC/(double)nodenum;
    //nem ártana azért egy standard error of mean se ehhez..
}

// average shortest path to a given node
template<typename T>
double avgPathLenFromNode(T const& startNode, graph<T> const& g)
{
    std::vector<std::vector<T>> adjList = g.getAdjList();
    std::vector<std::vector<T>> level;
    int lvl = 0;
    std::vector<T> canReach;
    std::vector<T> toReach;
    int nodenum = g.getNodeNum();
    if (nodenum == 0) 
    {
        std::cout << "empty graph is given.\n";
        return -1.0;
    }

    canReach.push_back(startNode); 
    level.push_back(std::vector<T>{startNode}); 
    std::for_each(adjList.begin(), adjList.end(), [&](std::vector<T> const& v) mutable
                {
                    if (v[0] != startNode) {toReach.push_back(v[0]);}                    
                });
    
    std::vector<T> neigh;
    int iternum = 0;
    while(!toReach.empty() && !level[lvl].empty() && iternum < 10)
    {
        std::cout << "melyseg: "<< lvl << std::endl;
        level.push_back(std::vector<T>{});
        std::for_each(level[lvl].begin(), level[lvl].end(), [&](T const& node) mutable
                    {
                        neigh = findNeighbors(node, g);
                        std::for_each(neigh.begin(), neigh.end(), [&](T const& nei) mutable
                                    {
                                        //std::cout << nei << std::endl;
                                        if(std::find(canReach.begin(), canReach.end(), nei) == canReach.end())
                                        {
                                            level[lvl+1].push_back(nei);
                                            canReach.push_back(nei);
                                            toReach.erase(std::find(toReach.begin(), toReach.end(), nei));
                                        }
                                    });                        
                    });
        lvl++;
        iternum++;
    }

    if(!toReach.empty())
    {
        std::cout << "a graf nem osszefuggo!\n";
        return -1.0;
    }

    int sumPathLen = 0;
    lvl = 0;
    std::for_each(level.begin(), level.end(), [&](std::vector<T> v) mutable
                {
                    sumPathLen += v.size()*lvl;
                    lvl++;
                });
    
    return (double)sumPathLen / (double)(nodenum-1);
}