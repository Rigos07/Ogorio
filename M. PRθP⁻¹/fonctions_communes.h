#define BLUE_SIGHT 1000
#define MAXLEN 26

typedef struct Point {
    unsigned int x, y;
} Point;

typedef struct Path {
    Path *prev;
    Point position;
    Path *next;
} Path;

typedef struct Node {
    unsigned char id;
    Point position;
    unsigned char *nickname;
} Node;

typedef struct NodeList {
    Node node;
    NodeList *next;
} NodeList;

Path *create_path();
Path *add_point(Path *last_element, Point new_point);
Path *get_point(Path *head,int index);
int get_path_size(Path *head);

Node *create_node();

NodeList *create_nodelist();
NodeList *add_node(NodeList *last_element, Node new_node);
int delete_node(NodeList *list, unsigned char id);
int update_node(NodeList *list, unsigned char id , Node node);
int get_nodelist_size(Path *head);
Node *get_node(NodeList *list, unsigned char id);


float distance(Point p1, Point p2);
