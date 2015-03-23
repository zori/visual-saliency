/*
最小二乗法

11/02 とりあえず、与えられたデータに対する最小二乗を動かしてみる

11/16 a^2+b^2 も最小化の対象に入れてみる

11/20 杉本先生と打ち合わせたことをやる　1, RGBで統一 2,2乗ではなく絶対値距離で 3,中央値のRANSAC 4,ガウシアンを最後に 5,a*b の最小化
	  というわけで、a^2+b^2 は外して、RGB統一をつくる　→できた。MiniMiniRGB3 として、MiniMini　に組み込み済み
	  2,2乗ではなく絶対値距離....をやろうとしたけど、これは数学的に難しい??
	  ようなので、先に3番の中央値を試す

11/22 と思ったけど、とりあえずカーネルかけてやる
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
char outputfile[]="karnel.bmp";//規格化後出力ファイル名

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

/*
A1x+B1y=E1
A2x+B2y=E2

解
x=(E1*B2-B1*E2)/(A1*B2-B1*A2)
y=(E2*A1-E1*A2)/(A1*B2-A2*B1) 
*/


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


	//RGBで同じにする
/*	double RGBa = 0.0;
	double RGBb = 0.0;
	double RGBsum_xy = 0.0, RGBsum_x = 0.0, RGBsum_y = 0.0, RGBsum_x2 = 0.0;
*/
	//gnuplotでepsのグラフ作成するため、datファイルをオープン
		FILE *fpR;
		FILE *fpG;
		FILE *fpB;
		fpR = fopen("R.dat", "w");
		fpG = fopen("G.dat", "w");
	fpB = fopen("B.dat", "w");

	//RGBで同じにする
