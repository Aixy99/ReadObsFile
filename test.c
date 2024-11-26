#include "rinex.h"
#include "utility.h"

int main() { 
	char* file = "C:/Users/w2151/Documents/C project/data/savo0160.22o";
	readObsFile(file);

	//int sat = setSatNum("C01");
	//printf("%d\n", sat);
	return 0; 
}