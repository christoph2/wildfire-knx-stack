vpath %.c ../src

SRC = knx_messaging.c knx_utl.c knx_et.c # mtest.c
OBJ = $(SRC:%.c=%.o)

ESRC = knx_messaging.c knx_utl.c knx_et.c  mtest.c
EOBJ = $(ESRC:%.c=%.o)

#UNAME_S = $(shell uname -s)

ifeq ($(OS),Windows_NT)
	CCFLAGS += -D WIN32
		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
	CCFLAGS += -D AMD64
	endif
		ifeq ($(PROCESSOR_ARCHITECTURE),x86)
	CCFLAGS += -D IA32
	endif
	else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CCFLAGS += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		CCFLAGS += -D OSX
	endif
	UNAME_P := $(shell uname -p)
		ifeq ($(UNAME_P),x86_64)
	CCFLAGS += -D AMD64
	endif
		ifneq ($(filter %86,$(UNAME_P)),)
	CCFLAGS += -D IA32
	endif
		ifneq ($(filter arm%,$(UNAME_P)),)
	CCFLAGS += -D ARM
	endif
endif

INCLUDE= -I . -I ../inc
CPPFLAGS += $(INCLUDE)
CFLAGS += -g -fprofile-arcs -ftest-coverage --coverage -fPIC
LDFLAGS= -shared -fPIC #-Wl,-soname,messaging

messaging.dll: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)	

messaging.exe: $(EOBJ)
	$(CC) $(CFLAGS) -o $@ $(EOBJ)

	
clean:
	rm -vf *o *dll *exe
	
.PHONY: clean