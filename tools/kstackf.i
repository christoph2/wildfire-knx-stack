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

%pointer_class(uint8_t, uint8p);
%array_class(uint8_t, uint8a);

%typemap(in) callback * {

    if (!PyCallable_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "parameter must be callable");
        return NULL;
    }

    $1 = $input;
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

void Ffi_SetIndividual_Address_ResCB(callback * fn);

#include "knx_apci.h"
%}

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

/*
** Scheduler.
*/
void KnxSched_Task(void);
void KnxSched_Init(void);

//
void Port_Init(void);
//void Dbg_Init(void);
//void Port_InstallExitHandler(void);
//void Port_TimerInit(void);
void Serial_Init(void);
void Serial_Deinit();

void StackTest(void);
