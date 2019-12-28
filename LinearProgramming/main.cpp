//
//  main.cpp
//  LinearProgramming
//
//  Created by Denis Deronjic on 16/11/2019.
//  Copyright © 2019 Denis Deronjic. All rights reserved.
//

#include <iostream>
#include <vector>
#include <math.h>
#include "Matrix.hpp"
#include "Simplex.hpp"
#include "LIP.hpp"

double isInteger(double val) {
    double rounded = floor(val);
    std::cout << rounded << std::endl;
    return (val - rounded);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::vector<std::vector<double>> m {
        { 0, -1, -1, 0, 0},
        {24,  6,  4, 1, 0},
        { 6,  3, -2, 0, 1}
    };
    std::vector<std::vector<double>> m2 {
        {-10,  5, -1, 0, -10, 0, 0, 0},
        {  8,  1, -4, 0,   1, 1, 0, 0},
        {  6, -1,  3, 1,   0, 0, 0, 0},
        {  1,  0, -2, 0,   3, 0, 1, 0},
        {  2,  3,  1, 0,  -2, 0, 0, 1},
    };
    std::vector<std::vector<double>> m3 {
        { 0, -3, -5, 0, 0, 0, 0},
        {12,  1,  1, 1, 0, 0, 0},
        {10,  1,  0, 0, 1, 0, 0},
        { 6,  0,  1, 0, 0, 1, 0},
        {16,  1,  2, 0, 0, 0, 1}
    };
    std::vector<std::vector<double>> m4 {
        {0, -3, -1, -3, 0, 0, 0},
        {2,  2,  1,  1, 1, 0, 0},
        {5,  1,  2,  3, 0, 1, 0},
        {6,  2,  2,  1, 0, 0, 1}
    };
    std::vector<std::vector<double>> m5 {
        {-11, -1, -3, -2, -1, 0, 0},
        {  5,  1,  2,  0,  1, 1, 0},
        {  6,  0,  1,  2,  0, 0, 1}
        
    };
    std::vector<std::vector<double>> m6 {
        {-11,  1,  1, -2,  1, 0, 0},
        {  5, -1, -2,  0, -1, 1, 0},
        {  6,  0,  1,  2,  0, 0, 1}
        
    };
    std::vector<std::vector<double>> m7 {
        {-2,  0, -2, -1,  4, 0, 0, 0},
        { 1,  0,  2,  0, -3, 1, 0, 0},
        { 0,  1,  0,  0, -1, 0, 1, 0},
        { 1, -1,  0,  1,  0, 0, 0, 1}
        
    };
    std::vector<std::vector<double>> m8 {
        {0, -3, -5, 0, 0, 0},
        {2,  7, -3, 1, 0, 0},
        {1, -1,  1, 0, 1, 0},
        {3,  1, -3, 0, 0, 1}
        
    };
    std::vector<std::vector<double>> m9 {
        {-6, -5, -3,  4, 0, 0},
        { 1,  2,  1, -3, 1, 0},
        { 5,  3,  2, -1, 0, 1}
        
    };
    std::vector<std::vector<double>> m10 {
        {0, -3, -2, 0, 0},
        {3,  2,  3, 1, 0},
        {4,  3,  2, 0, 1}
        
    };
    std::vector<std::vector<double>> m11 {
        { 0, -13, -16, -16, -14, -39, 0, 0, 0, 0, 0, 0, 0},
        {40,  11,  53,   5,   5,  29, 1, 0, 0, 0, 0, 0, 0},
        {20,   3,   6,   5,   1,  34, 0, 1, 0, 0, 0, 0, 0},
        { 1,   1,   0,   0,   0,   0, 0, 0, 1, 0, 0, 0, 0},
        { 1,   0,   1,   0,   0,   0, 0, 0, 0, 1, 0, 0, 0},
        { 1,   0,   0,   1,   0,   0, 0, 0, 0, 0, 1, 0, 0},
        { 1,   0,   0,   0,   1,   0, 0, 0, 0, 0, 0, 1, 0},
        { 1,   0,   0,   0,   0,   1, 0, 0, 0, 0, 0, 0, 1}
        
    };
    
    std::vector<std::vector<double>> m12 {
        {0, 90, 120, 170, 20, 20, 20, 0, 0, 0, 0, 0},
        {-10000, -100, -120, -160, 0, 0, 0, 1, 0, 0, 0, 0},
        {-5000, -110, -290, -330, 0, 0, 0, 0, 1, 0, 0, 0},
        {-8000, -320, -210, -130, 0, 0, 0, 0, 0, 1, 0, 0},
        {1500, 20, 16, 12, 8, 8, 8, 0, 0, 0, 1, 0},
        {-2, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 1}
        
    };
    
    std::vector<std::vector<double>> m13 {
        { 0, -3, -2, 0, 0, 0},
        {60,  6,  5, 1, 0, 0},
        {11,  1,  1, 0, 1, 0},
        {48,  8,  3, 0, 0, 1}
        
    };
    
    std::vector<std::vector<double>> m14 {
        { 0,  3,  4,  5, 0, 0},
        {-6, -2, -2, -1, 1, 0},
        {-5, -1, -2, -3, 0, 1}
        
    };
    
    std::vector<std::vector<double>> m15 {
        { 0,  3,  4,  5, 0, 0},
        {-6, -2, -2, -1, 1, 0},
        {-5, -1, -2, -3, 0, 1}
        
    };
    
    std::vector<std::vector<double>> m16 {
        {  0, 0,  0,  0,   4,  3,  1, 0},
        { -1, 0,  1,  0,  -5,  1,  3, 0},
        { -5, 1,  0,  0,  -1,  0,  4, 0},
        { -3, 0,  0,  1,   0, -1,  3, 0},
        { -5, 0,  0,  0,   0,  2, -3, 1},
        
    };
    
    std::vector<std::vector<double>> m17 {
        {0, -3, -2, 0, 0},
        {3,  2,  3, 1, 0},
        {4,  3,  2, 0, 1},
        
    };
    
    std::vector<std::vector<double>> m18 {
        {0, -2, -1, 0, 0},
        {0,  3,  -2, 1, 0},
        {6,  1,  2, 0, 1},
        
    };
    
    std::vector<std::vector<double>> m19 {
        {0, -3, -2, 0, 0, 0},
        {7,  2,  1, 1, 0, 0},
        {8,  3,  2, 0, 1, 0},
        {6,  1,  1, 0, 0, 1},
    };
    
    std::vector<std::vector<double>> m20 {
        {0, -4, -5, 0, 0, 0},
        {8,  2,  2, 1, 0, 0},
        {7,  1,  3, 0, 1, 0},
        {5,  2,  1, 0, 0, 1},
    };
    
    Matrix mat(m20);
    mat.visualize();
    Simplex simplex;
    //simplex.solve(mat);
    LIP lip;
    lip.solve(mat);
    
    return 0;
}
