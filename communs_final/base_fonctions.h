#define BLUE_SIGHT 1000
#define MAXLEN 26
#define MARGIN 5

///////////////// REPRENDRE COMMENTAIRES !!!!!!!!!!

/* Structure donnée par le prof, au cas où

typedef struct __attribute((__packed__)) Node {
    int nodeId, x, y;
    short size;
    char flag, r, g, b;
} Node */

typedef struct Point {
    unsigned int x, y;
} Point;

typedef struct Path {
    struct Path *prev;
    Point position;
    struct Path *next;
} Path;

typedef struct Node {
    unsigned char id;
    Point position;
    unsigned char *nickname;
} Node;

typedef struct NodeList {
    Node node;
    struct NodeList *next;
} NodeList;

typedef struct Dog {
    Node node;
    Node *target;
    NodeList *sheeps;
} Dog;

// ================== PATH PRIMITIVES ==================

/*
* Create new point
* x : x coordinate of point
* y : y coordinate of point
* Return point
*/
Point create_point(int x, int y);

/*
* Create a new path
* Return pointer to new path if successfully created, NULL otherwise
*/
Path *create_path(void);

/*
* Add a point to a path structure
* head : head of the path
* tail : tail of the path
* new_point : point that will be added to path
* Return pointer to new path point if successfully created, NULL otherwise
*/
Path *add_point(Path **head, Path **tail, Point new_point);

/*
* Get precise segment of path using its index
* head : head of the path
* index : index of the segment that will be get
* Return pointer to segment if found, NULL otherwise
*/
Path *get_segment(Path **head, int index);

/*
* Get the number of points in a path structure
* head : head of path
* Return size of path
*/
int get_path_size(Path **head);

// ================== NODE & NODELIST PRIMITIVES ==================

/*
* Create new node structure
* Return pointer to new node if successfully created, NULL otherwise
*/
Node create_node(unsigned char id, Point position, unsigned char *nickname);

/*
* Create new nodelist structure
* Return pointer to new nodelist if successfully created, NULL otherwise
*/
NodeList *create_nodelist(void);

/*
* Add a point to a nodelist structure
* tail : taild of nodelist
* new_node : node that will be added to list
* Return pointer to new node if successfully created, NULL otherwise
*/
NodeList *add_node(NodeList **tail, Node new_node);

/*
* Delete a node from a nodelist using its id
* head : head of nodelist
* id : id of the node to delete
* Return 1 if successfull, -1 if error occured
*/
int delete_node(NodeList **head, unsigned char id);

/*
* Update a node from a nodelist using its id
* head : head of nodelist
* id : id of the node to update
* node : node containing new node data to update
* Return 1 if successfull, -1 if error occured
*/
int update_node(NodeList **head, Node node);

/*
* Get the number of nodes in a nodelist structure
* head : head of nodelist
* Return size of nodelist
*/
int get_nodelist_size(NodeList **head);

/*
* Get a specific portion from nodelist
* head : head of nodelist
* id : id of the node to get
* Return pointer to portion if found, NULL otherwise
*/
NodeList *get_nodelist_portion(NodeList **head, unsigned char id);

// ================== FUNCTIONS ==================

/*
* Get the distance between to points
* p1, p2 : the two points you want to get the distance
* Return distance between p1 and p2
*/
float distance(Point p1, Point p2);

/*
* Generate Blue & Yellow dogs path
* head : head of the path
* max_width : width of map
* max_height : height of map
*/
Path *generate_path(int max_width, int max_height);


// Le path ne doit pas être vide !!!!!
Point closest_intersection(Path **path, Point point, int max_dist);