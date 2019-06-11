#include <string.h>
#include <stdlib.h>

#include "struct.h"

/**
\brief : Give all the node in the dog's vision
\param buf : the buffer that has to be decoded
\param result : a chained list of Node which is all the Node seen
\return : the number of node seen as a integer
*/
int getNodeInVision(unsigned char* buf, Node* result){
	int i=3, k=0;

	while(buf[i] + buf[i+1] + buf[i+2] + buf[i+3] != 0){

		(result+k)->id = buf[i]+(buf[i+1]<<8)+(buf[i+2]<<16);

	  i=i+4;
	  ((result+k)->position).x = buf[i]+(buf[i+1]<<8)+(buf[i+2]<<16);

		i=i+4;
	  ((result+k)->position).y = buf[i]+(buf[i+1]<<8)+(buf[i+2]<<16);

		i=i+10;
		int nameLen = strlen(buf+i);
		(result+k)->nickname = malloc((nameLen+1)*sizeof(char));
		strcpy((result+k)->nickname, buf+i);

		i=i+1+nameLen;

		k++;
	}
	return k;
}

//////////// A TESTER /////////////
int getMyId(unsigned char* buf){
  return  *buf+1//(buf[1] + (buf[2]<<8) + (buf[3]<<16));
}

double* getMapSize(unsigned char* buf){
  return (double *)(buf+1);
}
