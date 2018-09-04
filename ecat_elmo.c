/** \file
 * \brief Example code for Simple Open EtherCAT master
 *
 * Usage : simple_test [ifname1]
 * ifname is NIC interface, f.e. eth0
 *
 * This is a minimal test.
 *
 * (c)Arthur Ketels 2010 - 2011
 */

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include <soem/ethercattype.h>
#include <soem/nicdrv.h>
#include <soem/ethercatbase.h>
#include <soem/ethercatmain.h>
#include <soem/ethercatdc.h>
#include <soem/ethercatcoe.h>
#include <soem/ethercatfoe.h>
#include <soem/ethercatconfig.h>
#include <soem/ethercatprint.h>

#include "ecat_ng.h"

#define EC_TIMEOUTMON 500
#define ERR 0

boolean printSDO = FALSE;
boolean printMAP = FALSE;

char IOmap[4096];
pthread_t thread1;
int expectedWKC;
boolean needlf;
volatile int wkc;
boolean inOP;
uint8 currentgroup = 0;

typedef struct PACKED
{
int tarPos;
int digOut;
short int controlWord;
}out_ELMO;

typedef struct PACKED
{
int tarPos;
int tarVel;
short int maxTrq;
short int controlWord;
}out_ELMO_PVT;

void set_output_int32 (uint16 slave_no, uint8 module_index, int32 value);

