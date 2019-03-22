#pragma once

#include <iostream>
#include <vector>
#include<stdlib.h>
#include "bresenham.h"
#include "point.h"
using namespace std;

class DrawTri {
private:
	Point top;
	Point left;
	Point right;
public:
	DrawTri(Point a, Point b, Point c) {
		this->top = a;
		this->left = b;
		this->right = c;
	}
	vector<int> use();
	float* create(vector<int> v);
	vector<int> linearEquation(Point first, Point second);
	vector<int> rasterization();
};
