#include<iostream>
#include<random>



int main()
{
	// p = 1/2 success, q = 1 - 1/2 = 1/2 fail 
	std::bernoulli_distribution p(0.5);
	// Produce random unsigned integer
	std::random_device rd;  // generate a random number that will be used as a seed 
	std::mt19937 gen(rd()); // psudeo-random number generator 


	std::vector<int> list;
	list.resize(20);
	bool flag  = p(gen);

	for (auto ptr = list.begin(); ptr != list.end(); ptr++) 
	{
		*ptr = p(gen); 
	}


	for (auto ptr = list.begin(); ptr != list.end(); ptr++) 
	{
		printf("%d",*ptr);
	}

	return 0; 
}