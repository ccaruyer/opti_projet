/* 
 Author  : Sebastien Verel
 Date    : 02/02/2022
 Licence : GNU General Public License as published by the Free Software Foundation, version 3 of the License
*/

#ifndef __posterEval__
#define __posterEval__

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>

#include "solution.h"

using namespace rapidjson;
using namespace std;

struct Place {
    double x;
    double y;
    double width;
    double height;
};

class PosterEval {
public:
    PosterEval(const char * posterFileName, double _beta1 = 1, double _beta2 = 2) : beta1(_beta1), beta2(_beta2) {
        Document poster;

        // read the file
        std::ifstream f(posterFileName);
        std::string lines((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        f.close();

        // parser the json file
        poster.Parse(lines.c_str());

        computePhotoDistances(poster);
        readPositions(poster);
    }

    /**
     * Fonction objectif (à minimiser):
     *   distance entre les photos pondérées par le score entre les positions
     *
     */
    void operator()(Solution & solution) {
        double sum = 0.0;

        for(int i = 0; i < positions.size(); i++) {
            for(int j = i + 1; j < positions.size(); j++) {
                sum += posterScore(solution, solution.sigma[i], solution.sigma[j]) * photoDist[ i ][ j ];
            }
        }

        solution.fitness = sum;      
    }

    unsigned size() const {
        return positions.size();
    }

protected:
    // weight of balanced sizes
    double beta1;

    // penalty of intersection
    double beta2;

    // Distance between photos
    std::vector< std::vector<double> > photoDist;

    vector<Place> positions;

    /*
        Score between positions pi, and pj on the poster
    */
    double posterScore(Solution & solution, unsigned pi, unsigned pj) {
        // surface[pi] = solution.x[pi]^2 * width[pi] * height[pi]
        // surface[pj] = solution.x[pj]^2 * width[pj] * height[pj]

        // square distance between positions, can be pre-computed...
        double d_ci_cj_square = pow(positions[pi].x - positions[pj].x, 2) + pow(positions[pi].y - positions[pj].y, 2) ;

        // sum of surfaces
        double score = pow(solution.x[pi], 2) * positions[pi].width * positions[pi].height + pow(solution.x[pj], 2) * positions[pj].width * positions[pj].height ; 

        // geometric mean of surfaces (to push toward balanced surface sizes)
        score += beta1 * solution.x[pi] * solution.x[pj] * sqrt(positions[pi].width * positions[pi].height * positions[pj].width * positions[pj].height); // square root can be pre-computed...

        // penalty (linear) of the intersection
        double lx = l_intersection(positions[pi].x, positions[pj].x, positions[pi].width * solution.x[pi] / 2, positions[pj].width * solution.x[pj] / 2);

        if (lx != 0)
            score -= beta2 * lx * l_intersection(positions[pi].y, positions[pj].y, positions[pi].height * solution.x[pi] / 2, positions[pj].height * solution.x[pi] / 2) ;

        return score / d_ci_cj_square;
    }


    double l_intersection(double ci, double cj, double li, double lj) const {
        const double u = std::min(ci + li, cj + lj);
        const double l = std::max(ci - li, cj - lj);

        return std::max(0.0, u - l);
    }

    void readPositions(Document & poster) {
        // number of positions on the first page
        int size = poster["size"].GetInt();

        positions.resize(size);

        const Value& array = poster["positions"];

        for(unsigned i = 0; i < positions.size(); i++) {
            positions[i].x      = array[i]["x"].GetDouble();
            positions[i].y      = array[i]["y"].GetDouble();
            positions[i].width  = array[i]["width"].GetDouble();
            positions[i].height = array[i]["height"].GetDouble();
        }
    }

    void computePhotoDistances(Document & document) {
        double w1 = 1.0;
        double w2 = 0.5;
        double wt = 2.0;

        const Value& array = document["photos"];

        //unsigned photoSize = array.Size();
        unsigned photoSize = document["size"].GetInt();
        photoDist.resize(photoSize);
        for(int i = 0; i < photoSize; i++) 
          photoDist[i].resize(photoSize);

        // distance based on the distance between average hash
        double d1, d2, dt;
        for(int i = 0; i < photoSize; i++) {
          const Value& col1i = array[i]["color1"];   
          const Value& col2i = array[i]["color2"];   

          for(int j = 0; j < photoSize; j++) {
            const Value& col1j = array[j]["color1"];   
            const Value& col2j = array[j]["color2"];   

            d1 = (pow(col1i["r"].GetDouble() - col1j["r"].GetDouble(), 2) 
                + pow(col1i["g"].GetDouble() - col1j["g"].GetDouble(), 2)
                + pow(col1i["b"].GetDouble() - col1j["b"].GetDouble(), 2)) / (255 * 255);

            d2 = (pow(col2i["r"].GetDouble() - col2j["r"].GetDouble(), 2) 
                + pow(col2i["g"].GetDouble() - col2j["g"].GetDouble(), 2)
                + pow(col2i["b"].GetDouble() - col2j["b"].GetDouble(), 2)) / (255 * 255);

            dt = pow(tagsDistance(array[i]["tags"]["classes"], array[j]["tags"]["classes"]), 2);

            photoDist[i][j] = (w1 * d1 + w2 * d2 + wt * dt) / (w1 + w2 + wt);
          }
        }
    }

    double tagsDistance(const Value& tags1, const Value& tags2) {
      // highly stupid inefficient method...
      double d = 0.0;

      for (SizeType i = 0; i < tags1.Size(); i++) {
        SizeType j = 0; 
        while (j < tags2.Size() and (strcmp(tags1[i].GetString(), tags2[j].GetString()) != 0))
          j++;

        if (j >= tags2.Size()) d++;
      }

      return d / tags1.Size();
    }

};

#endif