/* File:        RandInt.cpp 
 * Author:      Caio Rodrigues 
 * Description: Class for simplification and easy use of C++11 uniform distribution 
 ********************************************************************************/ 

#include <iostream>
#include <random>
#include <iomanip>
#include <string>

template<typename TInt = int>
class RandInt {
private:
	std::random_device                 m_nextSeed;	
	unsigned int                       m_seed;
	std::default_random_engine         m_engine;
	std::uniform_int_distribution<int> m_dist;
public:
	using TSeed = long;

	// Intialize with a known seed 
	RandInt(TInt min, TInt max, unsigned int seed)
		: m_seed(seed), m_engine(seed), m_dist(min, max)
	{		
	}

	// Initialize with a random seed 
	RandInt(TInt min, TInt max)
		: m_nextSeed{},
		  m_seed(m_nextSeed()),
		  m_engine(m_seed),
		  m_dist(min, max)
	{		
	}
	TInt  Min()  const { return m_dist.min(); }
	TInt  Max()  const { return m_dist.max(); }
	TSeed Seed() const { return m_seed; }
	void  Seed(TSeed seed)
	{
		m_seed = seed;
		m_engine.seed(seed);
	}
	// Set seed to a new random (non-deterministic) value and return it 
	TSeed NextSeed()
	{
		m_seed = m_nextSeed();
		m_engine.seed(m_seed);
		return m_seed;
	}
	// Get NExt random 
	TInt operator()()
	{
		return m_dist(m_engine);
	}
};


int main()
{
	std::cout << "\n ===== Random numbers with a random seed ===="
			  << std::endl;
	{
		RandInt<int> rnd(1, 10);
		std::cout << " => rnd.Seed() = " << rnd.Seed() << std::endl;
		std::cout << " => rnd.Min() = "  << rnd.Min() << std::endl;
		std::cout << " => rnd.Max() = "  << rnd.Max() << std::endl;

		for(int i = 0; i < 15; i++){
			auto field = std::string(" x[") + std::to_string(i) + "] = "; 
			std::cout << std::setw(10)
					  << field
					  << std::setw(5) << rnd() << std::endl;
		}		
	}	

	std::cout << "\n ===== Random numbers with a non-random seed ===="
			  << std::endl;
	{
		// Initialize Random generator object with known and fixed
		// seed to reproduce computation results. 
		long seed = 1195785783;
		RandInt<int> rnd(1, 10, seed);
		std::cout << " => rnd.Seed() = " << rnd.Seed() << std::endl;
		std::cout << " => rnd.Min() = "  << rnd.Min() << std::endl;
		std::cout << " => rnd.Max() = "  << rnd.Max() << std::endl;

		/* Expected sequence: 7, 10, 9, 2, 2, 1, 3, 1, 6, 5, 1, 2, 3, 2 ... */ 
		for(int i = 0; i < 15; i++){
			auto field = std::string(" x[") + std::to_string(i) + "] = "; 
			std::cout << std::setw(10)
					  << field
					  << std::setw(5) << rnd() << std::endl;
		}		
	}	
	
	return 0;
}
