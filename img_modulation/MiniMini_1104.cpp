/*
最小二乗法

11/02 とりあえず、与えられたデータに対する最小二乗を動かしてみる



*/
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>

#include <math.h> //最小二乗で追加

#pragma comment(lib,"cv200d.lib")
#pragma comment(lib,"cxcore200d.lib")
#pragma comment(lib,"highgui200d.lib")

#pragma comment(lib,"cv200.lib")
#pragma comment(lib,"cxcore200.lib")
#pragma comment(lib,"highgui200.lib")

using namespace std;

char inputfile[] = "after_kaisuu001.bmp"; //入力元ファイル名
char editfile[]= "after_kaisuu036.bmp"; //更新後画像ファイル名
char outputfile[]="justMini.bmp";//規格化後出力ファイル名

//#define N 131072 /* データ数 */

int main(void){
	cv::Mat img;
	img = cv::imread(inputfile);

	cv::Mat edit;
	edit = cv::imread(editfile);
	
	int rows=img.rows;
	int cols=img.cols;
	int i;

	int N = rows*cols;


	//B
	double Ba = 0.0;
	double Bb = 0.0;
	double Bsum_xy = 0.0, Bsum_x = 0.0, Bsum_y = 0.0, Bsum_x2 = 0.0;

	double Ga = 0.0;
	double Gb = 0.0;
	double Gsum_xy = 0.0, Gsum_x = 0.0, Gsum_y = 0.0, Gsum_x2 = 0.0;

	double Ra = 0.0;
	double Rb = 0.0;
	double Rsum_xy = 0.0, Rsum_x = 0.0, Rsum_y = 0.0, Rsum_x2 = 0.0;




	//gnuplotでepsのグラフ作成するため、datファイルをオープン
		FILE *fpR;
		FILE *fpG;
		FILE *fpB;
		fpR = fopen("R.dat", "w");
		fpG = fopen("G.dat", "w");
		fpB = fopen("B.dat", "w");

	for (i=0; i<N; i++){
    Bsum_xy += double(img.data[i*3] * edit.data[i*3]);
    Bsum_x += double(edit.data[i*3]);
    Bsum_y += double(img.data[i*3]);
    Bsum_x2 += pow(double(edit.data[i*3]), 2);

	Gsum_xy += double(img.data[i*3+1] * edit.data[i*3+1]);
    Gsum_x += double(edit.data[i*3+1]);
    Gsum_y += double(img.data[i*3+1]);
    Gsum_x2 += pow(double(edit.data[i*3+1]), 2);

	Rsum_xy += double(img.data[i*3+2] * edit.data[i*3+2]);
    Rsum_x += double(edit.data[i*3+2]);
    Rsum_y += double(img.data[i*3+2]);
    Rsum_x2 += pow(double(edit.data[i*3+2]), 2);

			fprintf(fpR,"%d %d\n", img.data[i*3] , edit.data[i*3]  );
			fprintf(fpG,"%d %d\n", img.data[i*3+1] , edit.data[i*3+1]  );
			fprintf(fpB,"%d %d\n", img.data[i*3+2] , edit.data[i*3+2]  );
	}

	fclose(fpR);
	fclose(fpG);
	fclose(fpB);
	system("wgnuplot -persist RGBdot.plt");//グラフの書式設定は testSali.plt に書きこんでおく

 
  Ba = (N * Bsum_xy - Bsum_x * Bsum_y) / (N * Bsum_x2 - pow(Bsum_x, 2));
  Bb = (Bsum_x2 * Bsum_y - Bsum_xy * Bsum_x) / (N * Bsum_x2 - pow(Bsum_x, 2));

  Ga = (N * Gsum_xy - Gsum_x * Gsum_y) / (N * Gsum_x2 - pow(Gsum_x, 2));
  Gb = (Gsum_x2 * Gsum_y - Gsum_xy * Gsum_x) / (N * Gsum_x2 - pow(Gsum_x, 2));

  Ra = (N * Rsum_xy - Rsum_x * Rsum_y) / (N * Rsum_x2 - pow(Rsum_x, 2));
  Rb = (Rsum_x2 * Rsum_y - Rsum_xy * Rsum_x) / (N * Rsum_x2 - pow(Rsum_x, 2));

  printf("Ba = %f\n", Ba);
  printf("Bb = %f\n", Bb);

  printf("Ga = %f\n", Ga);
  printf("Gb = %f\n", Gb);

  printf("Ra = %f\n", Ra);
  printf("Rb = %f\n", Rb);

  cv::Mat out(rows, cols, CV_8UC3);
  for(int t=0; t<N; t++){

	  int R=0;
	  int G =0;
	  int B =0;
	  B = int(edit.data[t*3] * Ba + Bb +0.5);
	  G = int(edit.data[t*3+1] * Ga + Gb+0.5);
	  R = int(edit.data[t*3+2] * Ra + Rb+0.5);
 
	  if(B> 255)B=255;
	  if(G> 255)G=255;
	  if(R> 255)R=255;

	  if(B< 0)B=0;
	  if(G< 0)G=0;
	  if(R< 0)R=0;
	


	  out.data[t*3] = B;
	  out.data[t*3+1] = G;
	  out.data[t*3+2] = R;

  }
 

  cv::imwrite(outputfile,out);




  
	cv::waitKey();
  return 0;


}
