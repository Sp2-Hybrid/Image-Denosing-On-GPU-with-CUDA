#include "mult.h"

void mult(float *c,float *a,float *b,int a_r,int a_c,int b_r,int b_c) {
	float temp=0;
	int ans=0;
	for(int i=0; i<a_r; i++) {
		for(int j=0; j<b_c; j++) {
			for(int k=0; k<a_c; k++) {
				temp+=a[i*a_c+k]*b[k*b_c+j];
			}
			c[ans]=temp;
			ans++;
			temp=0;
		}
	}
}