//		FILE *fpRGB;
//		fpRGB = fopen("RGB.dat", "w");


	//カーネルを一時保管する
	cv::Mat img2(rows, cols, CV_8UC3); 
	cv::Mat edit2(rows, cols, CV_8UC3);
		for (int y=0; y<rows; y++){
		for (int x=0; x<cols; x++){

			double center = 1.0;
			double next = 8.0;
			double cross = 8.0;


			//中心
			double preiB  = 0*img.data[(y*cols+x)*3] /center;
			double preiG  = 0*img.data[(y*cols+x)*3+1] /center;
			double preiR  = 0*img.data[(y*cols+x)*3+2] /center;

			double preEB  = 0*edit.data[(y*cols+x)*3] /center;
			double preEG  = 0*edit.data[(y*cols+x)*3+1] /center;
			double preER  = 0*edit.data[(y*cols+x)*3+2] /center;

			if( x-1>=0 && y-1>=0 ){//左上
				preiB = preiB + img.data[((y-1)*cols+x-1)*3] /cross;
				preiG = preiG + img.data[((y-1)*cols+x-1)*3+1] /cross;
				preiR = preiR + img.data[((y-1)*cols+x-1)*3+2] /cross;

				preEB = preEB + edit.data[((y-1)*cols+x-1)*3] /cross;
				preEG = preEG + edit.data[((y-1)*cols+x-1)*3+1] /cross;
				preER = preER + edit.data[((y-1)*cols+x-1)*3+2] /cross;
			}

			if( y-1>=0 ){//上
				preiB = preiB + img.data[((y-1)*cols+x)*3] /next;
				preiG = preiG + img.data[((y-1)*cols+x)*3+1] /next;
				preiR = preiR + img.data[((y-1)*cols+x)*3+2] /next;

				preEB = preEB + edit.data[((y-1)*cols+x)*3] /next;
				preEG = preEG + edit.data[((y-1)*cols+x)*3+1] /next;
				preER = preER + edit.data[((y-1)*cols+x)*3+2] /next;
			}

			if( x+1<=cols && y-1>=0 ){//右上
				preiB = preiB + img.data[((y-1)*cols+x+1)*3] /cross;
				preiG = preiG + img.data[((y-1)*cols+x+1)*3+1] /cross;
				preiR = preiR + img.data[((y-1)*cols+x+1)*3+2] /cross;

				preEB = preEB + edit.data[((y-1)*cols+x+1)*3] /cross;
				preEG = preEG + edit.data[((y-1)*cols+x+1)*3+1] /cross;
				preER = preER + edit.data[((y-1)*cols+x+1)*3+2] /cross;

			}

			if( x-1>=0 ){//左
				preiB = preiB + img.data[(y*cols+x)*3] /next;
				preiG = preiG + img.data[(y*cols+x)*3+1] /next;
				preiR = preiR + img.data[(y*cols+x)*3+2] /next;

				preEB = preEB + edit.data[(y*cols+x)*3] /next;
				preEG = preEG + edit.data[(y*cols+x)*3+1] /next;
				preER = preER + edit.data[(y*cols+x)*3+2] /next;
			}
			
			if( x+1<=cols ){//右
				preiB = preiB + img.data[(y*cols+x+1)*3] /next;
				preiG = preiG + img.data[(y*cols+x+1)*3+1] /next;
				preiR = preiR + img.data[(y*cols+x+1)*3+2] /next;

				preEB = preEB + edit.data[(y*cols+x+1)*3] /next;
				preEG = preEG + edit.data[(y*cols+x+1)*3+1] /next;
				preER = preER + edit.data[(y*cols+x+1)*3+2] /next;

			}

			if( x-1>=0 && y+1<=rows ){//左下
				preiB = preiB + img.data[((y+1)*cols+x-1)*3] /cross;
				preiG = preiG + img.data[((y+1)*cols+x-1)*3+1] /cross;
				preiR = preiR + img.data[((y+1)*cols+x-1)*3+2] /cross;

				preEB = preEB + edit.data[((y+1)*cols+x-1)*3] /cross;
				preEG = preEG + edit.data[((y+1)*cols+x-1)*3+1] /cross;
				preER = preER + edit.data[((y+1)*cols+x-1)*3+2] /cross;
			}

			if( y+1<=rows ){//下
				preiB = preiB + img.data[((y+1)*cols+x)*3] /next;
				preiG = preiG + img.data[((y+1)*cols+x)*3+1] /next;
				preiR = preiR + img.data[((y+1)*cols+x)*3+2] /next;

				preEB = preEB + edit.data[((y+1)*cols+x)*3] /next;
				preEG = preEG + edit.data[((y+1)*cols+x)*3+1] /next;
				preER = preER + edit.data[((y+1)*cols+x)*3+2] /next;
			}

			if( x+1<=cols && y+1<=rows ){//右下
				preiB = preiB + img.data[((y+1)*cols+x+1)*3] /cross;
				preiG = preiG + img.data[((y+1)*cols+x+1)*3+1] /cross;
				preiR = preiR + img.data[((y+1)*cols+x+1)*3+2] /cross;

				preEB = preEB + edit.data[((y+1)*cols+x+1)*3] /cross;
				preEG = preEG + edit.data[((y+1)*cols+x+1)*3+1] /cross;
				preER = preER + edit.data[((y+1)*cols+x+1)*3+2] /cross;
			}

			img2.data[(y*cols+x)*3] = preiB;
			img2.data[(y*cols+x)*3+1] = preiG;
			img2.data[(y*cols+x)*3+2] = preiR;

			edit2.data[(y*cols+x)*3] = preEB;
			edit2.data[(y*cols+x)*3+1] = preEG;
			edit2.data[(y*cols+x)*3+2] = preER;
			}

		}

		cv::imshow("karnel",img2);
		cv::imshow("karnel2",edit2);

	for (i=0; i<N; i++){

	//RGBで同じにする
/*	RGBsum_xy += double( ((img.data[i*3] + img.data[i*3+1] + img.data[i*3+2])/3) * ((edit.data[i*3] + edit.data[i*3+1] + edit.data[i*3+2])/3) );
    RGBsum_x += double((edit.data[i*3] + edit.data[i*3+1] + edit.data[i*3+2])/3);
    RGBsum_y += double((img.data[i*3] + img.data[i*3+1] + img.data[i*3+2])/3);
    RGBsum_x2 += pow(double((edit.data[i*3] + edit.data[i*3+1] + edit.data[i*3+2])/3), 2);
*/

//カーネル　
	Bsum_xy += double(img2.data[i*3] * edit2.data[i*3]);
    Bsum_x += double(edit2.data[i*3]);
    Bsum_y += double(img2.data[i*3]);
    Bsum_x2 += pow(double(edit2.data[i*3]), 2);

	Gsum_xy += double(img2.data[i*3+1] * edit2.data[i*3+1]);
    Gsum_x += double(edit2.data[i*3+1]);
    Gsum_y += double(img2.data[i*3+1]);
    Gsum_x2 += pow(double(edit2.data[i*3+1]), 2);

	Rsum_xy += double(img2.data[i*3+2] * edit2.data[i*3+2]);
    Rsum_x += double(edit2.data[i*3+2]);
    Rsum_y += double(img2.data[i*3+2]);
    Rsum_x2 += pow(double(edit2.data[i*3+2]), 2);

			fprintf(fpR,"%d %d\n", img.data[i*3] , edit.data[i*3]  );
			fprintf(fpG,"%d %d\n", img.data[i*3+1] , edit.data[i*3+1]  );
			fprintf(fpB,"%d %d\n", img.data[i*3+2] , edit.data[i*3+2]  );


/*

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
*/




	}


	fclose(fpR);
	fclose(fpG);
	fclose(fpB);
	system("wgnuplot -persist RGBdot.plt");//グラフの書式設定は testSali.plt に書きこんでおく


	//(a^2 + b^2) を小さくするという制限もいれる。ここでの　L は仮の値として
