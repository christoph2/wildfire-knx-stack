vpath %.c ../src

#OBJDIR=.
PROGRAMS = uart messaging

SRC = knx_messaging.c knx_utl.c knx_et.c
OBJ = $(SRC:%.c=%.o)

uart_SRC = link-layer/uart_bif.c config_internal.c knx_utl.c knx_et.c knx_timer.c port/mock/mock_sync.c \
     port/mock/mock_serial.c link-layer/msgif_llonly.c # link-layer/msgif_full.c
uart_OBJS = $(uart_SRC:%.c=%.o)


ifeq ($(OS),Windows_NT)
	TARGET=messaging.dll
	UART_TARGET=uart.dll
	EXT=.dll
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		TARGET=messaging.so
		UART_TARGET=uart.so
		EXT=.do
	endif
	ifeq ($(UNAME_S),Darwin)
		TARGET=messaging.dylib
		UART_TARGET=uart.dylib
		EXT=.dylib
	endif
endif

# TARGET = $(addsuffix $(EXT), messaging)

INCLUDE= -I . -I ../inc
CPPFLAGS += $(INCLUDE)
CFLAGS += -g -fPIC #TODO: conditional: -fprofile-arcs -ftest-coverage --coverage
LDFLAGS= -shared -fPIC -Wl,-Map=uart.map


all: $(PROGRAMS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $(notdir $@)


messaging: $(OBJ)
	@echo Linking $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

uart: $(uart_OBJS)
	@echo Linking $(UART_TARGET)
	$(CC) $(CFLAGS) -o $(UART_TARGET) $(notdir $(uart_OBJS)) $(LDFLAGS)

#%.dll:
#	@echo Linking $(UART_TARGET)
#	$(CC) $(CFLAGS) -o $(UART_TARGET) $(notdir $(uart_OBJS)) $(LDFLAGS)

define DLL_template =
 @echo "Und nun? Linking"
 $(1): $$($(1)_OBJS)
# 	$(CC) $(CFLAGS) -o $$($(1)_target) $notdir $$($(1)_OBJS) $(LDFLAGS)
 ALL_OBJS   += $$($(1)_OBJS)
endef

#$(foreach prog,$(PROGRAMS),$(eval $(call DLL_template,$(prog))))

#$(PROGRAMS):
#	$(LINK.o) $^ $(LDLIBS) -o $@

clean:
	rm -vf *o *dll *exe
	#rm -f $(ALL_OBJS) $(PROGRAMS
	
.PHONY: clean all

# Target-specific variables.
#gui.o: CPPFLAGS += -DUSE_NEW_MALLOC=1
#gui.o: gui.h
#        $(COMPILE.c) $(OUTPUT_OPTION) $<
