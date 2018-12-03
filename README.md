# CPSC474-Project2
Project 2 for Distributing Computing

We will solve this using: Franklin’s algorithm (week8_1 slides).

We will start by using 5 number of processes, where 0 to 4 are the IDs.

0 has neighbors: 4 and 1 (4 on the left, 1 on the right).  
1 has neighbors: 0 and 2  
2 has neighbors: 1 and 3  
3 has neighbors: 2 and 4  
4 has neighbors: 3 and 0  
```
   0
 /   \
4     1
|     |
3     2
 \   /
   2
```
