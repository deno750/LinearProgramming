\Problem name: samp1.lp

Minimize
 400 gen1 + 400 feb1 + 400 mar1 + 400 apr1 + 400 mag1 + 400 giu1 + 700 gen2 + 700 feb2 + 700 mar2 + 700 apr2 + 700 mag2 + 700 giu2 + 900 gen3 + 900 feb3 + 900 mar3 + 900 apr3 + 900 mag3 + 900 giu3
Subject To
 gen1 + gen2 + gen3 >= 9
 feb1 + feb2 + feb3 + gen2 + gen3 >= 5
 mar1 + mar2 + mar3 + feb2 + feb3 + gen3 >= 7
 apr1 + apr2 + apr3 + mar2 + mar3 + feb3 >= 9
 mag1 + mag2 + mag3 + apr2 + apr3 + mar3 >= 10
 giu1 + giu2 + giu3 + mag2 + mag3 + apr3 >= 5
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