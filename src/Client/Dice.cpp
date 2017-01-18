#include "Includes/Dice.hpp"
#include <random>

int Dice::fRoll(const dices & dn)
{
	std::random_device ran_dev;   // creating random device
	std::mt19937 gen(ran_dev());  // Mersenne Twister pseudo random number generator
	std::uniform_int_distribution<> distr(1, dn); // uniform distribution from 1 to Dn
	return distr(gen);
}

int Dice::fMultipleRoll(const dices & dn, const int & times)
{
	if (times <= 0)  // checking if times less or equal 0
		return 0;

	int sum_of_rolls = 0;
	for(int i = 0; i < times; i++)
		sum_of_rolls += fRoll(dn);
	return sum_of_rolls;
}

