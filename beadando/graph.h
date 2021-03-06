#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cmath>
#include <random>


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
                        // ha [=](std::vector<T> v){...} a szintaxis, akkor nem tudja i-t növelni
                    });
    }

//operátor=
    graph<T>& operator=(graph<T> const& cpy)
    {
        if(&cpy == this) {return *this;}

        nodeNum = cpy.getNodeNum();
        linkNum = cpy.getLinkNum();
        //adjList = cpy.getAdjList();
        //várjunk csak. ez referencia szerint adja át, nem másolja az adatot!
        adjList.resize(nodeNum);
        std::copy(cpy.getAdjList().begin(), cpy.getAdjList().end(), adjList.begin());
        
        return *this;
    }
    /*
    graph<T>& operator=(graph<T> && mv)
    {
        if(&mv == this) {return *this;}

        adjList = std::move(mv.getAdjList());
        //ennek a sornak van értelme? tényleg az fog történni, aminek kéne? Le fog-e nullázódni az mv.adjList?

        nodeNum = std::move(mv.getNodeNum());
        linkNum = std::move(mv.getLinkNum());		
		//skalár nem move-olható, másolódni fog -> kinullázni érdemes
		mv.nodeNum = (T)0;
        mv.linkNum = (T)0;
        //nem valid parancs, nem férek hozzá az mv.nodeNUm-hoz!

        return *this;
    }
    */

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
    const std::vector<std::vector<T>>& getAdjList() const
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

//lekérdezések

    int indexOfNode(T const& node) const
    {
        if(nodeNum==0)
        {
            std::cout << "node search in empty graph.\n";
            return -1;
        }
        for (int i=0; i<nodeNum; i++)
        {
            if(adjList[i][0] == node) {return i;}
            //nem tudjuk túlindexelni, mert a node maga mindenképp eleme az adjList[i]-nek
        }
        std::cout << "graph does not contain this node.\n";
        return -1;
    }

    std::vector<int> nodeIndsOfLink(T const& nodeA, T const& nodeB) const
    {
        if (linkNum == 0)
        {
            std::cout << "link search in linkless graph.\n";
            return std::vector<int>{};
        }

        int A = indexOfNode(nodeA);
        int B = indexOfNode(nodeB);
        if (A == -1 || B == -1)
        {
            std::cout << "graph does not contain given node.\n";
            return std::vector<int>{};
        }
        
        if (std::find(adjList[A].begin(), adjList[A].end(), nodeB) != adjList[A].end())
            //adjList[B]-re nem kell letesztelni, hogy nodeA benne van-e, mert mindig szimmetrikus az állapot
        {
            return std::vector<int>{A, B};
        }

        return std::vector<int>{};
    }

    int degree(T const& node) const
    {
        int ind = indexOfNode(node);
        if (ind == -1) {return -1;}
        return adjList[ind].size()-1;
    }

    double avgDegree() const
    {
        /*
        double avgDegree = 0.0;        
        std::for_each(adjList.cbegin(), adjList.cend(), [&](std::vector<T> const& v)
        {
            avgDegree += v.size()-1;
        });
        return avgDegree/double(nodeNum);
        */
       //ráébredtem, hogy minden gráfra igaz, hogy a (fokszámok összege) = 2*linkNum, s ezt csak le kell osztani nodeNum-mal.

       return 2.0*double(linkNum)/(double)nodeNum;
    }


