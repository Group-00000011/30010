#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "sinlut.h"
#include "ansi.h"


#define winleft 0
#define winright 150
#define winup 0
#define windown 60

typedef struct {
	int32_t x,y;
} vector_t;

typedef struct {
	int32_t x,y;
	vector_t vel;
} ball_t;


void printFix(int32_t i);
int32_t expand(int32_t i);
int32_t calcSin(int32_t deg);
int32_t calcCos(int32_t deg);
void rotateVector(vector_t * vec, int32_t deg);
void drawhits(int32_t hits);
void initball(ball_t * ball, int32_t x, int32_t y, int32_t velx, int32_t vely);
void updateball(ball_t * ball, int8_t k);
void drawball(ball_t * ball, vector_t * last);
void checkcollision(ball_t * ball, int32_t * hits);




int main(void)
{
	int32_t hits = 0;
	uart_init( 9600 );
	clrscr();
	gotoxy(0,0);
	notitlewindow(winleft, winup, winright, windown, 1);
	notitlewindow(60, 29, 80, 32, 0);

	ball_t ball;
	vector_t lastxy;
	initball(&ball, 10 << 16, 10 << 16, 1 << 16, 1 << 16);

	drawhits(hits);



	while(1){
		drawball(&ball, &lastxy);
		drawhits(hits);

		checkcollision(&ball, &hits);

		updateball(&ball, 1);
	}
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


int32_t expand(int32_t i) {
	// Converts an 18.14 fixed point number to 16.16
	return i << 2;
}

int32_t calcSin(int32_t deg) {
	// Calculate sin(deg). Used LUT in sinlut.c. Takes in degrees in 512 instead of usual 360.
	// Number can be any positive int32, the function takes care of overflow.
	return SIN[deg % 512];
}

int32_t calcCos(int32_t deg) {
	// Same as calcSin() but cos(deg). No overflow needed since that is handled in calcSin().
	return calcSin(deg + 128);
}

void rotateVector(vector_t * vec, int32_t deg) {
	// Magic function, not 100% sure why but it works as intendend. Takes and returns 16.16 fixed point.
	int32_t tmpx = (((vec->x >> 2) * (calcCos(deg) >> 2)) >> 10) - (((vec->y >> 2) * (calcSin(deg) >> 2)) >> 10);
	vec->y = (((vec->x >> 2) * (calcSin(deg) >> 2)) >> 10) + (((vec->y >> 2) * (calcCos(deg) >> 2)) >> 10);
	vec->x = tmpx;
}

void drawhits(int32_t hits) {
	gotoxy(60,29);
	printf("%c", 218);
	for (int i = 0; i < 18; i++) {
		printf("%c", 196);
	}
	printf("%c", 191);
	movedown(1);
	moveleft(1);
	printf("%c", 179);
	movedown(1);
	moveleft(20);
	printf("%c", 192);
	for (int i = 0; i < 18; i++) {
		printf("%c", 196);
	}
	printf("%c", 217);
	gotoxy(60,30);
	printf("%c", 179);
	moveright(1);

	printf("Hits: %ld", hits);
}

void initball(ball_t * ball, int32_t x, int32_t y, int32_t velx, int32_t vely) {
	ball->x = x;
	ball->y = y;
	ball->vel.x = velx;
	ball->vel.y = vely;
}

void updateball(ball_t * ball, int8_t k) {
	int32_t tmpx = ball->x + ball->vel.x * k;
	ball->y = ball-> y + ball->vel.y * k;
	ball->x = tmpx;
}

void drawball(ball_t * ball, vector_t * last) {
	int16_t drawx, drawy;
	drawx = ball->x >> 16;
	drawy = ball->y >> 16;
	if (ball->x & 0x0080) {
		drawx++;
	}
	if (ball->y & 0x0080) {
		drawy++;
	}

	gotoxy(last->x, last->y);
	printf(" ");
	gotoxy(drawx, drawy);
	printf("0");

	last->x = drawx;
	last->y = drawy;
}

void checkcollision(ball_t * ball, int32_t * hits) {
	int8_t toadd = 0;
	if ((ball->x >> 16) < winleft + 3 || (ball->x >> 16) > winright - 2) {
		ball->vel.x *= -1;
		toadd++;
	}
	if ((ball->y >> 16) < winup + 3 || (ball->y >> 16) > windown - 2) {
		ball->vel.y *= -1;
		toadd += toadd ? 0 : 1;
	}
	*hits += toadd;

}



