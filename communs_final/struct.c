#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "struct.h"

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

int add_point(Path **head, Point new_point) {
    Path *new = malloc(sizeof(Path)), *element = *head;
    int i, i_max = get_path_size(head) - 1;

    if (new == NULL) {
        fprintf(stderr, "ERROR : No memory for path generation\n");
        return -1;
    }

    if (*head == NULL) {
        *head = new;
    
    } else {
        for (i = 0; i < i_max; i++) {
            element = element->next;
        }

        (*head)->prev = new;
        element->next = new;
    }

    new->prev = element;
    new->position = new_point;
    new->next = *head;

    return 1;
}

// ================== NODE & NODELIST PRIMITIVES ==================

Node create_node(int id, Point position, char *nickname) {
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

NodeList *get_nodelist_portion(NodeList **head, int id) {
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
        return NULL;
    }

    return this_element;
}

int add_node(NodeList **head, Node new_node) {
    NodeList *new = malloc(sizeof(NodeList)), *element = *head;
    int i, i_max = get_nodelist_size(head) - 1;

    if (new == NULL) {
        fprintf(stderr, "ERROR : No memory for node generation\n");
        return -1;
    }

    if (*head == NULL) {
        *head = new;

    } else {
        for (i = 0; i < i_max; i++) {
            element = element->next;
        }

        element->next = new;
    }

    new->node = new_node;
    new->next = NULL;

    return 1;
}

