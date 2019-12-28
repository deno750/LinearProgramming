//
//  Simplex.cpp
//  LinearProgramming
//
//  Created by Denis Deronjic on 16/11/2019.
//  Copyright © 2019 Denis Deronjic. All rights reserved.
//

#include "Simplex.hpp"
#include "Matrix.hpp"
#include <vector>
#include <iostream>
#include <float.h>

std::vector<unsigned> basisIndexes;

int Simplex::solve(std::vector<double> c, Matrix &a, std::vector<double> b) {
    return 0;
}

int Simplex::solve(Matrix &mat) {
    
    bool optimal = false;
    
    while (!optimal) {
        //Cheking for primal problem
        unsigned costsLessThan0 = 0;
        for (unsigned j = 1; j < mat.getColumnsCount(); ++j) {
            if (mat[0][j] < 0) { //When this is not never true, the optimal solution is found!
                costsLessThan0++;
            }
        }
        //std::cout << "Costs less than zero: " << costsLessThan0 << std::endl;
        if (costsLessThan0 == 0) {
            //Check for dual problem
            unsigned bLessThan0 = 0;
            for (unsigned j = 1; j < mat.getRowsCount(); ++j) {
                if (mat[j][0] < 0) {
                    bLessThan0++;
                }
            }
            if (bLessThan0 == 0) {
                optimal = true;
            } else {
                std::cout << "Solving with dual" << std::endl;
                int res = dualSolver(mat);
                if (res == -1) {
                    break;
                }
            }
        } else {
            std::cout << "Solving with primal" << std::endl;
            int res = primalSolver(mat);
            if (res == -1) {
                break;
            }
        }
    }
    if (optimal) {
        std::cout << "Optimal solution found!!!" << std::endl;
        return 0;
    } else {
        std::cout << "The problem is impossible!!" << std::endl;
        return -1;
    }
}

int Simplex::primalSolver(Matrix &mat) {
    unsigned newIndexBasis = chooseVectorToInsertInBasis(mat, false);
    double thetaIndex = findPivotIndex(mat, newIndexBasis, false);
    if (thetaIndex == 0) { //Thetaindex should  be greater than 0
        return -1;
    }
    //std::cout << "New index basis: " << newIndexBasis << std::endl << "Theta index: " << thetaIndex << std::endl;
    pivot(mat, thetaIndex, newIndexBasis, false);
    return 0;
}

int Simplex::dualSolver(Matrix &mat) {
    unsigned newIndexBasis = chooseVectorToInsertInBasis(mat, true);
    double thetaIndex = findPivotIndex(mat, newIndexBasis, true);
    if (thetaIndex == 0) { //Thetaindex should  be greater than 0
        return -1;
    }
    //std::cout << "New index basis: " << newIndexBasis << std::endl << "Theta index: " << thetaIndex << std::endl;
    pivot(mat, thetaIndex, newIndexBasis, true);
    return 0;
}

void findBasis(Matrix &mat) {
    std::vector<unsigned> basis = mat.findIdentityMatrixIndices(1, 1);
}

unsigned Simplex::chooseVectorToInsertInBasis(Matrix &mat, bool dual) {
    if (!dual) { //If primal
        for (unsigned j = 1; j < mat.getColumnsCount(); ++j) {
            if (mat[0][j] < 0) {
                return j;
            }
        }
    } else { //If dual
        for (unsigned j = 1; j < mat.getRowsCount(); ++j) {
            if (mat[j][0] < 0) {
                return j;
            }
        }
    }
    
    return 0;
}

unsigned Simplex::findPivotIndex(Matrix &mat, unsigned basisIndex, bool dual) {
    if (!dual) { //if primal
        double thetaMin = DBL_MAX;
        unsigned rowIndexMin = 0;
        for (unsigned i = 1; i < mat.getRowsCount(); ++i) {
            if (mat[i][basisIndex] <= 0) {
                continue;
            }
            double thetaTmp = mat[i][0] / mat[i][basisIndex];
            if (thetaTmp < thetaMin) {
                thetaMin = thetaTmp;
                rowIndexMin = i;
            }
        }
        if (rowIndexMin > 0) {
            std::cout << "\nSelected the pivot " << mat[rowIndexMin][basisIndex] << " at (" << rowIndexMin << ", " << basisIndex << ")\n\n\n";
        }
        return rowIndexMin;
    } else { //If dual
        double thetaMin = DBL_MAX;
        unsigned columnIndexMin = 0;
        for (unsigned j = 1; j < mat.getColumnsCount(); ++j) {
            if (mat[basisIndex][j] >= 0) {
                continue;
            }
            double thetaTmp = -1.0 * (mat[0][j] / mat[basisIndex][j]);
            if (thetaTmp < thetaMin) {
                thetaMin = thetaTmp;
                columnIndexMin = j;
            }
        }
        if (columnIndexMin > 0) {
            std::cout << "\nSelected the pivot " << mat[basisIndex][columnIndexMin] << " at (" << basisIndex << ", " << columnIndexMin << ")\n\n\n";
        }
        
        return columnIndexMin;
    }
    return -1;
}

void Simplex::pivot(Matrix &mat, unsigned basisIndexPivot, unsigned basisIndexForNewBasis, bool dual) {
    if (!dual) { // If primal
        double theta = mat[basisIndexPivot][basisIndexForNewBasis];
        //std::cout << "Theta: " << theta << std::endl;
        double mulForCosts = mat[0][basisIndexForNewBasis];
        for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
            mat[basisIndexPivot][j] /= theta;
            double costsRes = mulForCosts * mat[basisIndexPivot][j];
            mat[0][j] = mat[0][j] - costsRes;
        }
        for (unsigned i = 1; i < mat.getRowsCount(); ++i) {
            if (i == basisIndexPivot) {
                continue;
            }
            double mul = mat[i][basisIndexForNewBasis];
            for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
                double res = mul * mat[basisIndexPivot][j];
                mat[i][j] = mat[i][j] - res;
                
            }
        }
        mat.visualize();
    } else { //If dual
        double theta = mat[basisIndexForNewBasis][basisIndexPivot];
        double mulForCosts = mat[0][basisIndexPivot];
        for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
            mat[basisIndexForNewBasis][j] /= theta;
            double costsRes = mulForCosts * mat[basisIndexForNewBasis][j];
            mat[0][j] = mat[0][j] - costsRes;
        }
        for (unsigned i = 1; i < mat.getRowsCount(); ++i) {
            if (i == basisIndexForNewBasis) {
                continue;
            }
            double mul = mat[i][basisIndexPivot];
            for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
                double res = mul * mat[basisIndexForNewBasis][j];
                mat[i][j] = mat[i][j] - res;
                
            }
        }
        mat.visualize();
    }
    
}
