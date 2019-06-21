#define BLUE_SIGHT 1000
#define MARGIN 5
#define SHEEP_PUSHING_PERCENT 0.1
#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)
#define ORIGIN create_point(0, 0)

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

/*
* Give opposite (-x, -y) of a point (x, y)
* p point
* Return opposite point
*/
Point opposite(Point p);

/*
* Create new path
* Return pointer to new path if successfully created, NULL otherwise
*/
Path *create_path(void);

/*
* Get number of points in a path structure
* head : head of path
* Return size of path
*/
int get_path_size(Path **head);

/*
* Get precise segment of path using its index
* head : head of the path
* index : index of the segment that will be get
* Return pointer to segment if found, NULL otherwise
*/
Path *get_segment(Path **head, int index);

/*
* Add point to a path structure
* head : head of the path
* new_point : point that will be added to path
* Return -1 if error, 1 otherwise
*/
int add_point(Path **head, Point new_point);

// ================== NODE & NODELIST PRIMITIVES ==================

/*
* Create new node
* id : id of the node
* position : position of the node
* nickname : nickname of the node
* Return node
*/
Node create_node(int id, Point position, char *nickname);

/*
* Create new nodelist structure
* Return pointer to new nodelist if successfully created, NULL otherwise
*/
NodeList *create_nodelist(void);

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
* nick : nickname of the node to get
* Return pointer to portion if found, NULL otherwise
*/
NodeList *get_nodelist_portion(NodeList **head, int id);
NodeList *nl_portion_by_nick(NodeList **head, char* nick);

/*
* Get the closest portion of a nodelist to a dog
* head : head of nodelist
* self : the dog the program runs
* nick : nickname of the node to get
* Return pointer to portion if found, NULL otherwise
*/
NodeList *closest_nl_portion_by_nick(NodeList **head, Dog self,char* nick);

/*
* Add point to a nodelist structure
* head : head of nodelist
* new_node : node that will be added to list
* Return -1 if error, 1 otherwise
*/
int add_node(NodeList **head, Node new_node);

/*
* Delete node from a nodelist using its id
* head : head of nodelist
* id : id of the node to delete
* Return -1 if error, 1 otherwise
*/
int delete_node(NodeList **head, int id);

/*
* Empty a nodelist
* head : head of nodelist
* Return -1 if error, 1 otherwise
*/
int empty_nodelist(NodeList **head);

/*
* Update a node from a nodelist using its id
* head : head of nodelist
* node : node containing new node data to update
* Return -1 if error, 1 otherwise
*/
int update_node(NodeList **head, Node node);

// ================== MESSAGE PRIMITIVES ==================

/*
* Create new message
* id : id of the node
* position : position of the node
* Return message
*/
Message create_message(int id, Point point);

// ================== DOG PRIMITIVES ==================

/*
* Create new dog
* node : node of the dog
* sight_x : length of horizontal view
* sight_y : length of vertical view
* Return dog
*/
Dog create_dog(Node node, int x_sight, int y_sight);

// ================== MATHEMATICAL FUNCTIONS ==================

/*
* Get distance between to points
* p1, p2 : two points you want to get the distance
* Return distance between p1 and p2
*/
float distance(Point p1, Point p2);

/*
* Check if value is in range
* a : value to check
* b : smallest value of range
* c : largest value of range
*/
int is_between(int a, int b, int c);

/*
* Move the target to the destination
* target : sheep to move
* radius : pushing radius of dog
* destination : point to move sheep to
* Return next point to go
*/
Point bring_back_sheep(Node target, int radius, Point destination);

// ================== PATH FUNCTIONS ==================

/*
* Generate path using a dog's color
* dog : dog to generate the path for
* max_width : width of map
* max_height : height of map
* Return pointer to path if created, NULL otherwise
*/
Path *generate_path(Dog dog, int max_width, int max_height);

/*
* Generate Blue & Yellow dogs path
* max_width : width of map
* max_height : height of map
* Return pointer to path if created, NULL otherwise
*/
Path *generate_main_path(int max_width, int max_height);

