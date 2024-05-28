#include <iostream>
#include <fstream>
#include "complex.h"
using namespace std;
#define PI 3.141592

int main() {
	// 텍스트 파일 생성
	ofstream k_axis_file;
	ofstream mag_axis_file;
	ofstream phase_axis_file;
	k_axis_file.open("k_axis.txt");
	mag_axis_file.open("mag_axis.txt");
	phase_axis_file.open("phase_axis.txt");
	
	/*
	//  x(t) = 4cos(8πt)-2sin(5πt)
		fs = 20
		1. 4cos(8πt) => 4cos(0.2πn) => 2(exp(j0.2πn)+exp(-j0.2πn)) 
			f/fs = 4/20 = 200/1000 => k0 = 200, N = 1000
		2. sin(5πt) => 2sin(0.125πn) => exp(j(0.125πn-0.5π))+exp(-j(0.125πn-0.5π))
			f/fs = 2.5/20 = 125/1000 => k0 = 125, N = 1000
	*/
	// 변수 초기화
	int N = 1000;
	int k0_c = 200;
	int k0_s = 125;
	
	// make x[n] : x[n] =  2( exp(j0.2πn) + exp(-j0.2πn) ) - ( exp(j(0.125πn-0.5π)) + exp(-j0.125πn+0.5π) )
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
