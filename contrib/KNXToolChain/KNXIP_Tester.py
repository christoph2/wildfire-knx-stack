
#
#   KNX/IP
#
#   UDP-Multicast on: 224.0.23.12:3671
#

SERVERPORT= 3671
SERVERADDRESS= "128.130.60.107"
MULTICASTADDRESS= "224.0.23.12"


# cEMI
"""
#define	L_BUSMON_IND		0x2B
#define	L_RAW_IND			0x2D
#define	L_RAW_REQ			0x10
#define	L_RAW_CON			0x2F
#define	L_DATA_REQ			0x11
#define	L_DATA_CON			0x2E
#define	L_DATA_IND			0x29
#define	L_POLL_DATA_REQ	0x13
#define	L_POLL_DATA_CON	0x25
#define	M_PROP_READ_REQ	0xFC
#define	M_PROP_READ_CON	0xFB
#define	M_PROP_WRITE_REQ	0xF6
#define	M_PROP_WRITE_CON	0xF5
#define	M_PROP_INFO_IND	0xF7
#define	M_RESET_REQ			0xF1
#define	M_RESET_IND			0xF0

/*
 * ************************************ typedefinition ***
 */

typedef unsigned char byte;
typedef unsigned short word;


typedef struct {
    byte mc;
    byte ioth;
    byte iotl;
    byte oi;
    byte pid;
    word noe_six;
    byte *data;
} CEMI_MGMT_MESSAGE;

typedef struct {
    byte mc;
    byte addil;
    byte ctrl1;
    byte ctrl2;
    word saddr;
    word daddr;
    byte apci_length;
    byte apci;
} CEMI_L_DATA_MESSAGE;

typedef struct {
    byte mc;
    byte ctrl1;
    word saddr;			/*if message is a CON or REQ message this field is unused */
    word daddr;
    byte ctrl2;
    byte npdu;
} EMI_L_DATA_MESSAGE;

typedef struct {
    byte m_code;
    byte status;
    word timestamp;
    byte cfield;
    byte saddr;
    byte saddr1;
    byte daddr;
    byte daddr2;
    byte length;
    byte acpicontr;
} EMI_L_BUSMON_IND;

typedef struct {
    byte mc;
    byte length;
    word address;
} PC_GET_MESSAGE;

"""

"""
/**
 * Sends the passed packet to
 * the given address
 */
void sendPacket(EIBNETIP_PACKET * packet, struct sockaddr_in address)
{
    if (sendto(sock,
	       packet,
	       htons(packet->head.totalsize),
	       0, (struct sockaddr *) &address,
	       sizeof(struct sockaddr_in)) < 0) {
	bailOut("Error: can't sent UDP packet");
    }
}

"""

from twisted.internet.protocol import DatagramProtocol
from twisted.internet import reactor
from twisted.application.internet import MulticastServer

class MulticastServerUDP(DatagramProtocol):
    def startProtocol(self):
        print 'Started Listening'
        # Join a specific multicast group, which is the IP we will respond to
        self.transport.joinGroup('224.0.23.12')

    def datagramReceived(self, datagram, address):
        # The uniqueID check is to ensure we only service requests from ourselves
        if datagram == 'UniqueID':
            print "Server Received:" + repr(datagram)
            self.transport.write("data", address)

# Note that the join function is picky about having a unique object
# on which to call join.  To avoid using startProtocol, the following is
# sufficient:
#reactor.listenMulticast(3671, MulticastServerUDP()).join('224.0.23.12')

# Listen for multicast on 224.0.23.12:3671
reactor.listenMulticast(3671, MulticastServerUDP())
reactor.run()
