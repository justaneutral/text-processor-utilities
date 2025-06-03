#include "definitions.h"

//extern "C"

char isdelimiter(char c)
{
	char retval=1;
	//if((('0'<=c)&&(c<='9'))||(('a'<=c)&&(c<='z'))||(('A'<=c)&&(c<='Z')))
	if(c!=' ')
	{
		retval=0;
	}
	return retval;
}
