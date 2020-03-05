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
    unsigned pivotColumnIndex = primalFindPivotColumnIndex(mat);
    double pivotRowIndex = primalFindPivotRowIndex(mat, pivotColumnIndex);
    if (pivotRowIndex == 0) { //PivotRowindex should  be greater than 0, otherwise the problem is impossible
        return -1;
    }
    primalComputePivotOperation(mat, pivotRowIndex, pivotColumnIndex);
    return 0;
}

int Simplex::dualSolver(Matrix &mat) {
    unsigned pivotRowIndex = dualFindThetaRowIndex(mat);
    double pivotColumnIndex = dualFindPivotColumnIndex(mat, pivotRowIndex); 
    if (pivotColumnIndex == 0) { //pivotColumnIndex should  be greater than 0, otherwise so the problem is impossible
        return -1;
    }
    dualComputePivotOperation(mat, pivotRowIndex, pivotColumnIndex);
    return 0;
}

void findBasis(Matrix &mat) {
    std::vector<unsigned> basis = mat.findIdentityMatrixIndices(1, 1);
}

unsigned Simplex::primalFindPivotColumnIndex(Matrix &mat) {
    for (unsigned j = 1; j < mat.getColumnsCount(); ++j) {
        if (mat[0][j].getDoubleValue() < 0) { //Reducted cost < 0, so it should enter in basis.
            return j; //Returns the index of the vector that enters in basis
        }
    }
    return 0;
}

unsigned Simplex::primalFindPivotRowIndex(Matrix &mat, unsigned pivotColumnIndex) {
    Fraction thetaMin = Fraction::MAX_VALUE;
    unsigned rowIndexMin = 0; //The row index where the minimum theta is located
    for (unsigned i = 1; i < mat.getRowsCount(); ++i) {
        Fraction thetaCandidateDivider = mat[i][pivotColumnIndex]; //The aij value of the A matrix that would be used to calculate the value of the theta.
        if (thetaCandidateDivider.getDoubleValue() <= 0) { //In primal problem the aij value must be > 0
            continue;
        }
        Fraction thetaTmp = mat[i][0] / thetaCandidateDivider; //The theta candidate.
        
        if (thetaTmp.getDoubleValue() < thetaMin.getDoubleValue()) { //Checking if the theta canditate is the minimum theta.
            thetaMin = thetaTmp; //Updating the currently minimum theta
            rowIndexMin = i; //Updating the row index of the minimum theta
        }
    }
    if (rowIndexMin > 0) {
        std::cout << "Selected the pivot " << mat[rowIndexMin][pivotColumnIndex] << " at (" << rowIndexMin << ", " << pivotColumnIndex << ")\n\n\n";
    }
    return rowIndexMin; //Returns the pivot's row index for the primal problem
}

void Simplex::primalComputePivotOperation(Matrix &mat, unsigned pivotRowIndex, unsigned pivotColumnIndex) {
    Fraction pivot = mat[pivotRowIndex][pivotColumnIndex];
    Fraction mulForCosts = mat[0][pivotColumnIndex]; //The value of the cost in the column of the vector to insert in basis in order to compute pivot operations to obtain the 0 cost for the column that enters in basis
    for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
        mat[pivotRowIndex][j] = mat[pivotRowIndex][j] / pivot; //Each column of the row indicated by pivotRowIndex is divided by the pivot
        Fraction costsRes = mulForCosts * mat[pivotRowIndex][j]; //Result that is obtained by multypling mulForCosts in order to perform pivot operation in the costs row
        mat[0][j] = mat[0][j] - costsRes; //Subtracting the cost by the value costRes, the result is always 0.
    }
    for (unsigned i = 1; i < mat.getRowsCount(); ++i) { //Doing the pivot operations for the other rows, in order to have the basis column vector i.e. 0,...0,1,0,...,0
        if (i == pivotRowIndex) {
            continue;
        }
        Fraction mul = mat[i][pivotColumnIndex]; //The value that is premoltiplicated to the pivot row in order to subtract the current row by the result
        for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
            Fraction res = mul * mat[pivotRowIndex][j]; //premoltipication operation
            mat[i][j] = mat[i][j] - res; //The updated result is always 0
            
        }
    }
    //At the end of the operation, the vector with the column index "pivotColumnIndex" is in basis and has all values to 0 other than the value at 1 in the corresponding row index "pivotRowIndex"
    mat.visualize();
    
}

