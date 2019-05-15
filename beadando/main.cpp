#include <iostream>
#include "models.h"

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
    std::vector<std::vector<int>> const v = proba.getAdjList();
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

    std::cout << "link deletion then node deletion\n";
    proba.deleteLink(B,A);
    proba.deleteNode(B);    
    proba.deleteLink(C,A);
    proba.deleteNode(B);
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
    std::vector<std::vector<int>> const wat = proba2.getAdjList();

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
    //test for findNeighbors and commonNodes
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
   std::cout << "after adding links, coeff for node 3:\n" << avgPathLenFromNode(3, proba2) << "\n";
   proba2.addLink(0,4);
   std::cout << "after adding links, coeff for node 3:\n" << avgPathLenFromNode(3, proba2) << "\n";

   std::cout << "for the proba2 graph: " << avgPathLen(proba2) << "\n";
   for(int i=0; i<wat.size(); i++)
   {
       std::cout<<i<<": " <<avgPathLenFromNode(i, proba2) << std::endl;
   }

   std::cout << "-----------------\n";
   int nodenum = 200;
   double linkprob = 0.6;
   ER erdos(nodenum, linkprob);
   //std::cout << erdos.graf;
   ER_exp vartErdos(nodenum,linkprob);

   std::vector<double> differOfDist;
   std::cout<<"elementwise abs difference of degree distribution: ";
   for(int i=0; i<nodenum; i++)
   {
       differOfDist.push_back(std::abs(erdos.degreeDist.hist[i]-vartErdos.degreeDist_exp.hist[i]));
       std::cout << differOfDist[i] <<", ";
   }
   std::cout<<std::endl;
   std::cout<<"sum of elementwise abs difference between degree dist-s: "<<std::accumulate(differOfDist.begin(), differOfDist.end(), 0.0)<<std::endl;
   
   std::cout << "expected clustering coefficient = " << vartErdos.c_exp << std::endl;
   std::cout<<"   difference of average clustering coefficient: "<<std::abs(erdos.c-vartErdos.c_exp)<<"\n";
   std::cout << "expected average shortest path = " << vartErdos.l_exp << std::endl;
   std::cout<<"   difference of average shortest path: "<<std::abs(erdos.l-vartErdos.l_exp)<<"\n";
   std::cout << "expected number of links = " << vartErdos.M_exp << std::endl;
   std::cout<<"   difference of link number: "<<std::abs(erdos.M-vartErdos.M_exp)<<"\n";
   std::cout << "expected average degree = " << vartErdos.k_exp << std::endl;
   std::cout<<"   difference of average degree: "<<std::abs(erdos.k-vartErdos.k_exp)<<"\n";   
}
