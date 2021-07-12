//利用程序给原图像增加椒盐噪声
//图象模拟添加椒盐噪声是通过随机获取像素点斌那个设置为高亮度点来实现的
#include "mult.h"
#include "inverse.h"
#include "init_X_X_t.h"
#include "dluav.h"
#include "compute.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <fstream>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>


#define Maxsize 1000
#define DIM 256
#define Ksize 5
#define pai 3.141592
#define Max 100000.0f
#define MAX_ITERA 60
#define MIN_DOUBLE (1e-30)
#define NKsize 5

using namespace cv;
using namespace std;
 

double generateGaussianNoise(double m, double sigma);

int Test_CPU(string name);
int Test_self_CPU(string name);
int Avera_CPU(string name);
int Middl_CPU(string name);
