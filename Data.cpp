#include"Data.h"

//二维整型数组链表：

Node_int* init_Node_int(int number) {
	Node_int* newnode = (Node_int*)malloc(sizeof(Node_int));
	newnode->data = (int**)malloc(sizeof(int*) * number);
	for (int i = 0; i < number; i++) {
		newnode->data[i] = (int*)malloc(sizeof(int) * number);
	}

	newnode->index = -1;
	for (int i = 0; i < number; i++) {
		for (int j = 0; j < number; j++) {
			newnode->data[i][j] = -1;
		}
	}
	newnode->next = NULL;
	return newnode;
}

void insert_Node_int(Node_int* list, int number) {
	Node_int* current = list;
	while (current -> next) {
		current = current->next;
	}
	Node_int* newnode = init_Node_int(number);
	current->next = newnode;
}

//双向头结点链表：

Node_bi* init_Node_bi(int index) {
	Node_bi* node = (Node_bi*)malloc(sizeof(Node_bi));

	node->index = index;
	node->down = NULL;
	node->right = NULL;

	return node;
}

void insert_Node_bi_down(Node_bi* list, int index) {
	Node_bi* newnode = init_Node_bi(index);
	Node_bi* current = list;
	while (current->down) {
		current = current->down;
	}
	current->down = newnode;
}

void insert_Node_bi_right(Node_bi* list, int index) {
	Node_bi* newnode = init_Node_bi(index);
	Node_bi* current = list;
	while (current->right) {
		current = current->right;
	}
	current->right = newnode;
}

//二维字符数组链表：

