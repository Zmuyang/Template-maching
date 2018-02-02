/*
	highgui.hpp �����˴������ڵ�flag�������¼���flag��Qt���ڵ�flag���¼��ص�����ԭ�ͣ��Լ�����/�ؼ�������ص�ϵ�к�����openGL�İ�װ������
ͼ�����������ʾ����غ�������Ƶ����ͷ���������ʾ����غ�����VideoCapture��VideoWriter��
*/
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp"  //imgproc.hpp��������ͼ����ģ��֮ƽ������̬ѧ������
#include <iostream>


using namespace std;
using namespace cv;

Mat img; Mat templ; Mat result;


int match_method;
int max_Trackbar = 5;




void MatchingMethod(int, void*)
{

	Mat img_display;
	img.copyTo(img_display); //�� img�����ݿ����� img_display

							 //�����������ľ���
	int result_cols = img.cols - templ.cols + 1;     //�������ڴ���ƥ���������ͼ�����Ĵ�С��
	int result_rows = img.rows - templ.rows + 1;

	result.create(result_cols, result_rows, CV_32FC1);//�������������������������CV_32FC1��ʽ���档

	//����ƥ��ͱ�׼��
	matchTemplate(img, templ, result, match_method); //��ƥ��ͼ��ģ��ͼ��������ͼ��ƥ�䷽�����ɻ�����ֵ��������
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());//�������飬������飬range normalize����Сֵ��range normalize�����ֵ����һ�����ͣ���typeΪ������ʱ�������type�������type��ͬ��

	// ͨ������ minMaxLoc ��λ��ƥ���λ��
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());//���ڼ����������ֵ����Сֵ��λ��

	// ���ڷ��� SQDIFF �� SQDIFF_NORMED, ԽС����ֵ������ߵ�ƥ����. ��������������, ��ֵԽ��ƥ��Խ��
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}


	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0, 0, 255), 2, 8, 0); //���õ��Ľ���þ��ο�����
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0, 0, 255), 2, 8, 0);

	imshow("Source Image", img_display);//������յĵ��Ľ��
	imwrite("result.jpg", img_display); //���õ��Ľ��д��Դ����Ŀ¼�¡�
	imshow("Result window", result);   //���ƥ��������

	return;
}


int main(int argc, char** argv)
{
	//imread�������Խ�ͼƬ��ȡȻ��ŵ�Mat�����������ں���������
	img = imread("original.jpg");//�����ƥ��ͼ��
	templ = imread("template.jpg");//����ģ��ͼ��

/*	�������ڡ���һ�������Ǵ������ƣ��ڶ���������int���͵�Flag������д���µ�ֵ
	WINDOW_NORMAL���������ֵ���û�����Ըı䴰�ڵĴ�С��û�����ƣ�
	WINDOW_AUTOSIZE������������ֵ�����ڴ�С���Զ���������Ӧ����ʾ��ͼ�񣬲��Ҳ����ֶ��ı䴰�ڴ�С
*/
	namedWindow("Source Image", CV_WINDOW_AUTOSIZE); // �������ƣ����ڱ�ʶCV_WINDOW_AUTOSIZE���Զ��������ڴ�С����ӦͼƬ�ߴ硣
	namedWindow("Result window", CV_WINDOW_NORMAL);

	// ��������������һ��������ƥ�䷽�����ڶ���������ȷ�����������ڴ��ڣ������������Ƕ�Ӧ��������ֵ�����ĸ������ǻ����������ֵ������������ǻص�������
	createTrackbar("jackchen", "Source Image", &match_method, max_Trackbar, MatchingMethod); //��������ʾ��Ϣ�����������ڴ�������ƥ�䷽ʽ�������ƶ�֮���ƶ�����ֵ����ñ��������ص�������

	MatchingMethod(0, 0);//��ʼ����ʾ

	int logo = waitKey(27); //�ȴ������¼������ֵ0Ϊ�����õȴ���
	getchar();

	return 0;
}