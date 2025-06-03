#include "definitions.h"

int propagatemovingbuffer(movingbuffer *mbp, patternset *psp)
{
	int i;
	if(mbp!=NULL && psp!=NULL)
	{
			//propagate moving buffer and check if delimiter is behind the beginning of the buffer
			mbp->delimiter=mbp->movingbuffer[0];
			mbp->isdelim=isdelimiter(mbp->delimiter);
			mbp->movingbuffercounter--;
			for(i=1;i<=mbp->maxpatternlength1;i++)
				mbp->movingbuffer[i-1]=mbp->movingbuffer[i];
			//get new symbol
			mbp->movingbuffercounter+=fread((void*)(&(mbp->movingbuffer[psp->maxpatternlength])),sizeof(char),1,mbp->finp);
			//end propagate moving buffer
			return 0;
	}
	return -1;
}

int initmovingbuffer(movingbuffer *mbp)
{
	if(mbp!=NULL)
	{
		mbp->movingbuffer=NULL;
		mbp->patternbuffer=NULL;
		mbp->finp=NULL;
		mbp->foutp=NULL;
		mbp->foutp_main=NULL;
		return 0;
	}
	return -1;
}

int allocmovingbuffer(movingbuffer *mbp,char *inpfn,char *outpfn,char *outpfn_main,int maxmbuflen,int maxpatlen)
{
	int rtc=-5;
	mbp->maxmbuflen=maxmbuflen;
	mbp->maxpatlen=maxpatlen;
	mbp->movingbuffercounter=0;
	mbp->maxpatternlength1=0;
	mbp->patternconglamerationcounter=0;
	mbp->delimiter=0;
	mbp->isdelim=0;
	mbp->notfound=1;
	mbp->debuglevel=0;
	mbp->movingbuffer=NULL;
	mbp->patternbuffer=NULL;
	mbp->finp=NULL;
	mbp->foutp=NULL;
	mbp->foutp_main=NULL;


	if(mbp->maxmbuflen>0 && mbp->maxpatlen>0 && NULL!=(mbp->movingbuffer=(void *)malloc(sizeof(char)*mbp->maxmbuflen)))
	{
		rtc++;
	}
	if(mbp->movingbuffer && NULL!=(mbp->patternbuffer=(void *)malloc(sizeof(char)*mbp->maxpatlen)))
	{
		rtc++;
	}
	if(mbp->patternbuffer && (mbp->finp=fopen(inpfn,"r")))
	{
		rtc++;
	}
	if(mbp->finp && (mbp->foutp=fopen(outpfn,"w")))
	{
		rtc++;
	}
	if(mbp->foutp && (mbp->foutp_main=fopen(outpfn_main,"w")))
	{
		rtc++;
	}
	return rtc;
}


void freemovingbuffer(movingbuffer *mbp)
{
	if(mbp!=NULL)
	{
		if(mbp->movingbuffer!=NULL)
		{
			free(mbp->movingbuffer);
			mbp->movingbuffer=NULL;
		}
		if(mbp->patternbuffer!=NULL)
		{
			free(mbp->patternbuffer);
			mbp->patternbuffer=NULL;
		}
		if(mbp->foutp!=NULL)
		{
			fflush(mbp->foutp);
			fclose(mbp->foutp);
			mbp->foutp=NULL;
		}
		if(mbp->foutp_main!=NULL)
		{
			fflush(mbp->foutp_main);
			fclose(mbp->foutp_main);
			mbp->foutp_main=NULL;
		}
		if(mbp->finp!=NULL)
		{
			fclose(mbp->finp);
			mbp->finp=NULL;
		}

	}
}