void simpletest(char *ifname)
{
    int i, j, oloop, iloop, wkc_count, chk;
    needlf = FALSE;
    inOP = FALSE;

   printf("Starting simple test\n");
   
   /* initialise SOEM, bind socket to ifname */
   if (ec_init(ifname))
   {   
      printf("ec_init on %s succeeded.\n",ifname);
      /* find and auto-config slaves */


       if ( ec_config_init(FALSE) > 0 )
      {
         printf("%d slaves found and configured.\n",ec_slavecount);
	 
	
         ec_config_map(&IOmap);

         ec_configdc();

printf("slave 1 full name is %s\n",ec_slave[1].name);
   /** state of slave */
   //uint16           state;
printf("state %i\n",ec_slave[1].state);
   /** AL status code */
   //uint16           ALstatuscode;
printf("ALstatuscode %i\n",ec_slave[1].ALstatuscode);
   /** Configured address */
   //uint16           configadr;
printf("configadr %i\n",ec_slave[1].configadr);
   /** Alias address */
   //uint16           aliasadr;
printf("aliasadr %i\n",ec_slave[1].aliasadr);
   /** Manufacturer from EEprom */
   //uint32           eep_man;
printf("eep_man %i\n",ec_slave[1].eep_man);
   /** ID from EEprom */
   //uint32           eep_id;
printf("eep_id %i\n",ec_slave[1].eep_id);
   /** revision from EEprom */
   //uint32           eep_rev;
printf("eep_rev %i\n",ec_slave[1].eep_rev);
   /** Interface type */
   //uint16           Itype;
printf("Itype %i\n",ec_slave[1].Itype);
   /** Device type */
   //uint16           Dtype;
printf("Dtype %i\n",ec_slave[1].Dtype);
   /** output bits */
   //uint16           Obits;
printf("Obits %i\n",ec_slave[1].Obits);
   /** output bytes, if Obits < 8 then Obytes = 0 */
   //uint32           Obytes;
printf("Obytes %i\n",ec_slave[1].Obytes);
   /** output pointer in IOmap buffer */
   //uint8            *outputs;
//printf("aliasadr %i\n",ec_slave[1].aliasadr);
   /** startbit in first output byte */
   //uint8            Ostartbit;
printf("Ostartbit %i\n",ec_slave[1].Ostartbit);
   /** input bits */
   //uint16           Ibits;
printf("Ibits %i\n",ec_slave[1].Ibits);
   /** input bytes, if Ibits < 8 then Ibytes = 0 */
   //uint32           Ibytes;
printf("Ibytes %i\n",ec_slave[1].Ibytes);
   /** input pointer in IOmap buffer */
   //uint8            *inputs;
   /** startbit in first input byte */
   //uint8            Istartbit;
   /** SM structure */
   //ec_smt           SM[EC_MAXSM];
   /** SM type 0=unused 1=MbxWr 2=MbxRd 3=Outputs 4=Inputs */
   //uint8            SMtype[EC_MAXSM];
   /** FMMU structure */
   //ec_fmmut         FMMU[EC_MAXFMMU];
   /** FMMU0 function */
   //uint8            FMMU0func;
   /** FMMU1 function */
   //uint8            FMMU1func;
   /** FMMU2 function */
   //uint8            FMMU2func;
   /** FMMU3 function */
   //uint8            FMMU3func;
   /** length of write mailbox in bytes, if no mailbox then 0 */
   //uint16           mbx_l;
   printf("mbx_l %i\n",ec_slave[1].mbx_l);
   /** mailbox write offset */
   //uint16           mbx_wo;
   printf("mbx_wo %i\n",ec_slave[1].mbx_wo);
   /** length of read mailbox in bytes */
   //uint16           mbx_rl;
   printf("mbx_rl %i\n",ec_slave[1].mbx_rl);
   /** mailbox read offset */
   //uint16           mbx_ro;
   printf("mbx_ro %i\n",ec_slave[1].mbx_ro);
   /** mailbox supported protocols */
   //uint16           mbx_proto;
   /** Counter value of mailbox link layer protocol 1..7 */
   //uint8            mbx_cnt;
   /** has DC capabillity */
   //boolean          hasdc;
   /** Physical type; Ebus, EtherNet combinations */
   //uint8            ptype;
   /** topology: 1 to 3 links */
   //uint8            topology;
   /** active ports bitmap : ....3210 , set if respective port is active **/
   //uint8            activeports;
   /** consumed ports bitmap : ....3210, used for internal delay measurement **/
   //uint8            consumedports;
   /** slave number for parent, 0=master */
   //uint16           parent;
   /** port number on parent this slave is connected to **/
   //uint8            parentport;
   printf("parentport %i\n",ec_slave[1].parentport);
   /** port number on this slave the parent is connected to **/
   //uint8            entryport;
   printf("entryport %i\n",ec_slave[1].entryport);
   /** DC receivetimes on port A */
   //int32            DCrtA;
   /** DC receivetimes on port B */
   //int32            DCrtB; 
   /** DC receivetimes on port C */
   //int32            DCrtC;
   /** DC receivetimes on port D */
   //int32            DCrtD;
   /** propagation delay */
   //int32            pdelay;
   /** next DC slave */
   //uint16           DCnext;
   /** previous DC slave */
   //uint16           DCprevious;
   /** DC cyle time in ns */
   //int32            DCcycle;
   /** DC shift from clock modulus boundary */
   //int32            DCshift;
   /** DC sync activation, 0=off, 1=on */
   //uint8            DCactive;
   /** link to config table */
   //uint16           configindex;
   /** link to SII config */
   //uint16           SIIindex;
   /** 1 = 8 bytes per read, 0 = 4 bytes per read */
   //uint8            eep_8byte;
   /** 0 = eeprom to master , 1 = eeprom to PDI */
   //uint8            eep_pdi;
   /** CoE details */
   //uint8            CoEdetails;
   printf("CoEdetails %i\n",ec_slave[1].CoEdetails);
   /** FoE details */
   //uint8            FoEdetails;
   /** EoE details */
   //uint8            EoEdetails;
   /** SoE details */
   //uint8            SoEdetails;
   /** E-bus current */
   //int16            Ebuscurrent;
   /** if >0 block use of LRW in processdata */
   //uint8            blockLRW;
   /** group */
   //uint8            group;
   /** first unused FMMU */
   //uint8            FMMUunused;
   /** TRUE is slave is not responding at all */
   //boolean          islost;
   printf("islost %i\n",ec_slave[1].islost);
   /** registered configuration function PO->SO */
   //int              (*PO2SOconfig)(uint16 slave);
   /** readable name */
   //char             name[EC_MAXNAME + 1];


	printf("Slaves mapped, state to SAFE_OP.\n");
         /* wait for all slaves to reach SAFE_OP state */
         ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);

         oloop = ec_slave[0].Obytes;
         if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;
         if (oloop > 8) oloop = 8;
         iloop = ec_slave[0].Ibytes;
         //if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
         //if (iloop > 8) iloop = 8;

         printf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

         printf("Request operational state for all slaves\n");
         expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
         printf("Calculated workcounter %d\n", expectedWKC);
         ec_slave[0].state = EC_STATE_OPERATIONAL;
         /* send one valid process data to make outputs in slaves happy*/
         ec_send_processdata();
         ec_receive_processdata(EC_TIMEOUTRET);
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

	    //set_output_int32 (1, 0x1607, 0x00000001);
         
            wkc_count = 0;
            inOP = TRUE;
                /* cyclic loop */
            for(i = 1; i <= 10; i++)
            {
               ec_send_processdata();
               wkc = ec_receive_processdata(EC_TIMEOUTRET);
		
                    if(wkc >= expectedWKC)
                    {
                        printf("Processdata cycle %4d, WKC %d ,", i, wkc);
                        *ec_slave[0].outputs = (i%256);
                        printf(" O:");
			for(j = 0 ; j < oloop; j++)
                        {
                            printf(" %2.2x", *(ec_slave[0].outputs + j));
                        }

                        printf(" I:");                  
                        for(j = 0 ; j < iloop; j++)
                        {
                            printf(" %2.2x", *(ec_slave[0].inputs + j));
                        }   
                        printf(" T:%lld\r",ec_DCtime);
                        needlf = TRUE;
                    }
                    usleep(100000);
                    
                }
                inOP = FALSE;
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
            ec_slave[0].state = EC_STATE_INIT;
            /* request INIT state for all slaves */
            ec_writestate(0);
        }
        else
        {
            printf("No slaves found!\n");
        }
	
	
    }
    else
    {
        printf("No socket connection on %s\nExcecute as root\n",ifname);
    }   
}   

