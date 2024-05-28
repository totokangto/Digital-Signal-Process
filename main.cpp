#include<iostream>
#include<fstream>
#include "complex.h"
#include <math.h>
using namespace std;

#define BYTE unsigned char // 0~255 ������ unsigned char �ڷ����� ������ BYTE ��� �ܾ�� ��� 
#define PI 3.141592
#define INPUT_FILE_NAME1 "Lena_gray.bmp"
#define INPUT_FILE_NAME2 "Lena_gray_NOISE.bmp"
#define HEADERSIZE 54 // Lena_gray.bmp ������ ����� ũ��

int main()
{
	// �̹��� ���� �ڵ�

	ifstream In_Image; // ���� �б�
	In_Image.open(INPUT_FILE_NAME2, ios::in | ios::binary); // INPUT_FILE_NAME1 ������ binary�� �о��

	int M = 64, N = 64; // �̹����� ũ�� ����(����:�ȼ�) 
	BYTE* header = new BYTE[HEADERSIZE]; // �̹����� ��� ������ ��� ���� ���� ����
	BYTE** image = new BYTE*[N]; // Lena_gray.bmp ������ �̹��� �����͸� ��� ���� ���� ����
	BYTE** r = new BYTE*[N]; // �̹��� ������ �� red�� ���� �����͸� ���� ���� ����
	BYTE** g = new BYTE*[N]; // �̹��� ������ �� green�� ���� �����͸� ���� ���� ����
	BYTE** b = new BYTE*[N]; // �̹��� ������ �� blue�� ���� �����͸� ���� ���� ����
	BYTE** result = new BYTE*[N]; // DFT�� IDFT�� ���� ����� ������ �̹��� �����͸� ��� ���� ���� ����
	for (int i = 0; i < N; i++) { // ������ �����͸� ���� ������ 2������ ���·� ����� ����(�ֳ��ϸ� �̹����� 2�����̱� ����)
		image[i] = new BYTE[M * 3]; // �̹��� �������� ���� ������ ũ�� : N * (M*3) (�� �ȼ��� R,G,B �������� �̷���� �����Ƿ� ���� ũ��� 64*3�� �Ǿ�� ��) 
		r[i] = new BYTE[M]; // red�� ���� �����͸� ���� ������ ũ�� : N * M 
		g[i] = new BYTE[M]; // green�� ���� �����͸� ���� ������ ũ�� : N * M 
		b[i] = new BYTE[M]; // blue�� ���� �����͸� ���� ������ ũ�� : N * M 
		result[i] = new BYTE[M * 3]; // ����� ������ �̹��� �������� ���� ������ ũ�� : N * (M*3) (��� �̹��� ������ ���� �� �ȼ��� R,G,B ������ ��� ���� ��) 
	}

	In_Image.read((char*)header, HEADERSIZE); // ��� ������ HEADERSIZE�� ũ�⸸ŭ �о header ������ ����
	for (int i = 0; i < N; i++) {
		In_Image.read((char*)image[i], 3 * M); // Lena_gray.bmp ������ �̹��� �����͸� �а� �̸� image ������ ���� 
	}

	int place;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) { // �̹��� �����Ͱ� ����Ǿ� �ִ� image �������� R,G,B ���� ����(���� BMP ���Ͽ��� B,G,R ������ ��� ����)
			place = 3 * j;
			b[i][j] = image[i][place];  // blue ������ 
			g[i][j] = image[i][place + 1]; // green ������
			r[i][j] = image[i][place + 2];  // red ������
			
		}
	}

	// �̹��� �����͸� complex�� ����
	complex** b_comp = new complex*[N];
	complex** g_comp = new complex * [N];
	complex** r_comp = new complex * [N];
	for (int i = 0; i < N; i++) {
		b_comp[i] = new complex[M];
		g_comp[i] = new complex[M];
		r_comp[i] = new complex[M];
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			b_comp[i][j] = complex(double(b[i][j]), 0);
			g_comp[i][j] = complex(double(g[i][j]), 0);
			r_comp[i][j] = complex(double(r[i][j]), 0);
		}
	}
	
	// 2D DFT 
	complex** b_dft = new complex*[N];
	complex** g_dft = new complex*[N];
	complex** r_dft = new complex*[N];
	for (int i = 0; i < N; i++) {
		b_dft[i] = new complex[M];
		g_dft[i] = new complex[M];
		r_dft[i] = new complex[M];
	}
	
	for (int v = 0; v < N; v++) {
		for (int u = 0; u < M; u++) {
			for (int y = 0; y < N; y++) {
				for (int x = 0; x < M; x++) {
					b_dft[v][u] += complex(-2 * PI * (u * x / (double)M + v * y / (double)N)) * b_comp[y][x] ;
					g_dft[v][u] += complex(-2 * PI * (u * x / (double)M + v * y / (double)N)) * g_comp[y][x];
					r_dft[v][u] += complex(-2 * PI * (u * x / (double)M + v * y / (double)N)) * r_comp[y][x] ;
				}
			}
		}
	}

	// 2D DFT �̹��� ��� (Hint : DFT���� �ſ� ũ�� ������ ����ȭ �۾��� �����ؾ� �մϴ�)
	// min-max�� ���� ����ȭ ��Ŵ
	
	double bmax = 0, gmax = 0, rmax = 0;
	double bmin = b_dft[0][0].get_mag();
	double gmin = g_dft[0][0].get_mag();
	double rmin = r_dft[0][0].get_mag();
	complex bmin_comp(0,0),gmin_comp(0,0),rmin_comp(0,0); // ���߿� noise ���Ÿ� ���� magnitude�� �ּ��� ���� complex�� ����
	
	for (int u = 0; u < N; u++) {
		for (int v = 0; v < M; v++) {
			if (u == 0 and v == 0) {
				continue;
			}
			if (b_dft[u][v].get_mag() > bmax) {
				bmax = b_dft[u][v].get_mag();
			}
			else if(b_dft[u][v].get_mag() < bmin) {
				bmin = b_dft[u][v].get_mag();
				bmin_comp = b_dft[u][v];
			}
			if (g_dft[u][v].get_mag() > gmax) {
				gmax = g_dft[u][v].get_mag();
			}
			else if (g_dft[u][v].get_mag() < gmin) {
				gmin = g_dft[u][v].get_mag();
				gmin_comp = g_dft[u][v];
			}
			if (r_dft[u][v].get_mag() > rmax) {
				rmax = r_dft[u][v].get_mag();
			}
			else if (r_dft[u][v].get_mag() < rmin) {
				rmin = r_dft[u][v].get_mag();
				rmin_comp = r_dft[u][v];
			}
		}
	}
	
	BYTE** img_dft = new BYTE * [N]; // 2D DFT �̹��� ���� ���� ����
	for (int i = 0; i < N; i++) {
		img_dft[i] = new BYTE[M * 3];
	}
	
	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) { // 0~255 ������ ����ȭ
			place = 3 * j;
			img_dft[i][place] = (BYTE)((255*b_dft[i][j].get_mag()-bmin)/(bmax-bmin));  
			img_dft[i][place + 1] = (BYTE)((255 * g_dft[i][j].get_mag() - gmin) / (gmax - gmin)); 
			img_dft[i][place + 2] = (BYTE)((255 * r_dft[i][j].get_mag() - rmin) / (rmax - rmin));  
		}
	}

	ofstream Out_dft_noise;
	Out_dft_noise.open("result_dft_noise.bmp", ios::out | ios::binary); 
	Out_dft_noise.write((char*)header, HEADERSIZE); 
	for (int i = 0; i < N; i++) {
		Out_dft_noise.write((char*)img_dft[i], 3 * M);
	}

	// ������ ���� : ���� �̹����� ���ļ� �̹����� noise �̹����� ���ļ� �̹����� ���ؼ� noise�� ����
	
	for (int u = 12; u < 40; u++) {
		for (int v = 0; v < 15; v++) {
			b_dft[u][v] = bmin_comp;
			g_dft[u][v] = gmin_comp;
			r_dft[u][v] = rmin_comp;
		}
	}

	for (int u = 24; u < 52; u++) {
		for (int v = 49; v < 64; v++) {
			b_dft[u][v] = bmin_comp;
			g_dft[u][v] = gmin_comp;
			r_dft[u][v] = rmin_comp;
		}
	}
	/*
	������ �κ� ����
	for (int u = 45; u < 64; u++) {
		for (int v = 45; v < 64; v++) {
			b_dft[u][v] = bmin_comp;
			g_dft[u][v] = gmin_comp;
			r_dft[u][v] = rmin_comp;
		}
	}
	*/
	
	// noise�� �� ���� �Ǿ����� Ȯ��

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) { 
			place = 3 * j;
			img_dft[i][place] = (BYTE)((255 * b_dft[i][j].get_mag() - bmin) / (bmax - bmin));  
			img_dft[i][place + 1] = (BYTE)((255 * g_dft[i][j].get_mag() - gmin) / (gmax - gmin)); 
			img_dft[i][place + 2] = (BYTE)((255 * r_dft[i][j].get_mag() - rmin) / (rmax - rmin));  
		}
	}
	
	ofstream Out_remove_noise;
	Out_remove_noise.open("result_dft_nose_removed.bmp", ios::out | ios::binary);
	Out_remove_noise.write((char*)header, HEADERSIZE);
	for (int i = 0; i < N; i++) {
		Out_remove_noise.write((char*)img_dft[i], 3 * M);
	}

	// 2D IDFT 

	complex** b_idft = new complex * [N];
	complex** g_idft = new complex * [N];
	complex** r_idft = new complex * [N];

	for (int i = 0; i < N; i++) {
		b_idft[i] = new complex[M];
		g_idft[i] = new complex[M];
		r_idft[i] = new complex[M];
	}
	
	for (int y = 0; y < N; y++) {
		for (int x = 0; x < M; x++) {
			for (int v = 0; v < N; v++) {
				for (int u = 0; u < M; u++) {
					b_idft[y][x] += complex(2 * PI * (x * u / (double)M + y * v / (double)N)) * b_dft[v][u];
					g_idft[y][x] += complex(2 * PI * (x * u / (double)M + y * v / (double)N)) * g_dft[v][u];
					r_idft[y][x] += complex(2 * PI * (x * u / (double)M + y * v / (double)N)) * r_dft[v][u];
				}
			}
		b_idft[y][x] = b_idft[y][x] / ((double)(N * M));
		g_idft[y][x] = g_idft[y][x] / ((double)(N * M));
		r_idft[y][x] = r_idft[y][x] / ((double)(N * M));
		}
	}
	
	// �̹��� ���� �ڵ�

	for (int v = 0; v < N; v++) {
		for (int u= 0; u < M; u++) {
			b[v][u] = (BYTE)b_idft[v][u].re_n;
			g[v][u] = (BYTE)g_idft[v][u].re_n;
			r[v][u] = (BYTE)r_idft[v][u].re_n;
		}
	}
	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) { // DFT�� IDFT�� ���� ����� ������ �����͸� result ������ �ϳ��� ����(BMP ���� ������ ���� �� �ȼ��� 24 bit(R,G,B)�� �����Ǿ� ��)
			place = 3 * j;
			result[i][place] = b[i][j];  // gray �̹����� R,G,B ���� ��� �����Ƿ� �ϳ��� ������ �̿��ؼ� �����ص� ��
			result[i][place + 1] = g[i][j];
			result[i][place + 2] = r[i][j];
		}
	}
	
	ofstream Out;
	Out.open("result.bmp", ios::out | ios::binary); // ��� ���� ����
	Out.write((char*)header, HEADERSIZE); // ������Ͽ� �̹����� ������� �ۼ�
	for (int i = 0; i < N; i++) {
		Out.write((char*)result[i], 3 * M); // ����� �����ϰ� R,G,B�� �ϳ��� �ȼ��� ���� result �����͸� ��� ���Ͽ� �ۼ�
	}
	
	system("pause");
	return 1;
}