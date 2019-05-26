#include "matrix.h"
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <iomanip>

//időméréshez
double minOfTrials(int const& numOfTrials, int const& N, 
					std::random_device & rd, std::minstd_rand & gen, 
					std::uniform_real_distribution<double> & unif)
{
    std::vector<double> trialTimes(numOfTrials);
    //időmérés numOfTrial-szor:
    for(int i=0; i < numOfTrials; i++)
    {
	MatrixNxN<double> m1(N, [&](int i){return unif(gen);});
    	MatrixNxN<double> m2(N, [&](int i){return unif(gen);});
        auto t_begin = std::chrono::high_resolution_clock::now();
        MatrixNxN<double> m = m1*m2;
        auto t_end = std::chrono::high_resolution_clock::now();
        trialTimes[i]= (static_cast<std::chrono::duration<double, std::micro>>(t_end-t_begin)).count();
    }    
    return *std::min_element(trialTimes.begin(), trialTimes.end());
}

int main() {

    std::random_device rd{};
    std::minstd_rand gen(rd());
    std::uniform_real_distribution<double> unif(0, 100);

	std::vector<int> sizeNs;
    std::vector<double> mulTimes32;
	std::vector<double> mulTimes1;
    for(int i=2; i < 515; i+=4)
	{
        sizeNs.push_back(i);
        mulTimes32.push_back(minOfTrials(32, i, rd, gen, unif));
		mulTimes1.push_back(minOfTrials(1, i, rd, gen, unif));
    }
    unsigned int n = mulTimes32.size();
	std::ofstream output32;
	std::ofstream output1;
  	output32.open("timesOfMulti32.txt");
	output1.open("timesOfMulti1.txt");
	if(!output32.is_open() || !output1.is_open())
	{
		std::cout << "sikertelen fajliras!\n";
		return -1;
	}
	std::cout << std::setprecision(16);
  	for(unsigned int i=0; i<n; i++)
    {
        output32 << sizeNs[i] << " " << mulTimes32[i] << std::endl;
		output1  << sizeNs[i] << " " << mulTimes1[i]  << std::endl;
    }  
	output32.close();
	output1.close();
    
    return 0;
}
