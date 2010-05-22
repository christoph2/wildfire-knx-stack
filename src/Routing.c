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

#if 0
/*
    Abbreviations:
    ==============
    - C     hop count value contained in the N-protocol header
    - D     low order octet of the destination address, i.e. device address part
    - G     group address
    - SD    low nibble of high order octet plus low order octet, i.e. Line Address + Device Address
    - Z     high nibble of high order octet of the destination address, i.e. Area Address
    - ZS    high order octet of the destination address, i.e. hierarchy information part: Area Address + Line Address
*/

/* 
** 2.4.3.1 Routing in case of a Group Destination Address 
*/
    if routing condition = TRUE and 0H < C < 7H then 
        ROUTE_DECREMENTED
    if routing condition = TRUE and C = 0H then 
        IGNORE_ACKED
    elsif C = 7H then 
        ROUTE_UNMODIFIED
    else IGNORE_TOTALLY

        
/* 
** 2.4.3.2 Routing in case of an Individual Destination Address: Line Coupler 
*/
        

/* 2.4.3.2.1 Main Line to Subline Routing */
    if ZS = own subline address then
        if D <> 00H then
            if C = 7H then 
                ROUTE_UNMODIFIED
            elsif 0H < C < 7H then 
                ROUTE_DECREMENTED
            else 
                IGNORE_ACKED
        else 
            FORWARD_LOCALLY
    else 
        IGNORE_TOTALLY

/* 2.4.3.2.2 Subline to Main Line Routing */
    if ZS <> own subline address then
        if C = 7H then 
            ROUTE_UNMODIFIED
        elsif 0H < C < 7H then 
            ROUTE_DECREMENTED
        else 
            IGNORE_ACKED
        elseif D = 00H then 
            FORWARD_LOCALLY
    else 
        IGNORE_TOTALLY

        
/* 
**  2.4.3.3 Routing in case of an Individual Destination Address: Backbone Router 
*/

/* 2.4.3.3.1 Backbone to Main Line Routing */
    if Z = own area address then
        if SD <> 00H then
            if C = 7H then 
                ROUTE_UNMODIFIED
            elsif 0H < C < 7H then 
                ROUTE_DECREMENTED
            else 
                IGNORE_ACKED
        else 
            FORWARD_LOCALLY
    else 
        IGNORE_TOTALLY

/* 2.4.3.3.2 Main Line to Backbone Routing */
    if Z <> own area address then
        if C = 7H then 
            ROUTE_UNMODIFIED
        elsif 0H < C < 7H then 
            ROUTE_DECREMENTED
        else 
            IGNORE_ACKED
    elseif SD = 00H then 
        FORWARD_LOCALLY
    else 
        IGNORE_TOTALLY

/* 
** 2.4.3.4 Routing in case of a Broadcast Destination Address
*/

/* 2.4.3.4.1 Between closed Media */
    if C = 7H then 
        ROUTE_UNMODIFIED
    elsif 0H < C < 7H then 
        ROUTE_DECREMENTED
    else 
        IGNORE_ACKED

#endif

