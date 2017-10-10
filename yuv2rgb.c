#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX_BUF_SIZE (176*144*2)
#define MAX_RGB_SIZE (176*144*3)
#define R 0
#define G 1
#define B 2
static unsigned char buf[MAX_BUF_SIZE];
static int rgbbuf[MAX_RGB_SIZE];
static unsigned char rgb2[MAX_RGB_SIZE];
struct RGB {
	int r;
	int g;
	int b;
};
static struct RGB yuv_to_rgb(unsigned char Y, unsigned char U, unsigned char V) {
	struct RGB rgb;
	rgb.r = (int) (Y+1.4075*(V-128));
	rgb.g = (int) (Y - 0.3455*(U-128) - 0.7169*(V-128));
	rgb.b = (int) (Y + 1.1779*(U-128));
	rgb.r = (rgb.r<0? 0: rgb.r>255?255: rgb.r);
	rgb.g = (rgb.g<0? 0: rgb.g>255?255: rgb.g);
	rgb.b = (rgb.b<0? 0: rgb.b>255?255: rgb.b);
	printf("rgb r = %d, g = %d, b = %d\n", rgb.r, rgb.g, rgb.b);
	return rgb;
}

static void yuv422_to_rgb24(unsigned char* buf, int len) {

	int i = 0;
	int j = 0;
	int width = 176;
	int height = 144;
	int lineWidth = 176*2;
	for (i=0; i<height; i++) {
	   int startU = i*lineWidth;
	   for (j=0; j<lineWidth; j+=4) {
		   int U = j+startU;
		   int Y1 = U+1;
		   int Y2 = U+3;
		   int V = U+2;
		   int index = (U>>1)*3;
		   struct RGB tmp = yuv_to_rgb(buf[Y1], buf[U], buf[V]);
		   rgbbuf[index+R] = tmp.r;
		   rgbbuf[index+G] = tmp.g;
		   rgbbuf[index+B] = tmp.b;
		   index += 3;
		   struct RGB tmp2 = yuv_to_rgb(buf[Y2], buf[U], buf[V]);
		   rgbbuf[index+R] = tmp2.r;
		   rgbbuf[index+G] = tmp2.g;
		   rgbbuf[index+B] = tmp2.b;
	   }
	}


	return;
}
int main() {
	int n;
	int fd = open("colorbar", O_RDWR, S_IRWXU);
	n = read(fd, buf, 50688);
	if (n < 0) {
		printf("read error\n");
	}

	yuv422_to_rgb24(buf, 50688);
	int fd2 = open("colorbar_rgb", O_RDWR|O_CREAT, S_IRWXU);
	int i;
	for (i =0; i<MAX_RGB_SIZE; i++) {
		rgb2[i] = (unsigned char)rgbbuf[i];
	}
	int size = write(fd2, rgb2, 50688);
	if (size < 0) {
		printf("write error\n");
	}
	return 0;
}

