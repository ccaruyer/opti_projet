/* 
 Author  : Sebastien Verel
 Date    : 02/02/2022
 Licence : GNU General Public License as published by the Free Software Foundation, version 3 of the License
*/

#ifndef __Solution__
#define __Solution__

#include <vector>
#include <iostream>

class Solution {
public:
	/* can be completed */

	std::vector<unsigned> sigma;
	std::vector<double>   x;

	double fitness;

	std::ostream & printOn(std::ostream & _os) const {
		_os << fitness << " " << sigma.size() ;

		for(unsigned s : sigma) {
			_os << " " << s;
		}

		for(double xi : x) {
			_os << " " << xi;
		}

		return _os;
	}
};

std::ostream &operator<<(std::ostream & _os, Solution & s) { 
    return s.printOn(_os);
}

#endif