/*#include "libwebsockets.h"
#include "struct.h"*/

void sendToPoint(struct lws *wsi, Point p);

Point getPositionPointFromId(int id, Node* nodeList);
