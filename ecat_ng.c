/*############################################################################
#  Title: ecat_ng.c
#  Author: C. Johnson / S. Swindell
#  Date: 9/2/18
#  Args:  N/A
#  Description: this is largely a hack of simpletest.c from the SOEM website.
# 	This will eventually be the soem driver for TCS
#
#############################################################################*/

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include <math.h>

#include "soem/ethercattype.h"
#include "soem/nicdrv.h"
#include "soem/ethercatbase.h"
#include "soem/ethercatmain.h"
#include "soem/ethercatdc.h"
#include "soem/ethercatcoe.h"
#include "soem/ethercatfoe.h"
#include "soem/ethercatconfig.h"
#include "soem/ethercatprint.h"

#define EC_TIMEOUTMON 500

#define INITIAL_POS 342592238

#define VEL_MOD 0
#define POS_MOD 1

#define OK 0
#define NOK 1

char IOmap[4096];
pthread_t thread1;
int expectedWKC;
boolean needlf;
volatile int wkc;
boolean inOP;
uint8 currentgroup = 0;

#define CTRL_DEAD   		0
#define CTRL_SHUTDOWN   	6
#define CTRL_SWITCH_ON		7
#define CTRL_SWITCH_ON_ENABLE_OP 15
#define CTRL_HALT		128
typedef struct
{
unsigned short switchOn: 1;
unsigned short enableVoltage: 1;
unsigned short quickStop: 1;
unsigned short enableOperation: 1;
unsigned short homeOperationStop: 1;
unsigned short oms1: 2;
unsigned short faultReset: 1;
unsigned short halt: 1;
unsigned short oms2: 1;
unsigned short reserved1: 1;
unsigned short reserved2: 5;
} ecat_controlword;


#define STAT_NOT_READY 		0  //& statusword with 79
#define STAT_SWITCH_ON_DISABLED 64 //& with statusword 79
#define STAT_RDY_TO_SWITCH_ON 	33 //& with statusword 111
#define STAT_SWITCHED_ON	35 //& with statusword 111
#define STAT_OP_ENABLED 	39 //& with statusword 111
#define STAT_QUICK_STOP_ACTIVE	7  //& with statusword 111
#define STAT_FAULT_REACT_ACTIVE 15 //& with statusword 79
#define STAT_FAULT		8  //& with statusword 79
#define STAT_ALL_READY		567//& with statusword 0xfff
typedef struct
{
unsigned short readyToSwitchOn: 1;
unsigned short SwitchedOn: 1;
unsigned short OperationEnabled: 1;
unsigned short fault: 1;
unsigned short voltageEnabled: 1;
unsigned short quickStop: 1;
unsigned short SwitchOnDisabled: 1;
unsigned short warning: 1;
unsigned short oms1: 1;
unsigned short remote: 1;
unsigned short targetReached: 1;
unsigned short internalLimitActive: 1;
unsigned short homingAttained: 1;
unsigned short ms: 3;
} ecat_statusword;


typedef union
{
ecat_controlword cword;
uint16 cword_int;
}controlword_union;


typedef union
{
ecat_statusword sword;
uint16 sword_int;
}statusword_union;


struct PosOut { //0x1600 -- cyclic position rxpdo
    uint32 pos;
    uint32 dio;
    controlword_union control;
    //int16 control;
};

struct VelocOut { //0x1601 -- cyclic velocity rxpdo
    uint32 veloc;
    controlword_union control;
    //int16 control;
};



struct PVTOut { //0x1606
    uint32 pos;
    uint32 dio;
    uint32 veloc;
    uint32 voffset;
    uint16 toffset;
    uint16 control;
};

struct PosTrqVelIn {  //0x1A02  --  not useful
    int32 position;
    int16 torque;
    uint16 status;
    int8 profile;
};

struct PosVelDioIn {  //0x1A03 -- velovity AND position mode txpdo
    int32 position;
    uint32 dio;
    int32 velocity;
    statusword_union status;
};


