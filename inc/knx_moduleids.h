/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
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
#if !defined(__KNX_MODULE_IDS_H)
#define __KNX_MODULE_IDS_H


#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

// KNX_MODULE_ID_

#define KNX_MODULE_ID_MSG           (1)

#define KNX_MODULE_ID_AIL           (2)
#define KNX_MODULE_ID_ALG           (3)
#define KNX_MODULE_ID_ALM           (4)

#define KNX_MODULE_ID_TLC           (5)
#define KNX_MODULE_ID_TLG           (6)
#define KNX_MODULE_ID_TLSM          (7)

#define KNX_MODULE_ID_NL            (8)

#define KNX_MODULE_ID_DL            (9)


//#define KNX_MODULE_ID_
//#define KNX_MODULE_ID_
//#define KNX_MODULE_ID_
//#define KNX_MODULE_ID_

#define KNX_MODULE_ID_UART_BIF      (48)

#if 0
KNXConv.c
KNXNwps.c
knx_address.c

knx_conversion.c
knx_debug.c
knx_disp.c
knx_ios.c

knx_layer_network.c

knx_lsm.c
knx_rsm.c
knx_sched.c
knx_sys_objs.c
knx_timer.c

knx_tlc.c
knx_tlg.c
knx_utl.c

Memory.c
Routing.c
StateMachineKNX.c

0x30    // Drivers


0x50    // Port stuff
#endif


#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNX_MODULE_IDS_H */
