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

std::unordered_map<unsigned, unsigned> basisIndexes; //The key is the row index and the value is the column index. Example {1: 3} indicates that in the 1st row the basis vector is x3 where 3 is the index of the vector in basis.

bool appliesTwoPhasisMethod = false;

void printVectorInBasis(Matrix &mat) {
    std::cout << "Vector in basis are:" << std::endl;
    for (unsigned i = 1; i < mat.getRowsCount(); ++i) {
        std::cout << "x" << basisIndexes[i] << std::endl;
    }
}

int Simplex::solve(std::vector<double> c, Matrix &a, std::vector<double> b) {
    return 0;
}

int Simplex::solve(Matrix &mat) {
    
    bool optimal = false;
    basisIndexes = mat.findBasis(); //Initializes map that stores the basis
    std::vector<Fraction> costs = mat[0]; //Stores the costs in order to easly retrive it after the 1st phasis in the 2 phasis method
    while (!optimal) {
        //Checking for an initial basis
        unsigned sizeOfBasis = mat.getRowsCount() - 1;
        bool hasInitialBasis = basisIndexes.size() == sizeOfBasis;
        if (!hasInitialBasis) {
            //Uses 2 phases method
            std::cout << "\nAppling 2 phasis method\nStarting the 1st phase" << std::endl;
            appliesTwoPhasisMethod = true;
            mat.addColumns(sizeOfBasis);
            
            unsigned artificialStartIndex = mat.getColumnsCount() - mat.getRowsCount();
            for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
                if (j <= artificialStartIndex) {
                    mat[0][j] = Fraction::ZERO;
                } else {
                    mat[0][j] = Fraction::ONE;
                }
            }
            
            std::cout << "\nAdding " << sizeOfBasis << " artificial variables\n" << std::endl;
            
            for (unsigned i = 1; i < mat.getRowsCount(); ++i) {
                for (unsigned j = artificialStartIndex + 1; j < mat.getColumnsCount(); ++j) {
                    //std::cout << "i: " << i << " j: " << j - artificialStartIndex + 1 << std::endl;
                    if ((j - artificialStartIndex) == i) {
                        mat[i][j] = Fraction::ONE;
                        basisIndexes[i] = j;
                    } else {
                        mat[i][j] = Fraction::ZERO;
                    }
                }
            }
            mat.visualize();
            std::cout << "\nSubtracting the costs to insert the artificial variables in basis\n" << std::endl;;
            for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
                for (unsigned i = 1; i < mat.getRowsCount(); ++i) {
                    mat[0][j] = mat[0][j] - mat[i][j];
                }
            }
            
            mat.visualize();
            std::cout << std::endl;
            
        }
        
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
                if (appliesTwoPhasisMethod) {
                    appliesTwoPhasisMethod = false;
                    if (mat[0][0].getDoubleValue() == 0) {
                        //Check artificial variables level 0.
                        
                        for (unsigned i = 1; i < mat.getRowsCount(); ++i) {
                            if (basisIndexes[i] > mat.getColumnsCount() - mat.getRowsCount()) { //Checking if an artificial variable is still in basis
                                
                                //Find the pivot
                                Fraction pivot;
                                Fraction mulForCosts;
                                for (unsigned j = 1; j < mat.getColumnsCount(); ++j) {
                                    if (mat[i][j].getDoubleValue() != 0 && j != basisIndexes[i]) {
                                        pivot = mat[i][j];
                                        mulForCosts = mat[0][j];
                                        
                                        std::cout << "The artificial variable with index " << basisIndexes[i] << " is still in basis.\nComputing the pivot operation.\nTaken pivot " << pivot.toString() << " at (" << i << ", " << j << ")\n" << std::endl;
                                        
                                        
                                        basisIndexes[i] = j;
                                        break;
                                    }
                                }
                                
                                
                                
                                //Computing pivot operation
                                for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
                                    Fraction frac = mat[i][j];
                                    mat[i][j] = mat[i][j] / pivot;
                                    frac = mat[i][j];
                                    Fraction costsRes = mulForCosts * mat[i][j];
                                    mat[0][j] = mat[0][j] - costsRes;
                                }
                                
                                for (unsigned k = 0; k < mat.getRowsCount(); ++k) {
                                    if (k == i) {
                                        continue;
                                    }
                                    
                                    Fraction mul = mat[k][basisIndexes[i]]; //The value that is premoltiplicated to the pivot row in order to subtract the current row by the result
                                    for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
                                        Fraction res = mul * mat[k][j]; //premoltipication operation
                                        mat[k][j] = mat[k][j] - res; //The updated result is always 0
                                        
                                    }
                                }
                            }
                        }
                        
                        
                        
                        mat.visualize();
                        std::cout << "\nRemoving artifical variables" << std::endl;
                        
                        mat.removeColumns(mat.getRowsCount() - 1);
                    } else {
                        optimal = false;
                        break;
                    }
                    mat[0] = costs;
                    std::cout << "Adding the original costs value\n" << std::endl;
                    mat.visualize();
                    for (unsigned i = 1; i < mat.getRowsCount(); ++i) {
                        unsigned basisIndex = basisIndexes[i];
                        Fraction mulForCosts = mat[0][basisIndex];
                        for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
                            Fraction costsRes = mulForCosts * mat[i][j];
                            mat[0][j] = mat[0][j] - costsRes;
                        }
                    }
                    std::cout << "\nSubtracting the costs to insert the vectors in basis and terminate the phase 1\n" << std::endl;
                    mat.visualize();
                    std::cout << "\nPhase 1 terminated\n" << std::endl;
                } else {
                    optimal = true;
                }
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
        std::cout << "The result is:" << std::endl;
        for (unsigned i = 1; i < mat.getRowsCount(); ++i) {
            cout << "x" << basisIndexes[i] << "=" << mat[i][0] << std::endl;
        }
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
    basisIndexes[pivotRowIndex] = pivotColumnIndex;
    printVectorInBasis(mat);
    return 0;
}

int Simplex::dualSolver(Matrix &mat) {
    unsigned pivotRowIndex = dualFindThetaRowIndex(mat);
    double pivotColumnIndex = dualFindPivotColumnIndex(mat, pivotRowIndex); 
    if (pivotColumnIndex == 0) { //pivotColumnIndex should  be greater than 0, otherwise so the problem is impossible
        return -1;
    }
    dualComputePivotOperation(mat, pivotRowIndex, pivotColumnIndex);
    basisIndexes[pivotRowIndex] = pivotColumnIndex;
    printVectorInBasis(mat);
    return 0;
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
