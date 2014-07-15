/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 * (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
 *                                      cpu12.gems@googlemail.com>
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
#if !defined(__KNXNWPS_H)
#define __KNXNWPS_H

#include "KNXDefs.h"
#include "knx_ios.h"
#include "knx_messaging.h"
#include "knx_conversion.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
** Global types.
*/typedef enum tagNWPSServiceType {
    NWPS_READ,
    NWPS_WRITE
} NWPSServiceType;

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KNXNWPS_H */
