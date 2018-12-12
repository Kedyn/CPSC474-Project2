# CPSC474-Project2
Project 2 for Distributing Computing

We will solve this using: Franklin’s algorithm (week8_1 slides).

We will start by using 5 number of processes, where 0 to 4 are the IDs.

0 has neighbors: 1 and 2 (1 on the left, 2 on the right).  
1 has neighbors: 3 and 0  
2 has neighbors: 0 and 4  
3 has neighbors: 4 and 1  
4 has neighbors: 3 and 2  
```
   0
 /   \
1     2
|     |
3-----4
```
```
         00
        /  \
      10    13
     /        \
   01          05
  /              \
06                08
|                  | 
03                07
  \              /
   09          02
     \        /
      12    11
        \  /
         04
```
