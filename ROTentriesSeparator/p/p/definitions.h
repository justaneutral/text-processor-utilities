#ifndef _DEFINITIONS_
#define _DEFINITIONS_

#define _CRT_SECURE_NO_WARNINGS

#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <fcntl.h>


#define version 20140808_P_Dourbal

#define maxparameters 5

#define _MAXPATSIZE_ 300
#define _MAXPATLEN_ 11
#define _MAXMBUFLEN_ 12

#define print_header {printf("Root entries extractor started. %d parameters: \n\r", argc-1);}
#define print_arguments {if(argc>1){for(i=1;i<(argc>=maxparameters?maxparameters:argc);i++) printf("%s = %s\n\r", parname[i],argv[i]);}}
#define check_for_input_file {if(argc<2){printf("Need input file name - please specify\n\r");retval=1;}}

typedef struct patternsetstr
{
	char *patternsetname;
	int maxpatterns;
	int MAXPATSIZE;
	int MAXPATLEN;
	int maxpatternlength;
	FILE *fpat;
	char **pattern;
	int *patternlength;
	int *patternind;

} patternset;

typedef struct movingbufferstr
{
	int maxmbuflen;
	int maxpatlen;
	int movingbuffercounter;
	int maxpatternlength1;
	char *movingbuffer;
	int patternconglamerationcounter;
	FILE *finp;
	char delimiter;
	char isdelim;
	char notfound;
	int debuglevel;
	char *patternbuffer;
	char wasdelim;
	FILE *foutp;
	FILE *foutp_main;
	int firstentrysavedindex;
} movingbuffer;


//prototypes
int main(int argc, char* argv[]);
char isdelimiter(char c);

#endif //_DEFINITIONS_
