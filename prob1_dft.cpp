#include <iostream>
#include <fstream>
#include "complex.h"
using namespace std;
#define PI 3.141592

int main() {
	// �ؽ�Ʈ ���� ����
	ofstream k_axis_file;
	ofstream mag_axis_file;
	ofstream phase_axis_file;
	k_axis_file.open("k_axis.txt");
	mag_axis_file.open("mag_axis.txt");
	phase_axis_file.open("phase_axis.txt");
	
	/*
	//  x(t) = 4cos(8��t)-2sin(5��t)
		fs = 20
		1. 4cos(8��t) => 4cos(0.2��n) => 2(exp(j0.2��n)+exp(-j0.2��n)) 
			f/fs = 4/20 = 200/1000 => k0 = 200, N = 1000
		2. sin(5��t) => 2sin(0.125��n) => exp(j(0.125��n-0.5��))+exp(-j(0.125��n-0.5��))
			f/fs = 2.5/20 = 125/1000 => k0 = 125, N = 1000
	*/
	// ���� �ʱ�ȭ
	int N = 1000;
	int k0_c = 200;
	int k0_s = 125;
	
	// make x[n] : x[n] =  2( exp(j0.2��n) + exp(-j0.2��n) ) - ( exp(j(0.125��n-0.5��)) + exp(-j0.125��n+0.5��) )
	complex* x_n = new complex[N];
	for (int n = -N/2; n < N/2; n++) {
		//x_n[n + N / 2] = complex((2 * PI * n*k0_c/(double)N)) ;
		x_n[n+N/2] = (complex((2 * PI * n * k0_c) / (double)N) + complex(-(2 * PI * n * k0_c) / (double)N))*2
			- (complex((2 * PI * n * k0_s) / (double)N - 0.5*PI) + complex(-(2 * PI * n * k0_s) / (double)N + 0.5 * PI));
	}
	
	// DFT
	complex* x_k = new complex[N];
	for (int k = -N/2; k < N/2; k++) {
		for (int n = -N/2; n < N/2; n++) {
			x_k[k+N/2] += x_n[n+N/2] * complex(-2*PI*k*n/(double)N);
		}
	}
	
	for (int k = -300; k < 300; k++) {
		k_axis_file << k << endl;
		mag_axis_file << x_k[k+N/2].get_mag() << endl;
		phase_axis_file << x_k[k+N/2].get_phase() << endl;
	}
	
	system("pause");
	return 1;
}