typedef struct
{
unsigned short switchOn: 1;
unsigned short enableVoltage: 1;
unsigned short quickStop: 1;
unsigned short enableOperation: 1;
unsigned short homeOperationStop: 1;
unsigned short oms1: 2;
unsigned short faultReset: 1;
unsigned short halt: 1;
unsigned short oms2: 1;
unsigned short reserved1: 1;
unsigned short reserved2: 5;
} controlword;


typedef struct
{
unsigned short readyToSwitchOn: 1;
unsigned short SwitchedOn: 1;
unsigned short OperationEnabled: 1;
unsigned short fault: 1;
unsigned short voltageEnabled: 1;
unsigned short quickStop: 1;
unsigned short SwitchOnDisabled: 1;
unsigned short warning: 1;
unsigned short oms1: 1;
unsigned short remote: 1;
unsigned short targetReached: 1;
unsigned short internalLimitActive: 1;
unsigned short homingAttained: 1;
unsigned short ms: 3;
} statusword;


static int reachedInitial;

typedef struct
{
char name[50];
int position;
int velocity;
short current;
} axisdata;

axisdata axisdata_all[8];







/*############################################################################
#  Title: ecatRead()
#  Author: C. Johnson
#  Date: 9/2/18
#  Args:  N/A
#  Description: variable precision ethercat read function
#
#############################################################################*/
int READ(idx, sub, buf, comment)
    {
        buf=0;
        int __s = sizeof(buf);
        int __ret = ec_SDOread(1, idx, sub, FALSE, &__s, &buf, EC_TIMEOUTRXM);
        printf("Read at 0x%04x:%d => wkc: %d; data: 0x%.*x (%d)\t[%s]\n", idx, sub, __ret, __s, (unsigned int)buf, (unsigned int)buf, comment);
     }

/*############################################################################
#  Title: ecatWrite()
#  Author: C. Johnson
#  Date: 9/2/18
#  Args:  N/A
#  Description: variable precision ethercat write function
#
#############################################################################*/
int WRITE(idx, sub, buf, value, comment)
    {  
        int __s = sizeof(buf); 
        buf = value;
        int __ret = ec_SDOwrite(1, idx, sub, FALSE, __s, &buf, EC_TIMEOUTRXM);
        printf("Write at 0x%04x:%d => wkc: %d; data: 0x%.*x\t{%s}\n", idx, sub, __ret, __s, (unsigned int)buf, comment);
    }

/*############################################################################
#  Title: ecatErrorChk()
#  Author: C. Johnson
#  Date: 9/2/18
#  Args:  N/A
#  Description: ethercat error check
#
#############################################################################*/
int CHECKERROR()
{ 
    ec_readstate();
    printf("EC> \"%s\" %x - %x [%s] \n", (char*)ec_elist2string(), ec_slave[1].state, ec_slave[1].ALstatuscode, (char*)ec_ALstatuscode2string(ec_slave[1].ALstatuscode));
}