void ecatcheck( void *ptr )
{
    int slave;

    while(1)
    {
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
               printf("OK : all slaves resumed OPERATIONAL.\n");
        }
        usleep(10000);
    }   
}  

uint32 network_configuration(char *ifname)
{

   if (!ec_init(ifname))
	return ERR;
   /* Do we have expected number of slaves from config */
   if (ec_slavecount < 1)
      return 0;

   if ( ec_config_init(FALSE) > 0 )
   {
      printf("%d slaves found and configured.\n",ec_slavecount);
   }

   /* Verify slave by slave that it is correct*/
 /*  if (strcmp(ec_slave[EK1100_1].name,"EK1100"))
      return 0;
   else if (strcmp(ec_slave[EL4001_1].name,"EL4001"))
      return 0;

   else if (strcmp(ec_slave[EL2622_4].name,"EL2622"))
      return 0;*/

  return 1;
}
 

void slaveinfo(char *ifname)
{
   int cnt, i, j, nSM;
    uint16 ssigen;
    int expectedWKC;

   printf("Starting slaveinfo\n");

   /* initialise SOEM, bind socket to ifname */
   if (ec_init(ifname))
   {
      printf("ec_init on %s succeeded.\n",ifname);
      /* find and auto-config slaves */
      if ( ec_config(FALSE, &IOmap) > 0 )
      {
         ec_configdc();
         while(EcatError) printf("%s", ec_elist2string());
         printf("%d slaves found and configured.\n",ec_slavecount);
         expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
         printf("Calculated workcounter %d\n", expectedWKC);
         /* wait for all slaves to reach SAFE_OP state */
         ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 3);
         if (ec_slave[0].state != EC_STATE_SAFE_OP )
         {
            printf("Not all slaves reached safe operational state.\n");
            ec_readstate();
            for(i = 1; i<=ec_slavecount ; i++)
            {
               if(ec_slave[i].state != EC_STATE_SAFE_OP)
               {
                  printf("Slave %d State=%2x StatusCode=%4x : %s\n",
                     i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
               }
            }
         }


         ec_readstate();
         for( cnt = 1 ; cnt <= ec_slavecount ; cnt++)
         {
            printf("\nSlave:%d\n Name:%s\n Output size: %dbits\n Input size: %dbits\n State: %d\n Delay: %d[ns]\n Has DC: %d\n",         
                  cnt, ec_slave[cnt].name, ec_slave[cnt].Obits, ec_slave[cnt].Ibits,                                                     
                  ec_slave[cnt].state, ec_slave[cnt].pdelay, ec_slave[cnt].hasdc);                                                       
 /*           if (ec_slave[cnt].hasdc) printf(" DCParentport:%d\n", ec_slave[cnt].parentport);                                             
            printf(" Activeports:%d.%d.%d.%d\n", (ec_slave[cnt].activeports & 0x01) > 0 ,                                                
                                         (ec_slave[cnt].activeports & 0x02) > 0 ,                                                        
                                         (ec_slave[cnt].activeports & 0x04) > 0 ,                                                        
                                         (ec_slave[cnt].activeports & 0x08) > 0 );                                                       
            printf(" Configured address: %4.4x\n", ec_slave[cnt].configadr);                                                             
            printf(" Man: %8.8x ID: %8.8x Rev: %8.8x\n", (int)ec_slave[cnt].eep_man, (int)ec_slave[cnt].eep_id, (int)ec_slave[cnt].eep_rev);
            for(nSM = 0 ; nSM < EC_MAXSM ; nSM++)                                                                                        
            {                                                                                                                            
               if(ec_slave[cnt].SM[nSM].StartAddr > 0)                                                                                   
                  printf(" SM%1d A:%4.4x L:%4d F:%8.8x Type:%d\n",nSM, ec_slave[cnt].SM[nSM].StartAddr, ec_slave[cnt].SM[nSM].SMlength,  
                         (int)ec_slave[cnt].SM[nSM].SMflags, ec_slave[cnt].SMtype[nSM]);                                                 
            }                                                                                                                            
            for(j = 0 ; j < ec_slave[cnt].FMMUunused ; j++)                                                                              
            {                                                                                                                            
               printf(" FMMU%1d Ls:%8.8x Ll:%4d Lsb:%d Leb:%d Ps:%4.4x Psb:%d Ty:%2.2x Act:%2.2x\n", j,
(int)ec_slave[cnt].FMMU[j].LogStart, ec_slave[cnt].FMMU[j].LogLength, ec_slave[cnt].FMMU[j].LogStartbit,          
                       ec_slave[cnt].FMMU[j].LogEndbit, ec_slave[cnt].FMMU[j].PhysStart, ec_slave[cnt].FMMU[j].PhysStartBit,             
                       ec_slave[cnt].FMMU[j].FMMUtype, ec_slave[cnt].FMMU[j].FMMUactive);                                                
            }                                                                                                                            
            printf(" FMMUfunc 0:%d 1:%d 2:%d 3:%d\n",                                                                                    
                     ec_slave[cnt].FMMU0func, ec_slave[cnt].FMMU2func, ec_slave[cnt].FMMU2func, ec_slave[cnt].FMMU3func);                
            printf(" MBX length wr: %d rd: %d MBX protocols : %2.2x\n", ec_slave[cnt].mbx_l, ec_slave[cnt].mbx_rl, ec_slave[cnt].mbx_proto);
            ssigen = ec_siifind(cnt, ECT_SII_GENERAL);                                                                                   
//             SII general section                                                                                                     
            if (ssigen)                                                                                                                  
            {                                                                                                                            
               ec_slave[cnt].CoEdetails = ec_siigetbyte(cnt, ssigen + 0x07);                                                             
               ec_slave[cnt].FoEdetails = ec_siigetbyte(cnt, ssigen + 0x08);                                                             
               ec_slave[cnt].EoEdetails = ec_siigetbyte(cnt, ssigen + 0x09);                                                             
               ec_slave[cnt].SoEdetails = ec_siigetbyte(cnt, ssigen + 0x0a);                                                             
               if((ec_siigetbyte(cnt, ssigen + 0x0d) & 0x02) > 0)                                                                        
               {                                                                                                                         
                  ec_slave[cnt].blockLRW = 1;                                                                                            
                  ec_slave[0].blockLRW++;                                                                                                
               }                                                                                                                         
               ec_slave[cnt].Ebuscurrent = ec_siigetbyte(cnt, ssigen + 0x0e);                                                            
               ec_slave[cnt].Ebuscurrent += ec_siigetbyte(cnt, ssigen + 0x0f) << 8;                                                      
               ec_slave[0].Ebuscurrent += ec_slave[cnt].Ebuscurrent;                                                                     
            }                                                                                                                            
            printf(" CoE details: %2.2x FoE details: %2.2x EoE details: %2.2x SoE details: %2.2x\n",                                     
                    ec_slave[cnt].CoEdetails, ec_slave[cnt].FoEdetails, ec_slave[cnt].EoEdetails, ec_slave[cnt].SoEdetails);             
            printf(" Ebus current: %d[mA]\n only LRD/LWR:%d\n",                                                                          
                    ec_slave[cnt].Ebuscurrent, ec_slave[cnt].blockLRW);                                                                  
            if ((ec_slave[cnt].mbx_proto & 0x04) && printSDO)                                                                            
                    si_sdo(cnt);                                                                                                         
                if(printMAP)                                                                                                             
            {                                                                                                                            
                    if (ec_slave[cnt].mbx_proto & 0x04)                                                                                  
                        si_map_sdo(cnt);                                                                                                 
                    else                                                                                                                 
                        si_map_sii(cnt);                                                                                                 
            } */                                                                                                                           
         }                                                                                                                               
      }                                                                                                                                 
      else                                                                                                                               
      {                                                                                                                                  
         printf("No slaves found!\n");                                                                                                   
      }                                                                                                                                  
      printf("End slaveinfo, close socket\n");                                                                                           
      /* stop SOEM, close socket */                                                                                                      
      ec_close();                                                                                                                        
   }                                                                                                                                     
   else                                                                                                                                  
   {                                                                                                                                     
      printf("No socket connection on %s\nExcecute as root\n",ifname);                                                                   
   }                                                                                                                                     
}                  

