#include <iostream>
#include <string>
//#include <cstring>
//#include <sstream>
#include <fstream>
#include "statistics.h"

int main(int, char**) {
    
{    
    std::cout << "Hello, world!\n";
    graph< std::string > proba;
    std::string A = "elso";
    std::string B = "masodik";
    std::string C = "harmadik";
    proba.addNode(A);
    proba.addNode(B);
    proba.addNode(B);
    proba.addNode(C);
    proba.addLink(A, B);
    proba.addLink(A, B);
    std::vector<std::vector<std::string>> const v = proba.getAdjList();
    std::cout<<"---------\n";
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

    std::cout << "---link deletion then node deletion---\n";
    proba.deleteLink(B,A);
    proba.deleteNode(B);    
    proba.deleteLink(C,A);
    proba.deleteNode(B);
    std::cout << proba;

//------------------
    graph<std::string> proba1 = proba;
    proba1 = std::move(proba);
}
//------------------
{
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

//------------------

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
   std::cout << "after adding links, pathLen for node 3:\n" << avgPathLenFromNode(3, proba2) << "\n";
   proba2.addLink(0,4);
   std::cout << "after adding links, pathLen for node 3:\n" << avgPathLenFromNode(3, proba2) << "\n";

   std::cout << "for the proba2 graph: " << avgPathLen(proba2) << "\n";
   for(int i=0; i<wat.size(); i++)
   {
       std::cout<<i<<": " <<avgPathLenFromNode(i, proba2) << std::endl;
   }
   std::cout << "-----avg clustering coeff test-----\n";
   std::cout << "for node 3:\n" << clusCoeff(wat[3][0], proba2) << "\n";
   std::cout << "for the proba2 graph: " << avgClusCoeff(proba2) << "\n";
   std::cout << "---------\n";
}
//-------------------------------------------------------------------
{
   std::cout << "----------Erdos-Renyi test----------\n";
   int nodenum = 200;
   double linkprob = 0.3;
   graph<int> erdos = ER(nodenum, linkprob);
   //std::cout << erdos;
   statistics erdos_gen(erdos);
   statistics erdos_exp(nodenum, linkprob);
    //itt remélem hogy nem a BA model predikciót hívja meg, mert szeret konvertálgatni int-et double-lá -.-

   std::vector<double> differOfDist;
   differOfDist.resize(nodenum);
   //std::cout<<"elementwise abs difference of degree distribution: ";
   for(int i=0; i<nodenum; i++)
   {
       differOfDist[i] = (std::abs(erdos_gen.degreeDist.hist[i]-erdos_exp.degreeDist.hist[i]));
       //std::cout << differOfDist[i] <<", ";
   }
   //std::cout<<std::endl;
   std::cout<<"sum of elementwise abs difference between degree dist-s: "<<std::accumulate(differOfDist.begin(), differOfDist.end(), 0.0)<<std::endl;
   
   std::cout << "expected clustering coefficient = " << erdos_exp.avgClus << std::endl;
   std::cout << "   difference of average clustering coefficient: "<< std::abs(erdos_gen.avgClus - erdos_exp.avgClus) << "\n";
   std::cout << "expected average shortest path = " << erdos_exp.avgPath << std::endl;
   std::cout << "   difference of average shortest path: "<< std::abs(erdos_gen.avgPath - erdos_exp.avgPath) << "\n";
   std::cout << "expected number of links = " << erdos_exp.linkNum << std::endl;
   std::cout << "   difference of link number: "<< std::abs(erdos_gen.linkNum - erdos_exp.linkNum) << "\n";
   std::cout << "expected average degree = " << erdos_exp.avgDegr << std::endl;
   std::cout << "   difference of average degree: "<< std::abs(erdos_gen.avgDegr - erdos_exp.avgDegr) << "\n";
   std::cout << "----------------------\n";

   //statistics kiiratása
   std::ofstream outfile;
   outfile.open("erdos.txt");
   if(outfile.is_open())
   {
       outfile << erdos_gen;
       outfile << erdos_exp;
       outfile.close();
   }
   else{ std::cout << "Could not open output file.\n"; }
}

//--------------------------------------------------------------------------------------------------------
{
   std::cout << "---------Watts-Strogatz test---------\n";
   int q = 2;
   double beta = 0.0;
   graph<int> watts = WS(6, q, beta);
   std::cout << watts << "\n"; 
   
   watts = WS(62, q, 0.0);  
   std::cout << "----beta changing test----\n";
   std::cout << "WS(62,q,0): avg path length from node0 = " << avgPathLenFromNode(0,watts) << "\n";
   watts = WS(62, q, 0.2);
   std::cout << "WS(62,q,0.2): avg path length from node0 = " << avgPathLenFromNode(0,watts) << "\n";
}
//--------------------------------------------------------------------------------------------------------- 
{
   int nodenum = 200;
   int q = 3;
   double beta = 1.0;
   std::cout << "---------------WS(" << nodenum << ", " << q << ", " << beta << ") test--------------------\n";
   graph<int> watts = WS(nodenum, q, beta);
   statistics watts_gen(watts);
   statistics watts_exp(nodenum, q, beta);
   
   std::cout<<"there was no expectation for the degree distribution."<<std::endl;   
   std::cout << "expected clustering coefficient = " << watts_exp.avgClus << std::endl;
   std::cout << "   difference of average clustering coefficient: "<< std::abs(watts_gen.avgClus - watts_exp.avgClus) << "\n";
   std::cout << "expected average shortest path = " << watts_exp.avgPath << std::endl;
   std::cout << "   difference of average shortest path: "<< std::abs(watts_gen.avgPath - watts_exp.avgPath) << "\n";
   std::cout << "expected number of links = " << watts_exp.linkNum << std::endl;
   std::cout << "   difference of link number: "<< std::abs(watts_gen.linkNum - watts_exp.linkNum) << "\n";
   std::cout << "expected average degree = " << watts_exp.avgDegr << std::endl;
   std::cout << "   difference of average degree: "<< std::abs(watts_gen.avgDegr - watts_exp.avgDegr) << "\n";
   std::cout << "----------------------\n";
}
//---------------------------------------------------------------------------------------------------------
{
   int nodenum = 200;
   int q = 3;
   double beta = 0.0;
   std::cout << "-----------------WS(" << nodenum << ", " << q << ", "<< beta << ") test-------------------\n";
   graph<int> watts = WS(nodenum, q, beta);
   statistics watts_gen(watts);
   statistics watts_exp(nodenum, q, beta);
   
   std::cout<<"there was no expectation for the degree distribution."<<std::endl;   
   std::cout << "expected clustering coefficient = " << watts_exp.avgClus << std::endl;
   std::cout << "   difference of average clustering coefficient: "<< std::abs(watts_gen.avgClus - watts_exp.avgClus) << "\n";
   std::cout << "expected average shortest path = " << watts_exp.avgPath << std::endl;
   std::cout << "   difference of average shortest path: "<< std::abs(watts_gen.avgPath - watts_exp.avgPath) << "\n";
   std::cout << "expected number of links = " << watts_exp.linkNum << std::endl;
   std::cout << "   difference of link number: "<< std::abs(watts_gen.linkNum - watts_exp.linkNum) << "\n";
   std::cout << "expected average degree = " << watts_exp.avgDegr << std::endl;
   std::cout << "   difference of average degree: "<< std::abs(watts_gen.avgDegr - watts_exp.avgDegr) << "\n";
   std::cout << "----------------------\n";
}

//---------------------------------------------------------------------------------------------------------
{
    std::cout << "------------------Barabasi-Albert test-------------------\n";
    int m = 3;
    int T = 10;
    graph<int> barabasi = BA(m, T);
    //std::cout<<barabasi;
}
{
    int m = 3;
    int T = 200;
    graph<int> barabasi = BA(m, T);
    statistics BA_gen(barabasi);
    statistics BA_exp(m,T);
 
   std::cout << "expected clustering coefficient = " << BA_exp.avgClus << std::endl;
   std::cout << "   difference of average clustering coefficient: "<< std::abs(BA_gen.avgClus - BA_exp.avgClus) << "\n";
   std::cout << "expected average shortest path = " << BA_exp.avgPath << std::endl;
   std::cout << "   difference of average shortest path: "<< std::abs(BA_gen.avgPath - BA_exp.avgPath) << "\n";
   std::cout << "expected number of links = " << BA_exp.linkNum << std::endl;
   std::cout << "   difference of link number: "<< std::abs(BA_gen.linkNum - BA_exp.linkNum) << "\n";
   std::cout << "expected average degree = " << BA_exp.avgDegr << std::endl;
   std::cout << "   difference of average degree: "<< std::abs(BA_gen.avgDegr - BA_exp.avgDegr) << "\n";
   std::cout << "----------------------\n";

   //statistics kiiratása
   std::ofstream outfile;
   outfile.open("BA.txt");
   if(outfile.is_open())
   {
       outfile << BA_gen;
       outfile << BA_exp;
       outfile.close();
   }
   else{ std::cout << "Could not open output file.\n"; }
}
}