/*############################################################################
#  Title: initEcat()
#  Author: C. Johnson
#  Date: 9/2/18
#  Args:  N/A
#  Description: init ethercat.  hacked from simpletest.c
#
#############################################################################*/
int initEcat(char *ifname, int loopmode)
{
    int i, j, oloop, iloop, wkc_count, chk;
    needlf = FALSE;
    inOP = FALSE;
    int32 ob2;int os;
	

    uint32 buf32;
    uint16 buf16;
    uint8 buf8;

    struct PosTrqVelIn *val;
    struct VelocOut *target;

   printf("Starting simple test\n");

	reachedInitial = 0;

   /* initialise SOEM, bind socket to ifname */
   if (ec_init(ifname))
   {
      printf("ec_init on %s succeeded.\n",ifname);
      /* find and auto-config slaves */

      /** network discovery */
      if ( ec_config_init(FALSE) > 0 )
      {
         printf("%d slaves found and configured.\n",ec_slavecount);

         printf("Has CA? %s\n", ec_slave[1].CoEdetails & ECT_COEDET_SDOCA ? "true":"false" );

         /** CompleteAccess disabled for Elmo driver */
         ec_slave[1].CoEdetails ^= ECT_COEDET_SDOCA;

         ec_statecheck(0, EC_STATE_PRE_OP,  EC_TIMEOUTSTATE);

         /** set PDO mapping */
         /** OpMode: -3  => electronic gearing mode */
         /** OpMode: -2  => analog velocity mode */
         /** OpMode: -1  => analog current mode */
         /** OpMode: 1  => profile position mode */
         /** OpMode: 3  => profile velocity mode */
         /** OpMode: 4  => profile torque mode*/
         /** OpMode: 6  => homing mode */
         /** OpMode: 7  => interpolated position mode */
         /** OpMode: 8  => cyclic synchronous position mode */
         /** OpMode: 9  => cyclic synchronous velocity mode */
         /** OpMode: 10  => cyclic synchronous torque mode */

	/** rxPdo: 1601  => cyclic synchronous position mode */
        /** rxPdo: 1602  => cyclic synchronous velocity mode */
        /** txPdo: 1A01  => cyclic synchronous position mode */
        /** txPdo: 1A02  => cyclic synchronous velocity mode */

	if(loopmode == VEL_MOD)
		{ 
		WRITE(0x6060, 0, buf8, 9, "OpMode");
         	READ(0x6061, 0, buf8, "OpMode display");


         	READ(0x1c12, 0, buf32, "rxPDO:0");
         	READ(0x1c13, 0, buf32, "txPDO:0");

         	READ(0x1c12, 1, buf32, "rxPDO:1");
         	READ(0x1c13, 1, buf32, "txPDO:1");


         	//Velocity
	 	os=sizeof(ob2); ob2 = 0x16010001;
         	ec_SDOwrite(1,0x1c12,0,TRUE,os,&ob2,EC_TIMEOUTRXM);
         	os=sizeof(ob2); ob2 = 0x1a030001;
         	ec_SDOwrite(1,0x1c13,0, TRUE, os,&ob2,EC_TIMEOUTRXM);

	 	//!!!!!!original working!!!!!
	 	//os=sizeof(ob2); ob2 = 0x16010001;
         	//ec_SDOwrite(1,0x1c12,0,TRUE,os,&ob2,EC_TIMEOUTRXM);
         	//os=sizeof(ob2); ob2 = 0x1a020001;
         	//ec_SDOwrite(1,0x1c13,0, TRUE, os,&ob2,EC_TIMEOUTRXM);

		}
	
	else if (loopmode == POS_MOD)
		{
		WRITE(0x6060, 0, buf8, 8, "OpMode");
         	READ(0x6061, 0, buf8, "OpMode display");


         	READ(0x1c12, 0, buf32, "rxPDO:0");
         	READ(0x1c13, 0, buf32, "txPDO:0");

         	READ(0x1c12, 1, buf32, "rxPDO:1");
         	READ(0x1c13, 1, buf32, "txPDO:1");


        	//Position
         	os=sizeof(ob2); ob2 = 0x16000001;
         	ec_SDOwrite(1,0x1c12,0,TRUE,os,&ob2,EC_TIMEOUTRXM);
	 	os=sizeof(ob2); ob2 = 0x1a030001;
         	ec_SDOwrite(1,0x1c13,0, TRUE, os,&ob2,EC_TIMEOUTRXM);

	 	}


//         WRITE(0x1c12, 0, buf32, 0x16010001, "rxPDO");
//         WRITE(0x1c13, 0, buf32, 0x1A010001, "txPDO");

         READ(0x1c12, 0, buf32, "rxPDO:0");
         READ(0x1c13, 0, buf32, "txPDO:0");

         READ(0x1c12, 1, buf32, "rxPDO:1");
         READ(0x1c13, 1, buf32, "txPDO:1");


         /** if CA disable => automapping works */
         ec_config_map(&IOmap);

         /** let DC off for the time being */
//         ec_configdc();


         printf("\nSlave:%d\n Name:%s\n Output size: %dbits\n Input size: %dbits\n State: %d\n Delay: %d[ns]\n Has DC: %d\n",
               1, ec_slave[1].name, ec_slave[1].Obits, ec_slave[1].Ibits,
               ec_slave[1].state, ec_slave[1].pdelay, ec_slave[1].hasdc);


         /** disable heartbeat alarm */
         READ(0x10F1, 2, buf32, "Heartbeat?");
         WRITE(0x10F1, 2, buf32, 1, "Heartbeat");


         WRITE(0x60c2, 1, buf8, 2, "Time period");
         WRITE(0x2f75, 0, buf16, 2, "Interpolation timeout");

         printf("Slaves mapped, state to SAFE_OP.\n");

         int timestep = 700;

         /* wait for all slaves to reach SAFE_OP state */
         ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);

         /** old SOEM code, inactive */
         oloop = ec_slave[0].Obytes;
         if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;
         if (oloop > 20) oloop = 8;
         iloop = ec_slave[0].Ibytes;
         if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
         if (iloop > 20) iloop = 8;

         printf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

         printf("Request operational state for all slaves\n");
         expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
         printf("Calculated workcounter %d\n", expectedWKC);

         /** going operational */
         ec_slave[0].state = EC_STATE_OPERATIONAL;
         /* send one valid process data to make outputs in slaves happy*/
         ec_send_processdata();
         ec_receive_processdata(EC_TIMEOUTRET);


         READ(0x6083, 0, buf32, "Profile acceleration");
         READ(0x6084, 0, buf32, "Profile deceleration");
         READ(0x6085, 0, buf32, "Quick stop deceleration");

         /* request OP state for all slaves */
         ec_writestate(0);
         chk = 40;
         /* wait for all slaves to reach OP state */
         do
         {
            ec_send_processdata();
            ec_receive_processdata(EC_TIMEOUTRET);
            ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
         }
         while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
         if (ec_slave[0].state == EC_STATE_OPERATIONAL )
         {
            printf("Operational state reached for all slaves.\n");
            wkc_count = 0;
            inOP = TRUE;

            /**
             * Drive state machine transistions
             *   0 -> 6 -> 7 -> 15
             */
            READ(0x6041, 0, buf16, "*status word*");
            if(buf16 == 0x218){
                WRITE(0x6040, 0, buf16, 128, "*control word*"); usleep(100000);
                READ(0x6041, 0, buf16, "*status word*");
            }


            WRITE(0x6040, 0, buf16, 0, "*control word*"); usleep(100000);
            READ(0x6041, 0, buf16, "*status word*");

            WRITE(0x6040, 0, buf16, 6, "*control word*"); usleep(100000);
            READ(0x6041, 0, buf16, "*status word*");

            WRITE(0x6040, 0, buf16, 7, "*control word*"); usleep(100000);
            READ(0x6041, 0, buf16, "*status word*");

            WRITE(0x6040, 0, buf16, 15, "*control word*"); usleep(100000);
            READ(0x6041, 0, buf16, "*status word*");

   CHECKERROR();
            READ(0x1a0b, 0, buf8, "OpMode Display");



	    //added by cj
	    while(!reachedInitial)
		{
		elevateStat(0);
		}
            READ(0x1001, 0, buf8, "Error");

            
 	    return(OK);
            
            }
            else
            {
                printf("Not all slaves reached operational state.\n");
                ec_readstate();
                for(i = 1; i<=ec_slavecount ; i++)
                {
                    if(ec_slave[i].state != EC_STATE_OPERATIONAL)
                    {
                        printf("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
                            i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
                    }
                }
            }


            printf("\nRequest init state for all slaves\n");
            WRITE(0x10F1, 2, buf32, 0, "Heartbeat");
            ec_slave[0].state = EC_STATE_INIT;
            /* request INIT state for all slaves */
            ec_writestate(0);
        }
        else
        {
            printf("No slaves found!\n");
        }
        printf("End simple test, close socket\n");
        /* stop SOEM, close socket */
        ec_close();
    }
    else
    {
        printf("No socket connection on %s\nExcecute as root\n",ifname);
    }
    return(NOK);
}

