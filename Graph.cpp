#include"Graph.h"
#include"Data.h"

//ͼ��أ�

Graph* init_Graph(int edge) {
    //�ռ�����
    Graph* G = (Graph*)malloc(sizeof(Graph));
    G->edge = (int)malloc(sizeof(int));

    G->graph = (int**)malloc(sizeof(int*) * edge);
    for (int i = 0; i < edge; i++) {
        G->graph[i] = (int*)malloc(sizeof(int) * edge);
    }

    //��ֵ��ʼ��
    G->edge = edge;

    for (int i = 0; i < edge; i++) {
        for (int j = 0; j < edge; j++) {
            G->graph[i][j] = (i == j) ? 0 : INF; // ��INF��ʼ�����Խ�����Ϊ0   ��¼������վ��
        }
    }

    return G;
}

void add_Graph(Graph* G, int src, int des, int length) {    //G��ʾҪ������ͼ��src����㣬des���յ㣬length��վ����
    if (src > G->edge || des > G->edge) {
        printf("Illegal source or destination, the program is about to exit\n");
        exit(0);
    }

    G->graph[src][des] = length;     
    G->graph[des][src] = length;     // ��Ϊ��˫��ͼ������ֱ����ͼ�������������
}

void print_Graph(Graph* G) {
    for (int i = 0; i < G->edge; i++) {
        for (int j = 0; j < G->edge; j++) {
            printf("%d->%d(weight: %d)\t", i, j, G->graph[i][j]);
        }
        printf("\n");
    }
}

//���·����أ�

int** get_Floyd_result(Graph* G) {      //������ĵ�ͼ�����ظ������¾��󣬼�¼���·��
    Graph* result = init_Graph(G->edge);

    for (int i = 0; i < result->edge; i++) {
        for (int j = 0; j < result->edge; j++) {
            result->graph[i][j] = G->graph[i][j];
        }
    }

    for (int mid = 0; mid < result->edge; mid++) {    //��һ��for��ʾ�����ڵ�Ϊ�м̽ڵ�����
        for (int i = 0; i < result->edge; i++) {      //��������Ƕ��Ϊȷ���м̽ڵ�����µ����нڵ����
            for (int j = 0; j < result->edge; j++) {
                if (result->graph[i][mid] != INF && result->graph[mid][j] != INF &&
                    result->graph[i][j] > result->graph[i][mid] + G->graph[mid][j]) {

                    result->graph[i][j] = result->graph[i][mid] + G->graph[mid][j];     //�ؼ����裬ȡǰһ���������һ�������Ӧλ����Сֵ
                }
            }
        }
    }

    return result->graph;
}

int get_min_stop(Graph* G, int src, int des) {      //����G�д�src��des�����·���ڵ���
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
    // ��ʼ��������󣬼�¼���·������
    int** result = (int**)malloc(G->edge * sizeof(int*));
    for (int i = 0; i < G->edge; i++) {
        result[i] = (int*)malloc(G->edge * sizeof(int));
        for (int j = 0; j < G->edge; j++) {
            result[i][j] = G->graph[i][j];
        }
    }

    // ��ʼ���м̽ڵ���󣬼�¼�м̽ڵ���Ϣ
    int** relay_nodes = (int**)malloc(G->edge * sizeof(int*));
    for (int i = 0; i < G->edge; i++) {
        relay_nodes[i] = (int*)malloc(G->edge * sizeof(int));
        for (int j = 0; j < G->edge; j++) {
            relay_nodes[i][j] = (i == j) ? -1 : i;  // ��ʼ���м̽ڵ����-1��ʾ���м̽ڵ�
        }
    }

    // �������ʼ�ڵ㵽�����ڵ�����·���о����Ľڵ�
    int*** shortest_paths = (int***)malloc(G->edge * sizeof(int**));
    for (int i = 0; i < G->edge; i++) {
        shortest_paths[i] = (int**)malloc(G->edge * sizeof(int*));
        for (int j = 0; j < G->edge; j++) {
            shortest_paths[i][j] = (int*)malloc((G->edge + 1) * sizeof(int));
            for (int k = 0; k <= G->edge; k++) {
                shortest_paths[i][j][k] = -1;  // ��ʼ��Ϊ-1����ʾû�о����κ��м̽ڵ�
            }
        }
    }

    for (int mid = 0; mid < G->edge; mid++) {
        for (int i = 0; i < G->edge; i++) {
            for (int j = 0; j < G->edge; j++) {
                if (result[i][mid] != INF && result[mid][j] != INF &&
                    result[i][j] > result[i][mid] + result[mid][j]) {

                    // �������·������
                    result[i][j] = result[i][mid] + result[mid][j];

                    // �����м̽ڵ���Ϣ
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

                // ���һ���ڵ�����ʼ�ڵ�
                shortest_paths[i][j][count++] = i;
                while (current != i) {
                    shortest_paths[i][j][count++] = current;
                    current = relay_nodes[i][current];
                }
            }
        }
    }

    // �ͷ��м̽ڵ������ڴ�
    for (int i = 0; i < G->edge; i++) {
        free(relay_nodes[i]);
    }
    free(relay_nodes);

    // ���ش�start�������ڵ�����·���о����Ľڵ�
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

//����������

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