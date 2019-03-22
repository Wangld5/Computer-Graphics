#pragma once
#include <iostream>
#include "bresenham.h"
#include "point.h"
#include <vector>
using namespace std;

class DrawCir {
private:
	Point center;
	int radius;
public:
	DrawCir(Point c, int r);
	vector<int> use();
	void getAllPoint(Point nextPoint, vector<int>& points);
	float* create(vector<int> v);
};