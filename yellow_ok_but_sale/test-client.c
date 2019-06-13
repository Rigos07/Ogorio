#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

#include "client.h"
#include "package.h"
#include "struct.h"


// compile with gcc -Wall -g -o sock ./test-client.c -lwebsockets -lm

Point bring_back_sheep(Node target,int radius, Point destination){
	int dx, dy;
	int finalx,finaly;
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

	delta_x = ( ((radius - 90)*dx) / distance_to_destination );
	delta_y = ( ((radius - 90)*dy) / distance_to_destination );


	finalx = floor(target.position.x + delta_x);
	finaly = floor(target.position.y + delta_y);

	if(finalx < 0){
		finalx = 0;
	}
	if (finaly < 0) {
		finaly = 0;
	}
	objective.x = finalx;
	objective.y = finaly;
	return objective;
}

Point Yellow_behavior(Dog *yellow, Point sheepfold_center, int sheepfold_rad, NodeList **nodes_in_sight){
	Point objective;
	NodeList *pointer = *nodes_in_sight;
	float distance_to_destination;
	int target_found = 0;

	if(yellow->target != NULL){
		printf("I HAVE A TARGET\n");
		if((*nodes_in_sight) != NULL){
			pointer = get_nodelist_portion(nodes_in_sight,yellow->target->id);
			if(pointer != NULL){
				yellow->target = &(pointer->node);
				distance_to_destination = distance(yellow->target->position,sheepfold_center);
				if(distance_to_destination >= sheepfold_rad ){ //HAVE A TARGET AND TARGET IS IN SIGHT AND OUTSIDE SHEEPFOLD
					printf("\nMEINE TARGET\n");
					printnode(pointer->node);
					objective = bring_back_sheep(*(yellow->target), 100, sheepfold_center);
					printf("GOING TO : %d , %d\n", objective.x, objective.y );
					printf("DIST = %f\n", distance_to_destination);
					printf("SHEEPFOLD IS IN %d , %d !!!!\n",sheepfold_center.x,sheepfold_center.y );
					printf("KEINE GNADE, MEINE KINDER !\n" );
				}
				else{ //HAVE A TARGET AND TARGET IS IN SIGHT AND INSIDE SHEEPFOLD
					printf("TARGET BRINGED BACK TO SHEEPFOLD\nMISSION COMPLETE\n");
					objective.x = 4500;
					objective.y = 3000;
					yellow->target = NULL;
				}
			}
			else{ //HAVE A TARGET AND TARGET IS NOT IN SIGHT
				objective.x = 4500;
				objective.y = 3000;
				printf("GOING TO DEFAULT POSITION BECAUSE I LOST MY TARGET I AM VERY SAD\n");
			}
		}
		else{ //HAVE A TARGET AND NOTHING IN SIGHT
			objective.x = 4500;
			objective.y = 3000;
			printf("NODE LIST IS EMPTY THIS SHOULD NOT HAPPEN\n");
		}

	}
	else{
		if((*nodes_in_sight) != NULL){
			while(pointer != NULL && !target_found ){
				if(strncmp("bot",pointer->node.nickname,strlen("bot")) == 0){
					distance_to_destination = distance(pointer->node.position,sheepfold_center);
					if(distance_to_destination >= sheepfold_rad ){
						target_found = 1;
					}
				}
				if(target_found == 0){
					pointer = pointer->next;
				}

			}
			if(pointer != NULL){ //HAVE NO TARGET AND SHEEP IS IN SIGHT AND OUTSIDE SHEEPFOLD
				yellow->target = &(pointer->node);
				printf("\nNEW TARGET\n");
				printnode(pointer->node);
				objective = bring_back_sheep(*(yellow->target), 100, sheepfold_center);
				printf("SHEEPFOLD IS IN %d , %d !!!!\n",sheepfold_center.x,sheepfold_center.y );
			}
			else{ //HAVE NO TARGET AND NO POSSIBLE TARGET FOUND
				objective.x = 4500;
				objective.y = 3000;
				printf("GOING TO DEFAULT POSITION\n");
			}
		}
		else{ //HAVE NO TARGET AND NOTHING IN SIGHT
			objective.x = 4500;
			objective.y = 3000;
			printf("GOING TO DEFAULT POSITION\n");
		}

	}
	return objective;
}


// =====================================================================================================================================
//	Start of function definition
// =====================================================================================================================================

// Caught on CTRL C
void sighandler(int sig)
{
	forceExit = 1;
}

/**
\brief Allocate a packet structure and initialise it.
\param none
\return pointer to new allocated packet
****************************************************************************************************************************/
t_packet *allocatePacket()
{
	t_packet *tmp;

	if ((tmp=malloc(sizeof(t_packet))) == NULL ) return NULL;
	memset(tmp,0,sizeof(t_packet));
	return tmp;
}

