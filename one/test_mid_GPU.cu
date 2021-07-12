#include "test.h"

#include "book.h"
#include "cuda_runtime.h"

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
				temp=dev_y[j];
				dev_y[j]=dev_y[j+1];
				dev_y[j+1]=temp;
			}
		}
		dev_out[offset]=dev_y[NKsize*NKsize/2];

		offset+=blockDim.x*gridDim.x;
	}
}

extern "C" float Test_mid_GPU(string name){
	cudaEvent_t start,end;
	HANDLE_ERROR(cudaEventCreate(&start));
	HANDLE_ERROR(cudaEventCreate(&end));

	unsigned char *original;
	unsigned char *out;
	unsigned char *dev_original;
	unsigned char *dev_out;

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
	HANDLE_ERROR(cudaMalloc((void**)&dev_original,R*C*sizeof(unsigned char)));
	HANDLE_ERROR(cudaMalloc((void**)&dev_out,R*C*sizeof(unsigned char)));

	HANDLE_ERROR(cudaMemcpy(dev_original,original,R*C*sizeof(unsigned char),cudaMemcpyHostToDevice));

	HANDLE_ERROR(cudaEventRecord(start,0));
	mid_process<<<DIM,DIM>>>(dev_original,dev_out,R,C);
	HANDLE_ERROR(cudaMemcpy(out,dev_out,R*C*sizeof(unsigned char),cudaMemcpyDeviceToHost));
	HANDLE_ERROR(cudaEventRecord(end,0));
	HANDLE_ERROR(cudaEventSynchronize(end));
	float time;
	HANDLE_ERROR(cudaEventElapsedTime(&time,start,end));

	for(int i=0;i<R;i++){
		for(int j=0;j<C;j++){
			dstImage.at<uchar>(i,j)=out[i*C+j];
		}
	}
	imshow("ÖÐÖµÂË²¨GPU",dstImage);

	return time;
}