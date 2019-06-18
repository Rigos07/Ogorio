
#define GREEN_SIGHTX 400
#define GREEN_SIGHTY 400
#define GREEN_RADIUS 300
#include "../communs_final/struct.h"

// ================== PACKETS ==================
//Connection :
unsigned char connection1[] = {0xff,0x0,0x0,0x0,0x0};
unsigned char connection2[] = {0xfe,0x0d,0x0,0x0,0x0};

//Chiens :
unsigned char cyan1[] = {0x00,0x63,0x79,0x61,0x6E,0x31,0x00};
unsigned char green1[] = {0x00,0x67,0x72,0x65,0x65,0x6E,0x31,0x00};
unsigned char blue1[] = {0x00,0x62,0x6C,0x75,0x65,0x31,0x00};
unsigned char red1[] = {0x00,0x72,0x65,0x64,0x31,0x00};
unsigned char purple1[] = {0x00,0x70,0x75,0x72,0x70,0x6C,0x65,0x31,0x00};
unsigned char yellow1[] = {0x00,0x79,0x65,0x6C,0x6C,0x6F,0x77,0x31,0x00};

Node green_node;
Dog green_dog;
Path * path;


Point sheepfold_center;
int sheepfold_radius;