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
ECAT_TCS_OBJS = timerservice.o ecat-ng.o
###############binaries####################

all: clean simple_test ecat-standalone #ecat-tcs 

ecat_copley: $(COPLEY-OBJS)

simple_test: $(SIMPLE_OBJS)
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) simple_test.c -o simple_test -lsoem -loshw -losal -lsocket

simple_test.o: simple_test.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c simple_test.c

ecat-ng.o: ecat-ng.c ecat_ng.h
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c ecat-ng.c

ecat-standalone.o: ecat-standalone.c ecat_ng.h
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c ecat-standalone.c

ecat-standalone: $(ECAT_STANDALONE_OBJS)
	$(CC) $^ -o ecat-standalone -L$(LIB_PATH) -lsoem -loshw -losal -lsocket -lm

timerservice.o: timerservice.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c timerservice.c

ecat-tcs: $(ECAT_TCS_OBJS)
	$(CC) $^ -o ecat-tcs -L$(LIB_PATH) -lsoem -loshw -losal -lsocket -lm

###############utils#######################
clean: \
;rm -f *.o simple_test ecat-standalone ecat-tcs

