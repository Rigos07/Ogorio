
#define CYAN_SIGHTX 400
#define CYAN_SIGHTY 400
#define CYAN_RADIUS 300
#include "../communs_final/struct.h"

// ================== PACKETS ==================
//Connection :
unsigned char connection1[] = {0xff,0x0,0x0,0x0,0x0};
unsigned char connection2[] = {0xfe,0x0d,0x0,0x0,0x0};

//Chiens :
unsigned char cyan2[] = {0x00,0x63,0x79,0x61,0x6E,0x32,0x00};

Node cyan_node;
Dog cyan_dog;


Point ally_sheepfold_center;
Point ennemy_sheepfold_center;
int sheepfold_radius;
