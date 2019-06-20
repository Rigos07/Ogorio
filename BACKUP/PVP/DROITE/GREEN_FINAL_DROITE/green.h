
#define GREEN_SIGHTX 400
#define GREEN_SIGHTY 400
#define GREEN_RADIUS 300
#include "../communs_final/struct.h"

// ================== PACKETS ==================
//Connection :
unsigned char connection1[] = {0xff,0x0,0x0,0x0,0x0};
unsigned char connection2[] = {0xfe,0x0d,0x0,0x0,0x0};

//Chiens :
unsigned char green2[] = {0x00,0x67,0x72,0x65,0x65,0x6E,0x32,0x00};


Node green_node;
Dog green_dog;
Path * path;


Point ally_sheepfold_center;
Point ennemy_sheepfold_center;
int sheepfold_radius;
