#include "test.h"

float compute(float *X,float *X_t,float *W,float *y) {
	float WW[3*Ksize*Ksize];
	float XX[3*3];
	float YY[3*1];
	float rr[3*1];

	mult(WW,X_t,W,3,Ksize*Ksize,Ksize*Ksize,Ksize*Ksize);
	mult(XX,WW,X,3,Ksize*Ksize,Ksize*Ksize,3);
	mult(YY,WW,y,3,Ksize*Ksize,Ksize*Ksize,1);

	int in=inverse(XX);
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			printf("%f ",XX[i*3+j]);
		}
		printf("\n");
	}

	if(in==0)
		return (int )y[Ksize*Ksize/2];
	else {
		mult(rr,XX,YY,3,3,3,1);
		return (*rr);
	}
}