/**
\brief Add a packet to the list of packet to be sent
\param wsi websocket descriptor
\param buf buffer to be sent
\param len length of packet
\return pointer to new allocated packet
****************************************************************************************************************************/
int sendCommand(struct lws *wsi,unsigned char *buf,unsigned int len)
{
	t_packet *tmp,*list=packetList;

	if (len > MAXLEN ) return -1;
	if ((tmp=allocatePacket()) == NULL ) return -1;
	memcpy(&(tmp->buf)[LWS_PRE],buf,len);
	tmp->len=len;
	if (packetList == NULL )
		packetList=tmp;
	else {
		while (list && list->next) {
			list=list->next;
		}
		list->next=tmp;
	}
	lws_callback_on_writable(wsi);
	return 1;
}


/****************************************************************************************************************************/
int writePacket(struct lws *wsi)
{
	t_packet *tmp=packetList;
	int ret;

	if (packetList == NULL ) return 0;

	packetList=tmp->next;
	ret=lws_write(wsi,&(tmp->buf)[LWS_PRE],tmp->len,LWS_WRITE_BINARY);
	free(tmp);
	lws_callback_on_writable(wsi);
	return(ret);
}

/*void sendToPoint(struct lws *wsi, Point p){
	unsigned char buf[13] = {0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	buf[2]=p.x>>8;
	buf[1]=p.x;
	buf[6]=p.y>>8;
	buf[5]=p.y;
printf("BITIETIUOGEZKJEGHIHEGHIEFHJH\n" );
	sendCommand(wsi,buf,sizeof(buf));
}*/

/****************************************************************************************************************************/

//A ENLEVER QUAND ON VEUT PLUS FAIRE JOUJOU
unsigned int rand_a_b(int a, int b){
	return rand()%(b+1-a)+a;
}

NodeList* getNodeInVision(unsigned char* buf, NodeList** head){
	int i=3;
	Node node;
	printf("Salut :\n");

	/*int k=0;
	printf("Buffer :\n");
	for(k=0;k<200;k++){
		printf("%x", buf[k]);
	}
	printf("\n");*/

	while(buf[i] + buf[i+1] + buf[i+2] + buf[i+3] != 0){

		node.id = buf[i]+(buf[i+1]<<8)+(buf[i+2]<<16);

		i=i+4;
		(node.position).x = buf[i]+(buf[i+1]<<8)+(buf[i+2]<<16);

		i=i+4;
		(node.position).y = buf[i]+(buf[i+1]<<8)+(buf[i+2]<<16);

		i=i+10;
		int nameLen = strlen(buf+i);
		node.nickname = malloc((nameLen+1)*sizeof(char));
		strcpy(node.nickname, buf+i);

		printf("\n=========NODE==========\n");
		printnode(node);
		printf("\n=======================\n");

		if(*head == NULL || get_nodelist_portion(head,node.id) == NULL){
			add_node(head, node);
		}




		i=i+1+nameLen;

	}
	printf("Au revoir :\n");
	return NULL;
}

int getMyId(unsigned char* buf){
	return  *(int *)(buf+1);//(buf[1] + (buf[2]<<8) + (buf[3]<<16));
}

