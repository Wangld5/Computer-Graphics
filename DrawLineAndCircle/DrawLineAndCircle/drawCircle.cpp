#include "drawCircle.h"
DrawCir::DrawCir(Point c, int r) {
	this->center = c;
	this->radius = r;
}
//八分对称画圆
void DrawCir::getAllPoint(Point nextPoint, vector<int>& points) {
	//1
	points.push_back(center.x + nextPoint.x);
	points.push_back(center.y + nextPoint.y);
	//2
	points.push_back(center.x + nextPoint.x);
	points.push_back(center.y - nextPoint.y);
	//3
	points.push_back(center.x - nextPoint.x);
	points.push_back(center.y + nextPoint.y);
	//4
	points.push_back(center.x - nextPoint.x);
	points.push_back(center.y - nextPoint.y);
	//5
	points.push_back(center.x + nextPoint.y);
	points.push_back(center.y + nextPoint.x);
	//6
	points.push_back(center.x + nextPoint.y);
	points.push_back(center.y - nextPoint.x);
	//7
	points.push_back(center.x - nextPoint.y);
	points.push_back(center.y + nextPoint.x);
	//8
	points.push_back(center.x - nextPoint.y);
	points.push_back(center.y - nextPoint.x);
}
vector<int> DrawCir::use() {
	int x, y, d;
	x = 0;
	y = radius;
	d = 3 - 2 * radius;
	vector<int> points;
	Point np(x, y);
	getAllPoint(np, points);
	while (x < y) {
		if (d < 0) {
			d = d + 4 * x + 6;
		}
		else {
			d = d + 4 * (x - y) + 10;
			y--;
		}
		x++;
		Point np(x, y);
		getAllPoint(np, points);
	}
	return points;
}
float* DrawCir::create(vector<int> v) {
	int pointNum = v.size() / 2;
	int total = pointNum * 6;
	float *vertices = new float[total];
	for (int i = 0; i < pointNum; i++)
	{
		//位置
		vertices[i * 6 + 0] = (float)v[2 * i + 0] / (float)640;
		vertices[i * 6 + 1] = (float)v[2 * i + 1] / (float)360;
		vertices[i * 6 + 2] = 0.0f;
		//颜色
		vertices[i * 6 + 3] = 0.0f;
		vertices[i * 6 + 4] = 1.0f;
		vertices[i * 6 + 5] = 0.0f;
	}
	return vertices;
}