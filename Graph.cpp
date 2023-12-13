#include"Graph.h"
#include"Data.h"

//图相关：

Graph* init_Graph(int edge) {
    //空间申请
    Graph* G = (Graph*)malloc(sizeof(Graph));
    G->edge = (int)malloc(sizeof(int));

    G->graph = (int**)malloc(sizeof(int*) * edge);
    for (int i = 0; i < edge; i++) {
        G->graph[i] = (int*)malloc(sizeof(int) * edge);
    }

    //数值初始化
    G->edge = edge;

    for (int i = 0; i < edge; i++) {
        for (int j = 0; j < edge; j++) {
            G->graph[i][j] = (i == j) ? 0 : INF; // 用INF初始化，对角线上为0   记录经过的站点
        }
    }

    return G;
}

void add_Graph(Graph* G, int src, int des, int length) {    //G表示要操作的图，src是起点，des是终点，length是站点数
    if (src > G->edge || des > G->edge) {
        printf("Illegal source or destination, the program is about to exit\n");
        exit(0);
    }

    G->graph[src][des] = length;     
    G->graph[des][src] = length;     // 因为是双向图，所以直接在图矩阵中添加两次
}

void print_Graph(Graph* G) {
    for (int i = 0; i < G->edge; i++) {
        for (int j = 0; j < G->edge; j++) {
            printf("%d->%d(weight: %d)\t", i, j, G->graph[i][j]);
        }
        printf("\n");
    }
}

//最短路径相关：

int** get_Floyd_result(Graph* G) {      //处理传入的的图，返回弗洛伊德矩阵，记录最短路径
    Graph* result = init_Graph(G->edge);

    for (int i = 0; i < result->edge; i++) {
        for (int j = 0; j < result->edge; j++) {
            result->graph[i][j] = G->graph[i][j];
        }
    }

    for (int mid = 0; mid < result->edge; mid++) {    //第一层for表示各个节点为中继节点的情况
        for (int i = 0; i < result->edge; i++) {      //二、三层嵌套为确定中继节点情况下的所有节点遍历
            for (int j = 0; j < result->edge; j++) {
                if (result->graph[i][mid] != INF && result->graph[mid][j] != INF &&
                    result->graph[i][j] > result->graph[i][mid] + G->graph[mid][j]) {

                    result->graph[i][j] = result->graph[i][mid] + G->graph[mid][j];     //关键步骤，取前一个矩阵与后一个矩阵对应位置最小值
                }
            }
        }
    }

    return result->graph;
}

int get_min_stop(Graph* G, int src, int des) {      //返回G中从src到des的最短路径节点数
    int** temp = get_Floyd_result(G);
    int result = temp[src][des];
    free(temp);
    return result;
}

Node_int* init_List_int_for_Graph(Graph* G){
    Node_int* head = init_Node_int(G->edge);

    for (int i = 0; i < G->edge; i++) {
        insert_Node_int(head, G->edge);
    }
    Node_int* current = head;

    int i = 0;
    while (current) {
        current->index = i;
        i++;
        current = current->next;
    }

    return head;
}

int** get_stop_array(Graph* G, int start) {
    // 初始化结果矩阵，记录最短路径长度
    int** result = (int**)malloc(G->edge * sizeof(int*));
    for (int i = 0; i < G->edge; i++) {
        result[i] = (int*)malloc(G->edge * sizeof(int));
        for (int j = 0; j < G->edge; j++) {
            result[i][j] = G->graph[i][j];
        }
    }

    // 初始化中继节点矩阵，记录中继节点信息
    int** relay_nodes = (int**)malloc(G->edge * sizeof(int*));
    for (int i = 0; i < G->edge; i++) {
        relay_nodes[i] = (int*)malloc(G->edge * sizeof(int));
        for (int j = 0; j < G->edge; j++) {
            relay_nodes[i][j] = (i == j) ? -1 : i;  // 初始化中继节点矩阵，-1表示无中继节点
        }
    }

    // 输出从起始节点到各个节点的最短路径中经过的节点
    int*** shortest_paths = (int***)malloc(G->edge * sizeof(int**));
    for (int i = 0; i < G->edge; i++) {
        shortest_paths[i] = (int**)malloc(G->edge * sizeof(int*));
        for (int j = 0; j < G->edge; j++) {
            shortest_paths[i][j] = (int*)malloc((G->edge + 1) * sizeof(int));
            for (int k = 0; k <= G->edge; k++) {
                shortest_paths[i][j][k] = -1;  // 初始化为-1，表示没有经过任何中继节点
            }
        }
    }

    for (int mid = 0; mid < G->edge; mid++) {
        for (int i = 0; i < G->edge; i++) {
            for (int j = 0; j < G->edge; j++) {
                if (result[i][mid] != INF && result[mid][j] != INF &&
                    result[i][j] > result[i][mid] + result[mid][j]) {

                    // 更新最短路径长度
                    result[i][j] = result[i][mid] + result[mid][j];

                    // 更新中继节点信息
                    relay_nodes[i][j] = mid;
                }
            }
        }
    }

    for (int i = 0; i < G->edge; i++) {
        for (int j = 0; j < G->edge; j++) {
            if (relay_nodes[i][j] != -1) {
                int current = j;
                int count = 0;

                // 最后一个节点是起始节点
                shortest_paths[i][j][count++] = i;
                while (current != i) {
                    shortest_paths[i][j][count++] = current;
                    current = relay_nodes[i][current];
                }
            }
        }
    }

    // 释放中继节点矩阵的内存
    for (int i = 0; i < G->edge; i++) {
        free(relay_nodes[i]);
    }
    free(relay_nodes);

    // 返回从start到各个节点的最短路径中经过的节点
    return shortest_paths[start];
}

Node_int* get_stop_list(Graph* G) {
    Node_int* list = init_List_int_for_Graph(G);
    Node_int* current = list;
    while (current) {
        for (int i = 0; i < G->edge; i++) {
            for (int j = 0; j < G->edge; j++) {
                current->data = get_stop_array(G, current->index);
            }
        }
        current = current->next;
    }
    return list;
}

//其他函数：

Graph* example_graph() {
    Graph* graph = init_Graph(7);
    
    add_Graph(graph, 0, 1, 8);
    add_Graph(graph, 0, 3, 2);
    add_Graph(graph, 0, 6, 3);

    add_Graph(graph, 1, 2, 9);
    add_Graph(graph, 1, 3, 3);
    add_Graph(graph, 1, 4, 7);
    add_Graph(graph, 1, 5, 6);

    add_Graph(graph, 2, 3, 1);
    add_Graph(graph, 2, 6, 2);

    add_Graph(graph, 3, 4, 3);
    add_Graph(graph, 3, 5, 7);
    add_Graph(graph, 3, 6, 9);

    add_Graph(graph, 4, 5, 9);
    add_Graph(graph, 4, 6, 2);

    add_Graph(graph, 5, 6, 4);


    return graph;
}