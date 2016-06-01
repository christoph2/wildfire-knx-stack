vpath %.c ../src

SRC = knx_messaging.c knx_utl.c knx_et.c # mtest.c
OBJ = $(SRC:%.c=%.o)

ESRC = knx_messaging.c knx_utl.c knx_et.c  mtest.c
EOBJ = $(ESRC:%.c=%.o)



INCLUDE= -I . -I ../inc
CPPFLAGS += $(INCLUDE)
CFLAGS += -g -fprofile-arcs -ftest-coverage --coverage
LDFLAGS= -shared -fPIC #-Wl,-soname,messaging

messaging.dll: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)	

messaging.exe: $(EOBJ)
	$(CC) $(CFLAGS) -o $@ $(EOBJ)

	
clean:
	rm -vf *o *dll *exe
	
.PHONY: clean