/*############################################################################
#  Title: getPosition(int axis)
#  Author: C. Johnson
#  Date: 9/2/18
#  Args:  N/A
#  Description: command a velocity.  hacked from simpletest.c
#
#############################################################################*/
int ecat_getPosition(int axis)
{
return axisdata_all[axis].position;
}

/*############################################################################
#  Title: commandVel()
#  Author: C. Johnson
#  Date: 9/2/18
#  Args:  N/A
#  Description: command a velocity.  hacked from simpletest.c
#
#############################################################################*/
int commandVel(int targVel)
{
needlf = FALSE;
inOP = FALSE;
uint32 buf32;
uint16 buf16;
uint8 buf8;
uint16 ctrl_macro, stat_macro;

//struct PosTrqVelIn *val;
struct PosVelDioIn *val;
struct VelocOut *target;

int timestep = 700;

target = (struct VelocOut *)(ec_slave[1].outputs);
//val = (struct PosTrqVelIn *)(ec_slave[1].inputs);
val = (struct PosVelDioIn *)(ec_slave[1].inputs);


	/** PDO I/O refresh */
	ec_send_processdata();
	wkc = ec_receive_processdata(EC_TIMEOUTRET);

	if(wkc >= expectedWKC)
		{
		ctrl_macro=(uint16) target->control.cword_int;
		stat_macro=(uint16) val->status.sword_int;
		
		//printf("Processdata cycle %4d, WKC %d,", i, wkc);
		//printf("  pos: %li, tor: %li, stat: %li, mode: %li,", val->position, val->torque, val->status, val->profile);
		printf("  pos: %li, vel: %li, stat: %li,", val->position, val->velocity, stat_macro);
		
		/** if in fault or in the way to normal status, we update the state machine */
		switch(ctrl_macro)
			{
			case CTRL_DEAD:
				ctrl_macro = CTRL_SHUTDOWN;
				break;
			case CTRL_SHUTDOWN:
				ctrl_macro = CTRL_SWITCH_ON;
				break;
			case CTRL_SWITCH_ON:
				ctrl_macro = CTRL_SWITCH_ON_ENABLE_OP;
				break;
			case CTRL_HALT:
				ctrl_macro = CTRL_DEAD;
				break;
			default:
				if(stat_macro >> 3 & 0x01){
					READ(0x1001, 0, buf8, "Error");
					ctrl_macro = 128;
                            		}
//                            break;
                        }


		/** we wait to be in ready-to-run mode and with initial value reached */
		if(reachedInitial == 0 /*&& val->position == INITIAL_POS */&& (stat_macro & 0x0fff) == STAT_ALL_READY)
			{
			reachedInitial = 1;
			}

		if((stat_macro & 0x0fff) == STAT_ALL_READY && reachedInitial)
			{
			target->veloc = (int32) targVel;
			}

		printf("  Target: %li, Control: %li\n", target->veloc, ctrl_macro);
		
		memcpy(&target->control, &ctrl_macro, 2);

		printf("\r");
		needlf = TRUE;
                }
             

}

