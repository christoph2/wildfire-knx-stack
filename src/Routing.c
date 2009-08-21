
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
