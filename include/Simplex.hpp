//
//  Simplex.hpp
//  LinearProgramming
//
//  Created by Denis Deronjic on 16/11/2019.
//  Copyright © 2019 Denis Deronjic. All rights reserved.
//

#ifndef Simplex_HPP
#define Simplex_HPP

#include <stdio.h>
#include "Matrix.hpp"

/**
 * - param c: Vector of costs
 */
class Simplex {
public:
    int solve(std::vector<double> c, Matrix &a, std::vector<double> b);

    /**
     * Solves the Linear Programming problem with simplex algoritm. Based on the matrix, the algorithm will choose to use primal or dual method
     *
     * -Param mat: The matrix tableu that defines the constraints and the costs for the problem. The algorithm works with standard form Ax = b. 
     */
    int solve(Matrix &mat);

    /**
     *
     * The primal problem searches for the first element in the reducted costs that is < 0. So this function returns the column index of the first vector with reducted costs < 0 where will be computed the pivot operation in order to let the vector enter in the basis.
     *
     * -Param mat: The matrix tableu
     *
     * -Returns the column index where the pivot is located in the matrix
     */
    unsigned primalFindPivotColumnIndex(Matrix &mat);
    
    /**
     *
     * -Param mat: The matrix tableu
     *
     * -Param pivotColumnIndex: The column index of the pivot
     *
     * -Returns the row index where the pivot is located in the matrix
     */
    unsigned primalFindPivotRowIndex(Matrix &mat, unsigned pivotColumnIndex);
    
    /**
     *
     * Computes the pivot operation for the primal problem
     *
     * -Param mat: The matrix tableu
     *
     * -Param pivotRowIndex: The row index where the pivot is located in the matrix
     *
     * -Param pivotColumnIndex: The column index where the pivot is located in the matrix
     */
    void primalComputePivotOperation(Matrix &mat, unsigned pivotRowIndex, unsigned pivotColumnIndex);
    
    /**
     * Solves the primal problem
     *
     * -Param mat: The matrix tableu
     */
    int primalSolver(Matrix &mat);
    
    /**
     * Solves the dual problem
     *
     * -Param mat: The matrix tableu
     */
    int dualSolver(Matrix &mat);
    
    /**
     *
     * The dual problem searches for the first element in b vector that is < 0. So this function returns the row index of the first vector with b < 0 where will be computed the pivot operation in order to let the vector exit from the basis and let to another vector enter in basis.
     *
     * -Param mat: The matrix tableu
     *
     * -Returns the row index where the pivot is located in the matrix
     */
    unsigned dualFindThetaRowIndex(Matrix &mat);
    
    /**
     *
     * -Param mat: The matrix tableu
     *
     * -Param pivotRowIndex: The row index of the pivot
     *
     * -Returns the column index where the pivot is located in the matrix
     */
    unsigned dualFindPivotColumnIndex(Matrix &mat, unsigned pivotRowIndex);
    
    /**
     *
     * Computes the pivot operation for the dual problem
     *
     * -Param mat: The matrix tableu
     *
     * -Param pivotRowIndex: The row index where the pivot is located in the matrix
     *
     * -Param pivotColumnIndex: The column index where the pivot is located in the matrix
     */
    void dualComputePivotOperation(Matrix &mat, unsigned pivotRowIndex, unsigned pivotColumnIndex);
};

#endif /* Simplex_hpp */

