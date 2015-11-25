#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>

#include "maxflow/graph.h"
#include "image.h"

using namespace std;

// This section shows how to use the library to compute a minimum cut on the following graph:
//
//		        SOURCE
//		       /       \
//		     1/         \6
//		     /      4    \
//		   node0 -----> node1
//		     |   <-----   |
//		     |      3     |
//		     \            /
//		     5\          /1
//		       \        /
//		          SINK
//
///////////////////////////////////////////////////

void testGCuts()
{
	Graph<int,int,int> g(/*estimated # of nodes*/ 2, /*estimated # of edges*/ 1); 
	g.add_node(2); 
	g.add_tweights( 0,   /* capacities */  1, 5 );
	g.add_tweights( 1,   /* capacities */  6, 1 );
	g.add_edge( 0, 1,    /* capacities */  4, 3 );
	int flow = g.maxflow();
	cout << "Flow = " << flow << endl;
	for (int i=0;i<2;i++)
		if (g.what_segment(i) == Graph<int,int,int>::SOURCE)
			cout << i << " is in the SOURCE set" << endl;
		else
			cout << i << " is in the SINK set" << endl;
}

int main() {
	testGCuts();

	Image<Vec3b> Icolor=imread("../fishes.jpg");
	imshow("I", Icolor);

	Image<uchar> Igray;
	cvtColor(Icolor,Igray,CV_RGB2GRAY);
	Image<float> I;
	Igray.convertTo(I,CV_32F);

	float alpha=.001f,beta=500.f;
	Scalar Iin(250,249,200),Iext(4,160,160);

	Image<float> g(I.rows,I.cols,CV_32F);
	for (int i=0;i<I.rows;i++) {
		for (int j=0;j<I.cols;j++) {
			if (i==0 || i==I.rows-1 || j==0 || j==I.cols-1) {
				g(i,j)=0;
				continue;
			}
			float Iy=(I(i+1,j)-I(i-1,j))/2;
			float Ix=(I(i,j+1)-I(i,j-1))/2;
			g(i,j)=beta/(1+alpha*(Ix*Ix+Iy*Iy));
		}
	}
	imshow("G",g.float2byte());
	waitKey();
	
	Graph<double,double,double> G(I.cols*I.rows,2*I.cols*I.rows);
	G.add_node(I.cols*I.rows); 
	for (int i=0;i<I.rows;i++) {
		for (int j=0;j<I.cols;j++) {
			Scalar col(Icolor(i,j));
			double D0=norm(col-Iin);
			double D1=norm(col-Iext);
			G.add_tweights(i+j*I.rows,D0,D1);
			if (i<I.rows-1)
				G.add_edge(i+j*I.rows,(i+1)+j*I.rows,g(i,j),g(i,j));
			if (j<I.cols-1)
				G.add_edge(i+j*I.rows,i+(j+1)*I.rows,g(i,j),g(i,j));
		}
	}

	double flow=G.maxflow();
	cout << "flow=" << flow << endl;

	Image<uchar> label(I.rows,I.cols,CV_8U);
	for (int i=0;i<I.rows;i++) 
		for (int j=0;j<I.cols;j++)
			label(i,j)= (G.what_segment(i+j*I.rows) == Graph<double,double,double>::SOURCE) ? 0 : 255;

	imshow("L",label);

	waitKey();
	return 0;
}
