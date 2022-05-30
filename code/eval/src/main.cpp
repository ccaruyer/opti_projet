/* 
 Author  : Sebastien Verel
 Date    : 02/02/2022
 Licence : GNU General Public License as published by the Free Software Foundation, version 3 of the License
*/

// rapidjson/example/simpledom/simpledom.cpp`
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <random>

#include "posterEval.h"

//using namespace rapidjson;
using namespace std;

int main(int argc, char ** argv) {
    // Path to the poster information file in json format
    //const char * posterFileName = "/Users/verel/enseignement/21-22/m1-optimisation/eval/projet/prj-m1Optimisation_21-22/data/poster_48_0.json";
    const char * posterFileName = argv[1];

    PosterEval peval(posterFileName);

    // one basic solution : order of the index, same size of each position on the poster
    Solution solution;

    int numberOfPositions = peval.size();

    solution.sigma.resize(numberOfPositions);
    solution.x.resize(numberOfPositions);

    // basic solution
    for(int i = 0; i < numberOfPositions; i++) {
      solution.sigma[i] = i;
      solution.x[i] = 1.0;
    }

    // compute the fitness
    peval(solution);

    // print
    cout << solution << endl;

    // random solution

    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(solution.sigma), std::end(solution.sigma), rng);

    std::uniform_real_distribution<double> distribution(0.5, 1.5);
    double s = 0.0;
    for(int i = 0; i < numberOfPositions; i++) {
      solution.x[i] = distribution(rng);
      s += solution.x[i] * solution.x[i];
    }
    s = sqrt(s / solution.x.size());  // sum of square have to be equal n (size)

    for(int i = 0; i < numberOfPositions; i++)
      solution.x[i] /= s;

    // compute the fitness
    peval(solution);

    // print
    cout << solution << endl;

    return 0;
}
