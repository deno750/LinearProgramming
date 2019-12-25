//
//  Simplex.hpp
//  LinearProgramming
//
//  Created by Denis Deronjic on 16/11/2019.
//  Copyright © 2019 Denis Deronjic. All rights reserved.
//

#ifndef Simplex_hpp
#define Simplex_hpp

#include <stdio.h>
#include "Matrix.hpp"

#endif /* Simplex_hpp */

/**
 * - param c: Vector of costs
 */
class Simplex {
public:
    int solve(std::vector<double> c, Matrix &a, std::vector<double> b);

    int solve(Matrix &mat);

    //Temporary functions. These should be in tableu class
    unsigned chooseVectorToInsertInBasis(Matrix &mat, bool dual);
    unsigned findPivotIndex(Matrix &mat, unsigned basisIndex, bool dual);
    void pivot(Matrix &mat, unsigned rowIndexPivot, unsigned columnIndexForNewBasis, bool dual);
    int primalSolver(Matrix &mat);
    int dualSolver(Matrix &mat);
};

