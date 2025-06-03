#include "definitions.h"
	
int main(int argc, char* argv[])
{
	//char delimiter,isdelim,wasdelim=1,pattern[_MAXPATSIZE_][_MAXPATLEN_];
	
	movingbuffer mb;
	movingbuffer *mbp=&mb;
	patternset rot;
	patternset *psp=&rot;


	int i,j,retval=0;
	int index=0;
	
	const char *parname[]={"application name","input file name","output file name (ROT)","output file name (main)","Root of tree","log flag","verboze log flag"};
	
	print_header
	print_arguments
	check_for_input_file

	initpatternset(psp);
	initmovingbuffer(mbp);
	
	if(argc>3 && allocmovingbuffer(&mb,argv[1],argv[2],argv[3],_MAXMBUFLEN_,_MAXPATLEN_)>=0)
	{
		retval=3;
		printf("Input and output files opened successfully\n\r");
	}

	//getting and forming root vocab.
	if(argc>4)
	{
		printf("Opening root vocab. file");
		if(allocpatternset(&rot,argv[4],_MAXPATLEN_,_MAXPATSIZE_,parname[3])==NULL)
		{
			printf(" failed.\n\r");
			retval=6;
		}
		else
		{
			printf(" succeded.\n\r");
			retval=7;
			//read all patterns
			if(read_patterns(&rot)<0)
				retval=8;
			//add blank pattern
			//add_blank_to_patterns(&rot);
			//temp value
			mbp->maxpatternlength1=psp->maxpatternlength+1;
		}
	}

	//sort patterns by length - use indexing,
	if(retval==7)
	{
		sortpatternset(psp);
		retval=9;
	}


	if(retval==9)
	{
		mbp->movingbuffercounter=mbp->maxpatternlength1;
		mbp->movingbuffer[mbp->maxpatternlength1]=' ';
		for(i=1;i<mbp->maxpatternlength1;i++)
			propagatemovingbuffer(mbp,psp);
			//mbp->movingbuffer[i]=0;
		mbp->patternconglamerationcounter=0;
		while(!feof(mbp->finp) || mbp->movingbuffercounter>0)
		{
			propagatemovingbuffer(mbp,psp);
			//check for patterns and fast forwarding if found
			mbp->notfound=1;
			if(mbp->isdelim)
			{
				if(argc>6) //debug level # 2
				{
					printf("buffer: %s\n\r",mbp->movingbuffer);
				}
				for(i=0;i<psp->maxpatterns;i++)
				{
					//don't perform the check if the pattern is longer than rest of the buffer
					if(psp->patternlength[psp->patternind[i]]<mbp->movingbuffercounter)
					{
						strncpy(mbp->patternbuffer,mbp->movingbuffer,psp->patternlength[psp->patternind[i]]);
						//check that the word is followed by delimiter: <SPACE>
						if(isdelimiter(mbp->movingbuffer[psp->patternlength[psp->patternind[i]]]))
						{
							mbp->patternbuffer[psp->patternlength[psp->patternind[i]]]=0;
							if(argc>6) //debug level # 2
								printf("subbuffer: %s, delimiter: %c, pattern: %s\n\r",mbp->patternbuffer,mbp->movingbuffer[psp->patternlength[psp->patternind[i]]],psp->pattern[psp->patternind[i]]);
							if(0==strcmp(mbp->patternbuffer,psp->pattern[psp->patternind[i]])) //found pattern entry
							{
								mbp->notfound=0;
								//fast forward over the entry
								for(j=1;j<psp->patternlength[psp->patternind[i]];j++)
								{
									propagatemovingbuffer(mbp,psp);
								}
								//print ROT entry to out
								fprintf(mbp->foutp,"%s\n\r",psp->pattern[psp->patternind[i]]);
								if(argc>5)// debug level # 1
								{
									printf("%s\n\r",psp->pattern[psp->patternind[i]]);
								}
								//print delimiter <new line> instead of ROT entry to out_main
								fprintf(mbp->foutp_main,"\n\r");
							}
						}
					}
				}
			}
			if(mbp->notfound)
			{
				fprintf(mbp->foutp_main,"%c",mbp->movingbuffer[0]); //print part of not ROT entry.
				if(argc>5) //debug level # 3
					printf("found non-ROT entry: %c \n\r",mbp->movingbuffer[0]);
			}
		}
	}
	freepatternset(&rot);
	freemovingbuffer(&mb);
	printf("Stopped.\n\r");
	//printf("Hit any key.")
	//while(!kbhit());
	return retval;
}
