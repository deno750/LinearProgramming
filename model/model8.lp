\Problem name: samp1.lp

Maximize 
900 x1 + 800 x2 + 700 x3 + 600 x4 - 700 z1 - 500 z2 - 300 z3 + 0 y1 + 0 y2 + 0 y3
Subject To
 50 x1 + 50 x2 + 50 x3 + 50 x4 >= 20000
 z1 - 3 y1 <= 0
 z2 - 3 y2 <= 0
 z3 - 3 y3 <= 0
 x1 + y1 <= 40
 x2 + y2 - z1 <= 40
 x3 + y3 - z1 - z2 <= 40
 x4 - z1 - z2 - z3 <= 40
Bounds
 0 <= X1 <= 4
 2 <= X2 <= 5
 0 <= X3 <= 1
 3 <= X4 <= 8
Binaries
 X3 
Generals
 X2 
End