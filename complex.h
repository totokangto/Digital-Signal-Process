#pragma once
#ifndef __COMPLEX
#define __COMPLEX
#include <math.h>
class complex {
public:
	// �Ǽ� ��� ����
	double re_n, im_n;
	// ������
	complex() { re_n = im_n = 0; }; // argument ���� ��
	complex(double x, double y) { re_n = x; im_n = y; }; //argument 2��
	complex(double theta) { re_n = cos(theta); im_n = sin(theta); }; // argument 1��

	// ��� �Լ�
	double get_mag();
	double get_phase();

	// ������ �����ε�
	complex& operator=(const complex&);
	complex& operator+=(const complex&);
	complex operator+(const complex&);
	complex operator-(const complex&);
	complex operator*(const complex&); // ��� ���ϱ�
	complex operator/(const double&);  // �Ǽ��� ������
	complex operator/(const complex&); // ����� ������
	complex operator*(const double&);  // �Ǽ� ���ϱ�

};
#endif#pragma once