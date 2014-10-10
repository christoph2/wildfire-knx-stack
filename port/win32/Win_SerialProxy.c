/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
*                                       cpu12.gems@googlemail.com>
*
*   All Rights Reserved
*
*  This program is free softwKNXe; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free SoftwKNXe Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WKNXRANTY; without even the implied wKNXranty of
*  MERCHANTABILITY or FITNESS FOR A PKNXTICULKNX PURPOSE.  See the
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free SoftwKNXe Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/
#include "win\Win_SerialProxy.h"
#include "link-layer\uart_bif.h"
#include "knx_debug.h"

#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <process.h>
#include <Windows.h>


#define BUFFER_SIZE (128U)

void Error_Check(void);
void Serial_Receiver(void * context);
void Serial_Marshal(char * blob, uint8_t const * arr, uint16_t length);
void Serial_Unmarshal(char * blob, uint8_t * arr, uint16_t * length);

static uintptr_t serialThread;
void * Serial_TransmitterSocket;
static void * Serial_Context;

static const char * IDSerialReceiver = "TPUART_CLIENT_REQUESTOR";
static const char * IDSerialTransmitter = "TPUART_CLIENT_TRANSMITTER";

/**
 *  'Simulates' an Rx-Interrupt.
 *
 *
 */
void Serial_Receiver(void * context)
{
    void * Serial_ReceiverSocket;
    int rc;
    int nbytes;
    uint16_t idx;
    char buffer[BUFFER_SIZE];
    uint8_t resultArray[BUFFER_SIZE] = {0};
    uint16_t resultLength;
    zmq_pollitem_t items[1];
    int timeout = 100;

    Serial_ReceiverSocket = zmq_socket(context, ZMQ_PULL);
    rc = zmq_bind(Serial_ReceiverSocket, "tcp://*:5557");
    if (rc == -1) {
        Error_Check();
    }
    rc = zmq_setsockopt(Serial_ReceiverSocket, ZMQ_IDENTITY, IDSerialReceiver, strlen(IDSerialReceiver));
    if (rc == -1) {
        Error_Check();
    }

    ZeroMemory((void *)&items, sizeof(items));

    items[0].socket = socket;
    items[0].events = ZMQ_POLLIN;
    while (TRUE) {
        nbytes = zmq_recv(Serial_ReceiverSocket, buffer, BUFFER_SIZE, 0);
        Serial_Unmarshal(buffer, resultArray, &resultLength);        
        for (idx = 0; idx < resultLength; ++idx) {
            PORT_LOCK_TASK_LEVEL();
            KnxLL_FeedReceiver(resultArray[idx]);
            PORT_UNLOCK_TASK_LEVEL();
        }
        //Dbg_DumpHex(resultArray, resultLength);
    }
    rc = zmq_close(Serial_ReceiverSocket);
}

void Serial_Init(void)
{
    int major, minor, patch;    
    uint8_t arr[BUFFER_SIZE] = {0};
    int rc;
    int timeout = 250;
       
    zmq_version(&major, &minor, &patch);
    Serial_Context = zmq_ctx_new();

    //rc = zmq_socket_monitor(Serial_TransmitterSocket, "inproc://monitor.rep", ZMQ_EVENT_ALL);
    serialThread = _beginthread(Serial_Receiver, 0, Serial_Context);


    Port_SetThreadAffinity(serialThread, 0x00000001);
    
    Serial_TransmitterSocket = zmq_socket(Serial_Context, ZMQ_PUSH);
    rc = zmq_connect(Serial_TransmitterSocket, "tcp://localhost:5556");
    if (rc == -1) {
        Error_Check();
    }
#if 0
    rc = zmq_setsockopt(Serial_TransmitterSocket, ZMQ_RCVTIMEO, &timeout, sizeof(int));
    if (rc == -1) {
        Error_Check();
    }
#endif
    rc = zmq_setsockopt(Serial_TransmitterSocket, ZMQ_SNDTIMEO, &timeout, sizeof(int));
    if (rc == -1) {
        Error_Check();
    }
    rc = zmq_setsockopt(Serial_TransmitterSocket, ZMQ_IDENTITY, IDSerialTransmitter, strlen(IDSerialTransmitter));
    if (rc == -1) {
        Error_Check();
    }

}

void Serial_Deinit(void)
{
    int rc;

    rc = zmq_close(Serial_TransmitterSocket);
    if (rc == -1) {
        Error_Check();
    }
    rc = zmq_ctx_destroy(Serial_Context);
    if (rc == -1) {
        Error_Check();
    }
}

boolean Serial_Write(void * so, uint8_t const * arr, uint16_t length)
{
    int rc;
    char buffer[BUFFER_SIZE];
    uint8_t resultArray[BUFFER_SIZE] = {0};
    boolean result = TRUE;

    Serial_Marshal(buffer, arr, length);
    rc = zmq_send(so, buffer, length + 2, 0);
    if (rc == -1) {
        printf("SerialWrite/send: ");
        Error_Check();
        result = FALSE;
    }

    return result;
}

void Serial_Unmarshal(char * blob, uint8_t * arr, uint16_t * length)
{
    uint16_t tlen = Utl_Ntohs(KNX_CAST_ELEMENT(blob, 0, uint16_t));
    uint16_t idx;

    *length = tlen;
    for (idx = 0; idx < tlen; ++idx) {
        arr[idx] = blob[idx + 2];
    }
}

void Serial_Marshal(char * blob, uint8_t const * arr, uint16_t length)
{
    uint16_t idx;

    KNX_ASSIGN_ELEMENT(blob, 0, uint16_t, Utl_Htons, length);

    for (idx = 0; idx < length; ++idx) {
        blob[idx + 2] = arr[idx];
    }
}

void Error_Check(void)
{
    int errornum;

    errornum = zmq_errno();
    printf("%s\n", zmq_strerror(errornum));
}
