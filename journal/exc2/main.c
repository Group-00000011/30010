#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "LUTsin.h" // Precalced sin

// structs
typedef struct {
	int32_t x, y;
} vector_t;

// functions
void clrscr();
int32_t expand(int32_t i);
void printFix(int32_t i);
int32_t calcSin(int32_t i);
int32_t calcCos(int32_t i);
void initVector(vector_t *v, int32_t _x, int32_t _y);
void rotateVector(vector_t *v, int32_t i);


int main(void)
{
	uart_init(9600);

	clrscr();
	vector_t vec;

	initVector(&vec,-4,2);

	rotateVector(&vec, -50);

	printFix(expand(vec.x));
	printFix(expand(vec.y));

	printf("\n done.");

	while (1) { }
}

void clrscr(){
printf("%c[2J", 0x1B);
printf("%c[H", 0x1B);
}

void initVector(vector_t * v, int32_t _x, int32_t _y) {
	(*v).x = _x;
	(*v).y = _y;
}

void rotateVector(vector_t *v, int32_t angle){
    int32_t xt=(*v).x, yt=(*v).y;

	(*v).x = (xt*calcCos(angle)) - (yt*calcSin(angle));

	(*v).y = xt*calcSin(angle) + yt*calcCos(angle);
}

int32_t expand(int32_t i) {
	// Converts an 18.14 fixed point number to 16.16
	return i << 2;
}

int32_t contract(int32_t i) {
	// Converts an 16.16 fixed point number to 18.14
	return i >> 2;
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
	i+=128;
	i &= 0b1000000111111111;
	if(i < 0) i += 512;
	i &= 0b0111111111111111;
	return SIN[i];
}

int32_t calcSin(int32_t i){
	i &= 0b1000000111111111;
	if(i < 0) i += 512;
	i &= 0b0111111111111111;
	return SIN[i];
}
