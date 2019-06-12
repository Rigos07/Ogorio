#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "fonctions_communes.h"

Point bring_back_sheep(Node target,int radius, Point destination){
	int dx, dy;
	float delta_x, delta_y, distance_to_destination;
	Point objective;

	dx = target.position.x - destination.x;
	dy = target.position.y - destination.y;
	distance_to_destination = distance(target.position, destination);

	if(distance_to_destination == 0){
		objective.x = target.position.x;
		objective.y = target.position.y;
		return objective;
	}

	delta_x = ( ((radius - MARGIN)*dx) / distance_to_destination );
	delta_y = ( ((radius - MARGIN)*dy) / distance_to_destination );

	objective.x = floor(target.position.x + delta_x);
	objective.y = floor(target.position.y + delta_y);
	return objective;
}

Point Yellow_behavior(Dog yellow, Point sheepfold_center, int sheepfold_rad, NodeList *nodes_in_sight){
	Point objective;
	NodeList *pointer;
	float distance_to_destination;
	if(nodes_in_sight != NULL){
		while(pointer != NULL && strncmp("bot",pointer->node.nickname,strlen("bot"))){
			pointer = pointer->next;
		}
		if(pointer != NULL){
			yellow.target = &(pointer->node);
		}
	}
	if(yellow.target != NULL){
		distance_to_destination = distance(yellow.target->position,sheepfold_center);
		if(distance_to_destination > sheepfold_rad ){
			objective = bring_back_sheep(*(yellow.target), 100, sheepfold_center);
		}
		else{
			objective.x = 4500;
			objective.y = 3000;
		}
	}
	else{
		objective.x = 4500;
		objective.y = 3000;

	}
	return objective;
}

int main(){
	Dog *Yellow = malloc(sizeof(Dog));
	Node yellownode = {15,{0,0},"yellow"};
	Yellow->node = &yellownode;
	Yellow->sheeps = NULL;
	int yellow_rad = 100, sheepfold_rad = 900;
	Point sheepfold_center = {0,3000};
	return 1;
}