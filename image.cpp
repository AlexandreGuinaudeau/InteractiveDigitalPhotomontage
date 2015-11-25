#include "image.h"


// Harris points
vector<Point> harris(const Image<uchar>& I, double th,int n) {
	vector<Point> v;
	int r=I.rows,c=I.cols;
	Image<float> h;
	I.convertTo(h,CV_32F);
	cornerHarris(h,h,10,3,0.04);
	for (int j=n;j<c-n;j++)
		for (int i=n;i<r-n;i++)
			if (i>0 && i<r-1 && j>0 && j<c-1
				&& h(i,j) > th

				&& h(i,j)>h(i,j+1) && h(i,j)>h(i,j-1) && h(i,j)>h(i-1,j-1) && h(i,j)>h(i-1,j)
				&& h(i,j)>h(i-1,j+1) && h(i,j)>h(i+1,j-1) && h(i,j)>h(i+1,j) && h(i,j)>h(i+1,j+1))
				v.push_back(Point(j,i));
	return v;
}


// Correlation
double mean(const Image<float>& I,Point m,int n) {
	double s=0;
	for (int j=-n;j<=n;j++)
		for (int i=-n;i<=n;i++) 
			s+=I(m.y+i,m.x+j);
	return s/(2*n+1)/(2*n+1);
}

double corr(const Image<float>& I1,Point m1,const Image<float>& I2,Point m2,int n) {
	double M1=mean(I1,m1,n);
	double M2=mean(I2,m2,n);
	double rho=0;
	for (int j=-n;j<=n;j++)
		for (int i=-n;i<=n;i++) {
			rho+=(I1(m1.y+i,m1.x+j)-M1)*(I2(m2.y+i,m2.x+j)-M2);
		}
		return rho;
}

double NCC(const Image<float>& I1,Point m1,const Image<float>& I2,Point m2,int n) {
	if (m1.x<n || m1.x>=I1.cols-n || m1.y<n || m1.y>=I1.rows-n) return -1;
	if (m2.x<n || m2.x>=I2.cols-n || m2.y<n || m2.y>=I2.rows-n) return -1;
	double c1=corr(I1,m1,I1,m1,n);
	if (c1==0) return -1;
	double c2=corr(I2,m2,I2,m2,n);
	if (c2==0) return -1;
	return corr(I1,m1,I2,m2,n)/sqrt(c1*c2);
}
