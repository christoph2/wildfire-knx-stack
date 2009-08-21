
/*
**  Hinweis: ISO-konforme Initialien: Ph - Physical, DL - Datalink.
*/

/*
**  Hinweis: In Verbindung mit 'miniOSEK' werden vom Schnittstellen-Interrupt (ISR2)
**           Messages an die Task versendet, welche die LL-StateMachine implementiert.
*/

/* Hinweis: Allgemeines zur Link-Layer-Statemachine findet sich unter '3/2/2'-2.5 */

/*
    Parameters of Data Link Layer
    -----------------------------
    The following parameters influence the behaviour of Data Link Layer and are
    required inside Data Link Layer in order to operate correctly:
    
    - Individual Address:    unique Individual Address of this device
    - address table:         address table with the group address(es) of this device
    - nak_retry:             defines the number of retries in case of a NAK response
                             or a acknowledgment timeout
    - busy_retry:            defines the number of retries in case of a BUSY response
    - poll group:            address the poll group address of this device
    - response slot:         number the response slot number of this device
    - data link layer mode:  either the normal or the bus monitor mode of the data link layer.
*/

/*
    Hinweis: Der BusState-Handler kann nur verwendet werden, wenn die 'RESET'- und
    evtl. die 'SAVE'- Anschlüsse der TPUART mit der MCU verbunden sind.

    Hinweis: die Transition in den 'Busoff'-/'Saving'-State muss Objekte mit
    dem Status 'TRANSMITTING' auf 'IDLE_ERROR' setzen, um Blockaden zu vermeiden.

    Hinweis: 'TEMPERATURE_WARNING' führt (vorübergehend) zu 'BUSOFF'.

typedef enum tagKNX_BusStateType {
    E_BUSSTATE_OK,
    E_BUSSTATE_BUSOFF,
    E_BUSSTATE_SAVING   // evtl.    
} KNX_BusStateType;

void BusStateHandler(KNX_BusStateType); // System-defininierte Handler (s. 'TRANSMITTING), User-Callback möglich;
                                        // ausserdem wird von hieraus 'UserSave' aufgerufen.
KNX_BusStateType PH_GetBusState(void);
*/

/*  Check: Macht eine Funktion 'PH_IsTransmitting() ' Sinn (Prüfen, ob bereits ein Telegramm gesendet wird)??? */

/* todo: die Sache mit 'printf' elegant lösen!!! */

#if !defined(__HIWARE__)
#include <stdio.h>  /* wg. printf(); todo: durch DEBUG_PRINT() ersetzen. */
#endif

#include "tpuart.h"
#include "Messaging.h"
#include "Memory.h"

#define ACK_INFORMATION 0x10

#define ACK_ADDRESSED   0x01
#define ACK_BUSY        0x02
#define ACK_NACK        0x04

static void Disp_L_DataReq(void),Disp_L_PollDataReq(void);

static const KNXLayerServiceFunctionType LL_Services[]={
/*      Service                     Handler                 */
/*      ====================================================*/
/*      L_DATA_REQ              */  Disp_L_DataReq,
/*      L_POLL_DATA_REQ         */  Disp_L_PollDataReq,
/*      ====================================================*/
};

static const KNXLayerServicesType LL_ServiceTable[]={
    KNX_LL_SERVICES,2,LL_Services    
};


/*
**
**  todo: Mechanismus zur Erkennung von Übertragungsfehlern (SCI/COM), wg. INACK!!!
**
*/

/*
**  wichtiger Hinweis zum Senden: die TPUART sendet empfangene Telegramme unmittelbar an den Host-Controller,
**      ein zuvor initiierter Sendevorgang _muss_ unbedingt pausiert werden, kann aber nach dem Empfang fortgesetzt
**      werden (Kontext-Switch).
*/


/*
**  todo: Prüfung auf Adressierung und Busy-Zustand ('IMP_IsBusy()').
*/

/*
**  todo: Getrennte Struktur f. Busmonitor-Timestamps (BuffAddr,TimeStamp),
**          sowie Funktion 'uint16 LL_GetTimeStamp(pBuffer); bedingte Kompilierung.
**
*/


