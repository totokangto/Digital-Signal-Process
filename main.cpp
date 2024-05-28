#include<iostream>
#include<fstream>
#include "complex.h"
#include <math.h>
using namespace std;

#define BYTE unsigned char // 0~255 까지의 unsigned char 자료형을 앞으로 BYTE 라는 단어로 사용 
#define PI 3.141592
#define INPUT_FILE_NAME1 "Lena_gray.bmp"
#define INPUT_FILE_NAME2 "Lena_gray_NOISE.bmp"
#define HEADERSIZE 54 // Lena_gray.bmp 파일의 헤더의 크기

int main()
{
	// 이미지 저장 코드

	ifstream In_Image; // 파일 읽기
	In_Image.open(INPUT_FILE_NAME2, ios::in | ios::binary); // INPUT_FILE_NAME1 파일을 binary로 읽어옴

	int M = 64, N = 64; // 이미지의 크기 정의(단위:픽셀) 
	BYTE* header = new BYTE[HEADERSIZE]; // 이미지의 헤더 정보를 담기 위한 공간 생성
	BYTE** image = new BYTE*[N]; // Lena_gray.bmp 파일의 이미지 데이터를 담기 위한 공간 생성
	BYTE** r = new BYTE*[N]; // 이미지 데이터 중 red에 대한 데이터를 담을 공간 생성
	BYTE** g = new BYTE*[N]; // 이미지 데이터 중 green에 대한 데이터를 담을 공간 생성
	BYTE** b = new BYTE*[N]; // 이미지 데이터 중 blue에 대한 데이터를 담을 공간 생성
	BYTE** result = new BYTE*[N]; // DFT와 IDFT를 통해 노이즈를 제거한 이미지 데이터를 담기 위한 공간 생성
	for (int i = 0; i < N; i++) { // 각각의 데이터를 담을 공간을 2차원의 형태로 만드는 과정(왜냐하면 이미지가 2차원이기 때문)
		image[i] = new BYTE[M * 3]; // 이미지 데이터을 담을 공간의 크기 : N * (M*3) (한 픽셀은 R,G,B 성분으로 이루어져 있으므로 행의 크기는 64*3이 되어야 함) 
		r[i] = new BYTE[M]; // red에 대한 데이터를 담을 공간의 크기 : N * M 
		g[i] = new BYTE[M]; // green에 대한 데이터를 담을 공간의 크기 : N * M 
		b[i] = new BYTE[M]; // blue에 대한 데이터를 담을 공간의 크기 : N * M 
		result[i] = new BYTE[M * 3]; // 노이즈를 제거한 이미지 데이터을 담을 공간의 크기 : N * (M*3) (출력 이미지 생성을 위해 한 픽셀에 R,G,B 성분이 모두 들어가야 함) 
	}

	In_Image.read((char*)header, HEADERSIZE); // 헤더 정보를 HEADERSIZE의 크기만큼 읽어서 header 변수에 저장
	for (int i = 0; i < N; i++) {
		In_Image.read((char*)image[i], 3 * M); // Lena_gray.bmp 파일의 이미지 데이터를 읽고 이를 image 변수에 저장 
	}

	int place;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) { // 이미지 데이터가 저장되어 있는 image 변수에서 R,G,B 값을 복사(실제 BMP 파일에는 B,G,R 순으로 담겨 있음)
			place = 3 * j;
			b[i][j] = image[i][place];  // blue 데이터 
			g[i][j] = image[i][place + 1]; // green 데이터
			r[i][j] = image[i][place + 2];  // red 데이터
			
		}
	}

	// 이미지 데이터를 complex로 구현
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

	// 2D DFT 이미지 출력 (Hint : DFT값은 매우 크기 때문에 정규화 작업을 실행해야 합니다)
	// min-max를 통해 정규화 시킴
	
	double bmax = 0, gmax = 0, rmax = 0;
	double bmin = b_dft[0][0].get_mag();
	double gmin = g_dft[0][0].get_mag();
	double rmin = r_dft[0][0].get_mag();
	complex bmin_comp(0,0),gmin_comp(0,0),rmin_comp(0,0); // 나중에 noise 제거를 위해 magnitude가 최소일 때의 complex를 저장
	
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
	
	BYTE** img_dft = new BYTE * [N]; // 2D DFT 이미지 담을 공간 생성
	for (int i = 0; i < N; i++) {
		img_dft[i] = new BYTE[M * 3];
	}
	
	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) { // 0~255 범위로 정규화
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

	// 노이즈 제거 : 원본 이미지의 주파수 이미지와 noise 이미지의 주파수 이미지를 비교해서 noise를 제거
	
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
	고주파 부분 제거
	for (int u = 45; u < 64; u++) {
		for (int v = 45; v < 64; v++) {
			b_dft[u][v] = bmin_comp;
			g_dft[u][v] = gmin_comp;
			r_dft[u][v] = rmin_comp;
		}
	}
	*/
	
	// noise가 잘 제거 되었는지 확인

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
	
	// 이미지 생성 코드

	for (int v = 0; v < N; v++) {
		for (int u= 0; u < M; u++) {
			b[v][u] = (BYTE)b_idft[v][u].re_n;
			g[v][u] = (BYTE)g_idft[v][u].re_n;
			r[v][u] = (BYTE)r_idft[v][u].re_n;
		}
	}
	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) { // DFT와 IDFT를 통해 노이즈를 제거한 데이터를 result 변수에 하나로 묶기(BMP 파일 생성을 위해 한 픽셀은 24 bit(R,G,B)로 구성되야 함)
			place = 3 * j;
			result[i][place] = b[i][j];  // gray 이미지는 R,G,B 값이 모두 같으므로 하나의 변수만 이용해서 저장해도 됨
			result[i][place + 1] = g[i][j];
			result[i][place + 2] = r[i][j];
		}
	}
	
	ofstream Out;
	Out.open("result.bmp", ios::out | ios::binary); // 출력 파일 생성
	Out.write((char*)header, HEADERSIZE); // 출력파일에 이미지의 헤더정보 작성
	for (int i = 0; i < N; i++) {
		Out.write((char*)result[i], 3 * M); // 노이즈를 제거하고 R,G,B이 하나의 픽셀에 묶인 result 데이터를 출력 파일에 작성
	}
	
	system("pause");
	return 1;
}