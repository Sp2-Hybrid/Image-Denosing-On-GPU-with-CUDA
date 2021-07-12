#include "test.h"
#include "book.h"
#include "cuda_runtime.h"
//均值滤波GPU**************************************************************************************//
__global__ void ave_process(unsigned char* dev_original,unsigned char *dev_out,int R,int C){
	int x=threadIdx.x;
	int y=blockIdx.x;
	int offset=x+y*blockDim.x;
	
	while(offset<R*C){
		int dev_y[NKsize*NKsize];
		int gx=offset/C;
		int gy=offset%C;
		int sum=0;
		int fx=gx-NKsize/2;
		int fy=gy-NKsize/2;

		for(int i=0;i<NKsize;i++){
			for(int j=0;j<NKsize;j++){
				if((fx+i)<0||(fy+j)<0||(fx+i)>=R||(fy+j)>=C){
					dev_y[i*NKsize+j]=dev_original[offset];
				}
				else{
					dev_y[i*NKsize+j]=dev_original[(fx+i)*C+(fy+j)];
				}
				sum+=dev_y[i*NKsize+j];
			}
		}
		
		dev_out[offset]=sum/(NKsize*NKsize);
//		dev_out[offset]=dev_original[offset];

		offset+=blockDim.x*gridDim.x;
	}
}
//中值滤波GPU**************************************************************************************//
__global__ void  mid_process(unsigned char *dev_original,unsigned char *dev_out,int R,int C){
	int x=threadIdx.x;
	int y=blockIdx.x;
	int offset=x+y*blockDim.x;

	while(offset<R*C){
		int dev_y[NKsize*NKsize];
		int gx=offset/C;
		int gy=offset%C;
		int fx=gx-NKsize/2;
		int fy=gy-NKsize/2;
		for(int i=0;i<NKsize;i++){
			for(int j=0;j<NKsize;j++){
				if((fx+i)<0||(fy+j)<0||(fx+i)>=R||(fy+j)>=C){
					dev_y[i*NKsize+j]=dev_original[offset];
				}
				else{
					dev_y[i*NKsize+j]=dev_original[(fx+i)*C+(fy+j)];
				}
			}
		}
		int temp;
		for(int i=0;i<NKsize*NKsize-1;i++){
			for(int j=0;j<NKsize*NKsize-1-i;j++){
				if(dev_y[j]>dev_y[j+1]){
					temp=dev_y[j];
					dev_y[j]=dev_y[j+1];
					dev_y[j+1]=temp;
				}
			}
		}
		dev_out[offset]=dev_y[NKsize*NKsize/2];
		offset+=blockDim.x*gridDim.x;
	}
}
//经典核回归**************************************************************************************//
float Kcal(int x1,int y1,int x0,int y0) {
	float distance=(x1-x0)*(x1-x0)+(y1-y0)*(y1-y0);
	float h=1;
	float t=distance/h;
	float xishu=1/((2*pai));

//	float zhishu=(-0.5)*((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
	float r=xishu*exp(-0.5*t*t);
	return r;
}
void init_Wcal(float *W) {
	int offset;
	int x0=Ksize/2;
	int y0=Ksize/2;

	for(int i=0; i<Ksize*Ksize; i++) {
		for(int j=0; j<Ksize*Ksize; j++) {
			if(i==j) {
				int fx=i/Ksize;
				int fy=i%Ksize;
				W[i*Ksize*Ksize+j]=Kcal(fx,fy,x0,y0);
			} else {
				W[i*Ksize*Ksize+j]=0;
			}

		}
	}
}
//GPU中的矩阵乘法运算
__device__ void Mult(float *c,float* a,float *b,int a_r,int a_c,int b_r,int b_c){
	for(int i=0;i<a_r;i++){
		for(int j=0;j<b_c;j++){
			c[i*b_c+j]=0;
			for(int k=0;k<a_c;k++){
				c[i*b_c+j]+=a[i*a_c+k]*b[k*b_c+j];
			}
		}
	}
}
__global__ void cla_process(unsigned char *dev_original,unsigned char *dev_out,float *dev_mid,int R,int C){
	int x=blockIdx.x;
	int y=threadIdx.x;
	int offset=y+x*blockDim.x;
	while(offset<R*C){
	float dev_y[Ksize*Ksize];
	float dev_res[3*1];
	int fx=offset/C-Ksize/2;
	int fy=offset%C-Ksize/2;

	for(int i=0;i<Ksize;i++){
		for(int j=0;j<Ksize;j++){
			if((fx+i)<0||(fy+j)<0||(fx+i)>=R||(fy+j)>=C)
				dev_y[i*Ksize+j]=dev_original[offset];
			else
				dev_y[i*Ksize+j]=dev_original[(fx+i)*C+(fy+j)];
		}
	}
	Mult(dev_res,dev_mid,dev_y,3,Ksize*Ksize,Ksize*Ksize,1);
	float value=dev_res[0];
//	value/=1.3;
	int grayvalue=(int )value;
	if(grayvalue>255){
		grayvalue=255;
	}
	else if(grayvalue<0){
		grayvalue=0;
	}

	dev_out[offset]=(unsigned char)grayvalue;
	offset=offset+blockDim.x*gridDim.x;
	}
}
//自适应核回归**************************************************************************************//
__device__ float Kself(int x1,int y1,int x0,int y0,float *C){

	float h=4;
	float distance=(x1-x0)*(x1-x0)*C[0]+(x1-x0)*(y1-y0)*C[2]+(x1-x0)*(y1-y0)*C[1]+(y1-y0)*(y1-y0)*C[3];
	float xishu=sqrt(C[0]*C[3]-C[1]*C[2])/(2*pai*h*h);
	float r=xishu*exp(-0.5*distance/(h*h));
	return r;
}