/*
**  "The local layer-2 passes an L_Data.con primitive to the local user that indicates either a correct or
**  erroneous data transfer. Depending if an L2-acknowledgement is requested or not, this confirmation is
**  related to the reception of the L2-acknowledgement, or only to the transmission of the frame on the
**  medium."
*/


/* const uint8 T0[]={0xbc,0x11,0x64,0x09,0x01,0xe3,0x00,0x80,0x0d,0x14,0x44}; */
/*      ==> EIS5-Telegramm (26.00) */
/*      von     1.01.100 nach 1/1/001 */

/* const uint8 T[]={0xbc,0x11,0x01,0x49,0x01,0xe2,0x00,0x80,0x5f,0x26}; */
/*      (01.01.001==>09/1/001 Val: 103) */


/*uint8 T1[]={0xbc,0x11,0x64,0x11,0x01,0x61,0x43,0x80,0x84}; */
    /* A_Restart. */

/*uint8 T1[]={0xb0,0x11,0x64,0x00,0x00,0xE3,0x00,0xC0,0xaf,0xfe,0x48}; */
    /* A_PhysAddr_Write(0xAFFE). */

/*uint8 T1[]={0xb0,0x11,0x64,0x00,0x00,0xE1,0x01,0x00,0xda}; */
   /* A_PhysAddr_Read(). */

/*uint8 T1[]={0xb0,0x11,0x64,0x11,0x01,0x65,0x03,0xd5,0x01,0x05,0x10,0x00,0x8d}; */
    /* A_PropertyValueRead(). */

/* const uint8 T1[]={0xb0,0x11,0x64,0x11,0x01,0x64,0x03,0xd8,0x00,0x00,0x06,0x93}; */
    /* A_PropertyDescriptionRead(). */

const uint8 T0[]={0xbc,0x11,0x64,0x11,0x01,0x60,0x80,0xc6};
    /* T_Connect (01.01.100 ==> 01.01.001). */

const uint8 T1[]={0xbc,0x11,0x64,0x11,0x01,0x67,0x42,0x84,0x01,0x80,0xaa,0xbb,0xcc,0xdd,0x06};
    /* A_MemoryWrite */

const uint8 T2[]={0xbc,0x11,0x64,0x11,0x01,0x60,0x81,0xc7};
    /* T_Disconnect (01.01.100 ==> 01.01.001). */


/*
** todo: alle Variablen, die von der TPUART-StateMachine verwendent werden,
**       in einer Struktur zusammenfassen, dadurch sind mehrere
**       Instatanzen des LinkLayer-Treibers möglich!
*/

TPUART_RCV_STATE rcvState;      /* Empfangs-State-Machine. */
TPUART_RCV_SERVICE rcvService;

uint8 RcvLen;    /* Anzahl der Bytes, die hinterher kommen (todo: in 'BytesFollowing' umbenennen). */
uint8 RcvIdx;    /* Buffer-Pointer. */
uint8 Checksum;  /* Berechnete Prüfsumme. */
uint8 tsap;
Knx_AddressType dest_addr;
boolean addressed;

uint8 TpuartRcvBuf[BUF_LEN];     /* todo: Daten-Struktur mit Länge und Service (struct TPUART_BUFFER). */

PMSG_Buffer pBuffer;

uint8 AckService;

uint8 CalculateChecksum(PMSG_Buffer ptr);
void PH_AckInformation_req(uint8 flags);
void DBG_DUMP(PMSG_Buffer ptr);

void decode(uint8 b);

void StartTimeout(void);
void StopTimeout(void);
void OnTimeout(void);           /* Callback. */

void TPUARTInit(void)
{
        rcvState=TPSR_WAIT_RESET_IND;
        RcvLen=RcvIdx=0x00;
        AckService=0x00;
        rcvService=SERVICE_NONE;
        ZeroRAM(TpuartRcvBuf,BUF_LEN);
/*      TPUARTSendReset(); */
/*      wait 50ms (Timeout danach ??!!). */
/*      StartTimeout(); */
        rcvState=TPSR_WAIT;     /* auf gültigen Service warten. */
}

