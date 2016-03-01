SUBDIRS = .
vpath %.c ./src
bin_PROGRAMS = tpuart
tpuart_SOURCES = knx_conversion.c link-layer/uart_bif.c knx_utl.c knx_timer.c knx_messaging.c Posix_Serial.c\
	knx_disp.c knx_et.c config_internal.c port/posix/posix_timer.c port/posix/posix_serial.c
AM_CFLAGS = -std=c99 -ggdb -gdwarf-2 -I ./inc -Wextra


