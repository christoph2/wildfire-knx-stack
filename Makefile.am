SUBDIRS = .
vpath %.c ./src
bin_PROGRAMS = tpuart
tpuart_SOURCES = knx_conversion.c link-layer/uart_bif.c knx_utl.c knx_timer.c knx_messaging.c Posix_Serial.c\
	knx_disp.c knx_et.c config_internal.c port/posix/posix_timer.c port/posix/posix_serial.c\
	port/posix/posix_sync.c port/win/win_hrt.c
AM_CFLAGS = -std=c99 -ggdb -gdwarf-4 -I ./inc -Wextra
AM_LDFLAGS = -lm
TESTS_ENVIRONMENT = python
TESTS = testme.py

