#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "base_fonctions.h"



// ================== PATH PRIMITIVES ==================

Path *create_path(void){
	Path *path = NULL;
	return path;
}


Path *add_point(Path **head, Path **last, Point new_point){
    Path *new = malloc(sizeof(Path));

    if(new == NULL){
        fprintf(stderr, "ERROR : No memory for path generation\n");
        return NULL;
    }

    new->prev = *last;
    new->position = new_point;
    new->next = *head;

    if(*last == NULL){
        *last = new;
    }
    else{
        (*head)->prev = new;
        (*last)->next = new;
    }

    return new;
}

Point *get_point(Path **head,int index){
    int i;

    if(*head == NULL){
        fprintf(stderr, "ERROR : Trying to get point in empty path\n");
        return NULL;
    }

    Path *element = *head;
    for (i = 0; i < index; i++){
        element = element->next;
    }

    return &(element->position);
}

int get_path_size(Path **head){
    int size = 0;
    Path *pointer = *head;
    if(pointer != NULL) {
        do {
            size++;
            pointer = pointer->next;
        } while(pointer != *head);
    }
    return size;
}


// ================== NODE & NODELIST PRIMITIVES ==================

Node *create_node(void){
	Node *new_node = NULL;
	return new_node;
}


NodeList *create_nodelist(void){
	NodeList *new_list = NULL;
	return new_list;
}


NodeList *add_node(NodeList **last, Node new_node){
    NodeList *new = malloc(sizeof(NodeList));

    if(new == NULL){
        fprintf(stderr, "ERROR : No memory for node generation\n");
        return NULL;
    }

    new->node = new_node;
    new->next = NULL;

    if(*last == NULL){
        *last = new;
    }
    else{
        (*last)->next = new;
    }

    return new;
}

int delete_node(NodeList **head, unsigned char id){
	NodeList *last_element = *head;
	NodeList *element = *head;
	if(*head == NULL){
		fprintf(stderr, "ERROR : Trying to delete empty nodelist\n");
		return -1;
	}

	if( (*head)->next == NULL){
		element = (*head);
		*head = NULL;
	}
	else{
		element = (*head)->next;
		while(element->next != NULL && element->node.id != id){
			last_element = last_element->next;
			element = element->next;
		}
	}

	if(element->node.id != id){
		fprintf(stderr, "ERROR : Node to delete not found\n");
		return -1;
	}

	last_element->next = element->next;
	free(element);
	return 1;

}


int update_node(NodeList **head, Node node){
	if(*head == NULL){
		fprintf(stderr, "ERROR : Trying to update node in empty nodelist\n");
		return -1;
	}

	unsigned char id = node.id;
	NodeList *element = *head;
	while(element->next != NULL && element->node.id != id){
		element = element->next;
	}

	if(element->node.id != id){
		fprintf(stderr, "ERROR : Node to update not found\n");
		return -1;
	}

	element->node = node;
	return 1;
}

int get_nodelist_size(NodeList **head){
	int size = 0;
	NodeList *pointer = *head;
	while(pointer != NULL){
		size++;
		pointer = pointer->next;
	}
	return size;
}

Node *get_node(NodeList **head, unsigned char id){
	if(*head == NULL){
		fprintf(stderr, "ERROR : Trying to get node in empty nodelist\n");
		return NULL;
	}

	NodeList * element = *head;
	while(element->next != NULL && element->node.id != id){
		element = element->next;
	}

	if(element->node.id != id){
		fprintf(stderr, "ERROR : Node to get not found\n");
		return NULL;
	}

	return &(element->node);
}


// ================== FUNCTIONS ==================


float distance(Point p1, Point p2){
	return sqrt( pow((p1.x - p2.x),2) + pow((p1.y - p2.y),2) );
}

Path *generate_path(int max_width, int max_height){
    int xmin = max_width%(BLUE_SIGHT);
    int ymin = 0;
    int xmax = max_width;
    int ymax = max_height;

    Point point1 = {xmin + BLUE_SIGHT , ymax + BLUE_SIGHT};
    Point point2 = {xmax - BLUE_SIGHT , ymin + BLUE_SIGHT};
    Point point3 = {xmax - BLUE_SIGHT , ymin - BLUE_SIGHT};
    Point point4 = {xmin + BLUE_SIGHT , ymax - BLUE_SIGHT};

    Path *head = create_path();

    if(head == NULL){
        fprintf(stderr, "ERROR : No memory for path generation\n");
        return NULL;
    }
    
    Path *path = add_point(&head, &head, point1);

    path = add_point(&head, &path, point2);

    path = add_point(&head, &path, point3);

    int i;
    int inter_nb = max_width/(4*BLUE_SIGHT);
    //int number_of_points = 4 + 4*inter_nb; 
    for(i = 0 ; i < inter_nb - 1 ; i++){
        Point p1 = {xmin - BLUE_SIGHT*(4*i + 3) , ymin - BLUE_SIGHT};
        Point p2 = {xmin - BLUE_SIGHT*(4*i + 3) , ymin + BLUE_SIGHT*3};
        Point p3 = {xmin - BLUE_SIGHT*(4*i + 5) , ymin + BLUE_SIGHT*3};
        Point p4 = {xmin - BLUE_SIGHT*(4*i + 5) , ymin - BLUE_SIGHT};
        
        path = add_point(&head, &path, p1);
        path = add_point(&head, &path, p2);
        path = add_point(&head, &path, p3);
        path = add_point(&head, &path, p4);
    }

    path = add_point(&head, &path, point4);

    return head;
}


void printnode(Node node){
	printf("Node id : %d\n", node.id);
	printf("x : %d\n", node.position.x);
	printf("y : %d\n", node.position.y);
	printf("nom : %s\n", node.nickname);
}

void printlist(NodeList **head){
	NodeList *pointer;
	printf("Node list :\n");
	printf("de taille : %d\n", get_nodelist_size(head));
	if(*head == NULL){
		printf("Empty list !\n");
	}
	else{
		pointer = *head;
		while(pointer->next != NULL){
			printnode(pointer->node);
			pointer = pointer->next;
		}
		printnode(pointer->node);
	}
	printf("End\n\n");
}

int main(){
	NodeList * listhead = create_nodelist(),*test;
	unsigned char *nick = "arcenciel";
	unsigned char *nick1 = "blue";
	unsigned char *nick2 = "yellow";
	unsigned char *nick3 = "superchiendu47";
	Node woof = {12,{1,2},nick};
	Node woof1 = {5,{100,225},nick1};
	Node woof2 = {1,{10,4500},nick2};
	Node woof3 = {47,{9000,6000},nick3};

	printlist(&listhead);
	test = add_node(&listhead,woof);
	printlist(&listhead);

	if( delete_node(&listhead,12) == 1 ){
		printf("isok\n");
	}

	test = add_node(&listhead,woof);
  	test = add_node(&test,woof1);
  	test = add_node(&test,woof2);
  	test = add_node(&test,woof3);
  	printlist(&listhead);

  	if( delete_node(&listhead,1) == 1 ){
		printf("isok2\n");
	}
	printlist(&listhead);

	Node woof3_mais_en_fait_il_a_bouge = {47,{300,100},nick3};

	if(update_node(&listhead, woof3_mais_en_fait_il_a_bouge)){
		printf("isok3, update du 47\n");
	}
	printlist(&listhead);

	printf("node : \n");
	Node *test3 = get_node(&listhead,50);
	if(test3 != NULL)printnode(*test3);
	return 1;
}