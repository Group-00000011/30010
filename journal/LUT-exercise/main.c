#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "LUTsin.h" // Precalced sin
int32_t expand(int32_t i);
void printFix(int32_t i);
int32_t calcSin(int32_t i);

int main(void)
{
	uart_init(9600);
	printf("\n 1:");
	printFix(expand(calcSin(0)));
	printf("\n 2:");
	printFix(expand(calcSin(64))); //(45/360)*512
	printf("\n 3:");
	printFix(expand(calcSin(-111)));
	printf("\n 4:");
	printFix(expand(calcSin(923)));
	printf("\n done.");



	while (1) { }
}

int32_t expand(int32_t i) {
	// Converts an 18.14 fixed point number to 16.16
	return i << 2;


}

void printFix(int32_t i) {
	// Prints a signed 16.16 fixed point number
	if ((i & 0x80000000) != 0) { // Handle negative numbers
		printf("-");
		i = ~i + 1;
	}
	printf("%ld.%04ld", i >> 16, 10000 * (uint32_t)(i & 0xFFFF) >> 16);
	// Print a maximum of 4 decimal digits to avoid overflow
}

int32_t calcCos(int32_t i){
	i = calcSin(i+128);
	return SIN[i];
}

int32_t calcSin(int32_t i){
	i %= 512;
	if(i < 0) i += 512;
	return SIN[i];
}