//初始化W矩阵  (Ksize2 x Ksize2)
__device__ void init_Wself(float* W,float *C){
	int offset;
	int x0=Ksize/2;
	int y0=Ksize/2;

	for(int i=0;i<Ksize*Ksize;i++){
		for(int j=0;j<Ksize*Ksize;j++){
			if(i==j){
				int fx=i/Ksize;
				int fy=i%Ksize;
				W[i*Ksize*Ksize+j]=Kself(fx,fy,x0,y0,C);
			}
			else{
				W[i*Ksize*Ksize+j]=0;
			}
		}
	}
}
//初始化梯度矩阵
__device__ void init_Gself(float *G,float *y){
	int fx=Ksize/2;
	int fy=Ksize/2;
	int num=0;
	for(int i=0;i<Ksize;i++){
		for(int j=0;j<Ksize;j++){
			if(i!=fx){
				G[num*2]=(y[i*Ksize+j]-y[fx*Ksize+fy])/(i-fx);
			}
			else{
				G[num*2]=0;
			}
			if(j!=fy){
				G[num*2+1]=(y[i*Ksize+j]-y[fx*Ksize+fy])/(j-fy);
			}
			else{
				G[num*2+1]=0;
			}
		/*	if(i+1>=Ksize){
				G[num*2]=0;
			}
			else{
				G[num*2]=(y[(i+1)*Ksize+j]-y[i*Ksize+j]);
			}
			if(j+1>=Ksize){
				G[num*2+1]=0;
			}
			else{
				G[num*2+1]=y[i*Ksize+j+1]-y[i*Ksize+j];
			}*/
			num++;
		}
	}
}
__device__ void pppself(float a[],float e[],float s[],float v[],int m,int n) {
	int i,j,p,q;
	double d;
	if(m>=n)
		i=n;
	else
		i=m;
	for(j=1; j<=i-1; j++) {
		a[(j-1)*n+j-1]=s[j-1];
		a[(j-1)*n+j]=e[j-1];
	}
	a[(i-1)*n+i-1]=s[i-1];
	if(m<n)
		a[(i-1)*n+i]=e[i-1];
	for(i=1; i<=n-1; i++)
		for(j=i+1; j<=n; j++) {
			p=(i-1)*n+j-1;
			q=(j-1)*n+i-1;
			d=v[p];
			v[p]=v[q];
			v[q]=d;
		}
	return;
}

