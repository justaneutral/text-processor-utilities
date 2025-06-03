
extern "C"
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>


#define version 20140719_P_Dourbal

#define maxparameters 5

#define _MAXPATSIZE_ 300
#define _MAXPATLEN_ 11



int main(int argc, char* argv[])
{
	char pattern[_MAXPATSIZE_][_MAXPATLEN_];
	int patternind[_MAXPATSIZE_];
	int maxpatterns=0;
	int maxpatternlength,maxpatternlength1;

	int i,j,k,retval=0;
	int patternlength[_MAXPATSIZE_],patterncounter,movingbuffercounter,index=0;
	char movingbuffer[_MAXPATLEN_],patternbuffer[_MAXPATLEN_];
	const char *parname[]={"application name","input file name","output file name","pattern set file name","log flag","verboze log flag"};
	FILE *finp = NULL;
	FILE *foutp = NULL;
	FILE *fpat = NULL;

	printf("Root entries extractor rstarted. %d parameters: \n\r", argc-1);

	if(argc>1)
	{
		for(i=1;i<(argc>=maxparameters?maxparameters:argc);i++)
		printf("%s = %s\n\r", parname[i],argv[i]);
	}
	
	if(argc<2)
	{
		printf("Need input file name - please specify\n\r");
		retval=1;
	}
	
	if(argc>1)
	{
		printf("Opening input file");
		finp=fopen(argv[1],"r");
		if(finp==NULL)
		{
			printf(" failed.\n\r");
			retval=2;
		}
		else
		{
			printf(" succeded.\n\r");
			retval=3;
		}
	}

	if(argc>2 && retval==3)
	{
		printf("Opening and emptying output file");
		foutp=fopen(argv[2],"w");
		if(!foutp)
		{
			printf(" failed.\n\r");
			retval=4;
		}
		else
		{
			printf(" succeded.\n\r");
			retval=5;
		}
	}

	//getting and forming root vocab.
	if(argc>3)
	{
		printf("Opening root vocab. file");
		fpat=fopen(argv[3],"r");
		if(fpat==NULL)
		{
			printf(" failed.\n\r");
			retval=6;
		}
		else
		{
			printf(" succeded.\n\r");
			retval=7;
			//read all patterns
			maxpatterns=0;
			maxpatternlength=0;
			while(!feof(fpat) && maxpatterns<(_MAXPATSIZE_-1) && retval<8)
			{
				fscanf(fpat,"%s\n\r",pattern[maxpatterns]);
				patternlength[maxpatterns]=strlen(pattern[maxpatterns]);
				maxpatternlength=maxpatternlength>patternlength[maxpatterns]?maxpatternlength:patternlength[maxpatterns];
				if(_MAXPATLEN_< (maxpatternlength+1))
				{
					printf("Pattern %s is too long. Program will stop.\n\r",pattern[maxpatterns]);
					retval=8;
				}
				else
				{
					printf("pattern(%d) = %s, length = %d.\n\r",maxpatterns,pattern[maxpatterns],patternlength[maxpatterns]);
					maxpatterns++;
				}
			}
			//add blank
			pattern[maxpatterns][0]=' ';
			pattern[maxpatterns][1]=0;
			patternlength[maxpatterns]=1;
			maxpatterns++;
			maxpatternlength1=maxpatternlength+1;
		}
	}

	//sort patterns by length - use indexing,
	if(retval==7)
	{
		patterncounter=0;
		for(i=maxpatternlength;i>=0;i--)
		{
			for(j=0;j<maxpatterns;j++)
			{
				if(patternlength[j]==i)
				{
					patternind[patterncounter]=j;
					//printf("index = %d, pattern length = %d, pattern = %s\n\r",patternind[patterncounter],patternlength[patternind[patterncounter]],pattern[patternind[patterncounter]]);
					patterncounter++;
				}
			}
		}

		for(i=0;i<maxpatterns;i++)
			printf("index = %d, pattern length = %d, pattern = %s\n\r",i,patternlength[patternind[i]],pattern[patternind[i]]);

		retval=9;
	}


	if(retval==9)
	{
		movingbuffercounter=maxpatternlength1;
	
		for(i=0;i<maxpatternlength1;i++)
			movingbuffer[i]=0;

		while(!feof(finp) || movingbuffercounter>0)
		{
			//propagate moving buffer
			movingbuffercounter--;
			for(i=1;i<maxpatternlength1;i++)
				movingbuffer[i-1]=movingbuffer[i];
			//get new symbol
			movingbuffercounter+=fread((void*)(&(movingbuffer[maxpatternlength-1])),sizeof(char),1,finp);
			movingbuffer[maxpatternlength]=0;
			//end propagate moving buffer
			if(argc>5)
				printf("buffer: %s\n\r",movingbuffer);
			//check for patterns and fast forwarding if found
			for(i=0;i<maxpatterns;i++)
			{
				//don't check if pattern is longer than rest of the buffer
				if(patternlength[patternind[i]]<=movingbuffercounter /*&& strlen(movingbuffer)>=patternlength[patternind[i]]*/)
				{
					strncpy(patternbuffer,movingbuffer,patternlength[patternind[i]]);
					patternbuffer[patternlength[patternind[i]]]=0;
					if(argc>5)
						printf("subbuffer: %s, pattern: %s\n\r",patternbuffer,pattern[patternind[i]]);
					if(0==strcmp(patternbuffer,pattern[patternind[i]])) //found pattern entry
					{
						//fast forward over the entry
						for(j=1;j<patternlength[patternind[i]];j++)
						{
							//propagate moving buffer
							movingbuffercounter--;
							for(k=1;k<maxpatternlength1;k++)
								movingbuffer[k-1]=movingbuffer[k];
							//get new symbol
							movingbuffercounter+=fread((void*)(&(movingbuffer[maxpatternlength-1])),sizeof(char),1,finp);
							movingbuffer[maxpatternlength]=0;
							//end propagate moving buffer
						}
						//write the found entry to the output file.
						if(patternind[i]>=(maxpatterns-1))
						{
							fprintf(foutp,"<BLANK>\n\r");
							if(argc>4)
								printf("found entry: <BLANK>.\n\r");
						}
						else
						{
							fprintf(foutp,"%s\n\r",pattern[patternind[i]]);
							if(argc>4)
								printf("found entry: %s.\n\r",pattern[patternind[i]]);
						}

					}
				}
			}
		}
	}

	if(finp)
	{
		fclose(finp);
		printf("Input file closed\n\r");
	}

	if(foutp)
	{
		fflush(foutp);
		printf("Output file flushed\n\r");
		fclose(foutp);
		printf("Output file closed\n\r");
	}

	printf("Stopped.\n\r");
	//printf("Hit any key.")
	//while(!kbhit());
	return retval;
}
