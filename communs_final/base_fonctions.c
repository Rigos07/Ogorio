#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "base_fonctions.h"

// ================== PATH PRIMITIVES ==================

Point create_point(int x, int y) {
    Point new_point = {x, y};
    return new_point;
}

Path *create_path(void) {
	Path *path = NULL;
	return path;
}

int get_path_size(Path **head) {
    Path *pointer = *head;
    int size = 0;

    if (pointer != NULL) {
        do {
            pointer = pointer->next;
            size++;

        } while (pointer != *head);
    }

    return size;
}

Path *get_segment(Path **head, int index) {
    if (*head == NULL) {
        fprintf(stderr, "ERROR : Trying to get point in empty path\n");
        return NULL;
    }

    Path *this_element = *head;
    int i;

    for (i = 0; i < index; i++) {
        this_element = this_element->next;
    }

    return this_element;
}

Path *add_point(Path **head, Path **tail, Point new_point) {
    Path *new = malloc(sizeof(Path));

    if (new == NULL) {
        fprintf(stderr, "ERROR : No memory for path generation\n");
        return NULL;
    }

    if (*head == NULL) {
        *head = new;
    
    } else {
        (*head)->prev = new;
        (*tail)->next = new;
    }

    new->prev = *tail;
    new->position = new_point;
    new->next = *head;

    return new;
}

// ================== NODE & NODELIST PRIMITIVES ==================

Node create_node(unsigned char id, Point position, unsigned char *nickname) {
	Node new_node = {id, position, nickname};
	return new_node;
}

NodeList *create_nodelist(void) {
	NodeList *new_list = NULL;
	return new_list;
}

int get_nodelist_size(NodeList **head) {
    NodeList *pointer = *head;
    int size = 0;

    while (pointer != NULL) {
        pointer = pointer->next;
        size++;
    }

    return size;
}

NodeList *get_nodelist_portion(NodeList **head, unsigned char id) {
    if (*head == NULL) {
        fprintf(stderr, "ERROR : Trying to get node in empty nodelist\n");
        return NULL;
    }

    NodeList *this_element = *head;
    Node this_node = this_element->node;

    while (this_element->next != NULL && this_node.id != id) {
        this_element = this_element->next;
        this_node = this_element->node;
    }

    if (this_node.id != id) {
        fprintf(stderr, "ERROR : Node to get not found\n");
        return NULL;
    }

    return this_element;
}

NodeList *add_node(NodeList **tail, Node new_node) {
    NodeList *new = malloc(sizeof(NodeList));

    if (new == NULL) {
        fprintf(stderr, "ERROR : No memory for node generation\n");
        return NULL;
    }

    if(*tail == NULL) {
        *tail = new;
    
    } else {
        (*tail)->next = new;
    }

    new->node = new_node;
    new->next = NULL;

    return new;
}

int delete_node(NodeList **head, unsigned char id) {
	if (*head == NULL) {
		fprintf(stderr, "ERROR : Trying to delete empty nodelist\n");
		return -1;
	}

    NodeList *this_element = *head, *prev_element = *head, *next_element = this_element->next;
    Node this_node = this_element->node;

    while (next_element != NULL && this_node.id != id) {
        prev_element = this_element;
        this_element = this_element->next;
        next_element = next_element->next;
        this_node = this_element->node;
    }

	if (this_node.id != id) {
		fprintf(stderr, "ERROR : Node to delete not found\n");
		return -1;
	}

    if (*head == this_element) {
        *head = next_element;

    } else {
        prev_element->next = next_element;
    }

    free(this_element);
	return 1;
}

int update_node(NodeList **head, Node node) {
    NodeList *this_element = get_nodelist_portion(head, node.id);

	if (this_element == NULL) {
		return -1;
	}

	this_element->node = node;
	return 1;
}

// ================== FUNCTIONS ==================

float distance(Point p1, Point p2) {
    int dx = p1.x - p2.x, dy = p1.y - p2.y;
	return sqrt(pow(dx, 2) + pow(dy, 2));
}

Path* generate_path(int max_width, int max_height) {
    int xmin = max_width % BLUE_SIGHT, ymin = 0,
        xmax = max_width, ymax = max_height,
        inter_nb = max_width / (4 * BLUE_SIGHT),
        i;

    Path *head = create_path();

    Point point1 = create_point(xmin + BLUE_SIGHT, ymax + BLUE_SIGHT),
          point2 = create_point(xmax - BLUE_SIGHT, ymin + BLUE_SIGHT),
          point3 = create_point(xmax - BLUE_SIGHT, ymin - BLUE_SIGHT),
          point4 = create_point(xmin + BLUE_SIGHT, ymax - BLUE_SIGHT);

    Path *tail = add_point(&head, &head, point1);
    tail = add_point(&head, &tail, point2);
    tail = add_point(&head, &tail, point3);

    for (i = 0; i < inter_nb - 1; i++) {
        Point p1 = create_point(xmin - BLUE_SIGHT * (4 * i + 3), ymin - BLUE_SIGHT),
              p2 = create_point(xmin - BLUE_SIGHT * (4 * i + 3), ymin + BLUE_SIGHT * 3),
              p3 = create_point(xmin - BLUE_SIGHT * (4 * i + 5), ymin + BLUE_SIGHT * 3),
              p4 = create_point(xmin - BLUE_SIGHT * (4 * i + 5), ymin - BLUE_SIGHT);
        
        tail = add_point(&head, &tail, p1);
        tail = add_point(&head, &tail, p2);
        tail = add_point(&head, &tail, p3);
        tail = add_point(&head, &tail, p4);
    }

    tail = add_point(&head, &tail, point4);

    return head;
}
