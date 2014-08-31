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
#include <Windows.h>
#include "win/Win_Utils.h"
#include "win/Win_NVMem.h"

typedef struct tagNVMem_FileViewType {
    HANDLE mappingHandle;
    void * mappingAddress;
} NVMem_FileViewType;

typedef enum tagOpenCreateType {
    OPEN_ERROR,
    OPEN_EXSISTING,
    NEW_FILE
} OpenCreateType;


/*
**
**  Utility Functions.
**
*/
void NVMem_MemoryInfo(void * address)
{
    MEMORY_BASIC_INFORMATION info;

    VirtualQuery(address, &info, sizeof(MEMORY_BASIC_INFORMATION));
}


HANDLE NVMem_OpenCreateFile(wchar_t const * fileName, BOOL create)
{
    HANDLE handle;

    if (create == FALSE) {
        handle = CreateFile((LPCWSTR)fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
            (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, (HANDLE)NULL
        );
    }
    else {
        handle = CreateFile((LPCWSTR)fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
            (LPSECURITY_ATTRIBUTES)NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, (HANDLE)NULL
        );
    }

    return handle;
}


BOOL NVMem_CreateFileView(HANDLE handle, DWORD length, NVMem_FileViewType * fileView)
{
    fileView->mappingHandle = CreateFileMapping(handle, (LPSECURITY_ATTRIBUTES)NULL, PAGE_READWRITE, (DWORD)0, (DWORD)length, NULL);
    if (fileView->mappingHandle == NULL) {
        return FALSE;
    }

    fileView->mappingAddress = (void *)MapViewOfFile(fileView->mappingHandle, FILE_MAP_ALL_ACCESS, 0, 0, length);
    if (fileView->mappingAddress == NULL) {
        CloseHandle(fileView->mappingHandle);
        return FALSE;
    }

    return TRUE;
}

void NVMem_CloseFileView(NVMem_FileViewType * fileView)
{
    CloseHandle(fileView->mappingHandle);
}


OpenCreateType NVMem_CreatePersitentArray(wchar_t const * fileName, DWORD size, NVMem_PersistentArrayType * persistentArray)
{
    DWORD error;
    NVMem_FileViewType fileView;
    OpenCreateType result;
    boolean newFile = FALSE;

    persistentArray->fileHandle = NVMem_OpenCreateFile(fileName, FALSE);
    if (persistentArray->fileHandle == INVALID_HANDLE_VALUE) {
        error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND) {
            newFile = TRUE;
            persistentArray->fileHandle = NVMem_OpenCreateFile(fileName, TRUE);
            if (persistentArray->fileHandle == INVALID_HANDLE_VALUE) {
                return OPEN_ERROR;
            }
        }
        else {
            return OPEN_ERROR;
        }
    }

    if (!NVMem_CreateFileView(persistentArray->fileHandle, size, &fileView)) {
        return OPEN_ERROR;
    }

    persistentArray->mappingAddress = fileView.mappingAddress;
    persistentArray->mappingHandle = fileView.mappingHandle;

    NVMem_MemoryInfo(persistentArray->mappingAddress);

    if (newFile) {
        result = NEW_FILE;
    }
    else {
        result = OPEN_EXSISTING;
    }
    return result;
}


void NVMem_ClosePersitentArray(NVMem_PersistentArrayType const * persistentArray)
{
    UnmapViewOfFile(persistentArray->mappingAddress);
    CloseHandle(persistentArray->mappingHandle);
    CloseHandle(persistentArray->fileHandle);
}

/*
**
**  Global Functions.
**
*/

