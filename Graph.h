#ifndef GRAPH_H
#define GRAPH_H

#include"Data.h"

#pragma warning(disable:4996)
#define INF 999999

typedef struct Graph {
    int edge;
    int** graph;
}Graph;



//图相关函数
Graph* init_Graph(int edge);
void add_Graph(Graph* G, int src, int des, int length);
void print_Graph(Graph* G);

//弗洛伊德求最短路径
int** get_Floyd_result(Graph* G);
int get_min_stop(Graph* G, int src, int des);
int get_min_cost(Graph* G, int src, int des);
int get_min_time(Graph* G, int src, int des);

Node_int* init_List_int_for_Graph(Graph* G);
int** get_stop_array(Graph* G, int start);
Node_int* get_stop_list(Graph* G);

//其他函数
Graph* example_graph();





#endif // GRAPH_H