extern "C"
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>

#define _PDDBG_

#define maxparameters 4
#define maxpatternlength 80
#define maxpatternlength1 81

int main(int argc, char* argv[])
{
	int i,retval=0;
	int patternlength,patterncounter,movingbuffercounter,movingbuffercounter_prev,charcounter,maxind,index=0;
	char buffer,pat[2],pattern[maxpatternlength1],movingbuffer[maxpatternlength1];
	const char *parname[]={"application name","input file name","output file name","pattern"};
	FILE *finp = NULL;
	FILE *foutp = NULL;
	printf("Pattern replacer started. %d parameters: \n\r", argc-1);
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
			charcounter=0;
			maxind=index;
			while(!feof(finp))
			{
				fread((void*)(&buffer),sizeof(char),1,finp);
				maxind=maxind>(buffer+1)?maxind:(buffer+1);
				charcounter++;
			}
			if(-1L==fseek(finp,0L,SEEK_SET))
			{
				printf("Input file _lseek to beginning failed.\n\r" );
				retval=4;
			}
			else
			{
				printf( "Number of characters in the input file is %d.\n\rIndex can't be lower then %d.\n\r",charcounter,maxind);
				retval=5;
			}
		}
	}

	if(argc>2 && retval==5)
	{
		printf("Opening and emptying output file");
		foutp=fopen(argv[2],"w");
		if(!foutp)
		{
			printf(" failed.\n\r");
			retval=6;
		}
		else
		{
			printf(" succeded.\n\r");
			retval=7;
		}
	}

	if(argc>3 && retval==7)
	{
		//pattern sits in argv[3] as pairs of hexadecimal charcters representing 1 symbol of the pattern each
		if(maxpatternlength<(patternlength=strlen(argv[3])/2))
		{
			printf("Pattern too long - will be truncated from %d to %d.\n\r",patternlength,maxpatternlength);
			patternlength=maxpatternlength;
		}
		else
			printf("Pattern length is %d.\n\r",patternlength);

		printf("Decoded pattern = < ");
		for(i=0;i<patternlength;i++)
		{
			pat[0]=argv[3][2*i]<='9'?argv[3][2*i]-'0':argv[3][2*i]-'a'+10;
			pat[1]=argv[3][2*i+1]<='9'?argv[3][2*i+1]-'0':argv[3][2*i+1]-'a'+10;
			pattern[i]=(pat[0]<<4)|pat[1];
			printf("%d%d%c ", pat[0], pat[1], pattern[i]);
		}
		printf(">\n\r");
		pattern[i]=0;
		printf("Decoded pattern = <<<<%s>>>>, pattern length = %d\n\r",pattern,sizeof(pattern));
		retval=8;
	}

	
	if(argc>4 && retval==8)
	{
		sscanf(argv[4],"%d",&index);
		if(maxind>index)
		{
			printf("Pattern index had to be changed from %d to %d.\n\r",index,maxind);
			index=maxind;
		}
		printf("Pattern entries will be raplaced with the index: %d.\n\r",index);
		retval=9;
	}

	if(retval==9)
	{
		//initially populate moving buffer
		patterncounter=0;
		movingbuffercounter=fread((void*)movingbuffer,sizeof(char),patternlength,finp);
		movingbuffer[patternlength]=0;
		if(movingbuffercounter>=patternlength)
		{
			while(!feof(finp))
			{
#ifdef _PDDBG_
				printf("%d %s ",movingbuffercounter,movingbuffer);
#endif //_PDDBG_
				if(0==strcmp(movingbuffer,pattern)) //found pattern entry
				{
					patterncounter++;
					//place index
					fwrite((void*)(&index),sizeof(char),1,foutp);
					//repopulate buffer
					movingbuffercounter_prev=movingbuffercounter;
					movingbuffercounter+=(1+fread((void*)movingbuffer,sizeof(char),patternlength,finp));
					movingbuffer[patternlength]=0;
					printf(" entry # %d was replaced with index %d.\n\r",patterncounter,index);
				}
				else //not found
				{
					//store the oldest part.
					fwrite((void*)(&(movingbuffer[0])),sizeof(char),1,foutp);
					//shift buffer
					for(i=1;i<patternlength;i++)
						movingbuffer[i-1]=movingbuffer[i];
					//get new symbol
					movingbuffercounter_prev=movingbuffercounter;
					movingbuffercounter+=fread((void*)(&(movingbuffer[patternlength-1])),sizeof(char),1,finp);
					movingbuffer[patternlength]=0;
				}
#ifdef _PDDBG_
				printf("\n\r");
#endif //_PDDBG_
			}
		}
		//completing buffer processing
		//printf("%d %s ",movingbuffercounter,movingbuffer);
		if((movingbuffercounter-movingbuffercounter_prev>=patternlength) && 0==strcmp(movingbuffer,pattern)) //found pattern entry
		{
			patterncounter++;
			//place last index
			fwrite((void*)(&index),sizeof(char),1,foutp);
			printf(" entry # %d was replaced with index %d.\n\r",patterncounter,index);
		}
		else //not found
		{
			//store the buffer.
			fwrite((void*)movingbuffer,sizeof(char),movingbuffercounter-movingbuffercounter_prev+1,foutp);
		}
		printf("\n\r");
		printf("Found & replaced %d pattern entries from %d bytes of text.\n\r",patterncounter,movingbuffercounter);
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

	printf("Pattern replacer stopped.\n\r");
	//printf("Hit any key.")
	//while(!kbhit());
	return retval;
}
