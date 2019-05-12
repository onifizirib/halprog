#include <iostream>
#include "distr.h"

int main(int, char**) {
    std::cout << "Hello, world!\n";
    graph<int> proba;
    int A = 9;
    int B = 8;
    int C = 4;
    proba.addNode(A);
    proba.addNode(B);
    proba.addNode(B);
    proba.addNode(C);
    proba.addLink(A, B);
    proba.addLink(A, B);
    std::vector<std::vector<int>> v = proba.getAdjList();
    std::cout<< "adjList hossza: " << v.size() << ", NodeNum: " << proba.getNodeNum() <<"\n";
    std::cout<< "adjList elemeinek hossza: ";
    for(int i=0; i<v.size(); i++)
    {
        std::cout << v[i].size() << ", ";
    }
    std::cout << "linkNum: " << proba.getLinkNum() <<"\n";
    //std::cout<<v[0][0]<<", "<<v[0][1]<<"\n";
    //std::cout<<v[1][0]<<", "<<v[1][1]<<"\n";
    std::cout << proba;

    std::cout << "-----------------\n";
    graph<int> proba2(10);
    proba2.addLink(0,3);
    proba2.addLink(6,3);
    proba2.addLink(7,3);
    proba2.addLink(2,5);
    proba2.addLink(5,3);
    proba2.addLink(2,3);
    std::cout << proba2;
    std::vector<std::vector<int>> wat = proba2.getAdjList();

    std::cout << "---distr test---\n";
    int binNum = 3;
    distr D(0.0, 6.0, binNum);
    std::cout<<"binnum="<<D.binNum <<", binsize=" <<D.binsize << ", num of intervals=" << D.hist.size() <<"\n";
    // különben valamiért double(5)-ként értelmezi, és a binSize-os konstruktort hívja meg..
    D.feltolt(proba2);
    std::cout << "-----------------\n" <<D;

    std::cout << "---clustering coefficient test---\n";
    std::vector<double> clusterC;
    clusterC.resize(proba2.getNodeNum());
    int i = 0;
    std::for_each(clusterC.begin(), clusterC.end(), [&](double& coeff)
                {
                    coeff = clusCoeff(wat[i][0], proba2);
                    std::cout << "c" << i << " = " << coeff << std::endl;
                    i++;                    
                });
    double avgC = avgClusCoeff(proba2);
    std::cout << "avg clustering coeff = " << avgC << "\n";
    std::cout << "---------\n";
    /*
    std::vector<int> nei3 = findNeighbors(wat[3][0],proba2);
    //std::for_each(nei3.begin(), nei3.end(), [&](int const& node){ std::cout << node << std::endl;});
    std::cout << "wat[3][0]: " << wat[3][0] << " ->nei3.size: " << nei3.size() << "\n";
    for(int i=0; i<nei3.size();i++){std::cout<<nei3[i]<<" ";}

    std::vector<int> nei5 = findNeighbors(wat[5][0],proba2);
    std::cout << "\nwat[5][0]: " << wat[5][0] << " ->nei5.size: " << nei5.size() << "\n";
    for(int i=0; i<nei5.size();i++){std::cout<<nei5[i]<<" ";}

    std::vector<int> com35 = commonNodes(nei3, nei5);
    std::cout << "\ncommon element of nei3 and nei5:\n";
    for(int i = 0; i< com35.size(); i++){std::cout << com35[i] << " ";}
    */

   std::cout << "---avgPathLen test---\n";
   std::cout << "for node 3:\n" << avgPathLenFromNode(wat[3][0], proba2) << "\n";
   proba2.addLink(1,5);
   proba2.addLink(4,1);
   proba2.addLink(4,8);
   proba2.addLink(9,7);
   std::cout << "for node 3:\n" << avgPathLenFromNode(wat[3][0], proba2) << "\n";
   proba2.addLink(0,4);
   std::cout << "for node 3:\n" << avgPathLenFromNode(wat[3][0], proba2) << "\n";

   /*
   std::vector<std::vector<int>> tesztV;
   tesztV.push_back(std::vector<int>{});
   std::cout << "tesztV merete: " << tesztV.size() << ", elso elemenek merete:" <<tesztV[0].size() << std::endl;
   std::cout << "elso elem empty: " << tesztV[0].empty() << "\n";
   */
   
}