/*
 * Dice.hpp
 *
 *  Created on: Jan 17, 2017
 *      Author: softserve
 */

#pragma once
#ifndef SRC_DICE_HPP_
#define SRC_DICE_HPP_

#include <set>
using std::multiset

class Dice {
private:
	Dice();
public:
	//enumeration of dices
	enum dices {
		D4   = 4,
		D6   = 6,
		D8   = 8,
		D10  = 10,
		D20  = 20,
		D100 = 100
	};

	// Static method for single roll
	static int fRoll(const dices & dn);

	// Static method for multiple rolls
	static int fMultipleRoll(const dices & dn, const int & times);
    
    // Static method for multiple rolls where we count the sum of defined number of dice
    static int fMultipleRollSum(const dices & dn, const int & times, const int & dices_to_count);
};

#endif /* SRC_DICE_HPP_ */
