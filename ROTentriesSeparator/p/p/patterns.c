#include "definitions.h"

int read_patterns(patternset *psp)
{
	int retval=0;
	psp->maxpatterns=0;
	psp->maxpatternlength=0;
	
	while(!feof(psp->fpat) && psp->maxpatterns<(psp->MAXPATSIZE-1) && retval<1)
	{
		fscanf(psp->fpat,"%s\n\r",psp->pattern[psp->maxpatterns]);
		psp->patternlength[psp->maxpatterns]=strlen(psp->pattern[psp->maxpatterns]);
		psp->maxpatternlength=psp->maxpatternlength>psp->patternlength[psp->maxpatterns]?psp->maxpatternlength:psp->patternlength[psp->maxpatterns];
		if(psp->MAXPATLEN< (psp->maxpatternlength+1))
		{
			printf("Pattern %s is too long. Program will stop.\n\r",psp->pattern[psp->maxpatterns]);
			retval=-1;
		}
		else
		{
			printf("pattern(%d) = %s, length = %d.\n\r",psp->maxpatterns,psp->pattern[psp->maxpatterns],psp->patternlength[psp->maxpatterns]);
			psp->maxpatterns++;
		}
	}
	return retval;
}

int initpatternset(patternset *psp)
{
	if(psp!=NULL)
	{
		psp->patternlength=NULL;
		psp->pattern=NULL;
		psp->patternind=NULL;
		psp->fpat=NULL;
		psp->MAXPATSIZE=0;
		psp->MAXPATLEN=0;
		return 0;
	}
	return -1;
}


FILE *allocpatternset(patternset *psp, char *filename, int MAXLEN, int MAXPAT,const char *patternsetname)
{
	int i,j;
	psp->patternsetname=patternsetname;
	psp->MAXPATLEN=MAXLEN;
	psp->MAXPATSIZE=MAXPAT;
	psp->maxpatternlength=0;//not init 
	psp->maxpatterns=0;
	psp->patternlength=NULL;
	psp->pattern=NULL;
	psp->patternind=NULL;
	psp->fpat=NULL;
			
	if(filename != NULL && psp->MAXPATLEN>0 && psp->MAXPATSIZE>0 && NULL!=(psp->fpat=fopen(filename,"r")))
	{
		psp->patternlength=(void *)malloc(sizeof(int)*psp->MAXPATSIZE);
		psp->patternind=(void *)malloc(sizeof(int)*psp->MAXPATSIZE);
		psp->pattern=(void *)malloc(sizeof(char*)*psp->MAXPATSIZE);
		for(i=0;i<psp->MAXPATSIZE;i++)
		{
			psp->pattern[i]=(void *)malloc(sizeof(char)*psp->MAXPATLEN);
			for(j=0;j<psp->MAXPATLEN;j++)
			{
				psp->pattern[i][j]=0;
			}
		}

	}

	return psp->fpat;
}

void freepatternset(patternset *psp)
{
	int i;
	if(psp!=NULL)
	{
		if(psp->fpat)
			fclose(psp->fpat);
		for(i=0;i<psp->MAXPATSIZE;i++)
		{
			if(NULL!=psp->pattern[i])
			{
				free(psp->pattern[i]);
				psp->pattern[i]=NULL;
			}
		}
		if(NULL!=psp->pattern)
		{
			free(psp->pattern);
			psp->pattern=NULL;
		}
		if(NULL!=psp->patternind)
		{
			free(psp->patternind);
			psp->patternind=NULL;
		}
		if(NULL!=psp->patternlength)
		{
			free(psp->patternlength);
			psp->patternlength=NULL;
		}

	}
}

/* <BLANK> Will not be part of the pattern set in ROT
void add_blank_to_patterns(patternset *psp)
{
	psp->pattern[psp->maxpatterns][0]=' ';
	psp->pattern[psp->maxpatterns][1]=0;
	psp->patternlength[psp->maxpatterns]=1;
	psp->maxpatterns++;
}
*/

int sortpatternset(patternset *psp)
{
	int	i,j,patterncounter=0;
	for(i=psp->maxpatternlength;i>=0;i--)
	{
		for(j=0;j<psp->maxpatterns;j++)
		{
			if(psp->patternlength[j]==i)
			{
				psp->patternind[patterncounter]=j;
				//printf("index = %d, pattern length = %d, pattern = %s\n\r",psp->patternind[patterncounter],psp->patternlength[psp->patternind[patterncounter]],psp->pattern[psp->patternind[patterncounter]]);
				patterncounter++;
			}
		}
	}
	if(psp->patternsetname!=NULL)
	{
		for(i=0;i<psp->maxpatterns;i++)
			printf("patternset = %s, index = %d, pattern length = %d, pattern = %s\n\r",psp->patternsetname,i,psp->patternlength[psp->patternind[i]],psp->pattern[psp->patternind[i]]);
	}
	return patterncounter;
}	
