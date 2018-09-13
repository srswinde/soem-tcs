CROSS_COMPILER=/usr/local/gcc-cross/
#CC=$(CROSS_COMPILER)bin/i386-pc-nto-qnx-gcc
#C_INCLUDE_PATH=$(CROSS_COMPILER)i386-pc-nto-qnx/include
#LD_LIBRARY_PATH=$(CROSS_COMPILER)i386-pc-nto-qnx/lib
CC=qcc -V gcc_ntox86
INCLUDE_PATHS=-I./include -I./include/osal -I./include/qnx
LIB_PATH=./lib/qnx
#################################################
# makefile for ethercat TCS-NG Driver		#
################directories######################

SIMPLE-OBJS = simple_test.o
NG-OBJS = ecat_ng.o
STANDALONE-OBJS = ecat_ng.o ecat_standalone.o

###############binaries####################

all: simple_test ecat_standalone

ecat_ng: $(NG-OBJS)
	$(CC) $^ -o  ecat_ng -L$(LIB_PATH) -lsoem -loshw -losal -lsocket

ecat_ng.o: ecat_ng.c ecat_ng.h
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c ecat_ng.c

ecat_standalone: $(STANDALONE-OBJS)
	$(CC) $^ -o  ecat_standalone -L$(LIB_PATH) -lsoem -loshw -losal -lsocket -lm

ecat_standalone.o: ecat_standalone.c ecat_ng.h
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c ecat_standalone.c

simple_test: $(SIMPLE-OBJS)
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) simple_test.c -o simple_test -lsoem -loshw -losal -lsocket

simple_test.o: simple_test.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c simple_test.c


###############utils#######################
clean: \
;rm *.o simple_test ecat_standalone

