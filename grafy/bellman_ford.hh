#ifndef BELLMAN_FORD_HH
#define BELLMAN_FORD_HH

#include "graf_lista.hh"
#include "graf_macierz.hh"

#define neg_infNumber -1*infNumber  //wartość oznaczająca -nieskończoność

void exportSolution(int _pathArray[], int _costArray[], int _numV, int _startVert);
double solveBellman(bool exp, shared_ptr<ListGraph> _graph, int _startVert);
double solveBellman(bool exp, shared_ptr<MatrixGraph> _graph, int _startVert);
#endif