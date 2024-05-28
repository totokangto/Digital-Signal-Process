#include <iostream>
#include <math.h>
#include "complex.h"
#define PI 3.141592

using namespace std;

double complex::get_mag() {
	return sqrt(re_n * re_n + im_n * im_n); // magnitude 반환
}
double complex::get_phase() {
	// 복소수 --> 페이저 변환
	if (re_n == 0.0) {
		if (im_n >= 0.0) return PI / 2.0; // 0.5pi
		else return -PI / 2.0; // -0.5pi
	}
	else return atan(im_n / re_n);
}
complex complex::operator/(const complex& y) {

	complex c;
	c = complex(y.re_n, -y.im_n); // 복소수 y의 켤레복소수
	if (re_n == 0.0 && im_n == 0.0) { cout << " complex: divided by 0\n"; return c; } // 예외처리
	c = (*this + c) / (y.re_n * y.re_n + y.im_n * y.im_n);
	return c;
}
complex& complex::operator=(const complex& x) {
	re_n = x.re_n;
	im_n = x.im_n;
	return *this;
}
complex& complex::operator+=(const complex& y) {
	re_n += y.re_n;
	im_n += y.im_n;
	return *this;
}
complex complex::operator+(const complex& y) {
	complex c;
	c.re_n = re_n + y.re_n;
	c.im_n = im_n + y.im_n;
	return c;
}
complex complex::operator-(const complex& y) {
	complex c;
	c.re_n = re_n - y.re_n;
	c.im_n = im_n - y.im_n;
	return c;
}
complex complex::operator*(const complex& y) {
	complex c;
	c.re_n = re_n * y.re_n - im_n * y.im_n;
	c.im_n = re_n * y.im_n + im_n * y.re_n;
	return c;
}
complex complex::operator/(const double& y) {
	complex c;
	c.re_n = re_n / y;
	c.im_n = im_n / y;
	return c;
}
complex complex::operator*(const double& y) {
	complex c;
	c.re_n = re_n * y;
	c.im_n = im_n * y;
	return c;
}