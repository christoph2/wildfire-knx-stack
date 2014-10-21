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

/** @file knx_ffipython.c
 *
 *  Foreign-function-interface to Python, mainly callbacks.
 *
 */

#include <stdarg.h>

#include "knx_ffi.h"

#if defined(SWIG)

#include "Python.h"

static PyObject * LocalConfirmationCB = NULL;

static PyObject * Connect_IndCB = NULL;
static PyObject * Disconnect_IndCB = NULL;

static PyObject * Individual_Address_ResCB = NULL;
static PyObject * PropertyDescription_Read_IndCB = NULL;


static void Ffi_SetCallback(PyObject ** callback, void * func);


static void Ffi_SetCallback(PyObject ** callback, void * func)
{
    Py_XINCREF((PyObject *)func);   /* Add a reference to new callback */
    Py_XDECREF(*callback);          /* Dispose of previous callback */
    *callback = (PyObject *)func;
}

void Ffi_Callback(char const * phormat, ...)
{
    PyObject * arglist;
    PyObject * kwlist;
    PyObject * result;
    va_list arguments;

    va_start(arguments, phormat);

    if (Individual_Address_ResCB != NULL) {
        arglist = Py_BuildValue(phormat, arguments);
        //kwlist = Py_BuildValue("{HH}", "address", address, "dummy", "brummy");

        //result = PyObject_CallObject(Individual_Address_ResCB, arglist);
        result = PyObject_Call(Individual_Address_ResCB, arglist, NULL);
        //Py_DECREF(kwlist);
        Py_DECREF(arglist);
    }

    va_end(arguments);
}

void Ffi_SetLocalConfirmationCB(void * callback)
{
    Ffi_SetCallback(&LocalConfirmationCB, callback);
}

void Ffi_LocalConfirmation(boolean confirmed)
{    
    PyObject * arglist;
    PyObject * result;

    printf("Ffi_LocalConfirmation = %u\n", confirmed);

    if (LocalConfirmationCB != NULL) {
        arglist = Py_BuildValue("(B)", confirmed);
        printf("Ffi_LocalConfirmation [before]\n");
        result = PyObject_Call(LocalConfirmationCB, arglist, NULL);
        printf("Ffi_LocalConfirmation [after]\n");
        Py_DECREF(arglist);
    }
    printf("Ffi_LocalConfirmation [leaving fkt.]\n");
}


void Ffi_SetConnect_IndCB(void * callback)
{    
    Ffi_SetCallback(&Connect_IndCB, callback);
}

void Ffi_Connect_Ind(void)
{    
    PyObject * arglist;
    PyObject * result;

    if (Connect_IndCB != NULL) {
        arglist = Py_BuildValue("()");
        result = PyObject_Call(Connect_IndCB, arglist, NULL);
        Py_DECREF(arglist);
    }
}

void Ffi_SetDisconnect_IndCB(void * callback)
{    
    Ffi_SetCallback(&Disconnect_IndCB, callback);
}

void Ffi_Disconnect_Ind(void)
{       
    PyObject * arglist;
    PyObject * result;

    if (Disconnect_IndCB != NULL) {
        arglist = Py_BuildValue("()");
        result = PyObject_Call(Disconnect_IndCB, arglist, NULL);
        Py_DECREF(arglist);
    }
}

void Ffi_Individual_Address_Res(uint16_t address)
{
    PyObject * arglist;
    PyObject * result;

    if (Individual_Address_ResCB != NULL) {
        arglist = Py_BuildValue("(H)", address);
        result = PyObject_Call(Individual_Address_ResCB, arglist, NULL);
        Py_DECREF(arglist);
    }
}

void Ffi_SetIndividual_Address_ResCB(void * callback)
{
    //printf("Ffi_SetIndividual_Address_ResCB: %p\n", callback);
    Ffi_SetCallback(&Individual_Address_ResCB, callback);

    //Py_XINCREF((PyObject * )callback);      /* Add a reference to new callback */
    //Py_XDECREF(Individual_Address_ResCB);   /* Dispose of previous callback */
    //Individual_Address_ResCB = (PyObject * )callback;
}

void Ffi_Property_Description_Read_Ind(uint16_t source, uint8_t object_index, 
    uint8_t property_id, uint8_t property_index, uint8_t type, uint16_t max_nr_of_elem, uint8_t access)
{
    PyObject * arglist;
    PyObject * result;

    if (PropertyDescription_Read_IndCB != NULL) {
        arglist = Py_BuildValue("(HBBBBHB)", source, object_index, property_id, property_index, type, max_nr_of_elem, access);
        result = PyObject_Call(PropertyDescription_Read_IndCB, arglist, NULL);
        Py_DECREF(arglist);
    }
}

void Ffi_SetProperty_Description_Read_IndCB(void * callback)
{
    Ffi_SetCallback(&PropertyDescription_Read_IndCB, callback);
}
#endif