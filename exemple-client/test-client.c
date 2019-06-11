#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <sys/time.h>
#include <unistd.h>


#include "client.h"

// compile with gcc -Wall -g -o sock ./test-client.c -lwebsockets


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

/*
Fonction pour lire la position
*/
//A faire ptdr
int decodNode(unsigned char* paquet, node* resultat){
	int i=3, k=0;

	while(paquet[i] + paquet[i+1] + paquet[i+2] + paquet[i+3] != 0){

		(resultat+k)->id = paquet[i]+(paquet[i+1]<<8)+(paquet[i+2]<<16);

	  i=i+4;
	  (resultat+k)->posX = paquet[i]+(paquet[i+1]<<8)+(paquet[i+2]<<16);

		i=i+4;
	  (resultat+k)->posY = paquet[i]+(paquet[i+1]<<8)+(paquet[i+2]<<16);

		i=i+10;

		int tailleNom = strlen(paquet+i);
		(resultat+k)->nickname = malloc((tailleNom+1)*sizeof(char));
		strcpy((resultat+k)->nickname, paquet+i);

		i=i+1+tailleNom;

		k++;
	}
	return k;
}

int getPosFromID(int id, node* listeNode){
	int i = 0;
	while(listeNode[i].id != id){
		i++;
	}
	return {listeNode[i].posX,listeNode[i].posY};
}

void sendPosition(struct lws *wsi, int posX, int posY){
	unsigned char paquet[13] = {0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	paquet[2]=posX>>8;
	paquet[1]=posX-paquet[2];
	paquet[6]=posY>>8;
	paquet[5]=posY-paquet[2];

	sendCommand(wsi,paquet,sizeof(paquet));
}

/*
Fonction pour recevoir les packets
*/
int receive_packet(struct lws *wsi, unsigned char * buf){
	int i,x,j,posX,posY, nombreNode;
	char typeMsg = buf[0];
	node* listeVision;

	switch(typeMsg){
		case 18:
			sendCommand(wsi,blue, sizeof(blue));
			break;

		case 16 :
			listeVision = malloc(15*sizeof(node));
			nombreNode = decodNode(buf, listeVision);
			break;

		case 32 :
			monId = buf[1] + (buf[2]<<8) + (buf[3]<<16);
			break;

		case 64:
			if(monId != 0){
				printf("Id : %d\n", monId);
				posX = getPosX(monId,listeVision)[0];
				posY = getPosY(monId,listeVision)[1];
				printf("x : %d y : %d\n", posX, posY);
				if(depart == 1){
					sendPosition(wsi, 3000,4020);
					depart = 0;
				}
				if(posX==3000-11 && posY==4020-11){
					depart =1;
				}
				if(depart == 1){
					sendPosition(wsi, 3000,3000);
					depart = 0;
				}

				free(listeVision);
			}
			if(posX==3000-11 && posY==3000-11){
				depart = 1;
			}

			break;
	}

	//printf("Fin\n" );
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
		sendCommand(wsi,msg,sizeof(msg));
		sendCommand(wsi,msg2,sizeof(msg2));
		//sendCommand(wsi,msg3,sizeof(msg3));
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