/*############################################################################
#  Title: elevateStat
#  Author: C. Johnson
#  Date: 9/2/18
#  Args:  N/A
#  desc: run through one to try to stuff position
#
#############################################################################*/
int elevateStat(int axis)
{
needlf = FALSE;
inOP = FALSE;
uint32 buf32;
uint16 buf16;
uint8 buf8;
uint16 ctrl_macro, stat_macro;

//struct PosTrqVelIn *val;
struct PosVelDioIn *val;
struct PosOut *target;

int timestep = 700;

target = (struct PosOut *)(ec_slave[1].outputs);
//val = (struct PosTrqVelIn *)(ec_slave[1].inputs);
val = (struct PosVelDioIn *)(ec_slave[1].inputs);


	/** PDO I/O refresh */
	ec_send_processdata();
	wkc = ec_receive_processdata(EC_TIMEOUTRET);

	if(wkc >= expectedWKC)
		{
		ctrl_macro=(uint16) target->control.cword_int;
		stat_macro=(uint16) val->status.sword_int;
		//printf("Processdata cycle %4d, WKC %d,", i, wkc);
		printf("  Elevate State pos: %li, vel: %li, stat: %li,", val->position, val->velocity, val->status);
		
		/** if in fault or in the way to normal status, we update the state machine */
		switch(ctrl_macro)
			{
			case CTRL_DEAD:
				ctrl_macro = CTRL_SHUTDOWN;
				break;
			case CTRL_SHUTDOWN:
				ctrl_macro = CTRL_SWITCH_ON;
				break;
			case CTRL_SWITCH_ON:
				ctrl_macro = CTRL_SWITCH_ON_ENABLE_OP;
				break;
			case CTRL_HALT:
				ctrl_macro = CTRL_DEAD;
				break;
			default:
				if(stat_macro >> 3 & 0x01){
					READ(0x1001, 0, buf8, "Error");
					ctrl_macro = 128;
                            		}
//                            break;
                        }


		/** we wait to be in ready-to-run mode and with initial value reached */
		if(reachedInitial == 0 /*&& val->position == INITIAL_POS */&& (stat_macro & 0x0fff) == STAT_ALL_READY)
			{
			printf("FUCK MEEEEEEEEEE, ");
			reachedInitial = 1;
			target->pos = val->position;
			}

		/*if((stat_macro & 0x0fff) == STAT_ALL_READY && reachedInitial)
		//if((val->status & 0x0fff) == 0x0237)
			{
			target->pos = (int32) targPos;
			}*/

		printf("  Target: %li, Control: %li\n", target->pos, target->control);

		axisdata_all[0].position = val->position;

		memcpy(&target->control, &ctrl_macro, 2);

		printf("\r");
		needlf = TRUE;
		return reachedInitial;
                }
             

}

