#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{

	int index = 0;

	for( index = 0; index < 10; index++ )
	{
		/*malloc( 1000000 );*/
		malloc( atoi(argv[1]) );
		sleep(1);
	}

	return 0;
}
