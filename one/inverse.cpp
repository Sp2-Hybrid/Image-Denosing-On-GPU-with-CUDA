#include "inverse.h"

int inverse(float *a) {
	float A=a[0]*a[4]*a[8]+a[1]*a[5]*a[6]+a[2]*a[3]*a[7]-a[2]*a[4]*a[6]-a[1]*a[3]*a[8]-a[0]*a[5]*a[7];
	if(A==0)
		return 0;
	float b[9];

	b[0]=a[4]*a[8]-a[7]*a[5];
	b[1]=-1*(a[1]*a[8]-a[7]*a[2]);
	b[2]=a[1]*a[5]-a[2]*a[4];
	b[3]=a[5]*a[6]-a[8]*a[3];
	b[4]=-1*(a[2]*a[6]-a[8]*a[0]);
	b[5]=a[2]*a[3]-a[0]*a[5];
	b[6]=a[3]*a[7]-a[6]*a[4];
	b[7]=-1*(a[0]*a[7]-a[6]*a[1]);
	b[8]=a[0]*a[4]-a[1]*a[3];
	for(int i=0; i<9; i++) {
		b[i]=(float)b[i]/A;
		a[i]=b[i];
	}
	return 1;
}