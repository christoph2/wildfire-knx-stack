%module kstackf

%include <stdint.i>
%include <typemaps.i>
%include <cpointer.i>
%include <carrays.i>

typedef uint8_t boolean;

%inline %{
#include "types.h"
#include <stdio.h>
%}

//%pointer_class(uint8_t, uint8p);
//%array_class(uint8_t, uint8a);

typedef uint16_t Knx_AddressType;

typedef enum tagKnx_StatusType {
    KNX_E_OK,
    KNX_E_NOT_OK
} Knx_StatusType;

%typemap(in) callback * {

    if (!PyCallable_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "parameter must be callable");
        return NULL;
    }

    $1 = $input;
}

%typemap(in, numinputs = 0) KnxMsg_Buffer ** buf ( KnxMsg_Buffer ** tmp) {
    printf("MsgBuffer[in]\n");
    $1 = tmp;
}

%typemap(argout, numinputs = 0) KnxMsg_Buffer ** buf {
    printf("MsgBuffer[argout]\n");
    $result = (PyObject*)*tmp$argnum;
}

%typemap(out) boolean {
    if($1) {
        Py_INCREF(Py_True);
        $result = (PyObject *)Py_True;
    } else {
        Py_INCREF(Py_False);
        $result = (PyObject *)Py_False;
    }
}

%typemap(in) uint8_t * {
  /* Check if is a list */
  if (PyList_Check($input)) {
    int size = PyList_Size($input);
    //printf("Size of Python list: %u\n", size);
    int i = 0;
    $1 = (uint8_t *)malloc((size) * sizeof(uint8_t));
    for (i = 0; i < size; i++) {
      PyObject *o = PyList_GetItem($input,i);


      if (PyInt_Check(o)) {
        $1[i] = PyInt_AsLong(PyList_GetItem($input, i));
        //printf("0x%x ", $1[i]);
      } else {
        PyErr_SetString(PyExc_TypeError,"list must contain ints");
        free($1);
        return NULL;
      }

    }
  } else {
    PyErr_SetString(PyExc_TypeError,"not a list");
    return NULL;
  }
}


%{
#include "link-layer/uart_bif.h"
#include "knx_sched.h"
#include "knx_disp.h"
%}


%inline %{

void checkCallback(callback * fn);

void Ffi_SetApiErrorCB(callback * fn);
void Ffi_SetApiTraceCB(callback * fn);
void Ffi_SetLocalConfirmationCB(callback * fn);
void Ffi_SetIndividual_Address_ResCB(callback * fn);
void Ffi_SetProperty_Description_Read_IndCB(callback * fn);
void Ffi_SetConnect_IndCB(callback * fn);
void Ffi_SetDisconnect_IndCB(callback * fn);
void Ffi_SetConnect_ConCB(callback * fn);
void Ffi_SetDisconnect_ConCB(callback * fn);

#include "knx_apci.h"
%}

//%include "Port.h"

/*
**  Link-Layer.
**
*/

void KnxLL_FeedReceiver(uint8_t byte);
void KnxLL_Init(void);
void KnxLL_Task(void);
//void KnxLL_WriteFrame(uint8_t const * frame, uint8_t length);
void KnxLL_WriteFrame(uint8_t const * INPUT, uint8_t length);
boolean KnxLL_IsBusy(void);
void KnxLL_BusyWait(void);
boolean KnxLL_IsConfirmed(void);
void KnxLL_TimeoutCB(void);

void U_Reset_req(void);
void U_State_req(void);

/*!
 *
 *  Transport Layer.
 *
 */
void T_Connect_Req(KnxMsg_Buffer * pBuffer, Knx_AddressType source, Knx_AddressType dest);
void T_Disconnect_Req(KnxMsg_Buffer * pBuffer, Knx_AddressType source, Knx_AddressType dest);

/*
** Scheduler.
*/
void KnxSched_Task(void);
void KnxSched_Init(void);

/*!
 *  Messaging.
 *
 */

KnxMsg_Buffer * KnxMsg_AllocateBufferWrapper(void);

void KnxMsg_ReleaseBuffer(KnxMsg_Buffer * ptr);

/*!
 *  Application-Layer.
 */
void A_IndividualAddress_Read_Req(KnxMsg_Buffer * pBuffer, uint16_t source);
void A_DeviceDescriptor_Read_Req(KnxMsg_Buffer * pBuffer, uint16_t source, uint16_t dest, uint8_t descriptor_type);

//
void Port_Init(void);
//void Dbg_Init(void);
//void Port_InstallExitHandler(void);
//void Port_TimerInit(void);
void Serial_Init(void);
void Serial_Deinit();

void StackTest(void);

#if 0
%contract sqrt(double x) {
require:
    x >= 0;
ensure:
    sqrt >= 0;
}

...
double sqrt(double);
#endif

#if 0
%apply int *OUTPUT { int *result };
%apply int *INPUT  { int *x, int *y};
void add(int x, int y, int *result);
int  sub(int *x, int *y);
#endif

