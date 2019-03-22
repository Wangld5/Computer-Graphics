#include "bresenham.h"

Bresenham::Bresenham(int x0, int y0, int x1, int y1) {
	this->x0 = x0;
	this->y0 = y0;
	this->x1 = x1;
	this->y1 = y1;
}
vector<int> Bresenham::use() {
	vector<int> points;
	points.push_back(x0);
	points.push_back(y0);
	int dx = x1 - x0;
	int dy = y1 - y0;
	int direct_x = dx > 0 ? 1 : -1;
	int direct_y = dy > 0 ? 1 : -1;
	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	if (abs(dx) > abs(dy)) {
		int p = 2 * dy - dx;
		int x = x0;
		int y = y0;
		int two_dy = 2 * dy;
		int two_dy_sub_two_dx = 2 * dy - 2 * dx;
		while (x != x1) {
			points.push_back(x);
			points.push_back(y);
			if (p > 0) {
				y += direct_y;
				p = p + two_dy_sub_two_dx;
			}
			else {
				p = p + two_dy;
			}
			x += direct_x;
		}
	}
	else {
		int p = 2 * dx - dy;
		int x = x0;
		int y = y0;
		int two_dx = 2 * dx;
		int two_dx_sub_two_dy = 2 * dx - 2 * dy;
		while (y != y1) {
			points.push_back(x);
			points.push_back(y);
			if (p > 0) {
				x += direct_x;
				p = p + two_dx_sub_two_dy;
			}
			else {
				p = p + two_dx;
			}
			y += direct_y;
		}
	}
	points.push_back(x1);
	points.push_back(y1);
	return points;
}