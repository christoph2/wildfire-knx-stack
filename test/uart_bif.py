#!/usr/bin/env python
# -*- coding: utf-8 -*-

from collections import namedtuple
import ctypes
import logging
from ctypes import Structure, Union, c_char_p, c_bool, c_uint8, c_int32, c_uint32, POINTER, byref, CFUNCTYPE, pointer, ARRAY, create_string_buffer, CDLL
import enum

from common import ModuleIDs, FFI, loadLibrary
#from et import Et
#from utl import Utl

STATUS_T = ctypes.c_uint16

class State(enum.IntEnum):
    KNX_LL_STATE_IDLE                           = 0
    KNX_LL_STATE_RECEIVING                      = 1
    KNX_LL_STATE_SENDING                        = 2
    KNX_LL_STATE_AWAITING_RESPONSE_LOCAL        = 3
    KNX_LL_STATE_AWAITING_RESPONSE_TRANSMISSION = 4
    KNX_LL_STATE_AWAITING_RECEIPTION            = 5
    KNX_LL_STATE_TIMED_OUT                      = 6


class CommandsToHost(enum.IntEnum):

    L_ACKN_IND              = 0x00
    U_CONFIGURE_IND         = 0x01
    U_RESET_IND             = 0x03
    U_STATE_IND             = 0x07
    L_DATA_CON              = 0x0b
    L_DATA_EXTENDED_IND     = 0x10
    U_FRAMESTATE_IND        = 0x13
    U_STOPMODE_IND          = 0x2b
    U_SYSTEMSTAT_IND        = 0x4b
    L_DATA_STANDARD_IND     = 0x90
    U_FRAMEEND_IND          = 0xcb
    L_POLL_DATA_IND         = 0xf0


class CommandsFromHost(enum.IntEnum):
    # INTERNAL COMMANDS – DEVICE SPECIFIC
    U_RESET_REQ              = 0x01
    U_STATE_REQ              = 0x02

    """
    #if KNX_BUS_INTERFACE == KNX_BIF_NCN5120
    #define U_SETBUSY_REQ            = 0x03
    #define U_QUITBUSY_REQ           = 0x04
    #endif /* KNX_BUS_INTERFACE */

    #define U_BUSMON_REQ             = 0x05

    #if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
    #define U_PRODUCTID_REQUEST      = 0x14
    #define U_ACTIVATEBUSYMODE_REQ   = 0x15
    #define U_RESETBUSYMODE_REQ      = 0x16
    #define U_MXRSTCNT_REQ           = 0x18
    #define U_ACTIVATECRC_REQ        = 0x19
    #define U_SETADDRESS_REQ         = 0x1C
    #endif /* KNX_BUS_INTERFACE */

    #if KNX_BUS_INTERFACE == KNX_BIF_NCN5120
    #define U_L_DATAOFFSET_REQ       = 0x08 /* 08 - 0C */
    #define U_SYSTEMSTAT_REQ         = 0x0D
    #define U_STOPMODE_REQ           = 0x0E
    #define U_EXITSTOPMODE_REQ       = 0x0F
    #endif /* KNX_BUS_INTERFACE */

    #define U_ACKN_REQ               = 0x10 /* 10 - 17 */

    #if KNX_BUS_INTERFACE == KNX_BIF_NCN5120
    #define U_CONFIGURE_REQ          = 0x18 /* 18 - 1F */
    #define U_INTREGWR_REQ           = 0x28 /* 28 - 2B */
    #define U_INTREGRD_REQ           = 0x38 /* 38 - 3B */
    #endif /* KNX_BUS_INTERFACE */

    #define U_POLLINGSTATE_REQ       = 0xE0 /* E0 - EE */


    /* KNX TRANSMIT DATA COMMANDS */
    #define U_L_DATASTART_REQ        = 0x80
    #define U_L_DATACONT_REQ         = 0x80 /* 81 - BF */
    #define U_L_DATAEND_REQ          = 0x40 /* 47 - 7F */

    #if KNX_BUS_INTERFACE == KNX_BIF_NCN5120 /* TODO: Überprüfen!!!*/
    #define U_SETADDRESS_REQ         = 0xF1
    #define U_SETREPETITION_REQ      = 0xF2
    #endif
    """


"""
class Services(enum.IntEnum):
    INIT                      = 0x00
    ALLOCATE_BUFFER           = 0x01
    ALLOCATE_BUFFER_WRAPPER   = 0x02
    RELEASE_BUFFER            = 0x03
    CLEAR_BUFFER              = 0x04
    POST                      = 0x05
    GET                       = 0x06
    SET_LEN                   = 0x07
    GET_LEN                   = 0x08
    SET_ROUTING_COUNT         = 0x09
    GET_ROUTING_COUNT         = 0x0a
    GET_BUFFER_ADDRESS        = 0x0b
    GET_BUFFER_NUMBER         = 0x0c
    CLEAR_MESSAGE_BUFFER      = 0x0d


class Errors(enum.IntEnum):
    UNINIT                        = 0x01
    NULL_PTR                      = 0x02
    NO_BUFFER_AVAIL               = 0x03
    INVALID_BUFFER                = 0x04
    NOT_ALLOCATED                 = 0x05
"""

#KnxMsg_Buffer_Ptr = POINTER(KnxMsg_Buffer)
#CounterType = namedtuple("CounterType", "free used")

class UartBif(FFI):
    """
    void U_Reset_req(void);
    void U_State_req(void);
    """

    _functions_ = (
        ("KnxLL_Init", None, []),
        #("KnxLL_Task", None, []),
        ("KnxLL_FeedReceiver", None, [c_uint8]),
        ("KnxLL_GetState", c_uint32, []),
        ("KnxLL_WriteFrame", None, [c_char_p, c_uint8]),
        ("KnxLL_Checksum", c_uint8, [c_char_p, c_uint8]),
        ("KnxLL_IsBusy", c_bool, []),
        ("KnxLL_IsConfirmed", c_bool, []),
        ("KnxLL_BusyWait", None, []),
    )

    def init(self):
        self.KnxLL_Init()

    def task(self):
        self.KnxLL_Task()

    def getState(self):
        return State(self.KnxLL_GetState())

    def deinit(self):
        self.KnxMsg_Deinit()

    def feedReceiver(self, ch):
        self.KnxLL_FeedReceiver(ch)

    def writeFrame(self, frame):
        length = len(frame)
        self.KnxLL_WriteFrame(frame, length)

    def checksum(self, frame):
        length = len(frame)
        return self.KnxLL_Checksum(frame, length)

    def isBusy(self):
        return self.KnxLL_IsBusy()

    def isConfirmed(self):
        return self.KnxLL_IsConfirmed()

    def busyWait(self):
        self.KnxLL_BusyWait()

dll = loadLibrary("uart")
print(dll)
uartBif = UartBif(dll)
uartBif.init()
print(uartBif.getState())
uartBif.feedReceiver(CommandsToHost.L_DATA_STANDARD_IND)
#uartBif.feedReceiver(CommandsToHost.L_DATA_EXTENDED_IND)
print(uartBif.getState())