void TPTest(void)
{
        int b,len;
        static int cnt;

        switch (cnt) {
            case 0:
                len=sizeof(T0);

                for (b=0;b<len;b++) {
                    decode(T0[b]);
                }
                cnt++;
                break;
            case 1:
                len=sizeof(T1);

                for (b=0;b<len;b++) {
                    decode(T1[b]);
                }
                cnt++;            
                break;            
            case 2:
                len=sizeof(T2);

                for (b=0;b<len;b++) {
                    decode(T2[b]);
                }
                cnt++;            
                break;
            default:
                cnt=0;
                break;
        }

/*
        len=sizeof(T);

        for (b=0;b<len;b++) {
                decode(T[b]);
        }
*/
}

/*
**
** todo: Tabledriven Statemachine!!! 
**
*/
void decode(uint8 b)             /* Wird vom RX-Interrupt-Handler aufgerufen. */
{
        uint8 state;
        
        boolean stop;        

        TpuartRcvBuf[RcvIdx++]=b;       /* todo: Länge auf Overflow testen (TPSR_ERROR)!!! */

        switch (rcvState) {
                case TPSR_WAIT:

/*
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
*/                  
                if ((b & 0x13)==DATA_SERVICE_MASK) {
                        StartTimeout();         /* todo: nicht pauschal starten,  */
                                                /* nur bei Mehrbyte-Telegrammen. */

/*                      prio=(b & PRIO_MASK)>>2; */

/*                      repeated=((~b) & REPEATED_MASK)>>5;     // 1 ==> not repeated. */
                    /* todo: 'repeated' nicht so ohne weiteres übergehen!!! */

/*                      rcvService=SERVICE_DATA; */

                        if ((b & 0xd0)==L_DATA_IND_MASK) {
                                RcvLen=5;       /* zunächst fünf Bytes bis zum Längen-Byte einlesen. */
                                tsap=0x00;  /* 0xFE */
                                Checksum=b;
                                rcvState=TPSR_DATA_CONT1;
                        } else if ((b & 0xd0)==L_LONG_DATA_IND_MASK) {  /* todo: !!! TESTEN !!! */
/*                              rcvState=TPSR_LONG_DATA; */
                                rcvState=TPSR_WAIT;
                        } else if ((b & 0xff)==L_POLL_DATA_IND_MASK) {
                                rcvState=TPSR_POLL_DATA;
                        } else {        /* Fehlerhaftes Byte. */
                                rcvState=TPSR_WAIT;     /* auf gültigen Service warten. */
                                rcvService=SERVICE_NONE;
#if !defined(__HIWARE__)                                
/*    printf("0x%02x\tERROR\n",b); */
#endif                                
                        }               
                } else if ((b==IACK_IND) || (b==INACK_IND) || (b==IBUSY_IND)) { /* todo: IAcks 'ODER'-verkünpt in eine Zeile. */
                        rcvState=TPSR_WAIT;     /* auf nächsten Service warten. */
                        rcvService=SERVICE_IACK;
                        RcvIdx=0;
                        AckService=b;
                } else if ((b & 0x03)==0x03) {
                        rcvState=TPSR_WAIT;     /* auf nächsten Service warten. */
                        rcvService=SERVICE_CONTROL;
                        RcvIdx=0;
                        if (b==RESET_IND) {

                        } else if ((b & 0x7f)==0x0b) {

                        } else if ((b & 0x07)==0x07) {

                                state=(b & 0xf8)>>3;
                        } else {        /* Fehlerhaftes Byte. */
                                rcvService=SERVICE_NONE;
#if !defined(__HIWARE__)
/*    printf("0x%02x\tERROR\n",b); */
#endif                                
                        }
                }
/*                
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////                
*/

                        break;
                case TPSR_DATA_CONT1:
                        Checksum^=b;
                        if ((--RcvLen)==0x00) { /* Längenbyte erreicht. */
                            /* todo: BUSY-Handling. */
                            /* if (!PassthroughEveryTelegramm()) // RouteEvery */

                            dest_addr=btohs(*(uint16*)&TpuartRcvBuf[3]);
                                                        
                            if ((TpuartRcvBuf[5] & atMULTICAST)) {                            
                                addressed=ADR_IsAddressed(dest_addr,&tsap);
                            } else {
                                addressed=ADR_IsOwnPhysicalAddr(dest_addr);
                            }
                            
                            if (addressed) {
                                PH_AckInformation_req(ACK_ADDRESSED);
                            }
                                                        
                            RcvLen=(b & 0x0f)+2;    /* todo: begrenzen. */
                            rcvState=TPSR_DATA_CONT2;
                        }
                        break;
                case TPSR_DATA_CONT2:                   
                        if ((--RcvLen)==0x00) { /* O.K., komplettes Telegramm empfangen. */
                                Checksum^=0xff;
                                
                                if ((Checksum==TpuartRcvBuf[RcvIdx-1])) { /* Prüfsumme korrekt? */
                                    if (addressed) {
                                        /* OK, der Protokoll-Stack kann das Telegramm übernehmen. */
                                        pBuffer=MSG_AllocateBuffer();
                                        if (pBuffer!=(PMSG_Buffer)NULL) {
                                            /* todo: Message-Buffer fühlen. */
                                            pBuffer->service=L_DATA_IND;
                                            pBuffer->sap=tsap;
                                            pBuffer->len=RcvLen=(TpuartRcvBuf[5] & 0x0f)+7;  /* 8 mit Prüfsumme!!! */
                                                                                            /* todo: Konstante od. Makro statt 5!!! */
                                            CopyRAM((void*)pBuffer->msg,(void*)TpuartRcvBuf,RcvLen);                                                                                                
                                            (void)MSG_Post(pBuffer);           
                                        } else {
                                            stop=TRUE;
                                            /* Fehler-Behandlung. */
                                        }
                                    }

                                } else {
#if !defined(__HIWARE__)              
/*    printf("\n*** CHECKSUM-ERROR ***\n"); */   /* nein. */
#endif                                        
                                }

                                rcvService=SERVICE_DATA;
                                rcvState=TPSR_WAIT;     /* ... und von vorne. */
                                RcvIdx=0;
                        } else {
                                Checksum^=b;
                        }
                        break;
/*
                case TPSR_LONG_DATA:

                        break;
*/
                case TPSR_POLL_DATA:

                        break;
                default:
                        break;
        }
}

