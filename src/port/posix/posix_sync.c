
/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2016 by Christoph Schueler <github.com/Christoph2,
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

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct tagSync_MutexType {
    pthread_mutex_t mutex;
    pthread_mutexattr_t mattr;
} Sync_MutexType;


/*
**  Common Sync-Functions.
*/
void Sync_Init(Sync_MutexType * mutex)
{
    mutex->mutex = PTHREAD_MUTEX_INITIALIZER;
    int ret;

    ret = pthread_mutexattr_init(&mutex->mattr);
    if (ret != 0) {
        printf("error [pthread_mutexattr_init]: %d\n", ret);
    }

    ret = pthread_mutex_init(&mutex->mutex, &mutex->mattr);
    if (ret != 0) {
        printf("error [pthread_mutex_init]: %d\n", ret);
    }
}

void Sync_Deinit(Sync_MutexType * mutex)
{
    int ret;

    ret = pthread_mutex_destroy(&mutex->mutex);
    if (ret != 0) {
        printf("error [pthread_mutex_destroy]: %d\n", ret);
    }
    ret = pthread_mutexattr_destroy(&mutex->mattr);
    if (ret != 0) {
        printf("error [pthread_mutexattr_destroy]: %d\n", ret);
    }
}

void Sync_Lock(Sync_MutexType * mutex)
{
    int ret;

    ret = pthread_mutex_lock(&mutex->mutex);
    if (ret != 0) {
        printf("error [pthread_mutex_lock]: %d\n", ret);
    }

}

void Sync_Unlock(Sync_MutexType * mutex)
{
    int ret;

    ret = pthread_mutex_unlock(&mutex->mutex);
    if (ret != 0) {
        printf("error [pthread_mutex_unlock]: %d\n", ret);
    }
}

void Sync_TryLock(Sync_MutexType * mutex)
{
    int ret;

    ret = pthread_mutex_trylock(&mutex->mutex);
    if (ret != 0) {
        printf("error [pthread_mutex_trylock]: %d\n", ret);
    }
}

/*
**  Specific Sync-Functions.
*/
#if 0
#define TMR_LOCK_MAIN_TIMER()       Port_TimerLockMainTimer()
#define TMR_UNLOCK_MAIN_TIMER()     Port_TimerUnlockMainTimer()

#define TMR_LOCK_DL_TIMER()         Port_TimerLockDLTimer()
#define TMR_UNLOCK_DL_TIMER()       Port_TimerUnlockDLTimer()

#define PORT_LOCK_TASK_LEVEL()      Port_Lock_TaskLevel()
#define PORT_UNLOCK_TASK_LEVEL()    Port_Unlock_TaskLevel()
#endif

static Sync_MutexType taskLevelMutex;

void Sync_Setup(void)
{
    Sync_Init(&taskLevelMutex);
}

void Sync_Lock_TaskLevel(void)
{
    Sync_Lock(&taskLevelMutex);
}

void Sync_Unlock_TaskLevel(void)
{
    Sync_Unlock(&taskLevelMutex);
}

