#include <common.h>

double makeSmall(u32int InBytes, char *out)
{
	double Bytes = InBytes;
	if(Bytes < 2048){ *out = 'B'; return Bytes; }
	if(Bytes < 2048 * 1024){ *out = 'K'; return Bytes/1024; }
	*out = 'M'; return ((Bytes/1024)/1024);
}