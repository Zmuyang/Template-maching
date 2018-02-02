/*
	highgui.hpp 定义了创建窗口的flag，窗口事件的flag，Qt窗口的flag，事件回调函数原型，以及窗口/控件操作相关的系列函数，openGL的包装函数；
图像输入输出显示的相关函数；视频摄像头输入输出显示的相关函数，VideoCapture，VideoWriter。
*/
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp"  //imgproc.hpp：定义了图像处理模块之平滑和形态学操作。
#include <iostream>


using namespace std;
using namespace cv;

Mat img; Mat templ; Mat result;


int match_method;
int max_Trackbar = 5;




void MatchingMethod(int, void*)
{

	Mat img_display;
	img.copyTo(img_display); //将 img的内容拷贝到 img_display

							 //创建输出结果的矩阵
	int result_cols = img.cols - templ.cols + 1;     //计算用于储存匹配结果的输出图像矩阵的大小。
	int result_rows = img.rows - templ.rows + 1;

	result.create(result_cols, result_rows, CV_32FC1);//被创建矩阵的列数，行数，以CV_32FC1形式储存。

	//进行匹配和标准化
	matchTemplate(img, templ, result, match_method); //待匹配图像，模版图像，输出结果图像，匹配方法（由滑块数值给定。）
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());//输入数组，输出数组，range normalize的最小值，range normalize的最大值，归一化类型，当type为负数的时候输出的type和输入的type相同。

	// 通过函数 minMaxLoc 定位最匹配的位置
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());//用于检测矩阵中最大值和最小值的位置

	// 对于方法 SQDIFF 和 SQDIFF_NORMED, 越小的数值代表更高的匹配结果. 而对于其他方法, 数值越大匹配越好
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}


	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0, 0, 255), 2, 8, 0); //将得到的结果用矩形框起来
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0, 0, 255), 2, 8, 0);

	imshow("Source Image", img_display);//输出最终的到的结果
	imwrite("result.jpg", img_display); //将得到的结果写到源代码目录下。
	imshow("Result window", result);   //输出匹配结果矩阵。

	return;
}


int main(int argc, char** argv)
{
	//imread函数可以将图片读取然后放到Mat容器里面用于后续操作。
	img = imread("original.jpg");//载入待匹配图像
	templ = imread("template.jpg");//载入模版图像

/*	创建窗口。第一个参数是窗口名称，第二个窗口是int类型的Flag可以填写以下的值
	WINDOW_NORMAL设置了这个值，用户便可以改变窗口的大小（没有限制）
	WINDOW_AUTOSIZE如果设置了这个值，窗口大小会自动调整以适应所显示的图像，并且不能手动改变窗口大小
*/
	namedWindow("Source Image", CV_WINDOW_AUTOSIZE); // 窗口名称，窗口标识CV_WINDOW_AUTOSIZE是自动调整窗口大小以适应图片尺寸。
	namedWindow("Result window", CV_WINDOW_NORMAL);

	// 创建滑动条，第一个参数是匹配方法，第二个参数是确定滑动条所在窗口，第三个参数是对应滑动条的值，第四个参数是滑动条的最大值，第五个参数是回调函数。
	createTrackbar("jackchen", "Source Image", &match_method, max_Trackbar, MatchingMethod); //滑动条提示信息，滑动条所在窗口名，匹配方式（滑块移动之后将移动到的值赋予该变量），回调函数。

	MatchingMethod(0, 0);//初始化显示

	int logo = waitKey(27); //等待按键事件，如果值0为则永久等待。
	getchar();

	return 0;
}