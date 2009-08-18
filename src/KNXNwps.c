
#include "KNXNwps.h"

void NWPS_Dispatch(PMSG_Buffer pBuffer,uint8 service/*,boolean connected*/);

/*
Hinweis: In Supplement S08 "Distributed Address Assignment" wird ein 'A_NetworkParameter_Write.req'
         als AddressRegistration verwendet.
*/

/* Hinweis: Versand via 'T_Data_Broadcast' oder 'T_Data_Individual'. */

/* Hinweis: eine 'saubere' Implementation leitet die Requests an den Property-Server weiter!!! */

/*
· check the usage of a Group Address (Group Address Check)
    object_type: 1 (Group Address Table)
    PID: 23 (List of group addresses)
    test_info: range8 + start_address16
    test_result: none    
    comm_mode response: point-to-point, connectionless

- check the existence of an Application Interface Object (Functional Block Scan)
    object_type: [50..50.000]
    PID: 1 (PID_OBJECT_TYPE)
    test_info: reserved8 = 00h
    test_result: object_index
    comm_mode response: point-to-point, connectionless

- get the serial numbers of all devices object_type:
    0 (Device Object)
    PID: 11 (PID_SERIAL_NUMBER)
    test_info: reserved8 = 00h
    test_result: serial number
    comm_mode response: point-to-point, connectionless
*/

/*
4.1 Property for Device Object (0x0000) :
    SubnetAddress - PID_SUBNET_ADDRESS(0x39)
        This property includes the Subnetwork Address of the Device (size: 1 octet).

4.2 Property for Device Object :
    AddressReport - PID_ADDRESS_REPORT(0x3C)
        This property includes the serial number of the Device (size: 6 octets).
        This value shall in datagrams be set to 000000000000h if the device has no valid serial number.
        
4.3 Property for Device Object :
    Address_Check - PID_ADDRESS_CHECK (0x3D)
        This property is void (size: 1 octet; value = 00h always).
        Having no effect, it is used for checking the presence of a device at a given Individual Address.
*/


/*
    PDU/Read(Resp):
    -----------------------
    Object-Type:    8+9
    PID:            10
    Test-Info:      11..N
    Test-Result     N+1..21

    PDU/Write:
    -----------------------
    Object-Type:    8+9
    PID:            10
    Value:          11..21
*/

typedef boolean (*NWPS_FUNC)(PMSG_Buffer pBuffer);

typedef struct tagNWPSFunctions {
    uint16 ObjectType_from,ObjectType_to;
    uint8 Pid;
    NWPS_FUNC func;
} NWPSFunctions;


boolean NWPS_GroupAddressCheck(PMSG_Buffer pBuffer);
boolean NWPS_FunctionalBlockScan(PMSG_Buffer pBuffer);
boolean NWPS_GetSerialNumber(PMSG_Buffer pBuffer);

static NWPSFunctions NWPSReadFunctions[]={
    {KNX_OT_ADDRESSTABLE_OBJECT,KNX_OT_ADDRESSTABLE_OBJECT,KNX_PID_GROUP_ADDRESS_LIST,NWPS_GroupAddressCheck},
    {50,50000,KNX_PID_OBJECT_TYPE,NWPS_FunctionalBlockScan},
    {KNX_OT_DEVICE_OBJECT,KNX_OT_DEVICE_OBJECT,KNX_PID_SERIAL_NUMBER,NWPS_GetSerialNumber}
};


/* A_NetworkParameter_Read.req(hop_count_type, parameter_type, priority, test_info); */

/* Hinweis: nicht 'uint8' sondern '???ServiceType'!!! */

void NWPS_Dispatch(PMSG_Buffer pBuffer,uint8 service/*,boolean connected*/)
{
    KNX_StandardFrameRefType pmsg;
    uint8 len,Pid;
    uint16 objectType;
    
    pmsg=MSG_GetMessagePtr(pBuffer);

    objectType=btohs((uint16)pmsg->data[0]);  /* todo: besser Konstanten verwenden!!! */
    Pid=pmsg->data[2];

    len=MSG_GetLSDULen(pBuffer);    /* todo: GetAPDULen() */

    if (service==NWPS_READ) {

    } else if (service==NWPS_WRITE) {

    } else {
        (void)MSG_ReleaseBuffer(pBuffer);     /* Invalid Service. */
    }                                        
}

boolean NWPS_GroupAddressCheck(PMSG_Buffer pBuffer)
{
    return TRUE;
}

boolean NWPS_FunctionalBlockScan(PMSG_Buffer pBuffer)
{
    return TRUE;
}

boolean NWPS_GetSerialNumber(PMSG_Buffer pBuffer)
{
    return TRUE;
}
