#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

template <typename T> class Image : public Mat {
public:
	// Constructors
	Image() {}
	Image(const Mat& A):Mat(A) {}
	Image(int m,int n,int type):Mat(m,n,type) {}
	// Accessors
	inline T operator()(int i,int j) const { return at<T>(i,j); }
	inline T& operator()(int i,int j) { return at<T>(i,j); }
	inline T operator()(const Point& p) const { return at<T>(p.y,p.x); }
	inline T& operator()(const Point& p) { return at<T>(p.y,p.x); }

	// Pour afficher image flottant
	Image<uchar> float2byte() const {
		double minVal, maxVal;
		minMaxLoc(*this,&minVal,&maxVal);
		Image<uchar> Ib;
		convertTo(Ib, CV_8U, 255.0/(maxVal - minVal), -minVal);
		return Ib;
	}
};


// Harris
vector<Point> harris(const Image<uchar>& I, double th,int n);
// Correlation
double NCC(const Image<float>& I1,Point m1,const Image<float>& I2,Point m2,int n);


#endif