/*############################################################################
#  Title: commandPos
#  Author: C. Johnson
#  Date: 9/2/18
#  Args:  N/A
#  Description: command a position.  hacked from simpletest.c
#
#############################################################################*/
int commandPos(int axis, int targPos)
{
needlf = FALSE;
inOP = FALSE;
uint32 buf32;
uint16 buf16;
uint8 buf8;
uint16 ctrl_macro, stat_macro;

//struct PosTrqVelIn *val;
struct PosVelDioIn *val;
struct PosOut *target;

int timestep = 700;

target = (struct PosOut *)(ec_slave[1].outputs);
//val = (struct PosTrqVelIn *)(ec_slave[1].inputs);
val = (struct PosVelDioIn *)(ec_slave[1].inputs);


	/** PDO I/O refresh */

	if(wkc >= expectedWKC)
		{
		ctrl_macro=(uint16) target->control.cword_int;
		stat_macro=(uint16) val->status.sword_int;
		//printf("Processdata cycle %4d, WKC %d,", i, wkc);
		printf("  pos: %li, vel: %li, stat: %li,", val->position, val->velocity, val->status);
		
		/** if in fault or in the way to normal status, we update the state machine */
		switch(ctrl_macro)
			{
			case CTRL_DEAD:
				ctrl_macro = CTRL_SHUTDOWN;
				break;
			case CTRL_SHUTDOWN:
				ctrl_macro = CTRL_SWITCH_ON;
				break;
			case CTRL_SWITCH_ON:
				ctrl_macro = CTRL_SWITCH_ON_ENABLE_OP;
				break;
			case CTRL_HALT:
				ctrl_macro = CTRL_DEAD;
				break;
			default:
				if(stat_macro >> 3 & 0x01){
					READ(0x1001, 0, buf8, "Error");
					ctrl_macro = 128;
                            		}
//                            break;
                        }
	

		/** we wait to be in ready-to-run mode and with initial value reached */
		if(reachedInitial == 0 /*&& val->position == INITIAL_POS */&& (stat_macro & 0x0fff) == STAT_ALL_READY)
			{
			reachedInitial = 1;
			target->pos = val->position;
			}

		if((stat_macro & 0x0fff) == STAT_ALL_READY && reachedInitial)
		//if((val->status & 0x0fff) == 0x0237)
			{
			target->pos = (int32) targPos;
			}

		printf("  Target: %li, Control: %li\n", target->pos, target->control);

		axisdata_all[0].position = val->position;

		memcpy(&target->control, &ctrl_macro, 2);
		ec_send_processdata();
		wkc = ec_receive_processdata(EC_TIMEOUTRET);

		printf("\r");
		needlf = TRUE;
                }
             

}

