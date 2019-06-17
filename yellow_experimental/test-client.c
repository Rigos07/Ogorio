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
#include "yellow.h"


// compile with gcc -Wall -g -o sock ./test-client.c -lwebsockets -lm

Point Yellow_behavior(Dog *yellow, NodeList **nodes_in_sight){
	Point objective;
	NodeList *pointer = *nodes_in_sight;
	Point blue_pos = create_point(0,0);
	float distance_to_destination;
	if(is_near_point(yellow->node.position, create_point(4500,3000), MARGIN)){
		if((*nodes_in_sight) != NULL){
			pointer = nl_portion_by_nick(nodes_in_sight, "blue");
			if(pointer != NULL){
				blue_pos = pointer->node.position;
			}
			if(yellow->message.started){
				decode_msg(yellow,blue_pos);
				printf("blue pos : %d %d\n", blue_pos.x, blue_pos.y);
				printf("OK ALORS : size i : %d id i : %d x i : %d y i : %d\n", yellow->message.size_i,yellow->message.id_i,yellow->message.x_i,yellow->message.y_i);
				if(yellow->message.done){
					printf("J'AI COMPRIS : BREBIS N° %d  EN : %d , %d\n",yellow->message.id, yellow->message.position.x, yellow->message.position.y);
					yellow->message.started = 0;
					yellow->message.done = 0;
					objective = yellow->message.position;
				}
				else{
					objective = yellow->node.position;
				}
			}
			else{
				if(yellow->node.position.x == blue_pos.x && yellow->node.position.y == blue_pos.y ){
					yellow->message.started = 1;
				}
				objective = create_point(4500,3000);
			}
			
		}
		else{
			printf("EMPTY SIGHT\n");
		}
		objective = create_point(4500,3000);
	}
	else{
		objective = create_point(4500,3000);
	}
	return objective;
	/*printf("================= START ===============\n");
	if((*nodes_in_sight) != NULL){
		printf("CURRENT POSITION : %d , %d\n",yellow->node.position.x,yellow->node.position.y);
		if(yellow->target != NULL){
			//TARGET UPDATING
			pointer = get_nodelist_portion(nodes_in_sight,yellow->target->id);
			if(pointer != NULL){
				yellow->target = &(pointer->node);
				if(is_closest_to_sheep(yellow->target->position, yellow->node, pointer) == 0){
					//ABORTING
					printf("\nMEINE TARGET\n");
					printnode(*(yellow->target));
					printf("ABORTING : OTHER KOLLEGE IS CLOSEST TO SHEEP\n");
					free(yellow->target);
					yellow->target = NULL;
					objective = follow_path(&path, *yellow , 9999999);	
				}
				else{
					//SHEEP CHASING
					distance_to_destination = distance(yellow->target->position,sheepfold_center);
					if(distance_to_destination >= sheepfold_radius - MARGIN){ //HAVE A TARGET AND TARGET IS IN SIGHT AND OUTSIDE SHEEPFOLD
						objective = bring_back_sheep(*(yellow->target), YELLOW_RADIUS, sheepfold_center);
						printf("\nI'M BRINGING MY TARGET BACK HOME : \n");
						printf("I'M TARGETING %s, LOCATED AT %d , %d\n", yellow->target->nickname, yellow->target->position.x, yellow->target->position.y);
						printf("GOING TO : %d , %d\n", objective.x, objective.y );
						printf("DISTANCE TO SHEEPFOLD : %f\n", distance_to_destination);
						printf("KEINE GNADE, MEINE KINDER !\n" );
					}
					else{ //HAVE A TARGET AND TARGET IS IN SIGHT AND INSIDE SHEEPFOLD
						printf("TARGET BRINGED BACK TO SHEEPFOLD, AUF WIDERSEHEN MEINE SCHAF\n* MISSION COMPLETE *\n");
						objective = follow_path(&path, *yellow , 9999999);
						free(yellow->target);
						yellow->target = NULL;
					}
				}
			}
			else{ //HAVE A TARGET AND TARGET IS NOT IN SIGHT
				objective = yellow->target->position;
				printf("\nGOING TO A TARGET OUT OF SIGHT : \n");
				printf("I'M TARGETING %s, LOCATED AT %d , %d\n", yellow->target->nickname, yellow->target->position.x, yellow->target->position.y);
				printf("GOING TO : %d , %d\n", objective.x, objective.y );
				printf("FINDE ES MEINE BRÜDER !\n" );
			}
		}
		else{
			//TARGET FINDING
			if(yellow->sheeps != NULL){
				empty_nodelist(&yellow->sheeps);
				yellow->sheeps = NULL;
			}

			sheep_count(yellow, nodes_in_sight, sheepfold_center, sheepfold_radius);

			if(yellow->sheeps != NULL){
				yellow->target = malloc(sizeof(Node));
				*(yellow->target) = closest_sheep(*yellow, 9999999);

				pointer = *nodes_in_sight;
				if(is_closest_to_sheep(yellow->target->position, yellow->node, pointer) == 0){
					free(yellow->target);
					yellow->target = NULL;
				}

				if(yellow->target != NULL){
					printf("MEINE NEUES ZIEL : \n");
					objective = bring_back_sheep(*(yellow->target), YELLOW_RADIUS, sheepfold_center);
				}
				else{
					printf("FOLLOWING DEFAULT PATH, NO TARGET FOUND\n");
					objective = follow_path(&path, *yellow , 9999999);					
				}
			}
			else{ //HAVE NO TARGET AND NO POSSIBLE TARGET FOUND
				pointer = nl_portion_by_nick(nodes_in_sight, "blue");
				if(pointer != NULL){
					blue_pos = pointer->node.position;
					printf("YELLOW POS : %d %d\n", blue_pos.x, blue_pos.y);
					printf("BLUE POS : %d %d\n", yellow->node.position.x, yellow->node.position.y);
					if(yellow->message.started){
						decode_msg(yellow,blue_pos);
						printf("OUAIS OK JE VOIS\n");
						if(yellow->message.done){
							printf("J'AI COMPRIS : BREBIS N° %d  EN : %d , %d\n",yellow->message.id, yellow->message.position.x, yellow->message.position.y);
							yellow->target = malloc(sizeof(Node));
							*(yellow->target) = create_node(yellow->message.id, yellow->message.position, "UN TRUC");
							printnode(*(yellow->target));
							yellow->message.started = 0;
							objective = yellow->target->position;
						}
						else{
							objective = yellow->node.position;
							//objective = blue_pos;
						}
					}
					else{
						if(yellow->node.position.x == blue_pos.x && yellow->node.position.y == blue_pos.y){
							yellow->message.started = 1;
							printf("START COMMUNICATION\n");
						}
						printf("SYNCHRONISING....\n");
						objective = blue_pos;
					}
				}
				else{
					printf("FOLLOWING DEFAULT PATH\n");
					objective = follow_path(&path, *yellow , 9999999);
				}
			}
		}
	}
	else{ //NOTHING IN SIGHT
		objective = follow_path(&path, *yellow , 9999999);
		printf("NODE LIST IS EMPTY THIS SHOULD NOT HAPPEN\n");
	}
	printf("================ END ================\n");
	return objective;*/
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

/*A ENLEVER QUAND ON VEUT PLUS FAIRE JOUJOU
unsigned int rand_a_b(int a, int b){
	return rand()%(b+1-a)+a;
}*/

NodeList* getNodeInVision(unsigned char* buf, NodeList** head){
	int i=3;
	Node node;
	//printf("Salut :\n");
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

		/*printf("\n=========NODE==========\n");
		printnode(node);
		printf("\n=======================\n");*/

		if(*head == NULL || get_nodelist_portion(head,node.id) == NULL){
			add_node(head, node);
		}

		i=i+1+nameLen;
	}
	//printf("Au revoir :\n");
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
			sendCommand(wsi,yellow, sizeof(yellow));
			break;

		case 16 :
			nodeInVision = NULL;
			if(compteur !=0){
				getNodeInVision(buf,&nodeInVision);
				dog_node = get_nodelist_portion(&nodeInVision,yellow_dog.node.id);
				if(dog_node != NULL){
					yellow_dog.node = dog_node->node;
				}
				/*printf("\n=========1============\n");
				printlist(&nodeInVision);
				printf("=========2============\n");*/

			}else{
				compteur++;
			}
			break;

		case 32 :
			myId = getMyId(buf);
			yellow_node = create_node(myId, create_point(0, 0), "yellow");
			yellow_dog = create_dog(yellow_node, YELLOW_SIGHTX, YELLOW_SIGHTY);
			yellow_dog.message = create_message(0, create_point(0,0));
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
					sheepfold_radius = xMax/10;
				}
			}else{
				p = Yellow_behavior(&yellow_dog, &nodeInVision);

				sendToPoint(wsi,p);
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
