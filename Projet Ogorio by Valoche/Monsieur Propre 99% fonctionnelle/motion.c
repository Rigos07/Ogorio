#include "struct.h"

void sendToPoint(struct lws *wsi, Point p){
	unsigned char buf[13] = {0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	buf[2]=p.x>>8;
	buf[1]=p.x;
	buf[6]=p.y>>8;
	buf[5]=p.y;

	sendCommand(wsi,buf,sizeof(buf));
}


Point getPositionPointFromId(int id, Node* nodeList){
	Point p,p1;
	int i = 0;
	while(nodeList[i].id != id){
		i++;
	}
	p1 = nodeList[i].position;
	p.x=p1.x;
	p.y=p1.y;
	return p;
}
