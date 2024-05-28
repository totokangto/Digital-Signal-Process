#pragma once
#ifndef __COMPLEX
#define __COMPLEX
#include <math.h>
class complex {
public:
	// 실수 허수 정의
	double re_n, im_n;
	// 생성자
	complex() { re_n = im_n = 0; }; // argument 없을 때
	complex(double x, double y) { re_n = x; im_n = y; }; //argument 2개
	complex(double theta) { re_n = cos(theta); im_n = sin(theta); }; // argument 1개

	// 멤버 함수
	double get_mag();
	double get_phase();

	// 연산자 오버로딩
	complex& operator=(const complex&);
	complex& operator+=(const complex&);
	complex operator+(const complex&);
	complex operator-(const complex&);
	complex operator*(const complex&); // 허수 곱하기
	complex operator/(const double&);  // 실수로 나누기
	complex operator/(const complex&); // 허수로 나누기
	complex operator*(const double&);  // 실수 곱하기

};
#endif#pragma once