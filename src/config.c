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
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free Software Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/

#include "Wildfire_Config.h"
#include "knx_timer.h"

void KnxTlc_OnConnectionTimeoutTimer(void);

Tmr_CallbackFunction KnxTmr_Callbacks[TMR_NUM_TIMERS] = {
    (Tmr_CallbackFunction *)NULL,
    (Tmr_CallbackFunction *)KnxTlc_OnConnectionTimeoutTimer,
    (Tmr_CallbackFunction *)NULL,
    (Tmr_CallbackFunction *)NULL,
    (Tmr_CallbackFunction *)NULL,
};