/*
* Generate Green/Purple dogs path
* max_width : width of map
* max_height : height of map
* Return pointer to path if created, NULL otherwise
*/
Path *generate_secondary_path(int max_width, int max_height, int x_sight, int y_sight);

/*
* Give the path's closest instersection point to a position
* head : head of path
* point : position to test on
* max_dist : the highest distance possible
* Return path to follow
*/
Path *closest_intersection(Path **head, Point point, float max_dist);

/*
* Determine if point is near horizontal or vertical segment
* point : position to test on
* seg_point1, seg_point2 : segment points
* margin : maximum error (width of segment)
* Return 1 if point is near, 0 otherwise
*/
int is_near_segment(Point point, Point seg_point1, Point seg_point2, int margin);

/*
* Determine if point is near another
* point1, point2 : positions to test on
* margin : maximum error (square around the point)
* Return 1 if points are close, 0 otherwise
*/
int is_near_point(Point point1, Point point2, int margin);

/*
* Determine if point is near path
* head : head of path
* p : position to test on
* margin : maximum error (width of path's segments)
* Return path to follow if near, NULL otherwise
*/
Path *is_near_path(Path **head, Point p, int margin);

/*
* Give closest point of the path to a dog
* head : head of the path
* dog : dog to test on
* max_dist : highest distance possible
* Return path to follow
*/
Path *closest_point(Path **head, Dog dog, float max_dist);

/*
* Give closest point (if not near path) or next point (if near path) of path
* head : head of path
* dog : dog the program runs
* max_dist : the highest distance possible
* Return next point to go
*/
Point follow_path(Path **head, Dog dog, float max_dist);

// ================= SHEEP DETECTION FONCTIONS ===================

/*
* Determine if a sheep is being pushed by a yellow dog
* head : head of seen nodes
* n : sheep node to check on
* Return 1 if pushed, 0 otherwise
*/
int is_pushed_by_yellow(NodeList** head, Node n);

/*
* Update seen sheeps nodelist with dog's view
* dog : dog to update sheeps list on
* head : head of seen nodes nodelist
* sheepfold_center : position of sheepfold center
* sheepfold_radius : radius of sheepfold
*/
void sheep_count(Dog* dog, NodeList** head, Point sheepfold_center, int sheepfold_radius);

/*
* Give dog's closest sheep
* dog : dog to test on (dog.sheeps can't be empty)
* max_dist : highest distance possible
* Return closest node
*/
Node closest_sheep(Dog dog, float max_dist);

/*
* Determine if a dog is the closest to a target
* target : the target to test on
* self : node (dog) the program runs
* others : other nodes (dogs) in sight
* Return 1 if dog is closest, 0 otherwise
*/
int is_closest_to_sheep(Point target, Node self, NodeList *others);

// ================= COMMUNICATION FONCTIONS ===================

/*
* Give number of digits of an octal number
* x : number to get size from
* Return number of digits
*/
int get_octal_size(int x);

/*
* Give i^th digit of an octal number
* x : number to get the digit from
* index : index of the digit to get
* Return i^th digit
*/
int get_octal_digit(int x, int index);

/*
* Convert octal digit into encoded coordinates
* a : digit to encode
* Return encoded coordinates
*/
Point encode_coordinate(int a);

/*
* Convert encoded coordinates into an octal digit
* p : coordinates to decode
* Return decoded digit
*/
int decode_coordinate(Point p);

/*
* Give the next position to go to send a message
* dog : dog the program runs
* Return position to go
*/
Point encode_msg(Dog *dog);

/*
* Complete a receiver's message with the position of the emitter
* dog : dog the program runs
* info : encoded coordinates to decode
* Return 1 if the coordinates are significant, 0 otherwise
*/
int decode_msg(Dog *dog, Point info);

// ================= PRINTING FONCTIONS ===================

/*
* Print node
* node : node to print
*/
void printnode(Node node);

/*
* Print point
* point : point to print
*/
void printpoint(Point point);

/*
* Print nodelist
* head : head of nodelist
*/
void printlist(NodeList **head);

/*
* Print path
* head : head of path
*/
void printpath(Path **head);
