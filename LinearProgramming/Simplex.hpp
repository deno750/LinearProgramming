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

    unsigned chooseVectorToInsertInBasis(Matrix &mat, bool dual);
    
    /**
     * Finds in the matrix the index where the pivot 
     */
    unsigned findPivotIndex(Matrix &mat, unsigned basisIndex, bool dual);
    
    /**
     * This function does the pivot operation.
     * *
     * - Param mat: The matrix tableu
     * *
     * - Param rowIndexPivot: The index of the row where the pivot is.
     * *
     * - Param columnIndexForNewBasis: The column index of the variable that enters in the new basis
     * *
     * -Param dual: If true the pivot operation is computed for the dual problem, otherwise it's computed for the primal problem
     */
    void pivot(Matrix &mat, unsigned rowIndexPivot, unsigned columnIndexForNewBasis, bool dual);
    
    int primalSolver(Matrix &mat);
    
    int dualSolver(Matrix &mat);
};

