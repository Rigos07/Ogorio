#define BLUE_SIGHT 1000
//#define MAXLEN 26
#define MARGIN 5

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
    Node *node;
    Node *target;
    NodeList *sheeps;
} Dog;

Node yellownode = {0,{0,0},"yellow"};
Dog yellow = {&yellownode, NULL, NULL};

Point goal;

Point createPoint(unsigned int x,unsigned int y);


// ================== PATH PRIMITIVES ==================

/*
* Creates a new path structure
* Return pointer to new path if successfully created, NULL otherwise
*/
Path *create_path(void);

/*
* Add a point to a path structure
* head : first element added to path
* last : last element added to path
* new_point : point that will be added to path
* Return pointer to new path point if successfully created, NULL otherwise
*/
Path *add_point(Path **head, Path **last, Point new_point);

/*
* Get precise segment of path using its index
* head : head of the path
* index : index of the segment that will be get
* Return pointer to segment if found, NULL otherwise
*/
Path *get_segment(Path **head, int index);

/*
* Get precise point of path using its index
* head : head of the path
* index : index of the point that will be get
* Return pointer to point if found, NULL otherwise
*/
Point *get_point(Path **head, int index);

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
Node *create_node(void);

/*
* Create new nodelist structure
* Return pointer to new nodelist if successfully created, NULL otherwise
*/
NodeList *create_nodelist(void);

/*
* Add a point to a nodelist structure
* head : head of nodelist
* new_node : node that will be added to list
* Return pointer to new node if successfully created, NULL otherwise
*/
NodeList *add_node(NodeList **head, Node new_node);

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
* Get a specific node from nodelist
* head : head of nodelist
* id : id of the node to get
* Return pointer to node if found, NULL otherwise
*/
Node *get_node(NodeList **head, unsigned char id);

// ================== FUNCTIONS ==================

/*
* Get the distance between to points
* p1, p2 : the two points you want to get the distance
* Return distance between p1 and p2
*/
float distance(Point p1, Point p2);

/*
* Give the closest intersection to a point
* path : list of the path's intersections
* point : point to consider
* max_dist : maximum distance possible
* Return pointer to point if found, NULL otherwise
*/
Point *closest_intersection(Path **path, Point point, int max_dist);

/*
* Generate Blue & Yellow dogs path
* max_width : width of map
* max_height : height of map
* Return size of path
*/
Path *generate_path(int max_width, int max_height);


Point goal;

Point createPoint(unsigned int x,unsigned int y);

void printnode(Node node);

void printlist(NodeList **head);