//műveletek
    void addNode(T const& node)
    {
        if(indexOfNode(node) != -1) {return;}
        adjList.push_back(std::vector<T> {node});
        nodeNum++;
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
        //vector<vector> nulladik "oszlopában" elem keresése, index visszaadása, ha [megtaláltatott ÉS nincs abban a sorban egy spec. másik elem].
        int N1 = -1;
        int N2 = -1;
        std::vector<T> v;
        for(int i=0; i<nodeNum; i++)    //for ciklussal dolgozom, mert szükségem van a futóindex értékére.
        {
            v = adjList[i];
            if(v[0] == node1)
            {
                if (std::find(v.begin(), v.end(), node2) == v.end())
                    // && !v.empty()-t nem kell hozzátenni, elvégre v[0]=node1, tehát nem üres a v.
                {
                    N1 = i;
                }
            }
            if(v[0] == node2)
            {
                if (std::find(v.begin(), v.end(), node1) == v.end())
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
    }

    void deleteNode(T const& node)
    {
        int nodeInd = indexOfNode(node);
        if(nodeInd == -1){return;}

        std::vector<T> linkedWith = adjList[nodeInd];
        linkedWith.erase(linkedWith.begin());
        int neighInd = -1;
        std::for_each(linkedWith.begin(), linkedWith.end(), [&](T& neigh)
                    {
                        neighInd = indexOfNode(neigh);
                        adjList[neighInd].erase(std::find(adjList[neighInd].begin(), adjList[neighInd].end(), node));
                        linkNum--;
                        // elvileg ez nem hajtódik végre, ha linkedWith üres
                        // Q: érdemes-e itt valami ügyes referencia szerinti egyenlőség pl. neiLink = adjList[neighInd] és utána neiLink-et használata indexelés helyett?
                    });
        adjList.erase(adjList.begin() + nodeInd);
        nodeNum--;
    }

    void deleteLink(T const& nodeA, T const& nodeB)
    {
        std::vector<int> nodeInd = nodeIndsOfLink(nodeA, nodeB);
        if (nodeInd.empty()) {return;}
        
        int A = nodeInd[0];
        int B = nodeInd[1];
        adjList[A].erase(std::find(adjList[A].begin(), adjList[A].end(), nodeB));
        adjList[B].erase(std::find(adjList[B].begin(), adjList[B].end(), nodeA));
        linkNum--;
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
    std::vector<std::vector<T>> const& adjList = g.getAdjList();
    if (ind != -1)
    {
        neigh = adjList[ind];
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
    std::vector<std::vector<T>> const& adjList = g.getAdjList();
    T node;
    for(int i=0; i<nodenum; i++)
    {
        node = adjList[i][0];
        avgC += clusCoeff(node, g);
    }
    return avgC/(double)nodenum;
    //nem ártana azért egy standard error of mean se ehhez..
}

// average shortest path to a given node
template<typename T>
double avgPathLenFromNode(T const& startNode, graph<T> const& g)
{
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
    std::for_each(g.cbegin(), g.cend(), [&](std::vector<T> const& v) mutable
                {
                    if (v[0] != startNode) {toReach.push_back(v[0]);}                    
                });
    
    std::vector<T> neigh;
    int iternum = 0;
    while(!toReach.empty() && !level[lvl].empty())
    {
        //std::cout << "melyseg: "<< lvl << std::endl;
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
        //gráf nem összefüggő, úthossz végtelen -> ehelyett negatívat adok vissza return-nel.
    }

    int sumPathLen = 0;
    lvl = 0;
    std::for_each(level.begin(), level.end(), [&](std::vector<T>& v) mutable
                {
                    sumPathLen += v.size()*lvl;
                    lvl++;
                });
    
    return (double)sumPathLen / (double)(nodenum-1);
}

template<typename T>
double avgPathLen(graph<T> const& g)
{
    std::vector<std::vector<T>> const& adjList = g.getAdjList();
    double sumPathLen = 0.0;
    double pathLen = 0.0;
    double nodenum = (double)g.getNodeNum();
    for(int i = 0; i < nodenum; i++)
    {
        pathLen = avgPathLenFromNode(adjList[i][0], g);
        if(pathLen == -1)
        {
            //gráf nem összefüggő, úthossz végtelen -> ehelyett negatívat adok vissza return-nel.
            return -1.0;
        }
        sumPathLen += pathLen;
    }
    return sumPathLen/nodenum;
}

//Erdős-Rényi gráf generálása
graph<int> ER(int const& nodenum, double const& linkprob)
{       
    graph<int> graf(nodenum);
    //feltöltés élekkel
    std::random_device rd{};
    std::minstd_rand rng(rd());
    std::uniform_real_distribution<double> unif(0, 1);
    double rndNum;
    for(int i=0; i<nodenum; i++)
    {
        for(int j=nodenum-1; j>i; j--)
        {
            rndNum = unif(rng);
            if (rndNum < linkprob)
            {
                //std::cout << "unif random number: "<<rndNum<<" < "<< p <<"\n";
                graf.addLink(i,j);
            }
        }
    }
    return graf;
}

//Watts-Strogatz gráf generálása
graph<int> WS(int const& nodenum, int q, double const& rewireProb)
{
    int thres = floor((double)nodenum/2.0);
    if (q > thres) {q = thres;}

    std::random_device rd{};
    std::minstd_rand rng(rd());
    std::uniform_real_distribution<double> unif(0, 1);
    std::uniform_int_distribution<> rndNode(0, nodenum);
    std::uniform_int_distribution<> whichNode(0, 1);
    int remainNode = -1;

    graph<int> graf(nodenum);
    for(int i=1; i<q+1; i++)
    {
        for(int j=0; j<nodenum-i; j++)
        {
            if (unif(rng) < rewireProb)
             //random kell-e huzalozni (az egyik végét az élnek)?
            {
                if (whichNode(rng) == 0){ remainNode = j;  }
                else                    { remainNode = j+i;}
                graf.addLink(remainNode, rndNode(rng));
            }
            else
            {
                graf.addLink(j,j+i);
                 //ha nem kell random huzalozni, akkor gyűrű struktúra szerinti élhozzáadás
            }        
        }
        for(int j=0; j<i; j++)
        {
            if (unif(rng) < rewireProb)
             //random kell-e huzalozni (az egyik végét az élnek)?
            {
                if (whichNode(rng) == 0){ remainNode = j;           }
                else                    { remainNode = nodenum-i+j; }
                graf.addLink(remainNode, rndNode(rng));
            }
            else
            {
                graf.addLink(j,nodenum-i+j);
                 //ha nem kell random huzalozni, akkor gyűrű struktúra szerinti élhozzáadás
            }
        }
    }
    return graf;
}

//Barabási-Albert gráf generálása
graph<int> BA(int const& m, int const& T)
{
    if(m < 1) {std::cout << "m sould be at least 1.\n";}
    
    std::vector<int> ranks(m+T,0);
    std::vector<int> weights(m,0);
    
    //generate starting "full" graph
    graph<int> graf(m+T);
    for(int i=0; i<m; i++)
    {
        for(int j=m-1; j>i; j--)
        {
            graf.addLink(i,j);
        }
        ranks[i] = m-1;
    }
    weights[m-1] = m-1;
    
    std::random_device rd{};
    std::minstd_rand rng(rd());
    std::discrete_distribution<> disDist(weights.begin(), weights.end());
    std::uniform_int_distribution<> unif(4,5);
    
    std::vector<std::vector<int>> nodesWithRank(m);    
    int rndNum;
    int luckyNode;
    int aktMinRank = m-1;
    int aktMaxRank = m-1;    
    
    
    //adding m links to (m+t)-th node, links other side is random with a discrete distribution: p(node with degree k) ~ degree k
    for(int t=0; t<T; t++)
    {
        //diplaying actual state:
        /*
        {
            std::cout<<"------------iteration t="<<t<<"---------------\n";
            std::cout<<"ranks: ";
            for(int i=0; i<m+T; i++) {std::cout<<ranks[i]<<", ";}
            std::cout<<std::endl<<"weights: ";
            for(int i=0; i<weights.size(); i++) {std::cout<<weights[i]<<", ";}
            std::cout<<std::endl;
        }
        */

        //generating the other sides of the m links
        for(int i=0; i<m; i++)
        {
            rndNum = disDist(rng);
            //calculating which nodes have this randomised degree. at this point, we have m+t nodes
            for(int node=0; node<m+t; node++)
            {
                if(ranks[node] == rndNum) {nodesWithRank[i].push_back(node);}
            }
        }
        
        //from sufficient ranked nodes, we should choose according to uniform distribution
        for(int i=0; i<m; i++)
        {
            if (nodesWithRank[i].size() == 1)
            {
                luckyNode = nodesWithRank[i][0];
            }
            else
            {
                unif = std::uniform_int_distribution<> (0, (nodesWithRank[i].size()-1));
                 //ha erre az ágra esne a fenti is -> (unsigned int 1)-(int 1) -> valami random hatalmas szám, vagy épp int 0, a fene sem tudja
                rndNum = unif(rng);
                luckyNode = nodesWithRank[i][rndNum];
            }            
            graf.addLink(m+t, luckyNode);
            //accordingly, updating the rank of the lucky node
            ranks[luckyNode] = graf.degree(luckyNode);
            
            //clearing nodesWithRank[i] for the next t-iteration
            nodesWithRank[i].clear();
        }
        //now updating the rank of the (m+t)th node:
        ranks[m+t] = graf.degree(m+t);        
        //updating weight vector, for this we need the aktMinRank and aktMaxRank
        const auto [minIt, maxIt] = std::minmax_element(ranks.begin(), ranks.begin()+m+t+1);
        aktMinRank = *minIt;
        aktMaxRank = *maxIt;
        weights.resize(aktMaxRank+1);
        std::iota(weights.begin(), weights.end(), 0);
        for(int i=0; i<aktMinRank; i++)
        {
            weights[i]=0;
        }
        for(int i=aktMinRank; i<aktMaxRank; i++)
            {
                if(std::find(ranks.begin(), ranks.begin()+m+t+1, i) == ranks.begin()+m+t+1) {weights[i]=0;}
            }
        //updating discrete distribution according to weight vector
        disDist = std::discrete_distribution<> (weights.begin(), weights.end());
    }

//diplaying end state:
    /*
    {
        std::cout<<"........after T iterations, T="<<T<<".........\n";
        std::cout<<"ranks.size(): "<<ranks.size()<<std::endl;
        std::cout<<"ranks: ";
        for(int i=0; i<m+T; i++) {std::cout<<ranks[i]<<", ";}
        std::cout<<std::endl<<"weights.size(): "<<weights.size();
        std::cout<<std::endl<<"weights: ";
        for(int i=0; i<weights.size(); i++) {std::cout<<weights[i]<<", ";}
        std::cout<<std::endl<<"...................."<<std::endl;
    }
    */

    return graf;
}
