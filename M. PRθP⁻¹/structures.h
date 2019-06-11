typedef struct Point {
    int x, y;
} Point;

typedef struct Path {
    int nb_points;
    Path *prev;
    Point point;
    Path *next;
} Path;

typedef struct Node {
	unsigned char id;
    unsigned int x, y;
    unsigned char *nickname;
} Node;

typedef struct NodeList {
    int nb_nodes;
    Node node;
    nodelist *next;
} NodeList;