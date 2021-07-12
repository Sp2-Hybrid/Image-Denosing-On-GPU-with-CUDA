#include "test.h"

int Middl_CPU(string name){
	clock_t start,end;
	unsigned char *original;
	unsigned char *out;

	Mat srcImage=imread(name);
	cvtColor(srcImage, srcImage, CV_BGR2GRAY);
	if(srcImage.channels()!=1)
		return srcImage.channels();
	Mat dstImage=srcImage.clone();
	int R = srcImage.rows;
	int C = srcImage.cols;
	original=(unsigned char*)malloc(R*C*sizeof(unsigned char));
	out=(unsigned char*)malloc(R*C*sizeof(unsigned char));

	for(int i=0;i<R;i++){
		for(int j=0;j<C;j++){
			original[i*C+j]=srcImage.at<uchar>(i,j);
		}
	}
	int y[NKsize*NKsize]; 
	int offset=0;
	start=clock();
	for(int i=0;i<R;i++){
		for(int j=0;j<C;j++){
			offset=i*C+j;
			int fx=i-NKsize/2;
			int fy=j-NKsize/2;
			for(int i=0;i<NKsize;i++){
				for(int j=0;j<NKsize;j++){
					if((fx+i)<0||(fy+j)<0||(fx+i)>=R||(fy+j)>=C){
						y[i*NKsize+j]=original[offset];
					}
					else{
						y[i*NKsize+j]=original[(fx+i)*C+(fy+j)];
					}
				}
			}
			int temp;
			for(int i=0;i<NKsize*NKsize-1;i++){
				for(int j=0;j<NKsize*NKsize-1-i;j++){
					if(y[j]>y[j+1]){
						temp=y[j];
						y[j]=y[j+1];
						y[j+1]=temp;
					}
				}
			}
			out[offset]=y[NKsize*NKsize/2];
		}
	}
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
	tempname.insert(pos,"_medianCPU");
	imwrite(tempname,dstImage);
	imshow("ÖÐÖµÂË²¨CPU",dstImage);
	end=clock();
	return (end-start);
}