__device__ void sssself(float fg[2],float cs[2]) {
	float r,d;
	//if((fabs(fg[0])+fabs(fg[1]))==0.0)
	if((fabs(fg[0])+fabs(fg[1]))<MIN_DOUBLE) {
		cs[0]=1.0;
		cs[1]=0.0;
		d=0.0;
	} else {
		d=sqrt(fg[0]*fg[0]+fg[1]*fg[1]);
		if(fabs(fg[0])>fabs(fg[1])) {
			d=fabs(d);
			if(fg[0]<0.0)
				d=-d;
		}
		if(fabs(fg[1])>=fabs(fg[0])) {
			d=fabs(d);
			if(fg[1]<0.0)
				d=-d;
		}
		cs[0]=fg[0]/d;
		cs[1]=fg[1]/d;
	}
	r=1.0;
	if(fabs(fg[0])>fabs(fg[1]))
		r=cs[1];
	else
		//if(cs[0]!=0.0)
		if(fabs(cs[0])>MIN_DOUBLE)
			r=1.0/cs[0];
	fg[0]=d;
	fg[1]=r;
	return;
}
//对矩阵进行奇异值分解
__device__ int dluavself(float a[],int m,int n,float u[],float v[],float eps,int ka){
	int i,j,k,l,it,ll,kk,ix,iy,mm,nn,iz,ml,ks;
	float d,dd,t,sm,sml,eml,sk,ek,b,c,shh,fg[2],cs[2];

	float s[26];
	float w[26];
	float e[26];

	for(i=1; i<=m; i++) {
		ix=(i-1)*m+i-1;
		u[ix]=0;
	}
	for(i=1; i<=n; i++) {
		iy=(i-1)*n+i-1;
		v[iy]=0;
	}
	it=MAX_ITERA;
	k=n;
	if(m-1<n)
		k=m-1;
	l=m;
	if(n-2<m) l=n-2;
	if(l<0) l=0;
	ll=k;
	if(l>k) ll=l;
	if(ll>=1) {
		for(kk=1; kk<=ll; kk++) {
			if(kk<=k) {
				d=0.0;
				for(i=kk; i<=m; i++) {
					ix=(i-1)*n+kk-1;
					d=d+a[ix]*a[ix];
				}
				s[kk-1]=sqrt(d);
				//if(s[kk-1]!=0.0)
				if(fabs(s[kk-1])>MIN_DOUBLE) {
					ix=(kk-1)*n+kk-1;
					//if(a[ix]!=0.0)
					if(fabs(a[ix])>MIN_DOUBLE) {
						s[kk-1]=fabs(s[kk-1]);
						if(a[ix]<0.0) s[kk-1]=-s[kk-1];
					}
					for(i=kk; i<=m; i++) {
						iy=(i-1)*n+kk-1;
						a[iy]=a[iy]/s[kk-1];
					}
					a[ix]=1.0+a[ix];
				}
				s[kk-1]=-s[kk-1];
			}
			if(n>=kk+1) {
				for(j=kk+1; j<=n; j++) {
					//if((kk<=k)&&(s[kk-1]!=0.0))
					if((kk<=k)&&(fabs(s[kk-1])>MIN_DOUBLE)) {
						d=0.0;
						for(i=kk; i<=m; i++) {
							ix=(i-1)*n+kk-1;
							iy=(i-1)*n+j-1;
							d=d+a[ix]*a[iy];
						}
						d=-d/a[(kk-1)*n+kk-1];
						for(i=kk; i<=m; i++) {
							ix=(i-1)*n+j-1;
							iy=(i-1)*n+kk-1;
							a[ix]=a[ix]+d*a[iy];
						}
					}
					e[j-1]=a[(kk-1)*n+j-1];
				}
			}
			if(kk<=k) {
				for(i=kk; i<=m; i++) {
					ix=(i-1)*m+kk-1;
					iy=(i-1)*n+kk-1;
					u[ix]=a[iy];
				}
			}
			if(kk<=l) {
				d=0.0;
				for(i=kk+1; i<=n; i++)
					d=d+e[i-1]*e[i-1];
				e[kk-1]=sqrt(d);
				//if(e[kk-1]!=0.0)
				if(fabs(e[kk-1])>MIN_DOUBLE) {
					//if(e[kk]!=0.0)
					if(fabs(e[kk])>MIN_DOUBLE) {
						e[kk-1]=fabs(e[kk-1]);
						if(e[kk]<0.0)
							e[kk-1]=-e[kk-1];
					}
					for(i=kk+1; i<=n; i++)
						e[i-1]=e[i-1]/e[kk-1];
					e[kk]=1.0+e[kk];
				}
				e[kk-1]=-e[kk-1];
				//if((kk+1<=m)&&(e[kk-1]!=0.0))
				if((kk+1<=m)&&(fabs(e[kk-1])>MIN_DOUBLE)) {
					for(i=kk+1; i<=m; i++) w[i-1]=0.0;
					for(j=kk+1; j<=n; j++)
						for(i=kk+1; i<=m; i++)
							w[i-1]=w[i-1]+e[j-1]*a[(i-1)*n+j-1];
					for(j=kk+1; j<=n; j++)
						for(i=kk+1; i<=m; i++) {
							ix=(i-1)*n+j-1;
							a[ix]=a[ix]-w[i-1]*e[j-1]/e[kk];
						}
				}
				for(i=kk+1; i<=n; i++)
					v[(i-1)*n+kk-1]=e[i-1];
			}
		}
	}
	mm=n;
	if(m+1<n) mm=m+1;
	if(k<n) s[k]=a[k*n+k];
	if(m<mm) s[mm-1]=0.0;
	if(l+1<mm) e[l]=a[l*n+mm-1];
	e[mm-1]=0.0;
	nn=m;
	if(m>n) nn=n;
	if(nn>=k+1) {
		for(j=k+1; j<=nn; j++) {
			for(i=1; i<=m; i++)
				u[(i-1)*m+j-1]=0.0;
			u[(j-1)*m+j-1]=1.0;
		}
	}
	if(k>=1) { /////////////////////////////////
		for(ll=1; ll<=k; ll++) {
			kk=k-ll+1;
			iz=(kk-1)*m+kk-1;
			//if(s[kk-1]!=0.0)
			if(fabs(s[kk-1])>MIN_DOUBLE) {
				if(nn>=kk+1)
					for(j=kk+1; j<=nn; j++) {
						d=0.0;
						for(i=kk; i<=m; i++) {
							ix=(i-1)*m+kk-1;
							iy=(i-1)*m+j-1;
							d=d+u[ix]*u[iy]/u[iz];
						}
						d=-d;
						for(i=kk; i<=m; i++) {
							ix=(i-1)*m+j-1;
							iy=(i-1)*m+kk-1;
							u[ix]=u[ix]+d*u[iy];
						}
					}
				for(i=kk; i<=m; i++) {
					ix=(i-1)*m+kk-1;
					u[ix]=-u[ix];
				}
				u[iz]=1.0+u[iz];
				if(kk-1>=1)//////////////////////////////////////
					for(i=1; i<=kk-1; i++)
						u[(i-1)*m+kk-1]=0.0;
			} else {
				for(i=1; i<=m; i++)
					u[(i-1)*m+kk-1]=0.0;
				u[(kk-1)*m+kk-1]=1.0;
			}
		}
	}
	for(ll=1; ll<=n; ll++) {
		kk=n-ll+1;
		iz=kk*n+kk-1;
		//if((kk<=l)&&(e[kk-1]!=0.0))/////////////////////////////
		if((kk<=l)&&(fabs(e[kk-1])>MIN_DOUBLE)) {
			for(j=kk+1; j<=n; j++) {
				d=0.0;
				for(i=kk+1; i<=n; i++) {
					ix=(i-1)*n+kk-1;
					iy=(i-1)*n+j-1;
					d=d+v[ix]*v[iy]/v[iz];
				}
				d=-d;
				for(i=kk+1; i<=n; i++) {
					ix=(i-1)*n+j-1;
					iy=(i-1)*n+kk-1;
					v[ix]=v[ix]+d*v[iy];
				}
			}
		}
		for(i=1; i<=n; i++)
			v[(i-1)*n+kk-1]=0.0;
		v[iz-n]=1.0;
	}
	for(i=1; i<=m; i++)
		for(j=1; j<=n; j++)
			a[(i-1)*n+j-1]=0.0;
	ml=mm;
	it=MAX_ITERA;
	while(1==1) { //////////////////////////////////
		if(mm==0) {
			pppself(a,e,s,v,m,n);

//			free(s);free(e);free(w);
			return l;
		}
		if(it==0) {
			pppself(a,e,s,v,m,n);

//			free(s);free(e);free(w);
			return -1;
		}
		kk=mm-1;
		//while((kk!=0)&&(fabs(e[kk-1])!=0.0))
		while((kk!=0)&&(fabs(e[kk-1])>MIN_DOUBLE)) {
			d=fabs(s[kk-1])+fabs(s[kk]);
			dd=fabs(e[kk-1]);
			if(dd>eps*d)
				kk=kk-1;
			else
				e[kk-1]=0.0;
		}
		if(kk==mm-1) {
			kk=kk+1;
			if(s[kk-1]<0.0) {
				s[kk-1]=-s[kk-1];
				for(i=1; i<=n; i++) {
					ix=(i-1)*n+kk-1;
					v[ix]=-v[ix];
				}
			}
			while((kk!=ml)&&(s[kk-1]<s[kk])) {
				d=s[kk-1];
				s[kk-1]=s[kk];
				s[kk]=d;
				if(kk<n)
					for(i=1; i<=n; i++) {
						ix=(i-1)*n+kk-1;
						iy=(i-1)*n+kk;
						d=v[ix];
						v[ix]=v[iy];
						v[iy]=d;
					}
				if(kk<m)
					for(i=1; i<=m; i++) {
						ix=(i-1)*m+kk-1;
						iy=(i-1)*m+kk;
						d=u[ix];
						u[ix]=u[iy];
						u[iy]=d;
					}
				kk=kk+1;
			}
			it=MAX_ITERA;
			mm=mm-1;
		} else {
			ks=mm;
			//while((ks>kk)&&(fabs(s[ks-1])!=0.0))
			while((ks>kk)&&(fabs(s[ks-1])>MIN_DOUBLE)) {
				d=0.0;
				if(ks!=mm)
					d=d+fabs(e[ks-1]);
				if(ks!=kk+1) d=d+fabs(e[ks-2]);
				dd=fabs(s[ks-1]);
				if(dd>eps*d)
					ks=ks-1;
				else
					s[ks-1]=0.0;
			}
			if(ks==kk) {
				kk=kk+1;
				d=fabs(s[mm-1]);
				t=fabs(s[mm-2]);
				if(t>d)
					d=t;
				t=fabs(e[mm-2]);
				if(t>d)
					d=t;
				t=fabs(s[kk-1]);
				if(t>d)
					d=t;
				t=fabs(e[kk-1]);
				if(t>d)
					d=t;
				sm=s[mm-1]/d;
				sml=s[mm-2]/d;
				eml=e[mm-2]/d;
				sk=s[kk-1]/d;
				ek=e[kk-1]/d;
				b=((sml+sm)*(sml-sm)+eml*eml)/2.0;
				c=sm*eml;
				c=c*c;
				shh=0.0;
				//if((b!=0.0)||(c!=0.0))
				if((fabs(b)>MIN_DOUBLE)||(fabs(c)>MIN_DOUBLE)) {
					shh=sqrt(b*b+c);
					if(b<0.0)
						shh=-shh;
					shh=c/(b+shh);
				}
				fg[0]=(sk+sm)*(sk-sm)-shh;
				fg[1]=sk*ek;
				for(i=kk; i<=mm-1; i++) {
					sssself(fg,cs);
					if(i!=kk)
						e[i-2]=fg[0];
					fg[0]=cs[0]*s[i-1]+cs[1]*e[i-1];
					e[i-1]=cs[0]*e[i-1]-cs[1]*s[i-1];
					fg[1]=cs[1]*s[i];
					s[i]=cs[0]*s[i];
					//if((cs[0]!=1.0)||(cs[1]!=0.0))
					if((fabs(cs[0]-1.0)>MIN_DOUBLE)||(fabs(cs[1])>MIN_DOUBLE))
						for(j=1; j<=n; j++) {
							ix=(j-1)*n+i-1;
							iy=(j-1)*n+i;
							d=cs[0]*v[ix]+cs[1]*v[iy];
							v[iy]=-cs[1]*v[ix]+cs[0]*v[iy];
							v[ix]=d;
						}
					sssself(fg,cs);
					s[i-1]=fg[0];
					fg[0]=cs[0]*e[i-1]+cs[1]*s[i];
					s[i]=-cs[1]*e[i-1]+cs[0]*s[i];
					fg[1]=cs[1]*e[i];
					e[i]=cs[0]*e[i];
					if(i<m)
						//if((cs[0]!=1.0)||(cs[1]!=0.0))
						if((fabs(cs[0]-1.0)>MIN_DOUBLE)||(fabs(cs[1])>MIN_DOUBLE))
							for(j=1; j<=m; j++) {
								ix=(j-1)*m+i-1;
								iy=(j-1)*m+i;
								d=cs[0]*u[ix]+cs[1]*u[iy];
								u[iy]=-cs[1]*u[ix]+cs[0]*u[iy];
								u[ix]=d;
							}
				}
				e[mm-2]=fg[0];
				it=it-1;
			} else {
				if(ks==mm) {
					kk=kk+1;
					fg[1]=e[mm-2];
					e[mm-2]=0.0;
					for(ll=kk; ll<=mm-1; ll++) {
						i=mm+kk-ll-1;
						fg[0]=s[i-1];
						sssself(fg,cs);
						s[i-1]=fg[0];
						if(i!=kk) {
							fg[1]=-cs[1]*e[i-2];
							e[i-2]=cs[0]*e[i-2];
						}
						//if((cs[0]!=1.0)||(cs[1]!=0.0))
						if((fabs(cs[0]-1.0)>MIN_DOUBLE)||(fabs(cs[1])>MIN_DOUBLE))
							for(j=1; j<=n; j++) {
								ix=(j-1)*n+i-1;
								iy=(j-1)*n+mm-1;
								d=cs[0]*v[ix]+cs[1]*v[iy];
								v[iy]=-cs[1]*v[ix]+cs[0]*v[iy];
								v[ix]=d;
							}
					}
				} else {
					kk=ks+1;
					fg[1]=e[kk-2];
					e[kk-2]=0.0;
					for(i=kk; i<=mm; i++) {
						fg[0]=s[i-1];
						sssself(fg,cs);
						s[i-1]=fg[0];
						fg[1]=-cs[1]*e[i-1];
						e[i-1]=cs[0]*e[i-1];
						//if((cs[0]!=1.0)||(cs[1]!=0.0))
						if((fabs(cs[0]-1.0)>MIN_DOUBLE)||(fabs(cs[1])>MIN_DOUBLE))
							for(j=1; j<=m; j++) {
								ix=(j-1)*m+i-1;
								iy=(j-1)*m+kk-2;
								d=cs[0]*u[ix]+cs[1]*u[iy];
								u[iy]=-cs[1]*u[ix]+cs[0]*u[iy];
								u[ix]=d;
							}
					}
				}
			}
		}
	}

			free(s);free(e);free(w);
	return l;
}
//GPU中对3x3矩阵取逆
__device__ void Inverse(float *a){
	float A=a[0]*a[4]*a[8]+a[1]*a[5]*a[6]+a[2]*a[3]*a[7]-a[2]*a[4]*a[6]-a[1]*a[3]*a[8]-a[0]*a[5]*a[7];

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
}
__device__ float computeself(float *X,float *X_t,float *W,float *y){
	float WW[3*Ksize*Ksize];
	float XX[3*3];
	float YY[3*1];
	float XXWW[3*Ksize*Ksize];
	float XXy[3*1];

//上面的三个矩阵空间均有冗余

	Mult(WW,X_t,W,3,Ksize*Ksize,Ksize*Ksize,Ksize*Ksize);
	Mult(XX,WW,X,3,Ksize*Ksize,Ksize*Ksize,3);
	Inverse(XX);
	Mult(XXWW,XX,WW,3,3,3,Ksize*Ksize);
	Mult(XXy,XXWW,y,3,Ksize*Ksize,Ksize*Ksize,1);
	return (XXy[0]);
}
__constant__  float XXX[3*Ksize*Ksize];
__constant__  float XXXT[3*Ksize*Ksize];

