#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include<opencv.hpp>
#include <opencv2/imgproc/hal/hal.hpp>
#include<imgproc.hpp>
#include<vector>
#include<algorithm>
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
using namespace cv;
using namespace std;
void n_edgec(InputOutputArray&, size_t);//找多边形
void find_cir(InputOutputArray&);//找圆
bool ifsubstr(string papa, string son);//检测子序列
void combine();
int main()
{
	namedWindow("BB", WINDOW_KEEPRATIO);
	auto src=imread("2.jpg");
	Mat test3 = imread("3.png");
	imshow("BB", test3);waitKey(10);
	size_t n=0;
	do//交互阶段
	{	Mat test3_temp=test3.clone();
		cout << "\n输入识别图形的夹角数(negative num to quit)"<<endl;cin >> n;
		n_edgec(test3_temp, n);
		imshow("BB", test3_temp);
		waitKey(10);
	} while (!cin.fail());
	combine();//合并数列
	namedWindow("AA", WINDOW_KEEPRATIO);
	find_cir(src);
	imshow("AA", src);
	imshow("BB", test3);
	waitKey(0);
}
void n_edgec(InputOutputArray& src, size_t n) {		

	medianBlur(src, src, 3);
		//找出图形轮廓	
		Mat canny_out(src.size(), CV_8UC1);
		vector<vector<Point>>contours;
		Canny(src, canny_out, 150, 300);
		findContours(canny_out, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		//求出所有夹角坐标
		Mat src_gray;
		cvtColor(src, src_gray, COLOR_BGR2GRAY);
		vector<Point2f>corners;
		goodFeaturesToTrack(src_gray, corners, 30, 0.1, 0.001);
		//找出符合夹角数的单个轮廓
		for (auto v : contours) {
			int has_corners = 0;
			for (size_t i = 0; i < corners.size(); i++) {
				if (pointPolygonTest(v, corners[i], 1) >-2)has_corners++;
			}
		//框选
			if (has_corners == n) {
				Rect r = boundingRect(v);
				rectangle(src, r.tl(), r.br(), (255, 255, 255), 3);
			}	
		}
}
void find_cir(InputOutputArray& src) {
	Mat gray;
	//检测圆
	cvtColor(src, gray, COLOR_BGR2GRAY);
	medianBlur(gray, gray, 5);
	vector<Vec3f> circles;
	HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows / 16,
		100, 35, 50, 99
	);
	//框选
	for (size_t i = 0; i < circles.size(); i++)
	{
		Vec3i c = circles[i];
		Point center = Point(c[0], c[1]);		
		circle(src, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
		int radius = c[2];
		circle(src, center, radius, Scalar(0, 255, 0), 3, LINE_AA);
	}
};
bool ifsubstr(string papa, string son) {
	return papa.find(son)!=string::npos;
};
void combine() {
	int a[] = { 1,2,3 };
	int b[] = { -1,4,6 };
	int c[6] = {0};
	merge(a, a + 3, b, b + 3, c);
	for (auto v : c)cout << v<<ends;
}