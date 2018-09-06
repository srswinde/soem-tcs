#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>


#define VEL_MOD 0
#define POS_MOD 1



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
	mode=atoi(argv[2]);
      /* create thread to handle slave error handling in OP */
      //iret1 = pthread_create( &thread1, NULL, (void *) &ecatcheck, (void*) &ctime);
      /* start cyclic part */
      
      initEcat(argv[1], mode);
	

	for(i = 1; i <= 100000; i++)
                {
		ecatErr();
		if(mode == VEL_MOD){commandVel(100000);}
		if(mode == POS_MOD){commandPos(28863716);}
		usleep(1000);

                }
      
   }
   else
   {
      printf("Usage: simple_test ifname1\nifname = eth0 for example\n");
   }

   printf("End program\n");
   return (0);
}
