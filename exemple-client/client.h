#include "libwebsockets.h"
#define MAXLEN 20000

static int callbackOgar(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

int forceExit;

typedef struct s_packet {
        unsigned char buf[MAXLEN+LWS_PRE];
        unsigned int len;
        struct s_packet *next;
} t_packet;

/// Pointer on the list of packet to be send when socket is writeable
t_packet *packetList=NULL;

/// LibWebsocket interlan structure needed for API
struct lws_protocols protocols[] = {
	{
    	"ogar_protocol",
	callbackOgar,
    	0,
    	20
    	},
	{
	NULL,
	NULL,
	0,
	0
	}
};

typedef struct node{
        char id;
        int posX;
        int posY;
        char* nickname;
}node;

int compteur = 0;
int monId = 0;
char depart = 1;
unsigned char msg[] = {0xff,0x0,0x0,0x0,0x0};
unsigned char msg2[] = {0xfe,0x0d,0x0,0x0,0x0};
unsigned char cyan[] = {0x00,0x63,0x79,0x61,0x6E,0x00};
unsigned char green[] = {0x00,0x67,0x72,0x65,0x65,0x6E,0x00};
unsigned char blue[] = {0x00,0x62,0x6C,0x75,0x65,0x00};
unsigned char commande[]={0x10,0xB8,0x0B,0x00,0x00,0xB8,0x0B,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char commande2[]={0x10,0xB9,0x0B,0x00,0x00,0xB8,0x0B,0x00,0x00,0x00,0x00,0x00,0x00};
static int callbackOgar(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);
