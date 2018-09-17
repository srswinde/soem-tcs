CROSS_COMPILER=/usr/local/gcc-cross/
#CC=$(CROSS_COMPILER)bin/i386-pc-nto-qnx-gcc
#C_INCLUDE_PATH=$(CROSS_COMPILER)i386-pc-nto-qnx/include
#LD_LIBRARY_PATH=$(CROSS_COMPILER)i386-pc-nto-qnx/lib
CC=qcc -V gcc_ntox86
INCLUDE_PATHS=-I./include -I./include/osal -I./include/qnx
LIB_PATH=./lib/qnx
#################################################
# makefile for ethercat TCS-NG Drivers		#
################directories######################

SIMPLE_OBJS = simple_test.o
ECAT_STANDALONE_OBJS = ecat-standalone.o ecat-ng.o

###############binaries####################

all: simple_test ecat-standalone 

ecat_copley: $(COPLEY-OBJS)

simple_test: $(SIMPLE_OBJS)
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) simple_test.c -o simple_test -lsoem -loshw -losal -lsocket

simple_test.o: simple_test.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c simple_test.c

ecat-ng.o: ecat-ng.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c ecat-ng.c

ecat-standalone.o: ecat-standalone.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c ecat-standalone.c

ecat-standalone: $(ECAT_STANDALONE_OBJS)
	$(CC) $^ -o ecat-standalone -L$(LIB_PATH) -lsoem -loshw -losal -lsocket -lm

###############utils#######################
clean: \
;rm *.o simple_test ecat-standalone

