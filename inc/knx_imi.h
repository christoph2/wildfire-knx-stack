#if !defined(__KNX_IMI_H)
#define __KNX_IMI_H

/* knx_imi.h */

/*
**
**  Service Codes for Konnex/EIB Internal-Message-Interface.
**
**  Own coding of Services permits usage of Jump-Tables.
**
*/


#define KNX_LL_SERVICES     ((uint8)0x10)
#define KNX_NL_SERVICES     ((uint8)0x20)

#define KNX_TLG_SERVICES    ((uint8)0x30)
#define KNX_TLC_SERVICES    ((uint8)0x40)

#define KNX_ALG_SERVICES    ((uint8)0x70)
#define KNX_ALM_SERVICES    ((uint8)0x80)

typedef enum tagKNXServiceTypeType {
/*
**      Link-Layer.
*/
    L_DATA_REQ              =   KNX_LL_SERVICES,
    L_POLL_DATA_REQ,

/*
**      Network-Layer.
*/
    L_DATA_IND              =   KNX_NL_SERVICES,
    L_DATA_CON,    
    L_POLL_DATA_CON,
    L_BUSMON_IND,
    N_DATA_INDIVIDUAL_REQ,
    N_DATA_GROUP_REQ,
    N_DATA_BROADCAST_REQ,
    N_POLL_DATA_REQ,
    
/*
**      Transport-Layer/group-oriented.
*/
    N_DATA_GROUP_IND        =   KNX_TLG_SERVICES,
    N_DATA_GROUP_CON,
    N_POLL_DATA_CON,
    T_DATA_GROUP_REQ,
    T_POLL_DATA_REQ,    

/*
**      Transport-Layer/connection-oriented.
*/
    N_DATA_INDIVIDUAL_IND   =   KNX_TLC_SERVICES,
    N_DATA_INDIVIDUAL_CON,    
    N_DATA_BROADCAST_IND,
    N_DATA_BROADCAST_CON,     
    T_CONNECT_REQ,
    T_DISCONNECT_REQ,    
    T_DATA_CONNECTED_REQ,    
    T_DATA_INDIVIDUAL_REQ,
    T_DATA_BROADCAST_REQ,

/*
**      Application-Layer/group-oriented.
*/
    T_DATA_GROUP_IND        =   KNX_ALG_SERVICES,
    T_DATA_GROUP_CON,
    T_POLL_DATA_CON,    
    A_DATA_GROUP_REQ,
    A_POLL_DATA_REQ,    

/*
**      Application-Layer/managment-part.
*/
    T_CONNECT_IND           =   KNX_ALM_SERVICES,
    T_CONNECT_CON,
    T_DISCONNECT_IND,
    T_DISCONNECT_CON,
    T_DATA_CONNECTED_IND,
    T_DATA_CONNECTED_CON,
    T_DATA_INDIVIDUAL_IND,
    T_DATA_INDIVIDUAL_CON,
    T_DATA_BROADCAST_IND,
    T_DATA_BROADCAST_CON
    
} KNXServiceTypeType;

/*

    SCHL*PK*:
    =========

    L_DATA_req          = 0x11,
    L_POLLDATA_req      = 0x13,
    L_SYS_BROADCAST_req = 0x17,
    L_DATA_send         = 0x1f,

    N_DATA_req          = 0x21,
    N_GROUP_DATA_req    = 0x22,
    N_POLLDATA_req      = 0x23,
    L_POLLDATA_con      = 0x25,
    L_SYS_BROADCAST_con = 0x26,
    N_SYS_BROADCAST_req = 0x27,
    L_SYS_BROADCAST_ind = 0x28,
    L_DATA_ind          = 0x29,
    L_BUSMON_ind        = 0x2b,
    N_BROADCAST_req     = 0x2c, 
    L_DATA_con          = 0x2e,
 
    T_GROUP_DATA_req    = 0x32,
    T_POLLDATA_req      = 0x33,
    N_POLLDATA_con      = 0x35,
    N_GROUP_DATA_ind    = 0x3a,
    N_GROUP_DATA_con    = 0x3e,

    T_DATA_req          = 0x41,
    T_CONNECT_req       = 0x43,
    T_DISCONNECT_req    = 0x44,
    N_SYS_BROADCAST_con = 0x45,
    T_SYS_BROADCAST_req = 0x47,
    N_SYS_BROADCAST_ind = 0x48,
    N_DATA_ind          = 0x49,
    T_DATA_UNACK_req    = 0x4A,
    T_BROADCAST_req     = 0x4c,
    N_BROADCAST_ind     = 0x4d,
    N_DATA_con          = 0x4e,
    N_BROADCAST_con     = 0x4f,
  
    A_GROUP_DATA_req    = 0x72,
    T_POLLDATA_con      = 0x75,
    T_GROUP_DATA_ind    = 0x7a,
    T_GROUP_DATA_con    = 0x7e,
  
    T_CONNECT_ind       = 0x85,
    T_CONNECT_con       = 0x86,
    T_DISCONNECT_ind    = 0x87,
    T_DISCONNECT_con    = 0x88,
    T_DATA_ind          = 0x89,
    T_BUSMON_ind        = 0x8b,
    T_BROADCAST_ind     = 0x8d,
    T_DATA_con          = 0x8e,
    T_BROADCAST_con     = 0x8f,

    T_DATA_UNACK_ind    = 0x94,
    T_SYS_BROADCAST_con = 0x95,
    T_SYS_BROADCAST_ind = 0x98,
    T_DATA_UNACK_con    = 0x9C,

    TM_TIMER_TL         = 0xc1
*/

#endif /* __KNX_IMI_H */