__global__ void self_process(unsigned char *dev_original,unsigned char* dev_out,float *dev_X_t,float *dev_X,int R,int C){
	int x=threadIdx.x;
	int y=blockIdx.x;
	int offset=x+y*blockDim.x;
	while(offset<R*C){
		float dev_y[Ksize*Ksize];
		float dev_W[Ksize*Ksize*Ksize*Ksize];
		float G[Ksize*Ksize*2];
		float U[Ksize*Ksize*Ksize*Ksize];
		float V[2*2];
		float eps=0.000001;
		int fx=offset/C-Ksize/2;
		int fy=offset%C-Ksize/2;
		for(int i=0;i<Ksize;i++){
			for(int j=0;j<Ksize;j++){
				if((fx+i)<0||(fy+j)<0||(fx+i)>=R||(fy+j)>=C)
					dev_y[i*Ksize+j]=dev_original[offset];
				else
					dev_y[i*Ksize+j]=dev_original[(fx+i)*C+(fy+j)];
			}
		}
		init_Gself(G,dev_y);
		int re=dluavself(G,Ksize*Ksize,2,U,V,eps,Ksize*Ksize+1);
		float sita=atan(V[1]/V[3]);
		float gama;
		float deta;
		float C[4];
		float s1;
		float s2;

		s1=G[0];
		s2=G[3];
		gama=sqrt((s1*s2+1)/25);
		deta=(s1+1)/(s2+1);

		C[0]=gama*(deta*cos(sita)*cos(sita)+1/deta*sin(sita)*sin(sita));
		C[1]=gama*(1/deta*sin(sita)*cos(sita)-deta*cos(sita)*sin(sita));
		C[2]=C[1];
		C[3]=gama*(deta*sin(sita)*sin(sita)+1/deta*cos(sita)*cos(sita));

		init_Wself(dev_W,C);
		float value=computeself(XXX,XXXT,dev_W,dev_y);
//		value/=1.1;
		if(value>255){
			value=255;
		}
		else if(value<0){
			value=0;
		}
//		value/=1.3;
		dev_out[offset]=(unsigned char)value;
		offset=offset+gridDim.x*blockDim.x;
	}
}
//主函数**************************************************************************************//



