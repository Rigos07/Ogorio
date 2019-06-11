#include <stdio.h>
#include <math.h>
#include "structures.h"

float distance(Point p1, Point p2){
	return sqrt( pow((p1.x - p2.x),2) + pow((p1.y - p2.y),2) );
}
