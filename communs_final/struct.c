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

NodeList* nl_portion_by_nick(NodeList **head, char* nick) {
    if (*head == NULL) {
        fprintf(stderr, "ERROR : Trying to get node in empty nodelist\n");
        return NULL;
    }

    NodeList *this_element = *head;
    Node this_node = this_element->node;

    while (this_element->next != NULL && strcmp(nick, this_node.nickname)) {
        this_element = this_element->next;
        this_node = this_element->node;
    }

    if (strcmp(nick, this_node.nickname)) {
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
    Message new_message = {id, point, 0, 0, 0, 0, 0, 0};
    return new_message;
}

// ================== DOG PRIMITIVES ==================

Dog create_dog(Node node, int x_sight, int y_sight) {
    Dog new_dog = {node, x_sight, y_sight, NULL, create_nodelist(), create_message(0, create_point(0, 0))};
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

    add_point(&head, create_point(xmin + BLUE_SIGHT, ymax - BLUE_SIGHT));

    return head;
}

Path *generate_secondary_path(int max_width, int max_height, int x_sight, int y_sight) {
    int zone_min_x = 0, zone_max_x = max_width % (4 * BLUE_SIGHT),
        zone_min_y = 0, zone_max_y = max_height,
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

int is_near_segment(Point point, Point seg_point1, Point seg_point2, int margin) {
    int x = point.x, y = point.y,
                 seg_x1 = seg_point1.x, seg_y1 = seg_point1.y,
                 seg_x2 = seg_point2.x, seg_y2 = seg_point2.y,
                 margin_x1 = min(seg_x1, seg_x2) - margin, margin_y1 = min(seg_y1, seg_y2) - margin,
                 margin_x2 = max(seg_x1, seg_x2) + margin, margin_y2 = max(seg_y1, seg_y2) + margin;

    return (seg_x1 == seg_x2 && is_between(x, margin_x1, margin_x2) && is_between(y, margin_y1, margin_y2))
        || (seg_y1 == seg_y2 && is_between(y, margin_y1, margin_y2) && is_between(x, margin_x1, margin_x2));
}

int is_near_point(Point point1, Point point2, int margin) {
    return is_near_segment(point1, point2, point2, margin);
}

Path *is_near_path(Path **head, Point point) {
    int i = 0, i_max = get_path_size(head) + 1;
    Path *segment1 = get_segment(head, 0), *result = NULL, *segment2;

    if (segment1 == NULL) {
        return NULL;
    }

    segment2 = segment1->next;
    while (i < i_max && result == NULL) {
        if (is_near_segment(point, segment1->position, segment2->position, MARGIN)) {
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

            if (is_near_point(position, dest->position, MARGIN)) {
                dest = dest->next;
            }
        } else if (is_near_point(position, dest->position, MARGIN)) {
            dest = dest->prev;
        }
    }

    return dest->position;
}

void sheep_count(Dog* dog, NodeList** head, Point sheepfold_center, int sheepfold_radius) {
    float distance_to_sheepfold;
    NodeList* pointer = *head;
    Node n;

    while (pointer != NULL) {
        n = pointer->node;
        distance_to_sheepfold = distance(n.position, sheepfold_center);

        if (!strncmp("bot", n.nickname, 3) && distance_to_sheepfold >= sheepfold_radius - MARGIN) {
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

int is_closest_to_sheep(Point target, Node self, NodeList *others) {
    Node other;
    int dist_self = distance(self.position, target),
        dist_other, closest = 1;

    while (others != NULL && closest == 1) {
        other = others->node;

        if ((!strcmp("yellow", other.nickname) || !strcmp("green", other.nickname)) && self.id != other.id) {
            dist_other = distance(other.position, target);

            if (dist_self >= dist_other) {
                closest = 0;
            }
        }

        others = others->next;
    }

    return closest;
}

int get_octal_size(int x) {
    int size = 1;

    while (pow(8, size) <= x) {
        size++;
    };

    return size;
}

int get_octal_digit(int x, int index) {
    int i_max = get_octal_size(x) - 1, i, pow_i;

    for (i = i_max; i > index; i--) {
        pow_i = pow(8, i);
        x %= pow_i;
    }

    return x / pow(8, index);
}

Point encode_coordinate(int a) {
    int x = 0, y = 0;

    switch (a) {
        case 0:
            x = -100;
            y = -100;
            break;

        case 1:
            x = 0;
            y = -100;
            break;

        case 2:
            x = 100;
            y = -100;
            break;

        case 3:
            x = 100;
            y = 0;
            break;

        case 4:
            x = 100;
            y = 100;
            break;

        case 5:
            x = 0;
            y = 100;
            break;

        case 6:
            x = -100;
            y = 100;
            break;

        case 7:
            x = -100;
            y = 0;
    }

    return create_point(x, y);
}

int decode_coordinate(Point p) {
    int a;
    if (is_near_point(p, create_point(-40, -40), MARGIN)) a = 0;
    else if (is_near_point(p, create_point(0, -40), MARGIN)) a = 1;
    else if (is_near_point(p, create_point(40, -40), MARGIN)) a = 2;
    else if (is_near_point(p, create_point(40, 0), MARGIN)) a = 3;
    else if (is_near_point(p, create_point(40, 40), MARGIN)) a = 4;
    else if (is_near_point(p, create_point(0, 40), MARGIN)) a = 5;
    else if (is_near_point(p, create_point(-40, 40), MARGIN)) a = 6;
    else if (is_near_point(p, create_point(-40, 0), MARGIN)) a = 7;
    return a;
}

Point encode_msg(Message *msg, Point reset) {
    Point result = create_point(-reset.x, -reset.y);
    int size_i = msg->size_i,
        id_i = msg->id_i, x_i = msg->x_i, y_i = msg->y_i,
        id = msg->id, x = msg->position.x, y = msg->position.y,
        id_size = get_octal_size(id),
        x_size = get_octal_size(x),
        y_size = get_octal_size(y);

    if (size_i < 5) {
        switch (size_i) {
            case 0:
                result = encode_coordinate(id_size);
                break;

            case 2:
                result = encode_coordinate(x_size);
                break;

            case 4:
                result = encode_coordinate(y_size);
                break;
        }

        msg->size_i++;

    } else {
        if (id_i < id_size * 2) {
            if (id_i % 2) result = encode_coordinate(get_octal_digit(id, id_size - id_i / 2 - 1));
            msg->id_i++;

        } else if (x_i < x_size * 2) {
            if (x_i % 2) result = encode_coordinate(get_octal_digit(x, x_size - x_i / 2 - 1));
            msg->x_i++;

        } else {
            if (y_i % 2) result = encode_coordinate(get_octal_digit(y, y_size - y_i / 2 - 1));
            msg->y_i++;
        }

        if (y_i == y_size * 2 - 1) msg->done = 1;
    }

    return result;
}

int decode_msg(Dog *dog, Point info) {
    Message msg = dog->message;
    Point position = dog->node.position,
          point = create_point(info.x - position.x, info.y - position.y);

    int size_i = msg.size_i,
        id_i = msg.id_i, x_i = msg.x_i, y_i = msg.y_i,
        result = decode_coordinate(point);

    if (is_near_point(point, ORIGIN, MARGIN)) return 0;

    if (size_i < 3) {
        printf("RESULT : %d\n", result);
        switch (size_i) {
            case 0:
                dog->message.id_i = result;
                break;

            case 1:
                dog->message.x_i = result;
                break;

            case 2:
                dog->message.y_i = result;
        }

        dog->message.size_i++;
    
    } else {
        if (id_i > 0) {
            dog->message.id += result * pow(8, id_i - 1);
            dog->message.id_i--;

        } else if (x_i > 0) {
            dog->message.position.x += result * pow(8, x_i - 1);
            dog->message.x_i--;

        } else {
            dog->message.position.y += result * pow(8, y_i - 1);
            dog->message.y_i--;
        }

        if (y_i == 1) dog->message.done = 1;
    }

    return 1;
}

// ================= BRINGING FONCTIONS ===================

Point bring_back_sheep(Node target,int radius, Point destination){
    int dx, dy;
    int finalx,finaly;
    float delta_x, delta_y, distance_to_destination;
    Point objective;

    dx = target.position.x - destination.x;
    dy = target.position.y - destination.y;
    distance_to_destination = distance(target.position, destination);

    if(distance_to_destination == 0){
        objective.x = target.position.x;
        objective.y = target.position.y;
        return objective;
    }

    delta_x = ( ((radius*SHEEP_PUSHING_PERCENT)*dx) / distance_to_destination );
    delta_y = ( ((radius*SHEEP_PUSHING_PERCENT)*dy) / distance_to_destination );


    finalx = floor(target.position.x + delta_x);
    finaly = floor(target.position.y + delta_y);

    if(finalx < 0){
        finalx = 0;
    }
    if (finaly < 0) {
        finaly = 0;
    }
    objective.x = finalx;
    objective.y = finaly;
    return objective;
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