void StartTimeout(void)
{

}

void StopTimeout(void)
{

}

void OnTimeout(void)            /* Callback. */
{
/*      Differenzieren: z.B.: 'TPSR_WAIT_RESET_IND'. */
        rcvState=TPSR_WAIT;     /* wieder in den Warte-Zustand. */
}

void DBG_DUMP(PMSG_Buffer ptr)
{
    uint8 i/*,chk*/;
#if     !defined(__HIWARE__)
    for (i=0;i<ptr->len;i++) {
        
/*        printf("%02X ",ptr->msg[i]);  */
        
        if (i==6) {
/*            printf("["); */
        }
    }
/*    printf("]\t%02X\n",CalculateChecksum(ptr)); */
#endif    
}

void PH_AckInformation_req(uint8 flags)
{
/* PutSCI(ACK_INFORMATION | flags); */
}

/*
**
** Hinweis: diese Funktionen sind unabhängig von der TPUART - trennen !!!
**
*/
void LL_Task(void)
{
        KNXDispDispatchLayer(TASK_LL_ID,LL_ServiceTable);
}

/*
**
**  Services from Network-Layer.
**
*/

static void Disp_L_DataReq(void)
{
    uint8 chk;

    MSG_SetFrameType(MSG_ScratchBuffer,ftStandard);

    /* PREPARE_CONTROL_FIELD() */
    MSG_ScratchBuffer->msg[0] |= 0x30;        /* fixed one bit + repeated. */
    MSG_ScratchBuffer->msg[0] &= (~(uint8)3);  /* clear two LSBs. */
    /**/
                    
    chk=CalculateChecksum(MSG_ScratchBuffer);
    
    (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
    DBG_DUMP(MSG_ScratchBuffer);    
}

static void Disp_L_PollDataReq(void)
{
    /* todo: Implementieren!!! */
    MSG_SetFrameType(MSG_ScratchBuffer,ftPolling);
    /* todo: ebenfalls 'präparieren'!!! */
}

uint8 CalculateChecksum(PMSG_Buffer ptr)
{
    uint8 chk=0xff;
    uint8 i;

    for (i=0;i<ptr->len;i++) {
        chk^=ptr->msg[i];
    }
    
    return chk;
}
