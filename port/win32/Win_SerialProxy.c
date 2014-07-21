/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
 *                                       cpu12.gems@googlemail.com>
 *
 *   All Rights Reserved
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
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

// ØMQ

#define ACK         (0xe5)
#define BUFFER_SIZE (128U)

void Error_Check(void);
//void Serial_Receiver(void * param);
void Serial_Marshal(char * blob, uint8_t const * arr, uint16_t length);
void Serial_Unmarshal(char * blob, uint8_t * arr, uint16_t * length);


void * Serial_TransmitterSocket;

static void * Serial_Context;

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

    Serial_ReceiverSocket = zmq_socket(context, ZMQ_REP);
    rc = zmq_bind(Serial_ReceiverSocket, "tcp://*:5557");
    if (rc == -1) {
        Error_Check();
    }
    printf("Hello from Serial_Receiver!");

    while (TRUE) {
        nbytes = zmq_recv(Serial_ReceiverSocket, buffer, BUFFER_SIZE, 0);
        Serial_Unmarshal(buffer, resultArray, &resultLength);        
        printf("IND: ");
        for (idx = 0; idx < resultLength; ++idx) {
            KnxLL_FeedReceiver(resultArray[idx]);
        }
        Dbg_DumpHex(resultArray, resultLength);
        resultArray[0] = ACK;
        Serial_Marshal(buffer, resultArray, 1);
        rc = zmq_send(Serial_ReceiverSocket, buffer, 3, 0);
    }
    rc = zmq_close(Serial_ReceiverSocket);
}

void Serial_Init(void)
{
    int major, minor, patch;    
    uint8_t arr[BUFFER_SIZE] = {0};
    int rc;
       
    zmq_version(&major, &minor, &patch);
    Serial_Context = zmq_ctx_new();

    //rc = zmq_socket_monitor(Serial_TransmitterSocket, "inproc://monitor.rep", ZMQ_EVENT_ALL);
    _beginthread(Serial_Receiver, 0, Serial_Context);
    
    Serial_TransmitterSocket = zmq_socket(Serial_Context, ZMQ_REQ);
    rc = zmq_connect(Serial_TransmitterSocket, "tcp://localhost:5556");
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
    int nbytes;
    uint16_t resultLength;    
    char buffer[BUFFER_SIZE];
    uint8_t resultArray[BUFFER_SIZE] = {0};

    Serial_Marshal(buffer, arr, length);
    rc = zmq_send(so, buffer, length + 2, 0);
    if (rc == -1) {
        Error_Check();
    }

    nbytes = zmq_recv(so, buffer, BUFFER_SIZE, 0);
    if (nbytes == -1) {
        Error_Check();
    } else {
        Serial_Unmarshal(buffer, resultArray, &resultLength);
        printf("RES: ");
        Dbg_DumpHex(resultArray, resultLength);
    }
}

void Serial_Unmarshal(char * blob, uint8_t * arr, uint16_t * length)
{
    uint16_t tlen = MAKEWORD(blob[1], blob[0]);
    uint16_t idx;

    *length = tlen;
    for (idx = 0; idx < tlen; ++idx) {
        arr[idx] = blob[idx + 2];
    }
}

void Serial_Marshal(char * blob, uint8_t const * arr, uint16_t length)
{
    uint16_t idx;

    blob[0] = HIBYTE(length);
    blob[1] = LOBYTE(length);

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