/*	int L = N/2;

  Ba = ( (N-L) * Bsum_xy - Bsum_x * Bsum_y) / ( (N-L) * (Bsum_x2-L) - pow(Bsum_x, 2));
  Bb = ((Bsum_x2-L) * Bsum_y - Bsum_xy * Bsum_x) / ((N-L) * (Bsum_x2-L) - pow(Bsum_x, 2));

  Ga = ( (N-L) * Gsum_xy - Gsum_x * Gsum_y) / ( (N-L) * (Gsum_x2-L) - pow(Gsum_x, 2));
  Gb = ((Gsum_x2-L) * Gsum_y - Gsum_xy * Gsum_x) / ((N-L) * (Gsum_x2-L) - pow(Gsum_x, 2));

  Ra = ( (N-L) * Rsum_xy - Rsum_x * Rsum_y) / ( (N-L) * (Rsum_x2-L) - pow(Rsum_x, 2));
  Rb = ((Rsum_x2-L) * Rsum_y - Rsum_xy * Rsum_x) / ((N-L) * (Rsum_x2-L) - pow(Rsum_x, 2));
*/
 

  Ba = (N * Bsum_xy - Bsum_x * Bsum_y) / ( N * Bsum_x2 - pow(Bsum_x, 2));
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


	//RGBで同じにする
/*	RGBa = (N * RGBsum_xy - RGBsum_x * RGBsum_y) / (N * RGBsum_x2 - pow(RGBsum_x, 2));
  RGBb = (RGBsum_x2 * RGBsum_y - RGBsum_xy * RGBsum_x) / (N * RGBsum_x2 - pow(RGBsum_x, 2));

  printf("RGBa = %f\n", RGBa);
  printf("RGBb = %f\n", RGBb);
*/

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


  //RGBで同じ
/*    for(int t=0; t<3*N; t++){

	  int after = int(edit.data[t] * RGBa + RGBb +0.5);
	  if(after> 255)after=255;
	  if(after< 0)after=0;
		out.data[t] = after;
  }
*/

  cv::imwrite(outputfile,out);

	cv::waitKey();
  return 0;


}
