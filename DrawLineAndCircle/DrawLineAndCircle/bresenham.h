#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class Bresenham {
private:
	int x0, y0;
	int x1, y1;
public:
	Bresenham(int x0, int y0, int x1, int y1);
	vector<int> use();
};
