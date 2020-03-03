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
#include "Fraction.hpp"

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
            Fraction reductedCost = mat[0][j];
            if (reductedCost.getDoubleValue() < 0) { //When this is not never true, the optimal solution is found for primal!
                costsLessThan0++;
            }
        }
        if (costsLessThan0 == 0) { //If costs are all greater than 0, optimal for primal is found, but for doual it could not be optimal though.
            //Check for dual problem
            unsigned bLessThan0 = 0;
            for (unsigned j = 1; j < mat.getRowsCount(); ++j) {
                Fraction bVal = mat[j][0];
                if (bVal.getDoubleValue() < 0) {
                    bLessThan0++;
                }
            }
            if (bLessThan0 == 0) { //All b values are greater than 0. Optimal solution for dual is found. Also it is optimal for dual.
                optimal = true;
            } else {
                std::cout << "\nSolving with dual" << std::endl;
                int res = dualSolver(mat);
                if (res == -1) { //Problem is impossible
                    break;
                }
            }
        } else {
            std::cout << "\nSolving with primal" << std::endl;
            int res = primalSolver(mat); //Problem is impossible
            if (res == -1) {
                break;
            }
        }
    }
    if (optimal) {
        std::cout << "\nOptimal solution found!!!" << std::endl;
        return 0;
    } else {
        std::cout << "\nThe problem is impossible!!" << std::endl;
        return -1;
    }
}

int Simplex::primalSolver(Matrix &mat) {
    unsigned newIndexBasis = chooseVectorToInsertInBasis(mat, false);
    double thetaIndex = findPivotIndex(mat, newIndexBasis, false);
    if (thetaIndex == 0) { //Thetaindex should  be greater than 0, so the problem is impossible
        return -1;
    }
    //std::cout << "New index basis: " << newIndexBasis << std::endl << "Theta index: " << thetaIndex << std::endl;
    pivot(mat, thetaIndex, newIndexBasis, false);
    return 0;
}

int Simplex::dualSolver(Matrix &mat) {
    unsigned newIndexBasis = chooseVectorToInsertInBasis(mat, true);
    double thetaIndex = findPivotIndex(mat, newIndexBasis, true);
    if (thetaIndex == 0) { //Thetaindex should  be greater than 0, so the problem is impossible
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
            if (mat[0][j].getDoubleValue() < 0) {
                return j;
            }
        }
    } else { //If dual
        for (unsigned j = 1; j < mat.getRowsCount(); ++j) {
            if (mat[j][0].getDoubleValue() < 0) {
                return j;
            }
        }
    }
    
    return 0;
}

unsigned Simplex::findPivotIndex(Matrix &mat, unsigned basisIndex, bool dual) {
    if (!dual) { //if primal
        Fraction thetaMin = Fraction::MAX_VALUE;
        unsigned rowIndexMin = 0;
        for (unsigned i = 1; i < mat.getRowsCount(); ++i) {
            Fraction dividerForThetaCandidateCandidate = mat[i][basisIndex];
            if (dividerForThetaCandidateCandidate.getDoubleValue() <= 0) {
                continue;
            }
            Fraction thetaTmp = mat[i][0] / dividerForThetaCandidateCandidate;
            if (thetaTmp.getDoubleValue() < thetaMin.getDoubleValue()) {
                thetaMin = thetaTmp;
                rowIndexMin = i;
            }
        }
        if (rowIndexMin > 0) {
            std::cout << "Selected the pivot " << mat[rowIndexMin][basisIndex] << " at (" << rowIndexMin << ", " << basisIndex << ")\n\n\n";
        }
        return rowIndexMin;
    } else { //If dual
        Fraction thetaMin = Fraction::MAX_VALUE;
        unsigned columnIndexMin = 0;
        for (unsigned j = 1; j < mat.getColumnsCount(); ++j) {
            Fraction dividerForThetaCandidateCandidate = mat[basisIndex][j];
            if (dividerForThetaCandidateCandidate.getDoubleValue() >= 0) {
                continue;
            }
            Fraction thetaTmp =  (mat[0][j] / dividerForThetaCandidateCandidate) * -1;
            if (thetaTmp.getDoubleValue() < thetaMin.getDoubleValue()) {
                thetaMin = thetaTmp;
                columnIndexMin = j;
            }
        }
        if (columnIndexMin > 0) {
            std::cout << "Selected the pivot " << mat[basisIndex][columnIndexMin] << " at (" << basisIndex << ", " << columnIndexMin << ")\n\n\n";
        }
        
        return columnIndexMin;
    }
    return -1;
}

void Simplex::pivot(Matrix &mat, unsigned basisIndexPivot, unsigned basisIndexForNewBasis, bool dual) {
    if (!dual) { // If primal
        Fraction theta = mat[basisIndexPivot][basisIndexForNewBasis];
        //std::cout << "Theta: " << theta << std::endl;
        Fraction mulForCosts = mat[0][basisIndexForNewBasis];
        for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
            mat[basisIndexPivot][j] = mat[basisIndexPivot][j] / theta;
            Fraction costsRes = mulForCosts * mat[basisIndexPivot][j];
            mat[0][j] = mat[0][j] - costsRes;
        }
        for (unsigned i = 1; i < mat.getRowsCount(); ++i) {
            if (i == basisIndexPivot) {
                continue;
            }
            Fraction mul = mat[i][basisIndexForNewBasis];
            for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
                Fraction res = mul * mat[basisIndexPivot][j];
                mat[i][j] = mat[i][j] - res;
                
            }
        }
        mat.visualize();
    } else { //If dual
        Fraction theta = mat[basisIndexForNewBasis][basisIndexPivot];
        Fraction mulForCosts = mat[0][basisIndexPivot];
        for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
            mat[basisIndexForNewBasis][j] = mat[basisIndexForNewBasis][j] / theta;
            Fraction costsRes = mulForCosts * mat[basisIndexForNewBasis][j];
            mat[0][j] = mat[0][j] - costsRes;
        }
        for (unsigned i = 1; i < mat.getRowsCount(); ++i) {
            if (i == basisIndexForNewBasis) {
                continue;
            }
            Fraction mul = mat[i][basisIndexPivot];
            for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
                Fraction res = mul * mat[basisIndexForNewBasis][j];
                mat[i][j] = mat[i][j] - res;
                
            }
        }
        mat.visualize();
    }
    
}