extern "C" float Test_GPU(string name,int choice){
	int liR=512;
	int liC=512;

	cudaEvent_t start,end;
	HANDLE_ERROR(cudaEventCreate(&start));
	HANDLE_ERROR(cudaEventCreate(&end));
	HANDLE_ERROR(cudaEventRecord(start,0));
	unsigned char *original;
	unsigned char *out;
	unsigned char *dev_original;
	unsigned char *dev_out;
	unsigned char *temp_original;
	unsigned char *temp_out;
	temp_original=(unsigned char*)malloc(liR*liC*sizeof(unsigned char));
	temp_out=(unsigned char*)malloc(liR*liC*sizeof(unsigned char));

	float *X;
	float *X_t;
	float *W;
	float *dev_mid;
	float *XX;
	float *WW;
	float *Mid;
	float *dev_X_t;
	float *dev_X;

	Mat srcImage=imread(name);
	cvtColor(srcImage, srcImage, CV_BGR2GRAY);
	if(srcImage.channels()!=1)
		return srcImage.channels();
	Mat dstImage=srcImage.clone();

	int R= srcImage.rows;
	int C= srcImage.cols;

	original=(unsigned char*)malloc(R*C*sizeof(unsigned char));
	for(int i=0;i<R;i++){
		for(int j=0;j<C;j++){
			original[i*C+j]=srcImage.at<uchar>(i,j);
		}
	}
	out=(unsigned char*)malloc(R*C*sizeof(unsigned char));
	X=(float*)malloc(3*Ksize*Ksize*sizeof(float));
	X_t=(float*)malloc(3*Ksize*Ksize*sizeof(float));
	W=(float*)malloc(Ksize*Ksize*Ksize*Ksize*sizeof(float));
	XX=(float*)malloc(sizeof(float)*3*Ksize*Ksize);
	WW=(float*)malloc(sizeof(float)*3*Ksize*Ksize);
	Mid=(float*)malloc(sizeof(float)*3*Ksize*Ksize);
	init_X(X);
	init_X_t(X_t);
	init_Wcal(W);

	HANDLE_ERROR(cudaMalloc((void**)&dev_original,R*C*sizeof(unsigned char)));
	HANDLE_ERROR(cudaMalloc((void**)&dev_out,R*C*sizeof(unsigned char)));
	//for classical
	HANDLE_ERROR(cudaMalloc((void**)&dev_mid,3*Ksize*Ksize*sizeof(float)));
	mult(WW,X_t,W,3,Ksize*Ksize,Ksize*Ksize,Ksize*Ksize);
	mult(XX,WW,X,3,Ksize*Ksize,Ksize*Ksize,3);
	//for self
	HANDLE_ERROR(cudaMalloc((void**)&dev_X,3*Ksize*Ksize*Ksize*sizeof(float)));
	HANDLE_ERROR(cudaMalloc((void**)&dev_X_t,3*Ksize*Ksize*sizeof(float)));
	HANDLE_ERROR(cudaMemcpy(dev_X,X,3*Ksize*Ksize*sizeof(float),cudaMemcpyHostToDevice));
	HANDLE_ERROR(cudaMemcpy(dev_X_t,X_t,3*Ksize*Ksize*sizeof(float),cudaMemcpyHostToDevice));

	HANDLE_ERROR(cudaMemcpyToSymbol(XXX,X,3*Ksize*Ksize*sizeof(float)));
	HANDLE_ERROR(cudaMemcpyToSymbol(XXXT,X_t,3*Ksize*Ksize*sizeof(float)));


	if(inverse(XX)!=0)
		mult(Mid,XX,WW,3,3,3,Ksize*Ksize);
	HANDLE_ERROR(cudaMemcpy(dev_mid,Mid,3*Ksize*Ksize*sizeof(float),cudaMemcpyHostToDevice));
	

	
	if(choice==3){
		HANDLE_ERROR(cudaMemcpy(dev_original,original,R*C*sizeof(unsigned char),cudaMemcpyHostToDevice));
		ave_process<<<DIM,DIM>>>(dev_original,dev_out,R,C);
		HANDLE_ERROR(cudaMemcpy(out,dev_out,R*C*sizeof(unsigned char),cudaMemcpyDeviceToHost));
	}
	else if(choice==4){
		HANDLE_ERROR(cudaMemcpy(dev_original,original,R*C*sizeof(unsigned char),cudaMemcpyHostToDevice));
		mid_process<<<DIM,DIM>>>(dev_original,dev_out,R,C);
		HANDLE_ERROR(cudaMemcpy(out,dev_out,R*C*sizeof(unsigned char),cudaMemcpyDeviceToHost));
	}
	else if(choice==1){
		HANDLE_ERROR(cudaMemcpy(dev_original,original,R*C*sizeof(unsigned char),cudaMemcpyHostToDevice));
		cla_process<<<DIM/2,DIM/2>>>(dev_original,dev_out,dev_mid,R,C);
		HANDLE_ERROR(cudaMemcpy(out,dev_out,R*C*sizeof(unsigned char),cudaMemcpyDeviceToHost));
	}
	else{
		int numR=(R+255)/256;
		int numC=(C+255)/256;
		for(int i=0;i<numR;i++){
			for(int j=0;j<numC;j++){
				int off=i*liR*C+j*liC;
				int tempR=liR;
				int tempC=liC;
				if(i==numR-1)
					tempR=R-liR*i;
				if(j==numC-1)
					tempC=C-liC*j;
				for(int i=0;i<tempR;i++){
					for(int j=0;j<tempC;j++){
						temp_original[i*tempC+j]=original[off+i*C+j];
					}
				}
				
				HANDLE_ERROR(cudaMemcpy(dev_original,temp_original,tempR*tempC*sizeof(unsigned char),cudaMemcpyHostToDevice));
				self_process<<<DIM/2,DIM/2>>>(dev_original,dev_out,dev_X_t,dev_X,tempR,tempC);
				HANDLE_ERROR(cudaMemcpy(temp_out,dev_out,tempR*tempC*sizeof(unsigned char),cudaMemcpyDeviceToHost));
				for(int i=0;i<tempR;i++){
					for(int j=0;j<tempC;j++){
						out[off+i*C+j]=temp_out[i*tempC+j];
					}
				}
			}
		}
	}
	HANDLE_ERROR(cudaEventRecord(end,0));
	HANDLE_ERROR(cudaEventSynchronize(end));
	float time;
	HANDLE_ERROR(cudaEventElapsedTime(&time,start,end));

	for(int i=0;i<R;i++){
		for(int j=0;j<C;j++){
			dstImage.at<uchar>(i,j)=out[i*C+j];
		}
	}
	/*string name1=name+"classical";
	string name2=name+"self";
	string name3=name+"average";
	string name4=name+"middle";*/
	String tempname="";
	int pos;
	for(int i=0;i<name.length();i++){
		if(name[i]=='.')
		{
			pos=i;
			break;
		}
	}
	if(choice==1){
		tempname=name;
		tempname.insert(pos,"_classical_GPU");
		imshow("经典核回归GPU",dstImage);
		imwrite(tempname,dstImage);
	}
	else if(choice==2){
		tempname=name;
		tempname.insert(pos,"_adaptive_GPU");
		imshow("自适应核回归GPU",dstImage);
		imwrite(tempname,dstImage);
	}
	else if(choice==3){
		tempname=name;
		tempname.insert(pos,"_mean_GPU");
		imshow("均值滤波GPU",dstImage);
		imwrite(tempname,dstImage);
	}
	else{
		tempname=name;
		tempname.insert(pos,"_median_GPU");
		imshow("中值滤波GPU",dstImage);
		imwrite(tempname,dstImage);
	}
	
	
	
//	waitKey();
	return time;
}