CC = icc12w
CFLAGS =  -ID:\icc\include\ -e  -l 
ASFLAGS = $(CFLAGS) 
LFLAGS =  -LD:\icc\lib\ -btext:0x400 -bdata:0x1000 -dinit_sp:0x4000 -fmots19
FILES = 

KNXStack:	$(FILES)
	$(CC) -o KNXStack $(LFLAGS) @KNXStack.lk  
