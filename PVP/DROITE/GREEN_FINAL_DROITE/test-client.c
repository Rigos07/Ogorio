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
#include "green.h"

// compile with gcc -Wall -g -o sock ./test-client.c -lwebsockets -lm

Point green_behavior(Dog *green, NodeList **nodes_in_sight){
	Point objective;
	NodeList *pointer = *nodes_in_sight;
	float distance_to_ally_sheepfold, distance_to_ennemy_sheepfold;
	printf("================= START ===============\n");
	if((*nodes_in_sight) != NULL){
		printf("CURRENT POSITION : %d , %d\n",green->node.position.x,green->node.position.y);
		if(green->target != NULL){
			//TARGET UPDATING
			pointer = get_nodelist_portion(nodes_in_sight,green->target->id);
			if(pointer != NULL){
				green->target = &(pointer->node);
				if(is_closest_to_sheep(green->target->position, green->node, pointer) == 0){
					//ABORTING
					printf("\nMa startup CIBLE\n");
					printnode(*(green->target));
					printf("Oh non un autre marcheur. Sur ceux je m'en vais !\n");
					free(green->target);
					green->target = NULL;
					objective = follow_path(&path, *green , 9999999);
				}
				else{
					//SHEEP CHASING
					distance_to_ally_sheepfold = distance(green->target->position,ally_sheepfold_center);
					distance_to_ennemy_sheepfold = distance(green->target->position,ennemy_sheepfold_center);
					if((distance_to_ally_sheepfold >= sheepfold_radius - MARGIN) && (distance_to_ennemy_sheepfold >= sheepfold_radius - MARGIN)){ //HAVE A TARGET AND TARGET IS IN SIGHT AND OUTSIDE SHEEPFOLD
						objective = bring_back_sheep(*(green->target), GREEN_RADIUS, ally_sheepfold_center);
						printf("\nI'M BRINGING MY TARGET BACK HOME : \n");
						printf("I'M TARGETING %s, LOCATED AT %d , %d\n", green->target->nickname, green->target->position.x, green->target->position.y);
						printf("GOING TO : %d , %d\n", objective.x, objective.y );
						printf("DISTANCE TO SHEEPFOLD : %f\n", distance_to_ally_sheepfold);
						printf("Je suis enfin un marcheur jeune et dynamique. Ma startup est en marche !!!!!\n" );
					}
					else{ //HAVE A TARGET AND TARGET IS IN SIGHT AND INSIDE SHEEPFOLD
						printf("Oh non des entrepreneurs véreux dans le SHEEPFOLD, vite, je quitte le projet\n* Je vais sur le chemin NORMAL.\nPARCE C'EST NOTRE PROJET !!!!\n");
						objective = follow_path(&path, *green , 9999999);
						free(green->target);
						green->target = NULL;
					}
				}
			}
			else{ //HAVE A TARGET AND TARGET IS NOT IN SIGHT
				objective = green->target->position;
				printf("\nGOING TO A TARGET OUT OF SIGHT : \n");
				printf("I'M TARGETING %s, LOCATED AT %d , %d\n", green->target->nickname, green->target->position.x, green->target->position.y);
				printf("GOING TO : %d , %d\n", objective.x, objective.y );
				printf("AH ENFIN, j'ai trouvé ma startup du futur\n");
			}
		}
		else{
			//TARGET FINDING
			if(green->sheeps != NULL){
				empty_nodelist(&green->sheeps);
				green->sheeps = NULL;
			}

			sheep_count(green, nodes_in_sight, ally_sheepfold_center, ennemy_sheepfold_center, sheepfold_radius);

			if(green->sheeps != NULL){
				green->target = malloc(sizeof(Node));
				*(green->target) = closest_sheep(*green, 9999999);

				pointer = *nodes_in_sight;
				if(is_closest_to_sheep(green->target->position, green->node, pointer) == 0){
					free(green->target);
					green->target = NULL;
				}

				if(green->target != NULL){
					printf("MA NOUVELLE STARTUP : \n");
					objective = bring_back_sheep(*(green->target), GREEN_RADIUS, ally_sheepfold_center);
				}
				else{
					printf("Je retourne sur le chemin NORMAL, ce JOBDATING ne m'a pas permis de touver une STARTUP.\n");
					objective = follow_path(&path, *green , 9999999);
				}
			}
			else{ //HAVE NO TARGET AND NO POSSIBLE TARGET FOUND
				printf("FOLLOWING DEFAULT PATH\n");
				objective = follow_path(&path, *green , 9999999);
			}
		}
	}
	else{ //NOTHING IN SIGHT
		objective = follow_path(&path, *green , 9999999);
		printf("NODE LIST IS EMPTY THIS SHOULD NOT HAPPEN\n");
	}
	printf("================ END ================\n");
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


/****************************************************************************************************************************/

NodeList* getNodeInVision(unsigned char* buf, NodeList** head){
	int i=3;
	Node node;

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

		if(*head == NULL || get_nodelist_portion(head,node.id) == NULL){
			add_node(head, node);
		}

		i=i+1+nameLen;
	}
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

	srand(time(NULL));

	switch(typeMsg){
		case 18:
			sendCommand(wsi,green2, sizeof(green2));
			break;

		case 16 :
			nodeInVision = NULL;
			if(compteur !=0){
				getNodeInVision(buf,&nodeInVision);
				dog_node = get_nodelist_portion(&nodeInVision,green_dog.node.id);
				if(dog_node != NULL){
					green_dog.node = dog_node->node;
				}

			}else{
				compteur++;
			}
			break;

		case 32 :
			myId = getMyId(buf);
			green_node = create_node(myId, create_point(0, 0), "green2");
			green_dog = create_dog(green_node, GREEN_SIGHTX, GREEN_SIGHTY);
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
					green_node = create_node(myId, create_point(0, 0), "green2");
					green_dog = create_dog(green_node, GREEN_SIGHTX, GREEN_SIGHTY);
					path = generate_path(green_dog, xMax, yMax);
					ally_sheepfold_center.x = xMax;
					ally_sheepfold_center.y = yMax/2;
					ennemy_sheepfold_center.x = xMin;
					ennemy_sheepfold_center.y = yMax/2;
					sheepfold_radius = xMax/10;
				}
			}else{
				p = green_behavior(&green_dog, &nodeInVision);

				if(!is_near_point(green_dog.node.position,ORIGIN,0)){
					sendToPoint(wsi,p);
				}
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