int delete_node(NodeList **head, int id) {
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

int empty_nodelist(NodeList **head) {
    int size = get_nodelist_size(head), i;

    for (i = 0; i < size; i++) {
        delete_node(head, (*head)->node.id);
    }

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

// ================== MESSAGE PRIMITIVES ==================

Message create_message(int id, Point point) {
    Message new_message = {id, point, 0};
    return new_message;
}

// ================== DOG PRIMITIVES ==================

Dog create_dog(Node node, x_sight, y_sight) {
    Dog new_dog = {node, NULL, create_nodelist(), x_sight, y_sight};
    return new_dog;
}

// ================== FUNCTIONS ==================

float distance(Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

int is_between(int a, int b, int c) {
    return a >= b && a < c;
}

Path *generate_path(Dog dog, int max_width, int max_height) {
    char* color = dog.node.nickname;
    int x_sight = dog.x_sight, y_sight = dog.y_sight;

    if (!strcmp(color, "yellow") || !strcmp(color, "blue")) {
        return generate_main_path(max_width, max_height);
    }

    if (!strcmp(color, "green") || !strcmp(color, "purple")) {
        return generate_secondary_path(max_width, max_height, x_sight, y_sight);
    }

    return NULL;
}

Path* generate_main_path(int max_width, int max_height) {
    int xmin = max_width % (4 * BLUE_SIGHT), ymin = 0,
        xmax = max_width, ymax = max_height,
        inter_nb = max_width / (4 * BLUE_SIGHT),
        i;

    Path *head = create_path();
    add_point(&head, create_point(xmin + BLUE_SIGHT, ymin + BLUE_SIGHT));
    add_point(&head, create_point(xmax - BLUE_SIGHT, ymin + BLUE_SIGHT));
    add_point(&head, create_point(xmax - BLUE_SIGHT, ymax - BLUE_SIGHT));

    for (i = 0; i < inter_nb - 1; i++) {
        add_point(&head, create_point(xmax - BLUE_SIGHT * (4 * i + 3), ymax - BLUE_SIGHT));
        add_point(&head, create_point(xmax - BLUE_SIGHT * (4 * i + 3), ymin + BLUE_SIGHT * 3));
        add_point(&head, create_point(xmax - BLUE_SIGHT * (4 * i + 5), ymin + BLUE_SIGHT * 3));
        add_point(&head, create_point(xmax - BLUE_SIGHT * (4 * i + 5), ymax - BLUE_SIGHT));
    }

    add_point(&head, create_point(xmin + BLUE_SIGHT, ymax - BLUE_SIGHT););

    return head;
}

Path *generate_secondary_path(int max_width, int max_height, int x_sight, int y_sight) {
    int zone_min_x = 0, zone_max_x = 0,
        zone_min_y = max_width % (4 * BLUE_SIGHT), zone_max_y = max_height,
        i, i_max;
    Path *head = create_path();
    Point last_point = create_point(zone_min_x + x_sight, zone_max_y - y_sight);

    if (zone_max_x == zone_min_x) {
        return generate_main_path(max_width,max_height);
    }

    if ((zone_max_x - zone_min_x) <= (2 * x_sight)) {
        add_point(&head, create_point(zone_max_x / 2, zone_min_y + y_sight));
        add_point(&head, create_point(zone_max_x / 2, zone_max_y - y_sight));
        return head;
    }

    if ((zone_max_y - zone_min_y) <= (2 * y_sight)) {
        add_point(&head, create_point(zone_min_x + x_sight, zone_max_y / 2));
        add_point(&head, create_point(zone_max_x - x_sight, zone_max_y / 2));
        return head;
    }

    add_point(&head, create_point(zone_min_x + x_sight, zone_min_y + y_sight));
    add_point(&head, create_point(zone_max_x - x_sight, zone_min_y + y_sight));
    add_point(&head, create_point(zone_max_x - x_sight, zone_max_y - y_sight));

    if ((zone_max_y - zone_min_y) <= (4*y_sight)) {
        add_point(&head, last_point);
        return head;
    }

    i_max = (zone_max_x - zone_min_x) / (4 * x_sight) - 1;
    for (i = 0; i < i_max; i++) {
        add_point(&head, create_point(zone_max_x - x_sight * (4 * i + 3), zone_max_y - y_sight));
        add_point(&head, create_point(zone_max_x - x_sight * (4 * i + 3), zone_min_y + y_sight * 3));
        add_point(&head, create_point(zone_max_x - x_sight * (4 * i + 5), zone_min_y + y_sight * 3));
        add_point(&head, create_point(zone_max_x - x_sight * (4 * i + 5), zone_max_y - y_sight));
    }

    if ((zone_max_x - zone_min_x) % (4 * x_sight) != zone_max_x - zone_min_x) {
        if ((zone_max_x - zone_min_x) % (4 * x_sight) > 2 * x_sight) {
            add_point(&head, create_point(zone_max_x - x_sight * (4 * i_max + 3), zone_max_y - y_sight));
            add_point(&head, create_point(zone_max_x - x_sight * (4 * i_max + 3), zone_min_y + y_sight * 3));
            add_point(&head, create_point(zone_max_x - x_sight * (4 * i_max + 5), zone_min_y + y_sight * 3));
            add_point(&head, create_point(zone_max_x - x_sight * (4 * i_max + 5), zone_max_y - y_sight));
        
        } else if ((zone_max_x - zone_min_x) % (4 * x_sight) > 0) {
            add_point(&head, create_point(zone_max_x - x_sight * (4 * i_max + 3), zone_max_y - y_sight));
            add_point(&head, create_point(zone_max_x - x_sight * (4 * i_max + 3), zone_min_y + y_sight * 3));
            add_point(&head, create_point(zone_max_x - x_sight * (4 * i_max + 3), zone_max_y - y_sight));
        }
    }

    add_point(&head, last_point);
    return head;
}

Path *closest_intersection(Path **head, Point point, float max_dist) {
    float dist = max_dist, dist_i;
    int i;
    Path* segment_i = get_segment(head, 0), *result;
    Point point_i;

    if (segment_i == NULL) {
        return NULL;
    }

    point_i = segment_i->position;
    for (i = 0; i < get_path_size(head); i++) {
        dist_i = distance(point, point_i);

        if (dist_i < dist) {
            dist = dist_i;
            result = segment_i;
        }

        segment_i = segment_i->next;
        point_i = segment_i->position;
    }

    return result;
}

int is_near_segment(Point point, Point seg_point1, Point seg_point2) {
    int x = point.x, y = point.y,
                 seg_x1 = seg_point1.x, seg_y1 = seg_point1.y,
                 seg_x2 = seg_point2.x, seg_y2 = seg_point2.y,
                 margin_x1 = min(seg_x1, seg_x2) - MARGIN, margin_y1 = min(seg_y1, seg_y2) - MARGIN,
                 margin_x2 = max(seg_x1, seg_x2) + MARGIN, margin_y2 = max(seg_y1, seg_y2) + MARGIN;

    return (seg_x1 == seg_x2 && is_between(x, margin_x1, margin_x2) && is_between(y, margin_y1, margin_y2))
        || (seg_y1 == seg_y2 && is_between(y, margin_y1, margin_y2) && is_between(x, margin_x1, margin_x2));
}

int is_near_point(Point point1, Point point2) {
    return is_near_segment(point1, point2, point2);
}

Path *is_near_path(Path **head, Point point) {
    int i = 0, i_max = get_path_size(head) + 1;
    Path *segment1 = get_segment(head, 0), *result = NULL, *segment2;

    if (segment1 == NULL) {
        return NULL;
    }

    segment2 = segment1->next;
    while (i < i_max && result == NULL) {
        if (is_near_segment(point, segment1->position, segment2->position)) {
            result = segment1;
        }

        segment1 = segment1->next;
        segment2 = segment1->next;
        i++;
    }

    return result;
}

Path *closest_point(Path **head, Dog dog, float max_dist) {
    Point point = dog.node.position, dest, up, down, left, right;

    Path *inters_point = closest_intersection(head, point, max_dist),
         *closest_point = inters_point,
         *inters_up, *inters_down, *inters_left, *inters_right,
         *result;

    int i = 0, stop = 0;
    int x = point.x, y = point.y;
    float dist;

    dist = distance(point, inters_point->position);
    dest = inters_point->position;

    while (i < ceil(dist) && !stop) {
        up = create_point(x - i, y);
        down = create_point(x + i, y);
        left = create_point(x, y - i);
        right = create_point(x, y + i);
        stop = 1;

        inters_up = is_near_path(head, up);
        inters_down = is_near_path(head, down);
        inters_left = is_near_path(head, left);
        inters_right = is_near_path(head, right);

        if (inters_up != NULL) {
            dest = up;
            closest_point = inters_up;

        } else if (inters_down != NULL) {
            dest = down;
            closest_point = inters_down;

        } else if (inters_left != NULL) {
            dest = left;
            closest_point = inters_left;

        } else if (inters_right != NULL) {
            dest = right;
            closest_point = inters_right;

        } else {
            stop = 0;
        }

        i++;
    }

    result = create_path();
    add_point(&result, dest);
    result->next = closest_point;

    return result;
}

Point follow_path(Path **head, Dog dog, float max_dist) {
    Point position = dog.node.position;
    Path *prev_inters = is_near_path(head, position), *dest = prev_inters;
    
    if (prev_inters == NULL) {
        dest = closest_point(head, dog, max_dist);

    } else {
        if (!strcmp(dog.node.nickname, "yellow")) {
            dest = dest->next;

            if (is_near_point(position, dest->position)) {
                dest = dest->next;
            }
        } else if (is_near_point(position, dest->position)) {
            dest = dest->prev;
        }
    }

    return dest->position;
}

void sheep_count(Dog* dog, NodeList** head) {
    NodeList* pointer = *head;
    Node n;

    while (pointer != NULL) {
        n = pointer->node;

        if (!strncmp("bot", n.nickname, 3)) {
            if (get_nodelist_portion(&dog->sheeps, n.id) == NULL) {
                add_node(&dog->sheeps, n);

            } else {
                update_node(&dog->sheeps, n);
            }
        }

        pointer = pointer->next;
    }
}

Node closest_sheep(Dog dog, float max_dist) {
    Point point = dog.node.position;
    NodeList *sheeps = dog.sheeps;
    int dist = max_dist, dist_i;
    Node sheep, result;

    while (sheeps != NULL) {
        sheep = sheeps->node;
        dist_i = distance(point, sheep.position);

        if (dist_i < dist) {
            dist = dist_i;
            result = sheep;
        }

        sheeps = sheeps->next;
    }

    return result;
}

int get_octal_size(int x) {
    int size = 1;

    while (pow(8, size) <= x) {
        size++;
    };

    return size;
}

Point (int a) {
    int x = 0, y = 0;

    switch (x) {
        case 0:
            x = -40;
            y = -40;
            break;

        case 1:
            x = 0;
            y = -40;
            break;

        case 2:
            x = 40;
            y = -40;
            break;

        case 3:
            x = 40;
            y = 0;
            break;

        case 4:
            x = 40;
            y = 40;

        case 5:
            x = 0;
            y = 40;
            break;

        case 6:
            x = -40;
            y = 40;
            break;

        case 7:
            x = -40;
            y = 0;
    }

    return create_point(x, y);
}


void printpoint(Point point){
    printf("x : %d\n", point.x);
    printf("y : %d\n", point.y);
}

void printnode(Node node){
    printf("Node id : %d\n", node.id);
    printf("x : %d\n", node.position.x);
    printf("y : %d\n", node.position.y);
    printf("nom : %s\n", node.nickname);
}

void printpath(Path **head){
    Path *pointer;
    int i;
    printf("Node list :\n");
    if(*head == NULL){
        printf("Empty list !\n");
    }
    else{
        pointer = *head;
        for (i = 0; i < get_path_size(head); i++) {
            printpoint(pointer->position);
            pointer = pointer->next;
        }
    }
    printf("End\n\n");
}

void printlist(NodeList **head){
    NodeList *pointer;
    int i;
    printf("Node list :\n");
    if(*head == NULL){
        printf("Empty list !\n");
    }
    else{
        pointer = *head;
        for (i = 0; i < get_nodelist_size(head); i++) {
            printnode(pointer->node);
            pointer = pointer->next;
        }
    }
    printf("End\n\n");
}

/*Point send_message(Dog *dog) {
    Message msg = dog->message;
    int size_i = msg.size_i, msg,_i = msg.size_i;

    if (size_i < 3) {
        switch (size_i) {
            case 0:
                send_coordinate(get_octal_size(msg.id));
                break;

            case 1:
                send_coordinate(get_octal_size(msg.position.x));
                break;

            case 2:
                send_coordinate(get_octal_size(msg.position.y));
        }

        dog->message.size_i++;

    } else {
        switch (msg_i) 
    }

}*/

/*void read_message(Dog *dog) {

}*/