int main(int argc, char *argv[])
{
   int iret1;
   printf("SOEM (Simple Open EtherCAT Master)\nSimple test\n");

   init();
   configure();

	//network_configuration(DEVID);

//   if (argc > 1)
//   {      
      /* create thread to handle slave error handling in OP */
   	iret1 = pthread_create( &thread1, NULL, (void *) &ecatcheck, (void*) &ctime); 
	//slaveinfo(DEVID);  
      /* start cyclic part */
	simpletest(DEVID);
      //simpletest(argv[1]);//  }
//   else
//   {
//      printf("Usage: simple_test ifname1\nifname = eth0 for example\n");
//   }   
   
   printf("End program\n");
   return (0);
}

void set_output_int16 (uint16 slave_no, uint8 module_index, int16 value)
{
   uint8 *data_ptr;

   data_ptr = ec_slave[slave_no].outputs;
   /* Move pointer to correct module index*/
   data_ptr += module_index * 2;
   /* Read value byte by byte since all targets can't handle misaligned
    * addresses
    */
   *data_ptr++ = (value >> 0) & 0xFF;
   *data_ptr++ = (value >> 8) & 0xFF;
}

void set_output_int32 (uint16 slave_no, uint8 module_index, int32 value)
{
   uint8 *data_ptr;

   data_ptr = ec_slave[slave_no].outputs;
   /* Move pointer to correct module index*/
   data_ptr += module_index * 2;
   /* Read value byte by byte since all targets can't handle misaligned
    * addresses
    */
   *data_ptr++ = (value >> 0) & 0xFF;
   *data_ptr++ = (value >> 8) & 0xFF;
   *data_ptr++ = (value >> 16) & 0xFF;
   *data_ptr++ = (value >> 24) & 0xFF;
   
}
