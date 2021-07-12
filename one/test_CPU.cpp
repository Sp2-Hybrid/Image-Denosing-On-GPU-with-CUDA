#include "test.h"

float K(int x1,int y1,int x0,int y0) {
	float distance=(x1-x0)*(x1-x0)+(y1-y0)*(y1-y0);
	float h=1;
	float t=distance/(h*h);
	float xishu=1/((2*pai*h));

//	float zhishu=(-0.5)*((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
	float r=xishu*exp(-0.5*t);
	return r;
}
void init_W(float *W) {
	int offset;
	int x0=Ksize/2;
	int y0=Ksize/2;

	for(int i=0; i<Ksize*Ksize; i++) {
		for(int j=0; j<Ksize*Ksize; j++) {
			if(i==j) {
				int fx=i/Ksize;
				int fy=i%Ksize;
				W[i*Ksize*Ksize+j]=K(fx,fy,x0,y0);
			} else {
				W[i*Ksize*Ksize+j]=0;
			}

		}
	}
}

int Test_CPU(string name)
{
	clock_t start,end;
	unsigned char *original;
	unsigned char *out;
	float *X;
	float *X_t;
	float *y;
	float *W;
//读取图片，将图片转换为灰度图获取图片得到长宽数据便于开辟空间
	Mat srcImage=imread(name);
	cvtColor(srcImage, srcImage, CV_BGR2GRAY);

	if(srcImage.channels()!=1)
		return srcImage.channels();

	Mat dstImage=srcImage.clone();

	int R = srcImage.rows;
	int C = srcImage.cols;

	original=(unsigned char*)malloc(R*C*sizeof(unsigned char));
	out=(unsigned char*)malloc(R*C*sizeof(unsigned char));
	X=(float*)malloc(3*Ksize*Ksize*sizeof(float));
	X_t=(float*)malloc(3*Ksize*Ksize*sizeof(float));
	W=(float*)malloc(Ksize*Ksize*Ksize*Ksize*sizeof(float));
	y=(float *)malloc(Ksize*Ksize*sizeof(float));

	for(int i=0;i<R;i++){
		for(int j=0;j<C;j++){
			original[i*C+j]=srcImage.at<uchar>(i,j);
		}
	}

	init_X(X);
	init_X_t(X_t);
	init_W(W);

	int offset=0;
	start=clock();
	for(int i=0;i<R;i++){
		for(int j=0;j<C;j++){
			offset=i*C+j;
			int fx=i-Ksize/2;
			int fy=j-Ksize/2;

			for(int i=0;i<Ksize;i++){
				for(int j=0;j<Ksize;j++){
					if((fx+i)<0||(fy+j)<0||(fx+i)>=R||(fy+j)>=C){
						y[i*Ksize+j]=original[offset];
					}
					else{
						y[i*Ksize+j]=original[(fx+i)*C+(fy+j)];
					}
				}
			}
				int value=compute(X,X_t,W,y);
				if(value>255)
					out[offset]=255;
				else if(value<0)
					out[offset]=0;
				else
					out[offset]=(unsigned char)value;

				if(abs(out[offset]-original[offset])>=200)
					out[offset]=original[offset];
		}
	}
	end=clock();

	for(int i=0;i<R;i++){
		for(int j=0;j<C;j++){
			dstImage.at<uchar>(i,j)=out[i*C+j];
		}
	}
	int pos;
	for(int i=0;i<name.length();i++){
		if(name[i]=='.'){
			pos=i;
			break;
		}
	}
	string tempname=name;
	tempname.insert(pos,"_classical_CPU");
	imwrite(tempname,dstImage);
	imshow("经典核回归去噪CPU",dstImage);
//	waitKey();
	return (end-start);
}
