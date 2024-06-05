#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#define pi 3.14159265358979323846
using namespace std;

class HoughTransform {
public:
	int numR;
	int numC;
	int minV;
	int maxV;
	int houghDist;
	int houghAngle;
	int** imgAry;
	int** CartesianAry;
	int** polarAry;
	int angleDegree;
	double angleRadian;
	int offset;

	HoughTransform() {

	}

	void loadImg(ifstream& in) {
		for (int i = 0; i < numR; i++) {
			for (int j = 0; j < numC; j++) {
				in >> imgAry[i][j];
			}
		}

	}

	void aryReformat(int** inAry, string d, ofstream& out) {
		string s = to_string(10);

		int w = s.length();
		int r = 0;
		while (r < houghDist) {
			int c = 0;
			while (c < 180) {
				if (inAry[r][c] == 0) {
					out << d;
				}
				else {
					out << inAry[r][c];
				}
				s = to_string(inAry[r][c]);
				int ww = s.length();
				while (ww <= w) {
					out << " ";
					ww++;
				}
				c++;
			}
			out << endl;
			r++;
		}
	}

	void imgReformat(int** inAry, string d, ofstream& out) {
		string s = to_string(maxV);

		int w = s.length();
		int r = 0;
		while (r < numR) {
			int c = 0;
			while (c < numC) {
				if (inAry[r][c] == 0) {
					out << d;
				}
				else {
					out << inAry[r][c];
				}
				s = to_string(inAry[r][c]);
				int ww = s.length();
				while (ww <= w) {
					out << " ";
					ww++;
				}
				c++;
			}
			out << endl;
			r++;
		}
	}

	void buildHoughSpace(ofstream& debug) {
		debug << "Entering buildHoughSpace" << endl;
		for (int x = 0; x < numR; x++) {
			for (int y = 0; y < numC; y++) {
				if (imgAry[x][y] > 0)
					computeSinusoid(x, y, debug);
			}
		}
		debug << "Leaving buildHoughSpace" << endl;
	}

	void computeSinusoid(int x, int y, ofstream& debug) {
		debug << "Entering computeSinusoid" << endl;
		angleDegree = 0;

		while(angleDegree <= 179)
		{
			angleRadian = (((double)angleDegree / 180) * (pi));
			double dist = CartesianDist(x, y);
			int distInt = (int)dist;
			CartesianAry[distInt][angleDegree]++;
			dist = PolarDist(x, y);
			distInt = (int)dist;
			polarAry[distInt][angleDegree]++;
			angleDegree++;
		}

		debug << "Leaving computeSinusoid" << endl;
	}

	double CartesianDist(int x, int y) {
		double t = angleRadian - atan((double)y / x) - (pi / 2);
		return sqrt((double)pow(x, 2) + (double)pow(y, 2)) * cos(t) + offset;
	}

	double PolarDist(int x, int y) {
		return (double)x * cos(angleRadian) + (double)y * sin(angleRadian) + offset;
	}
};


int main(int argc, char* argv[]) {
	ifstream in(argv[1]);
	ofstream out(argv[2]);
	ofstream debug(argv[3]);

	HoughTransform e;
	in >> e.numR >> e.numC >> e.minV >> e.maxV;
	e.houghAngle = 180;
	int diagonal = sqrt((e.numR * e.numR) + e.numC * e.numC);
	e.houghDist = 2 * diagonal;
	e.offset = diagonal;
	e.imgAry = new int* [e.numR];
	for (int j = 0; j < e.numR; j++) {
		e.imgAry[j] = new int[e.numC];
	}

	e.polarAry = new int* [e.houghDist];
	e.CartesianAry = new int* [e.houghDist];
	for (int j = 0; j < e.houghDist; j++) {
		e.polarAry[j] = new int[e.houghAngle];
		e.CartesianAry[j] = new int[e.houghAngle];
	}

	for (int i = 0; i < (e.houghDist); i++) {
		for (int j = 0; j < e.houghAngle; j++) {
			e.polarAry[i][j] = 0;
			e.CartesianAry[i][j] = 0;
		}
	}

	e.loadImg(in);
	out << "Img Array:" << endl;
	e.imgReformat(e.imgAry, ".", out);

	e.buildHoughSpace(debug);
	out << "Cartesian Array:" << endl;
	e.aryReformat(e.CartesianAry, ".", out);
	out << "Polar Array:" << endl;
	e.aryReformat(e.polarAry, ".", out);


	in.close();
	out.close();
	debug.close();
}