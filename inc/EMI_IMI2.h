/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
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
#if !defined(__EMI_IMI2_H)
#define __EMI_IMI2_H


/*
L_SystemBroadcast.req           15h 15h 17h
L_SystemBroadcast.con           2Ch 4Ch 26h
L_SystemBroadcast.ind           2Dh 4Dh 28h
T_Data_SystemBroadcast.req      25h 25h -
T_Data_SystemBroadcast.con      3Ch 4Ch -
T_Data_SystemBroadcast.ind      3Dh 4Dh -
*/

typedef enum tagKNXServiceTypeType {
/*
**      Link-Layer.
*/
    L_PLAIN_DATA_REQ            =   ((uint8)0x10),
    L_DATA_REQ                  =   ((uint8)0x11),
    L_POLL_DATA_REQ             =   ((uint8)0x13),

/*
**      Network-Layer.
*/
    N_DATA_INDIVIDUAL_REQ       =   ((uint8)0x21),
    N_DATA_GROUP_REQ            =   ((uint8)0x22),
    N_POLL_DATA_REQ             =   ((uint8)0x23),
    L_POLL_DATA_CON             =   ((uint8)0x25),
    L_DATA_IND                  =   ((uint8)0x29),
    L_BUSMON_IND                =   ((uint8)0x2B),
    N_DATA_BROADCAST_REQ        =   ((uint8)0x2C),
    L_DATA_CON                  =   ((uint8)0x2E),

/*
**      Transport-Layer/group-oriented.
*/
    T_DATA_GROUP_REQ            =   ((uint8)0x32),
    T_POLL_DATA_REQ             =   ((uint8)0x33),
    N_POLL_DATA_CON             =   ((uint8)0x35),
    N_DATA_GROUP_IND            =   ((uint8)0x3A),
    N_DATA_GROUP_CON            =   ((uint8)0x3E),

/*
**      Transport-Layer/connection-oriented.
*/
    T_DATA_CONNECTED_REQ        =   ((uint8)0x41),
    T_CONNECT_REQ               =   ((uint8)0x43),
    T_DISCONNECT_REQ            =   ((uint8)0x44),
    N_DATA_INDIVIDUAL_IND       =   ((uint8)0x49),
    T_DATA_INDIVIDUAL_REQ       =   ((uint8)0x4A),
    T_DATA_BROADCAST_REQ        =   ((uint8)0x4C),
    N_DATA_BROADCAST_IND        =   ((uint8)0x4D),
    N_DATA_INDIVIDUAL_CON       =   ((uint8)0x4E),
    N_DATA_BROADCAST_CON        =   ((uint8)0x4F),

/*
**      Application-Layer/group-oriented.
*/
    U_VALUE_WRITE_REQ           =   ((uint8)0x71),
    A_DATA_GROUP_REQ            =   ((uint8)0x72),
    A_POLL_DATA_REQ             =   ((uint8)0x73),
    U_VALUE_READ_REQ            =   ((uint8)0x74),
    T_POLL_DATA_CON             =   ((uint8)0x75),
    T_DATA_GROUP_IND            =   ((uint8)0x7A),
    U_FLAGS_READ_REQ            =   ((uint8)0x7C),
    T_DATA_GROUP_CON            =   ((uint8)0x7E),

/*
**      Application-Layer/managment-part.
*/
    M_USER_DATA_INDIVIDUAL_REQ  =   ((uint8)0x81),
    M_USER_DATA_CONNECTED_REQ   =   ((uint8)0x82),
    T_CONNECT_IND               =   ((uint8)0x85),
    T_CONNECT_CON               =   ((uint8)0x86),
    T_DISCONNECT_IND            =   ((uint8)0x87),
    T_DISCONNECT_CON            =   ((uint8)0x88),
    T_DATA_CONNECTED_IND        =   ((uint8)0x89),
    T_DATA_CONNECTED_CON        =   ((uint8)0x8E),
    T_DATA_BROADCAST_IND        =   ((uint8)0x8D),
    T_DATA_BROADCAST_CON        =   ((uint8)0x8F),

    T_DATA_INDIVIDUAL_IND       =   ((uint8)0x94),
    M_INTERFACEOBJ_DATA_REQ     =   ((uint8)0x9A),
    T_DATA_INDIVIDUAL_CON       =   ((uint8)0x9C),

/*
**      Physical-External-Interface.
*/
    PC_SETVALUE_REQ             =   ((uint8)0xA6),
    PEI_IDENTIFY_REQ            =   ((uint8)0xA7),
    PEI_IDENTIFY_CON            =   ((uint8)0xA8),
    PEI_SWITCH_REQ              =   ((uint8)0xA9),
    PC_GETVALUE_CON             =   ((uint8)0xAB),
    PC_GETVALUE_REQ             =   ((uint8)0xAC),

/*
**      Application running in the BAU (PEI if stopped).
*/
    TM_TIMER_IND                =   ((uint8)0xC1),

/* #define U_USER_DATA                           >D0     USER */
    M_USER_DATA_CONNECTED_CON   =   ((uint8)0xD1),
    M_USER_DATA_CONNECTED_IND   =   ((uint8)0xD2),
    M_INTERFACEOBJ_DATA_IND     =   ((uint8)0xD4),
    M_CONNECT_IND               =   ((uint8)0xD5),
    M_DISCONNECT_IND            =   ((uint8)0xD7),
    M_USER_DATA_INDIVIDUAL_IND  =   ((uint8)0xD9),
    M_INTERFACEOBJ_DATA_CON     =   ((uint8)0xDC),
    M_USER_DATA_INDIVIDUAL_CON  =   ((uint8)0xDE),
/* #define M_CONNECT_REQ */
/* #define M_CONNECT_CON */
/* #define M_DISCONNECT_REQ */
/* #define M_DISCONNECT_CON */

    U_VALUE_READ_CON            =   ((uint8)0xE4),
    A_POLL_DATA_CON             =   ((uint8)0xE5),
    U_EVENT_IND                 =   ((uint8)0xE7),
    A_DATA_GROUP_IND            =   ((uint8)0xEA),
    U_FLAGS_READ_CON            =   ((uint8)0xEC),
    A_DATA_GROUP_CON            =   ((uint8)0xEE)
    
} KNXServiceTypeType;

#endif /* __EMI_IMI2_H */