/*############################################################################
#  Title: ecatErr( void *ptr )
#  Author: C. Johnson
#  Date: 9/2/18
#  Args:  N/A
#  Description: check errors on ethercat bus
#
#############################################################################*/
int ecatErr()
{
int slave;

if( inOP && ((wkc < expectedWKC) || ec_group[currentgroup].docheckstate))
        {
            if (needlf)
            {
               needlf = FALSE;
               printf("\n");
            }
            /* one ore more slaves are not responding */
            ec_group[currentgroup].docheckstate = FALSE;
            ec_readstate();
            for (slave = 1; slave <= ec_slavecount; slave++)
            {
               if ((ec_slave[slave].group == currentgroup) && (ec_slave[slave].state != EC_STATE_OPERATIONAL))
               {
                  ec_group[currentgroup].docheckstate = TRUE;
                  if (ec_slave[slave].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
                  {
                     printf("ERROR : slave %d is in SAFE_OP + ERROR, attempting ack.\n", slave);
                     ec_slave[slave].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
                     ec_writestate(slave);
                  }
                  else if(ec_slave[slave].state == EC_STATE_SAFE_OP)
                  {
                     printf("WARNING : slave %d is in SAFE_OP, change to OPERATIONAL.\n", slave);
                     ec_slave[slave].state = EC_STATE_OPERATIONAL;
                     ec_writestate(slave);
                  }
                  else if(ec_slave[slave].state > 0)
                  {
                     if (ec_reconfig_slave(slave, EC_TIMEOUTMON))
                     {
                        ec_slave[slave].islost = FALSE;
                        printf("MESSAGE : slave %d reconfigured\n",slave);
                     }
                  }
                  else if(!ec_slave[slave].islost)
                  {
                     /* re-check state */
                     ec_statecheck(slave, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
                     if (!ec_slave[slave].state)
                     {
                        ec_slave[slave].islost = TRUE;
                        printf("ERROR : slave %d lost\n",slave);
                     }
                  }
               }
               if (ec_slave[slave].islost)
               {
                  if(!ec_slave[slave].state)
                  {
                     if (ec_recover_slave(slave, EC_TIMEOUTMON))
                     {
                        ec_slave[slave].islost = FALSE;
                        printf("MESSAGE : slave %d recovered\n",slave);
                     }
                  }
                  else
                  {
                     ec_slave[slave].islost = FALSE;
                     printf("MESSAGE : slave %d found\n",slave);
                  }
               }
            }
            if(!ec_group[currentgroup].docheckstate)
               printf(".");
        }
        
}


