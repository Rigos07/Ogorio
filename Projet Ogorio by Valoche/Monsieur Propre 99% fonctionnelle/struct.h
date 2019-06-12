//This is a Point :
typedef struct Point{
  unsigned int x,y;
}Point;

//This is a Node :
typedef struct Node{
        unsigned char id;
        Point position;
        char* nickname;
}Node;

Point goal;

Point createPoint(unsigned int x,unsigned int y);
