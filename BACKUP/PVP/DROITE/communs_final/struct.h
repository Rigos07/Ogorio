#define BLUE_SIGHT 1000
#define MARGIN 5
#define SHEEP_PUSHING_PERCENT 0.1
#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)
#define ORIGIN create_point(0, 0)

///////////////// REPRENDRE COMMENTAIRES !!!!!!!!!!

/* Structure donnée par le prof, au cas où

typedef struct __attribute((__packed__)) Node {
    int nodeId, x, y;
    short size;
    char flag, r, g, b;
} Node */

typedef struct Point {
    int x, y;
} Point;

typedef struct Path {
    struct Path *prev;
    Point position;
    struct Path *next;
} Path;

typedef struct Node {
    int id;
    Point position;
    char *nickname;
} Node;

typedef struct NodeList {
    Node node;
    struct NodeList *next;
} NodeList;

typedef struct Message {
    int id;
    Point position;
    int size_i, id_i, x_i, y_i, started, done;
} Message;

typedef struct Dog {
    Node node;
    int x_sight, y_sight;
    Node *target;
    NodeList *sheeps;
    Message message;
} Dog;

// ================== PATH PRIMITIVES ==================

/*
* Create new point
* x : x coordinate of point
* y : y coordinate of point
* Return point
*/
Point create_point(int x, int y);

Point opposite(Point p);

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
int add_point(Path **head, Point new_point);

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
Node create_node(int id, Point position, char *nickname);

/*
* Create new nodelist structure
* Return pointer to new nodelist if successfully created, NULL otherwise
*/
NodeList *create_nodelist(void);

/*
* Add a point to a nodelist structure
* tail : tail of nodelist
* new_node : node that will be added to list
* Return pointer to new node if successfully created, NULL otherwise
*/
int add_node(NodeList **head, Node new_node);

/*
* Delete a node from a nodelist using its id
* head : head of nodelist
* id : id of the node to delete
* Return 1 if successfull, -1 if error occured
*/
int delete_node(NodeList **head, int id);

int empty_nodelist(NodeList **head);

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
NodeList *get_nodelist_portion(NodeList **head, int id);

NodeList* nl_portion_by_nick(NodeList **head, char* nick);

NodeList* closest_nl_portion_by_nick(NodeList **head, Dog self,char* nick);

// ================== MESSAGE PRIMITIVES ==================

Message create_message(int id, Point point);

// ================== DOG PRIMITIVES ==================

Dog create_dog(Node node, int x_sight, int y_sight);

// ================== FUNCTIONS ==================

/*
* Get the distance between to points
* p1, p2 : the two points you want to get the distance
* Return distance between p1 and p2
*/
float distance(Point p1, Point p2);

int is_between(int a, int b, int c);

/*
* Generate a path using a dog's color
* color : color of the dog
* max_width : width of map
* max_height : height of map
*/
Path *generate_path(Dog dog, int max_width, int max_height);

/*
* Generate Blue & Yellow dogs path
* head : head of the path
* max_width : width of map
* max_height : height of map
*/
Path *generate_main_path(int max_width, int max_height);

/*
* Generate Green/Purple dogs path
* head : head of the path
* max_width : width of map
* max_height : height of map
*/
Path *generate_secondary_path(int max_width, int max_height, int x_sight, int y_sight);

Path *closest_intersection(Path **head, Point point, float max_dist);

int is_near_segment(Point point, Point seg_point1, Point seg_point2, int margin);

int is_near_point(Point point1, Point point2, int margin);

Path *is_near_path(Path **head, Point p, int margin);

Path *closest_point(Path **head, Dog dog, float max_dist);

Point follow_path(Path **head, Dog dog, float max_dist);

int is_pushed_by_yellow(NodeList** head, Node n);

int is_pushed_by_green(NodeList** head, Node n);

void sheep_count(Dog* dog, NodeList** head, Point ally_sheepfold_center, Point ennemy_sheepfold_center,int sheepfold_radius);

// Dog.sheeps doit être non vide
Node closest_sheep(Dog dog, float max_dist);

int is_closest_to_sheep(Point target, Node self, NodeList *others);

int get_octal_size(int x);

int get_octal_digit(int x, int index);

Point encode_coordinate(int a);

int decode_coordinate(Point p);

Point encode_msg(Dog *dog);

int decode_msg(Dog *dog, Point info);


// ================= BRINGING FONCTIONS ===================

Point bring_back_sheep(Node target, int radius, Point destination);

void printnode(Node node);
void printpoint(Point point);

void printlist(NodeList **head);
void printpath(Path **head);
