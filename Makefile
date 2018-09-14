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

SIMPLE-OBJS = simple_test.o
COPLEY-OBJS = ecat_copley.o configure.o
ELMO-OBJS = ecat_elmo.o configure.o
ELMO-TEST-OBJS = elmo_test.o configure.o
TEST_TORQUE-OBJS = simple_test_torque.o
TEST_VELOC-OBJS = simple_test_veloc.o
ECAT_STANDALONE_OBJS = ecat-standalone.o ecat-ng.o

###############binaries####################

all: ecat-standalone

ecat_copley: $(COPLEY-OBJS)
	$(CC) $^ -o  ecat_copley -L$(LIB_PATH) -lsoem -loshw -losal -lsocket

ecat_elmo: $(ELMO-OBJS)
	$(CC) $^ -o  ecat_elmo -L$(LIB_PATH) -lsoem -loshw -losal -lsocket

elmo_test: $(ELMO-TEST-OBJS)
	$(CC) $^ $(INCLUDE_PATHS) -L$(LIB_PATH) -o elmo_test -lsoem -loshw -losal -lsocket

simple_test: $(SIMPLE-OBJS)
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) simple_test.c -o simple_test -lsoem -loshw -losal -lsocket

simple_test.o: simple_test.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c simple_test.c

ecat_elmo.o: ecat_elmo.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c ecat_elmo.c

elmo_test.o: elmo_test.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c elmo_test.c

ecat_copley.o: ecat_copley.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c ecat_copley.c

configure.o: configure.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c configure.c


simple_test_torque.o: simple_test_torque.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c simple_test_torque.c

simple_test_torque: $(TEST_TORQUE-OBJS)
	$(CC) $^ -o simple_test_torque -L$(LIB_PATH) -lsoem -loshw -losal -lsocket -lm

simple_test_veloc.o: simple_test_veloc.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c simple_test_veloc.c

simple_test_veloc: $(TEST_VELOC-OBJS)
	$(CC) $^ -o simple_test_veloc -L$(LIB_PATH) -lsoem -loshw -losal -lsocket -lm

ecat-ng.o: ecat-ng.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c ecat-ng.c

ecat-standalone.o: ecat-standalone.c
	$(CC) $(INCLUDE_PATHS) -L$(LIB_PATH) -c ecat-standalone.c

ecat-standalone: $(ECAT_STANDALONE_OBJS)
	$(CC) $^ -o ecat-standalone -L$(LIB_PATH) -lsoem -loshw -losal -lsocket -lm

###############utils#######################
clean: \
;rm *.o simple_test_veloc

