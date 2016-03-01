
#if !defined(__CONFIG_H)
#define __CONFIG_H

#define DISABLE_ALL_INTERRUPTS()
#define ENABLE_ALL_INTERRUPTS()

#define KSTACK_MEMORY_MAPPING       STD_OFF
#define KNX_BUILD_TYPE              KNX_RELEASE_BUILD
#define KNX_ENDIANESS               KNX_LITTLE_ENDIAN
#define TMR_NUM_TIMERS              (4)
#define TMR_TICK_RESOLUTION         (10)
#define KNX_TL_STATEMACHINE_STYLE   (1)
#define KNX_BUS_INTERFACE           KNX_BIF_TPUART_1
#define KNX_TARGET_TYPE             KNX_TARGET_POSIX

#define Port_Lock_TaskLevel()
#define Port_Unlock_TaskLevel()
#define Port_TimerLockMainTimer()
#define Port_TimerUnlockMainTimer()
#define Port_TimerLockDLTimer()
#define Port_TimerUnlockDLTimer()

#define TMR_LOCK_MAIN_TIMER()       Port_TimerLockMainTimer()
#define TMR_UNLOCK_MAIN_TIMER()     Port_TimerUnlockMainTimer()
#define TMR_LOCK_DL_TIMER()         Port_TimerLockDLTimer()
#define TMR_UNLOCK_DL_TIMER()       Port_TimerUnlockDLTimer()

#define TMR_START_DL_TIMER()        KnxTmr_DataLinkTimerStart()
#define TMR_STOP_DL_TIMER()         KnxTmr_DataLinkTimerStop()

#define PORT_LOCK_TASK_LEVEL()      Port_Lock_TaskLevel()
#define PORT_UNLOCK_TASK_LEVEL()    Port_Unlock_TaskLevel()

#endif /* __CONFIG_H */

// Für interne Zwecke (PRE_COMPILE config) benötigen wir 'config_internal.h'.