unsigned Simplex::dualFindThetaRowIndex(Matrix &mat) {
    for (unsigned j = 1; j < mat.getRowsCount(); ++j) {
        if (mat[j][0].getDoubleValue() < 0) { //B value is < 0, so it should exit from basis
            return j; //Returns the index of the vector that exits the basis
        }
    }
    return 0;
}

unsigned Simplex::dualFindPivotColumnIndex(Matrix &mat, unsigned pivotRowIndex) {
    Fraction thetaMin = Fraction::MAX_VALUE;
    unsigned columnIndexMin = 0; //The column index where the minimum theta is located
    for (unsigned j = 1; j < mat.getColumnsCount(); ++j) {
        Fraction thetaCandidateDivider = mat[pivotRowIndex][j]; //The aij value of the A matrix that would be used to calculate the value of the theta.
        if (thetaCandidateDivider.getDoubleValue() >= 0) { //In dual problem the aij value must be < 0
            continue;
        }
        Fraction thetaTmp =  (mat[0][j] / thetaCandidateDivider) * -1; //The theta candidate. Is multiplied with -1 in order to make it positive since theta is calculated as theta = cj / |aij|
        if (thetaTmp.getDoubleValue() < thetaMin.getDoubleValue()) { //Checking if the theta canditate is the minimum theta.
            thetaMin = thetaTmp; //Updating the currently minimum theta
            columnIndexMin = j; //Updating the column index of the minimum theta
        }
    }
    if (columnIndexMin > 0) {
        std::cout << "Selected the pivot " << mat[pivotRowIndex][columnIndexMin] << " at (" << pivotRowIndex << ", " << columnIndexMin << ")\n\n\n";
    }
    
    return columnIndexMin; //Returns the pivot's column index for the dual problem
}

void Simplex::dualComputePivotOperation(Matrix &mat, unsigned pivotRowIndex, unsigned pivotColumnIndex) {
    Fraction pivot = mat[pivotRowIndex][pivotColumnIndex];
    Fraction mulForCosts = mat[0][pivotColumnIndex]; //The value of the cost in the column of the vector to insert in basis in order to compute pivot operations to obtain the 0 cost for the column that enters in basis
    for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
        mat[pivotRowIndex][j] = mat[pivotRowIndex][j] / pivot; //Each column of the row indicated by pivotRowIndex is divided by the pivot
        Fraction costsRes = mulForCosts * mat[pivotRowIndex][j]; //Result that is obtained by multypling mulForCosts in order to perform pivot operation in the costs row
        mat[0][j] = mat[0][j] - costsRes; //Subtracting the cost by the value costRes, the result is always 0.
    }
    for (unsigned i = 1; i < mat.getRowsCount(); ++i) { //Doing the pivot operations for the other rows, in order to have the basis column vector i.e. 0,...0,1,0,...,0
        if (i == pivotRowIndex) {
            continue;
        }
        Fraction mul = mat[i][pivotColumnIndex]; //The value that is premoltiplicated to the pivot row in order to subtract the current row by the result
        for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
            Fraction res = mul * mat[pivotRowIndex][j]; //premoltipication operation
            mat[i][j] = mat[i][j] - res; //The updated result is always 0
            
        }
    }
    //At the end of the operation, the vector with the column index "pivotColumnIndex" is in basis and has all values to 0 other than the value at 1 in the corresponding row index "pivotRowIndex"
    mat.visualize();
}
