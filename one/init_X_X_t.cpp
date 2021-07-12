#include "test.h"
#include "init_X_X_t.h"

void init_X(float *X) {
	int cen=Ksize/2;
	int offset=0;
	for(int i=0; i<Ksize; i++) {
		for(int j=0; j<Ksize; j++) {
			X[offset*3+0]=1;
			X[offset*3+1]=(float)(i-cen);
			X[offset*3+2]=(float)(j-cen);
			offset=offset+1;
		}
	}
}

void init_X_t(float *X_t) {
	int offset=0;
	int cen=Ksize/2;
	for(int i=0; i<Ksize; i++) {
		for(int j=0; j<Ksize; j++) {
			X_t[offset+0]=1;
			X_t[offset+Ksize*Ksize]=(i-cen);
			X_t[offset+2*Ksize*Ksize]=(j-cen);
			offset=offset+1;
		}
	}
}