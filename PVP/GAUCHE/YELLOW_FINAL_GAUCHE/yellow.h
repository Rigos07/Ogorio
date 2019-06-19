
#define YELLOW_SIGHTX 400
#define YELLOW_SIGHTY 400
#define YELLOW_RADIUS 100
#include "../communs_final/struct.h"

// ================== PACKETS ==================
//Connection :
unsigned char connection1[] = {0xff,0x0,0x0,0x0,0x0};
unsigned char connection2[] = {0xfe,0x0d,0x0,0x0,0x0};

//Chiens :
unsigned char yellow1[] = {0x00,0x79,0x65,0x6C,0x6C,0x6F,0x77,0x31,0x00};

Node yellow_node;
Dog yellow_dog;
Path * path;


Point sheepfold_center;
int sheepfold_radius;
