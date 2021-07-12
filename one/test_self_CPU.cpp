#include "test.h"

void init_G(float *G,float *y) {
	int fx=Ksize/2;
	int fy=Ksize/2;
	int num=0;
	for(int i=0; i<Ksize; i++) {
		for(int j=0; j<Ksize; j++) {
			if(i!=fx) {
				G[num*2]=(y[i*Ksize+j]-y[fx*Ksize+fy])/(i-fx);
			} else {
				G[num*2]=0;
			}
			if(j!=fy) {
				G[num*2+1]=(y[i*Ksize+j]-y[fx*Ksize+fy])/(j-fy);
			} else {
				G[num*2+1]=0;
			}
			num++;
		}
	}
}

float K(int x1,int y1,int x0,int y0,float *C) {
	float h=4.5;
	float distance=(x1-x0)*(x1-x0)*C[0]+(x1-x0)*(y1-y0)*C[2]+(x1-x0)*(y1-y0)*C[1]+(y1-y0)*(y1-y0)*C[3];
	float xishu=sqrt(C[0]*C[3]-C[1]*C[2])/(2*pai*h);
	float r=xishu*exp(-0.5*distance/(h*h));
	return r;
}
void init_W(float *W,float *C) {
	int offset;
	int x0=Ksize/2;
	int y0=Ksize/2;

	for(int i=0; i<Ksize*Ksize; i++) {
		for(int j=0; j<Ksize*Ksize; j++) {
			if(i==j) {
				int fx=i/Ksize;
				int fy=i%Ksize;
				W[i*Ksize*Ksize+j]=K(fx,fy,x0,y0,C);
			} else {
				W[i*Ksize*Ksize+j]=0;
			}

		}
	}
}

int Test_self_CPU(string name){
	clock_t start,end;
	start=clock();
	unsigned char *original;
	unsigned char *out;
	
	float *X;
	float *X_t;
	float *y;
	float *W;
	float *G;
	float *U;
	float *A;
	float *V;
//读取指定位置的图片
	Mat srcImage=imread(name);
	cvtColor(srcImage, srcImage, CV_BGR2GRAY);
	if(srcImage.channels()!=1)
		return srcImage.channels();

	Mat dstImage=srcImage.clone();
//获取图片长宽数据
	int R = srcImage.rows;
	int C = srcImage.cols;
//为计算所用到的数组开辟空间
	original=(unsigned char*)malloc(R*C*sizeof(unsigned char));
	out=(unsigned char*)malloc(R*C*sizeof(unsigned char));
	X=(float*)malloc(3*Ksize*Ksize*sizeof(float));
	X_t=(float*)malloc(3*Ksize*Ksize*sizeof(float));

	W=(float*)malloc(Ksize*Ksize*Ksize*Ksize*sizeof(float));
	y=(float *)malloc(Ksize*Ksize*sizeof(float));
	U=(float *)malloc(Ksize*Ksize*Ksize*Ksize*sizeof(float));

	A=(float *)malloc(Ksize*Ksize*sizeof(float));
	V=(float *)malloc(Ksize*Ksize*sizeof(float));
	G=(float *)malloc(Ksize*Ksize*2*sizeof(float));

	for(int i=0; i<Ksize; i++) {
		for(int j=0; j<Ksize; j++) {
			U[i*Ksize+j]=0;
		}
	}
	for(int i=0;i<R;i++){
		for(int j=0;j<C;j++){
			original[i*C+j]=srcImage.at<uchar>(i,j);
		}
	}
	init_X(X);
	init_X_t(X_t);

	int offset;
	

	float sita;
	float gama;
	float deta;
	float *Ca;
	Ca=(float*)malloc(4*sizeof(float));
	float s1;
	float s2;

	for(int i=0;i<R;i++){
		int num=0;
		for(int j=0;j<C;j++){
			offset=i*C+j;
			int fx=i-Ksize/2;
			int fy=j-Ksize/2;
			for(int i=0; i<Ksize; i++) {
				for(int j=0; j<Ksize; j++) {
					if((fx+i)<0||(fy+j)<0||(fx+i)>=R||(fy+j)>=C)
						y[i*Ksize+j]=original[offset];
					else
						y[i*Ksize+j]=original[(fx+i)*C+(fy+j)];
				}
			}
			init_G(G,y);

			double eps=0.000001;
			int re=dluav(G,Ksize*Ksize,2,U,V,eps,Ksize*Ksize+1);

			
			sita=atan(V[1]/V[3]);
			s1=G[0];
			s2=G[3];
			gama=sqrt((s1*s2+1)/25);
			deta=(s1+1)/(s2+1);

			Ca[0]=gama*(deta*cos(sita)*cos(sita)+1/deta*sin(sita)*sin(sita));
			Ca[1]=gama*(1/deta*sin(sita)*cos(sita)-deta*cos(sita)*sin(sita));
			Ca[2]=Ca[1];
			Ca[3]=gama*(deta*sin(sita)*sin(sita)+1/deta*cos(sita)*cos(sita));

			init_W(W,Ca);

			float value=compute(X,X_t,W,y);
//			value=value/1.05;
			if(value>255)
				out[offset]=255;
			else if(value<0)
				out[offset]=0;
			else
			{
				int gray=(int )value;
				out[offset]=(unsigned char)gray;
			}
		}
	}
	
	for(int i=0;i<R;i++){
		for(int j=0;j<C;j++){
			dstImage.at<uchar>(i,j)=out[i*C+j];
		}
	}
	end=clock();

	int pos;
	for(int i=0;i<name.length();i++){
		if(name[i]=='.'){
			pos=i;
			break;
		}
	}
	string tempname=name;
	tempname.insert(pos,"_adaptive_CPU");
	imshow("自适应核回归CPU",dstImage);
	imwrite(tempname,dstImage);
	return (end-start);

}