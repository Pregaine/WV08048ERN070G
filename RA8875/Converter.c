#include "ht32.h"
#include "Converter.h"

void SWAP_U16( u16 *data )
{
	unsigned short hi, lo;

	hi = ( ( *data ) & 0xFF00 ) >> 8;

	lo = ( ( *data ) & 0x00FF ) << 8;

	*data = hi | lo;

}

