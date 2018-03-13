#include <stdio.h>
#include <unistd.h>



int main(int argc, char * const argv[])
{
	int ret = 0; 
	while( (ret = getopt(argc, argv, "p:ab")) != -1 )
	{
		printf("opt = %c, optarg = %s, optind = %d, argv[%d] = %s\n", ret, optarg, optind, optind, argv[optind]);
		
		fflush(stdout);
		switch ( ret ){
		case 'a' : printf("is a opt\n"); break;
		case 'b' : printf("is b \n"); break;
		case 'p' : printf("print: %s\n", optarg); break;
		default : return -1; break;

		}
	}
	
	return 0;
}
