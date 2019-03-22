#include "drawTriangle.h"
vector<int> DrawTri::use() {
	vector<int> allPoints;
	vector<vector<int>> edges;
	Bresenham left_line(top.x, top.y, left.x, left.y);
	Bresenham right_line(top.x, top.y, right.x, right.y);
	Bresenham bottom_line(right.x, right.y, left.x, left.y);
	edges.push_back(left_line.use());
	edges.push_back(right_line.use());
	edges.push_back(bottom_line.use());

	for (int i = 0; i < edges.size(); i++) {
		for (int j = 0; j < edges[i].size(); j++) {
			allPoints.push_back(edges[i][j]);
		}
	}
	return allPoints;
}
float* DrawTri::create(vector<int> v) {
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
		vertices[i * 6 + 3] = rand() / double(RAND_MAX);
		vertices[i * 6 + 4] = rand() / double(RAND_MAX);
		vertices[i * 6 + 5] = rand() / double(RAND_MAX);
	}
	return vertices;
}
vector<int> DrawTri::linearEquation(Point first, Point second) {
	vector<int> parameters;
	int A = second.y - first.y;
	int B = first.x - second.x;
	int C = second.x*first.y - first.x*second.y;
	parameters.push_back(A);
	parameters.push_back(B);
	parameters.push_back(C);
	return parameters;
}
vector<int> DrawTri::rasterization() {
	vector<vector<int>> lines;
	lines.push_back(linearEquation(top, left));
	lines.push_back(linearEquation(top, right));
	lines.push_back(linearEquation(left, right));
	//中心化
	for (int i = 0; i < 3; i++) {
		int x, y;
		if (i == 0) {
			x = right.x;
			y = right.y;
		}
		else if (i == 1) {
			x = left.x;
			y = left.y;
		}
		else {
			x = top.x;
			y = top.y;
		}
		if (lines[i][0]*x + lines[i][1]*y + lines[i][2] < 0) {
			for (int j = 0; j < 3; j++) {
				lines[i][j] *= -1;
			}
		}
	}
	//找到可以填充的像素
	int minX = min(top.x, min(left.x, right.x));
	int maxX = max(top.x, max(left.x, right.x));
	int minY = min(top.y, min(left.y, right.y));
	int maxY = max(top.y, max(left.y, right.y));
	vector<int> pixels;
	//找三角形的外接矩形
	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++) {
			bool isInside = true;
			for (int i = 0; i < 3; i++) {
				if (lines[i][0] * x + lines[i][1] * y + lines[i][2] < 0) {
					isInside = false;
					break;
				}
			}
			if (isInside)
			{
				pixels.push_back(x);
				pixels.push_back(y);
			}
		}
	}
	return pixels;


}