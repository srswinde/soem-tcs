/*############################################################################
#  Title: Configure.c
#  Author: C. Johnson
#  Date: 6/2/14
#  Description: This is a configuration system.  '#' denotes a comment. 1
#  variable is configured per line
#
#############################################################################*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecat_ng.h"


#define KEYWORD 0
#define VALUE 1
#define VALUE2 2
#define MINFIELDS 2
#define MAXFIELDS 3


/*############################################################################
#  Title: init()
#  Author: C. Johnson
#  Date: 6/2/14
#  Args:  N/A
#  Description: Initializes the system to a basic set of data so it will run 
#
#############################################################################*/
void init()
{
sprintf(DEVID, "%s", "rt0");

}

/*############################################################################
#  Title: configure()
#  Author: C. Johnson
#  Date: 6/2/14
#  Args:  N/A
#  Description: Reads a config file named "ngindi.conf".  each line should
#  have a keword and a value, whitespace delimited.  ignores everything
#  on a line after the first '#'
#
#############################################################################*/
int configure()
{
FILE *fp;
char line[80];
char buff[80];
char *search = " \t";
char *token;
char words[MAXFIELDS][50];
fp=fopen("soem.conf", "r");
int wordctr=0, linectr=0;
char *ticky;

init(); //init with defaults

while( fgets(line,1024,fp) ) 
	{
	linectr++;
	if(line[strlen(line)-1]=='\n')
		line[strlen(line)-1]='\0';
	wordctr=0;
        //printf("Configure:  %s\n",line);
	ticky=strtok(line, search);
	if(ticky!=NULL)
		{
		if(ticky[0] == '#'){continue;}
		}    		
	while (ticky!= NULL)
		{
		if(ticky[0] == '#'){break;}
		//printf( "Word %i = %s\n", wordctr, ticky);
		sprintf(words[wordctr], "%s", ticky);
    		wordctr++;
		if (wordctr>=MAXFIELDS){break;}
		ticky=strtok(NULL, search);
		}
	//printf("Line %i: ", linectr);
	if (wordctr < MINFIELDS)
		{
		printf("invalid line: not enough fields -- \" %s \" -- ignoring\n", line);
		}
	else if (strcmp(words[KEYWORD], "DEVID") == 0) //device ID (eg. eth0, fxp0, en0)
		{
		printf("\tdevID = %s\n", words[VALUE]);	
		sprintf(DEVID, "%s", words[VALUE]);
		
		}
	
	else
		{
		//printf("unrecognized keyword: %s -- ignoring\n", words[KEYWORD]);
		}
	 
	}

//printf("Configuration Done!\n");



}