void sendToPoint(struct lws *wsi, Point p){
	unsigned char buf[13] = {0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	buf[2]=p.x>>8;
	buf[1]=p.x;
	buf[6]=p.y>>8;
	buf[5]=p.y;

	sendCommand(wsi,buf,sizeof(buf));
}


/*
Fonction pour recevoir les packets
*/
int receive_packet(struct lws *wsi, unsigned char * buf){
	char typeMsg = buf[0];
	NodeList *nodeInVision, *dog_node;
	Point p;
	double xMin,yMin,xMax,yMax;
	int radius = 900;

	srand(time(NULL));

	switch(typeMsg){
		case 18:
			sendCommand(wsi,yellow, sizeof(yellow));
			break;

		case 16 :
			nodeInVision = NULL;
			if(compteur !=0){
				getNodeInVision(buf,&nodeInVision);
				dog_node = get_nodelist_portion(&nodeInVision,yellowdog.node.id);
				if(dog_node != NULL){
					yellowdog.node = dog_node->node;
				}
				printf("\n=========1============\n");
				printlist(&nodeInVision);
				printf("=========2============\n");

			}else{
				compteur++;
			}
			//printlist(&nodeInVision);
			break;

		case 32 :
			myId = getMyId(buf);
			yellownode.id = myId;
			yellowdog.node = yellownode;
			yellowdog.target = NULL;
			yellowdog.sheeps = NULL;
			//yellowdog.node->id = myId;
			break;

		case 64:
			if(myId == 0){
				double* border=malloc(4*sizeof(double));
				border = (double *)(buf+1);
				if(border[2] > 0 || border[3] > 0){ //Pour éviter le paquet malformé négatif
					xMin = border[0];
					yMin = border[1];
					xMax = border[2];
					yMax = border[3];
					path = generate_main_path(xMax, yMax);
					sheepfold_center.x = xMin;
					sheepfold_center.y = yMax/2;
				}
			}else{
				p = Yellow_behavior(&yellowdog, sheepfold_center, radius, &nodeInVision);
				/*if(get_node(&nodeInVision, myId) != NULL){
					p = get_node(&nodeInVision, myId)->position;
				}
				if(depart == 1){
					unsigned int x = rand()%(8960-40)+40;
					unsigned int y = rand()%(5960-40)+40;

					goal = createPoint(x,y);

					sendToPoint(wsi,goal);
					depart = 0;
				}
				if(p.x==goal.x && p.y==goal.y){
					depart =1;
				}*/
				sendToPoint(wsi,p);
				//printf("\nMon id : %d\nPOSITION : x : %d  y : %d\nOBJECTIF : x : %d  y : %d\n",myId, p.x,p.y,goal.x,goal.y);
				//affichageVisionFromId(myId, nodeInVision);


				//for(i=0; i<nbrNode; i++) free(nodeInVision[i].nickname);
				//free(nodeInVision);
			}
			break;
	}
	return 1;
}

/****************************************************************************************************************************/

static int callbackOgar(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
	static unsigned int offset=0;
	static unsigned char rbuf[MAXLEN];

	switch (reason) {
	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		fprintf(stderr, "ogar: LWS_CALLBACK_CLIENT_ESTABLISHED\n");

		//Connection :
		sendCommand(wsi,connection1,sizeof(connection1));
		sendCommand(wsi,connection2,sizeof(connection2));

		break;

 	case LWS_CALLBACK_CLIENT_WRITEABLE:
		if (writePacket(wsi) < 0 ) forceExit=1;
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		// we have receive some data, check if it can be written in static allocated buffer (length)

		if (offset + len < MAXLEN ) {
			memcpy(rbuf+offset,in,len);
			offset+=len;
			// we have receive some data, check with websocket API if this is a final fragment
			if (lws_is_final_fragment(wsi)) {
				// call recv function here
				receive_packet(wsi,rbuf);
				offset=0;
			}
		} else {	// length is too long... get others but ignore them...
			offset=MAXLEN;
		 	if ( lws_is_final_fragment(wsi) ) {
				offset=0;
			}
		}
		break;
	case LWS_CALLBACK_CLOSED:
		lwsl_notice("ogar: LWS_CALLBACK_CLOSED\n");
		forceExit = 1;
		break;
	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		lwsl_err("ogar: LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
		forceExit = 1;
		break;

	case LWS_CALLBACK_COMPLETED_CLIENT_HTTP:
		lwsl_err("ogar: LWS_CALLBACK_COMPLETED_CLIENT_HTTP\n");
		forceExit = 1;
		break;

	default:
		break;
	}

	return 0;
}





/****************************************************************************************************************************/
int main(int argc, char **argv)
{
	int n = 0;

	struct lws_context_creation_info info;
	struct lws_client_connect_info i;

	struct lws_context *context;
	const char *protocol,*temp;

	memset(&info, 0, sizeof info);
	memset(&i, 0, sizeof(i));

	if (argc < 2)
		goto usage;


	while (n >= 0) {
		n = getopt(argc, argv, "hsp:P:o:");
		if (n < 0)
			continue;
		switch (n) {
		case 's':
			i.ssl_connection = 2;
			break;
		case 'p':
			i.port = atoi(optarg);
			break;
		case 'o':
			i.origin = optarg;
			break;
		case 'P':
			info.http_proxy_address = optarg;
			break;
		case 'h':
			goto usage;
		}
	}

	srandom(time(NULL));

	if (optind >= argc)
		goto usage;

	signal(SIGINT, sighandler);

	if (lws_parse_uri(argv[optind], &protocol, &i.address, &i.port, &temp))
		goto usage;

	if (!strcmp(protocol, "http") || !strcmp(protocol, "ws"))
		i.ssl_connection = 0;
	if (!strcmp(protocol, "https") || !strcmp(protocol, "wss"))
		i.ssl_connection = 1;

	i.host = i.address;
	i.ietf_version_or_minus_one = -1;
	i.client_exts = NULL;
	i.path="/";

	info.port = CONTEXT_PORT_NO_LISTEN;
	info.protocols = protocols;
	info.gid = -1;
	info.uid = -1;

	context = lws_create_context(&info);
	if (context == NULL) {
		fprintf(stderr, "Creating libwebsocket context failed\n");
		return 1;
	}

	i.context = context;

	if (lws_client_connect_via_info(&i)); // just to prevent warning !!

	forceExit=0;
	// the main magic here !!
	while (!forceExit) {

		lws_service(context, 1000);
	}
	// if there is some errors, we just quit
	lwsl_err("Exiting\n");
	lws_context_destroy(context);

	return 0;

usage:
	fprintf(stderr, "Usage: ogar-client -h -s -p <port> -P <proxy> -o <origin>  <server address> \n");
	return 1;
}
