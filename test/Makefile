vpath %.c ../src

SRC = knx_messaging.c knx_utl.c knx_et.c # mtest.c
OBJ = $(SRC:%.c=%.o)

#UNAME_S = $(shell uname -s)


ifeq ($(OS),Windows_NT)
	TARGET=messaging.dll
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		TARGET=messaging.so
	endif
	ifeq ($(UNAME_S),Darwin)
		TARGET=messaging.dylib
	endif
endif

# TARGET = $(addsuffix $(EXT), messaging)

INCLUDE= -I . -I ../inc
CPPFLAGS += $(INCLUDE)
CFLAGS += -g -fprofile-arcs -ftest-coverage --coverage -fPIC
LDFLAGS= -shared -fPIC #-Wl,-soname,messaging

messaging: $(OBJ)
	@echo Linking $(TARGET)
	$(CC) $(CCFLAGS) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)	

clean:
	rm -vf *o *dll *exe
	
.PHONY: clean
