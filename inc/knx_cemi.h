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
/*
** cEMI
*/
#if !defined(__CEMI_H)
#define __CEMI_H

#include "target.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

#define L_BUSMON_IND        ((BYTE)0x2B)
#define L_RAW_IND           ((BYTE)0x2D)
#define L_RAW_REQ           ((BYTE)0x10)
#define L_RAW_CON           ((BYTE)0x2F)
#define L_DATA_REQ          ((BYTE)0x11)
#define L_DATA_CON          ((BYTE)0x2E)
#define L_DATA_IND          ((BYTE)0x29)
#define L_POLL_DATA_REQ     ((BYTE)0x13)
#define L_POLL_DATA_CON     ((BYTE)0x25)
#define M_PROP_READ_REQ     ((BYTE)0xFC)
#define M_PROP_READ_CON     ((BYTE)0xFB)
#define M_PROP_WRITE_REQ    ((BYTE)0xF6)
#define M_PROP_WRITE_CON    ((BYTE)0xF5)
#define M_PROP_INFO_IND     ((BYTE)0xF7)
#define M_RESET_REQ         ((BYTE)0xF1)
#define M_RESET_IND         ((BYTE)0xF0)

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __CEMI_H  */

