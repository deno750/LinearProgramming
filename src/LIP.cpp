//
//  LIP.cpp
//  LinearProgramming
//
//  Created by Denis Deronjic on 28/12/2019.
//  Copyright © 2019 Denis Deronjic. All rights reserved.
//

#include <iostream>
#include "LIP.hpp"
#include "Simplex.hpp"
#include <math.h>

struct BDNode {
    Fraction cost;
    Matrix mat;
};

struct BDNode nodes[50];
int minimumCostNodeIndex = -1;

int LIP::solve(std::vector<double> c, Matrix &a, std::vector<double> b) {
    return 0;
}

int LIP::solve(Matrix &mat, bool useBranchAndBound) {
    
    bool optimal = false; //Integer optimal solution
    Simplex simplex;
    unsigned currentNodeIndex = 0; //Helper variable for branch and bound
    Matrix matrix = mat;
    while (!optimal) {
        int res = simplex.solve(matrix);
        if (res == -1) { //The problem is impossible!
            if (!useBranchAndBound) { //When using branch and bound we don't want to stop the algoritmh if a node is impossible
                return -1;
            }
        }
        //Find if there's one solution that is not integer
        unsigned nonIntegerIndex = 0;
        for (unsigned i = 1; i < matrix.getRowsCount(); ++i) {
            Fraction b = matrix[i][0];
            if (!b.isInteger()) {
                nonIntegerIndex = i;
                break;
            }
        }
        if (nonIntegerIndex == 0) {
            if (!useBranchAndBound) { //When we use branch and bound, we don't want to find the first integer solution, but the solution with the minimum integer cost
                optimal = true;
                return 0;
            } else { //Cheks the integer solution with minimum cost
                Fraction currentCost = matrix[0][0] * -1;
                nodes[currentNodeIndex].cost = currentCost;
                if (minimumCostNodeIndex == -1) {
                    minimumCostNodeIndex = currentNodeIndex;
                }
                if (currentCost.getDoubleValue() < nodes[minimumCostNodeIndex].cost.getDoubleValue()) {
                    minimumCostNodeIndex = currentNodeIndex;
                }
                cout << "Nodes costs ";
                for (int i = 0; i < 10; i++) {
                    cout << nodes[i].cost.toString() << ", ";
                }
                cout << endl;
                return 0;
                
            }
            
        }
        
        std::cout << "The optimal solution is not integer." << std::endl;
        if (useBranchAndBound) {
            if (res == 0) { //Soulution found
                nodes[currentNodeIndex].cost = matrix[0][0] * -1;
            } else { //Problem is impossible
                if (matrix[0][0].getDoubleValue() > 0) {
                    nodes[currentNodeIndex].cost = Fraction(INT_MIN, 1);
                } else {
                    nodes[currentNodeIndex].cost = Fraction(INT_MAX, 1);
                }
                
            }
            cout << "Cost" << nodes[currentNodeIndex].cost.toString() << endl;
            nodes[currentNodeIndex].mat = matrix;
            solveWithBranchAndBound(nodes[currentNodeIndex].mat, nonIntegerIndex, currentNodeIndex);
            currentNodeIndex += 1;
            matrix = nodes[currentNodeIndex].mat;
        } else {
            solveWithGomoryCuts(matrix, nonIntegerIndex);
        }
    }
    return 0;
}

Fraction LIP::calculateDecimal(Fraction val) {
    Fraction decimal = val - Fraction((int)floor(val.getDoubleValue()));
    return decimal;
}

void LIP::solveWithGomoryCuts(Matrix &mat, unsigned nonIntegerIndex) {
    mat.increaseMatrix();
    //Solving with gomory cuts
    for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
        if (j == mat.getColumnsCount() - 1) {
            mat[mat.getRowsCount() - 1][j] = Fraction::ONE;
            mat.insertInBasis(mat.getRowsCount() - 1, j);
        } else {
            Fraction decimal = calculateDecimal(mat[nonIntegerIndex][j]) * -1;
            mat[mat.getRowsCount() - 1][j] = decimal;
        }
    }
    
    std::cout << "Added gomory cut\n" << std::endl;
    mat.visualize();
}

void LIP::solveWithBranchAndBound(Matrix &mat, unsigned nonIntegerIndex, unsigned currentNodeIndex) {
    Matrix mat1 = mat;
    Matrix mat2 = mat;
    mat1.increaseMatrix();
    mat2.increaseMatrix();
    Fraction lowerBound = Fraction((int) floor(mat[nonIntegerIndex][0].getDoubleValue()));
    Fraction upperBound = Fraction((int) ceil(mat[nonIntegerIndex][0].getDoubleValue()));
    
    for (unsigned j = 0; j < mat1.getColumnsCount(); ++j) {
        if (j == 0) { //Pivot operation with lower and upper bounds of the current non integer solution of the variable in basis
            mat1[mat1.getRowsCount() - 1][j] = lowerBound - mat1[nonIntegerIndex][0];
            mat2[mat2.getRowsCount() - 1][j] = (upperBound * -1) + mat2[nonIntegerIndex][0];
        } else if (j == mat1.getColumnsCount() - 1) { //Inserting the new variable in basis
            mat1[mat1.getRowsCount() - 1][j] = Fraction::ONE;
            mat1.insertInBasis(mat1.getRowsCount() - 1, j);
            mat2[mat2.getRowsCount() - 1][j] = Fraction::ONE;
            mat2.insertInBasis(mat2.getRowsCount() - 1, j);
        } else {
            if (j == mat.getBasisIndexAtRow(nonIntegerIndex)) { //Removing the current vector in basis from basis
                mat1[mat1.getRowsCount() -1][j] = Fraction::ZERO;
                mat2[mat2.getRowsCount() -1][j] = Fraction::ZERO;
            } else { //Pivot operation
                mat1[mat1.getRowsCount() -1][j] = mat1[nonIntegerIndex][j] * -1;
                mat2[mat2.getRowsCount() -1][j] = mat2[nonIntegerIndex][j];
            }
        }
    }
    unsigned leftChildIndex = 2 * currentNodeIndex + 1;
    unsigned rightChildIndex = 2 * currentNodeIndex + 2;
    nodes[leftChildIndex].mat = mat1;
    nodes[rightChildIndex].mat = mat2;
    cout << "\n\nleft child\n\n" << endl;
    mat1.visualize();
    cout << "\n\nright child\n\n" << endl;
    mat2.visualize();
}



