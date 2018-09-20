#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <stdint.h> /*int32_t*/
#include <stdlib.h> /*int32_t*/
#include "ecat_ng.h"

#define VEL_MOD 0
#define POS_MOD 1
#define IFNAME "rt0"
#define ALT 0

#define OK 0
#define NOK 1


void usage(char *argv[])
{
	printf("Usage: %s <ifname> <testname> <arg1>\nLike: %s rt0 CJ 1\n", argv[0], argv[0]);
}



void test_constant_vel(char *ifname)
{
	int init_resp;
	const double ASECS2CNTS = pow(2, 26)/(360.0*3600);
	init_resp=initEcat(ifname, VEL_MOD);
	if( init_resp == NOK )
	{
		printf("initEcat returned NOK\n");
		return;
	}
	while (1)
	{
		ecatErr();
		commandVel( 100000 ); //Tracking Speed
		usleep(1000);
	}
}

void test_sin_pos(char *ifname)
{
	const double ASECS2CNTS = pow(2, 26)/(360.0*3600);
	int init_resp;
	init_resp = initEcat(ifname, POS_MOD );	

	if( init_resp == NOK )
	{
		printf("initEcat returned NOK\n");
		return;
	}

	int32_t zpos;
	zpos = ecat_getPosition(ALT)-500;
	int encpos;
	int out=0;
	float angle = 0.0;
	while (1)
	{
		zpos += (int) (1000*sin(angle));
		commandPos( zpos   );
		//zpos = ecat_getPosition(ALT);
		//commandPos( zpos );
		//out = ecat_getPosition( ALT );
		angle = angle+3.14159*0.001;
		usleep(500);
		ecat_debug();

	}
	ec_close();
}



void test_constant_pos(char *ifname)
{
	const double ASECS2CNTS = pow(2, 26)/(360.0*3600);
	int init_resp;
	init_resp = initEcat(ifname, POS_MOD);	
	if( init_resp == NOK )
	{
		printf("initEcat returned NOK\n");
		return;
	}

	int zpos;
	zpos = ecat_getPosition(ALT) + 500000;
	int encpos;
	int out=0;
	float angle = 0.0;
	ecatErr();
	while (1)
	{
		
		commandPos( zpos );//+ (int) 3*500000  );
		//commandPos( -38742744  );
		//out = ecat_getPosition( ALT );
		//angle = angle+3.14159*0.0001;
		

		usleep(1000);
	}
}

void test_increment_pos(char *ifname)
{
	const double ASECS2CNTS = pow(2, 26)/(360.0*3600);
	int init_resp;
	init_resp = initEcat(ifname, POS_MOD);	

	if( init_resp == NOK )
	{
		printf("initEcat returned NOK\n");
		return;
	}

	int zpos;
	zpos = ecat_getPosition(ALT) - 500;
	int encpos;
	int out=0;
	float angle = 0.0;
	while (1)
	{
		
		//zpos = ecat_getPosition( ALT );
		zpos -= 500;
		commandPos( zpos );
		//commandPos( -38742744  );
		//encpos = ecat_getPosition( ALT );
		//printf("Encoder Position %d\n", encpos);
		//angle = angle+3.14159*0.0001;
		

		usleep(500);
	}
}
void test_init(char *ifname)
{
	int init_resp;
	init_resp = initEcat(ifname, POS_MOD);//, ALT);	

	if( init_resp == NOK )
	{
		printf("initEcat returned NOK\n");
		return;
	}
	while (1)
	{
		//commandRead();
		usleep(700);
	}

}

void test_test()
{
	printf("IT WORKED\n");
}

void test_angular_pos(char *ifname, char *pos)
{
	const double ASECS2CNTS = pow(2, 26)/(360.0*3600);
	int init_resp;
	init_resp = initEcat(ifname, POS_MOD);	
	if( init_resp == NOK )
	{
		printf("initEcat returned NOK\n");
		return;
	}
	
	ecatErr();
	//printf("incoming= %s\n", pos);
	double fpos;
        fpos = atof(pos);
	//printf("converted to float %f\n", fpos);
	fpos *= (double)COUNTS_PER_DEGREE;
	//printf("converted to counts %f\n", fpos);
	int ipos = (int)fpos;
	int zpos;
	zpos = ipos;//ecat_getPosition(ALT) + 500000;
	int encpos;
	int out=0;
	float angle = 0.0;
	//printf("converted to int %i\n", ipos);
	//exit(0);
	while (1)
	{
		
		commandPos( zpos );//+ (int) 3*500000  );
		//commandPos( -38742744  );
		//out = ecat_getPosition( ALT );
		//angle = angle+3.14159*0.0001;
		

		usleep(1000);
	}
}


/*############################################################################
#  Title: main
#  Author: C. Johnson
#  Date: 9/2/18
#  Args:  N/A
#  Description: DUH!!!
#
#############################################################################*/
int main(int argc, char *argv[])
{
   int mode;
    int iret1,i;
   printf("SOEM (Simple Open EtherCAT Master)\nSimple test\n");

   if (argc > 2)
   {
	

		if( strcmp( argv[2], "CONS_VEL" ) == 0 )
			test_constant_vel( argv[1] ); 

		else if( strcmp( argv[2], "SIN_POS" ) == 0 )
			test_sin_pos(  argv[1] );
		else if( strcmp( argv[2], "TEST" ) == 0 )
			test_test(  argv[1] );

		else if( strcmp( argv[2], "INIT" ) == 0 )
			test_init(  argv[1] );

		else if( strcmp( argv[2], "CONS_POS" ) == 0 )
			test_constant_pos(  argv[1] );
		
		else if( strcmp( argv[2], "INC_POS" ) == 0 )
			test_increment_pos(  argv[1] );
		
		else if( strcmp( argv[2], "ANG_POS" ) == 0 )
			test_angular_pos(  argv[1], argv[3] );

		else
			usage(argv);


   }	
   else
   {
	   usage(argv);
   }

   printf("End program\n");
   return (0);
}

