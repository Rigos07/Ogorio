
#define BLUE_SIGHTX 1000
#define BLUE_SIGHTY 1000
#include "../communs_final/struct.h"

// ================== PACKETS ==================
//Connection :
unsigned char connection1[] = {0xff,0x0,0x0,0x0,0x0};
unsigned char connection2[] = {0xfe,0x0d,0x0,0x0,0x0};

//Chiens :
unsigned char blue2[] = {0x00,0x62,0x6C,0x75,0x65,0x32,0x00};

Node blue_node;
Dog blue_dog;
Path * path;


Point ally_sheepfold_center;
Point ennemy_sheepfold_center;
int sheepfold_radius;
