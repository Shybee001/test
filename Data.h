#ifndef DATA_H
#define DATA_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#pragma warning(disable:4996)

typedef struct Node_int {
	int index;
	int** data;
	struct Node_int* next;
}Node_int;

typedef struct Node_bi {
	int index;
	struct Node_bi* right;
	struct Node_bi* down;
}Node_bi;

typedef struct Node_char {
	int src;
	int des;
	char** data;
	struct Node_char* next;
}Node_char;

//二维整型数组链表
Node_int* init_Node_int(int number);
void insert_Node_int(Node_int* list, int number);

//双向头结点链表
Node_bi* init_Node_bi(int index);
void insert_Node_bi(Node_bi* list, int index);

//二维字符数组链表



#endif // GRAPH_H