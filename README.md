# Resistive Circuit Analyzer

This is a project created to solve complex electric circuits containing resistors. The Goal of this project is to efficiently implement the algorithm used to solve electric circuits using concepts of Graph Theory and Network Analysis using Kirchoffs laws.

This highly multidisciplinary approach combines abstract Mathematics, Linear Algebra, physics of circuits and computer programming to reach the ambitious goal of implementing automated circuit solving.

## Data Structures and Algorithms Implemented

1. Data Structures
- Graphs
- Trees
- Arrays [1D, 2D, 3D]

2. Algorithms
- Gotlieb and Corneil Algorithm
- Vector-Centric Algorithm for Spanning Tree
- Paton Algorithm for Pruning
- LU decomposition methods

## Conclusions and Scope

The project provides strongly motivating introduction to intermediate level programming and Scientic programming required in Computational Physics along with Mathematics of Graphs.

It also demonstrates the algorthims used in much more efficient and broad Simulating softwares like the **SPICE**(Simulation Program with Integrated Circuit Emphasis). It focuses on complex mathematical calculations using graph algorithms. The idea could be used for fluid flow analysis using analogy between electric and hydraulic circuits

### Scope 
There is room for improvement to this project in the following ways:-

1. Since all the matrices are sparse, the program efficiency could be boosted with the
use of sparse matrix code.

2.  The program could be extended to handle time-dependent voltages and currents;
instead of the system (1) we should manipulate the corresponding differential
equations and use a differential equation solver.

3.  The program could also be extended to include non-linear elements like
semiconductor diodes, transistors, etc., and this would require in turn the
introduction of a nonlinear equation solver.

4.  The program could be modified using objects and the powerful concepts of object-
oriented programming: this would open up new possibilities, also because of the
availability of efficient C++ libraries like Boost 

## References
- Complex Circuit Analysis - [Automated Circuit Solving](https://core.ac.uk/download/pdf/53745212.pdf)
- Keith Paton Pruning Algorithm - [Pruning Paper](https://dl.acm.org/doi/pdf/10.1145/363219.363232)
- Cycle Finding Algorithm - [Graph Algorithm](https://www.codeproject.com/Articles/1158232/Enumerating-All-Cycles-in-an-Undirected-Graph)
- Applications of Graph Theory - [N. Deo](https://archive.org/details/GraphTheoryWithApplicationsToEngineeringAndComputerScience)
- Solving linear non-homogenuous equations - [Numerical Recipes](http://e-maxx.ru/bookz/files/numerical_recipes.pdf) 