void NVMem_Create(NVMem_Traits * traits)
{
    int pos;
    wchar_t * pdest;
    wchar_t adm[128];
    NVMem_PersistentArrayType temp;
    OpenCreateType result;

    traits->persistentArray = (NVMem_PersistentArrayType *)malloc(sizeof(NVMem_PersistentArrayType));

    pdest = wcschr(traits->name, L'.');
    if (pdest != NULL) {
        pos = (pdest - traits->name + 1);

        wcsncpy_s((wchar_t *)adm, _countof(adm), (wchar_t *)traits->name, pos);
        wcscat_s(adm, _countof(adm), L"adm");
    }
    else {
        wcsncpy_s((wchar_t *)adm, _countof(adm), (wchar_t *)traits->name, lstrlen(traits->name));
        wcscat_s(adm, _countof(adm), L".adm");
        wcscat_s((wchar_t *)traits->name, _countof(traits->name), L".rom");
    }

    result = NVMem_CreatePersitentArray(traits->name, traits->memSize, traits->persistentArray);
    if (result == OPEN_ERROR) {
        Win_Error("NVMem_Create/NVMem_CreatePersitentArray");
    }
    else if (result == NEW_FILE) {
        FillMemory(traits->persistentArray->mappingAddress, traits->memSize, traits->erasedValue);
    }

    result = NVMem_CreatePersitentArray(adm, sizeof(NVMem_Traits)+(traits->memSize * sizeof(unsigned __int32)), &temp);
    if (result == OPEN_ERROR) {
        Win_Error("NVMem_Create/NVMem_CreatePersitentArray");
    }
    else if (result == NEW_FILE) {
        memcpy(temp.mappingAddress, traits, sizeof(NVMem_Traits));
        ZeroMemory((unsigned __int8 *)temp.mappingAddress + sizeof(NVMem_Traits), sizeof(unsigned __int32));
    }
}

void NVMem_Close(NVMem_Traits * traits)
{
    NVMem_ClosePersitentArray(traits->persistentArray);
    free(traits->persistentArray);
}


void NVMem_SetData8(NVMem_Traits * traits, unsigned __int32 address, unsigned __int8 data)
{
    unsigned __int8 * ptr = (unsigned __int8 *)traits->persistentArray->mappingAddress;

    *(ptr + address) = data;
}


void NVMem_GetData8(NVMem_Traits * traits, unsigned __int32 address, unsigned __int8 * data)
{
    unsigned __int8 * ptr = (unsigned __int8 *)traits->persistentArray->mappingAddress;

    *data = *(ptr + address);
}

void NVMem_SetData16(NVMem_Traits * traits, unsigned __int32 address, unsigned __int16 data)
{
    unsigned __int16 * ptr = (unsigned __int16 *)traits->persistentArray->mappingAddress;

    *(ptr + address) = data;
}


void NVMem_GetData16(NVMem_Traits * traits, unsigned __int32 address, unsigned __int16 * data)
{
    unsigned __int16 * ptr = (unsigned __int16 *)traits->persistentArray->mappingAddress;

    *data = *(ptr + address);
}

void NVMem_SetData32(NVMem_Traits * traits, unsigned __int32 address, unsigned __int32 data)
{
    unsigned __int16 * ptr = (unsigned __int32 *)traits->persistentArray->mappingAddress;

    *(ptr + address) = data;
}


void NVMem_GetData32(NVMem_Traits * traits, unsigned __int32 address, unsigned __int32 * data)
{
    unsigned __int16 * ptr = (unsigned __int32 *)traits->persistentArray->mappingAddress;

    *data = *(ptr + address);
}


void NvSim_EraseBlock(NVMem_Traits * traits, unsigned __int32 address)
{
    unsigned __int32 mask = (unsigned __int32)traits->sectorSize - 1UL;
    unsigned __int16 * ptr = (unsigned __int16 *)traits->persistentArray->mappingAddress;

    if ((address & mask) != 0UL) {
        // TODO: warn misalignment.
        // ("address (%#X) should be aligned to %u-byte sector boundary.", address, traits->sectorSize)
    }

    FillMemory(ptr + (address & ~mask), traits->sectorSize, traits->erasedValue);
}
