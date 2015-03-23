/*
最小二乗法

11/02 とりあえず、与えられたデータに対する最小二乗を動かしてみる

11/16 a^2+b^2 も最小化の対象に入れてみる

11/20 杉本先生と打ち合わせたことをやる　1, RGBで統一 2,2乗ではなく絶対値距離で 3,中央値のRANSAC 4,ガウシアンを最後に 5,a*b の最小化
	  というわけで、a^2+b^2 は外して、RGB統一をつくる　→できた。MiniMiniRGB3 として、MiniMini　に組み込み済み
	  2,2乗ではなく絶対値距離....をやろうとしたけど、これは数学的に難しい??
	  ようなので、先に3番の中央値を試す

11/22 と思ったけど、とりあえずカーネルかけてやる

11/24 中央値をやる
*/
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>

#include <math.h> //最小二乗で追加

#include <ctime> // for time()
#include <cstdlib> // for srand(), rand()


#pragma comment(lib,"cv200d.lib")
#pragma comment(lib,"cxcore200d.lib")
#pragma comment(lib,"highgui200d.lib")

#pragma comment(lib,"cv200.lib")
#pragma comment(lib,"cxcore200.lib")
#pragma comment(lib,"highgui200.lib")

using namespace std;


double median(cv::Mat in);
double HistMedi(cv::Mat In);

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

	//仮の最良値
	int preMedi=257;//とにかく大きい値

	//乱数生成
	int MIN =0;//乱数の最小
	int MAX =N;//最大
	int COUNT=5000; //試験回数

	int RAN[10000]; //試験回数の2倍作っておく

	 srand(time(NULL)); // 現在時刻を乱数の種の設定  
	for (int i = 0; i< 2*COUNT; i++) { // 100個生成    
		 RAN[i] = MIN + rand() % (MAX - MIN) ;  
	}

	cv::Mat outMedi(rows, cols,CV_8UC3);
				for(int j=0 ; j<COUNT ; j++){
					int RAN1 = RAN[2*j];
					int RAN2 = RAN[2*j+1];

					if(RAN1==RAN2){
						RAN2 = RAN[j];
					}

	int b1=img.data[RAN1*3];
	int g1=img.data[RAN1*3+1];
	int r1=img.data[RAN1*3+2];
	int b2=img.data[RAN2*3];
	int g2=img.data[RAN2*3+1];
	int r2=img.data[RAN2*3+2];

//	cout<<b1<<","<<g1<<","<<r1<<endl;
//	cout<<b2<<","<<g2<<","<<r2<<endl;
//	cout<<"y1,"<<b1<<endl;
//	cout<<"y2,"<<b2<<endl;


		int b12=edit.data[RAN1*3];
	int g12=edit.data[RAN1*3+1];
	int r12=edit.data[RAN1*3+2];
	int b22=edit.data[RAN2*3];
	int g22=edit.data[RAN2*3+1];
	int r22=edit.data[RAN2*3+2];

//	cout<<b12<<","<<g12<<","<<r12<<endl;
//	cout<<b22<<","<<g22<<","<<r22<<endl;
//	cout<<"x1,"<<b12<<endl;
//	cout<<"x2,"<<b22<<endl;


	Bsum_xy = double(img.data[RAN1*3] * edit.data[RAN1*3]) + double(img.data[RAN2*3] * edit.data[RAN2*3]);
    Bsum_x = double(edit.data[RAN1*3]) + double(edit.data[RAN2*3]);
    Bsum_y = double(img.data[RAN1*3]) + double(img.data[RAN2*3]);
    Bsum_x2 = pow(double(edit.data[RAN1*3]), 2) + pow(double(edit.data[RAN2*3]), 2);
/*
		cout<<"sumxy,"<<Bsum_xy<<endl;
				cout<<"sumx,"<<Bsum_x<<endl;
						cout<<"sumy,"<<Bsum_y<<endl;
						cout<<"sumx2,"<<Bsum_x2<<endl;
*/

	Gsum_xy = double(img.data[RAN1*3+1] * edit.data[RAN1*3+1]) + double(img.data[RAN2*3+1] * edit.data[RAN2*3+1]);
    Gsum_x = double(edit.data[RAN1*3+1]) + double(edit.data[RAN2*3+1]);
    Gsum_y = double(img.data[RAN1*3+1]) + double(img.data[RAN2*3+1]);
    Gsum_x2 = pow(double(edit.data[RAN1*3+1]), 2) + pow(double(edit.data[RAN2*3+1]), 2);

	Rsum_xy = double(img.data[RAN1*3+2] * edit.data[RAN1*3+2]) + double(img.data[RAN2*3+2] * edit.data[RAN2*3+2]);
    Rsum_x = double(edit.data[RAN1*3+2]) + double(edit.data[RAN2*3+2]);
    Rsum_y = double(img.data[RAN1*3+2]) + double(img.data[RAN2*3+2]);
    Rsum_x2 = pow(double(edit.data[RAN1*3+2]), 2) + pow(double(edit.data[RAN2*3+2]), 2);

  Ba = (2 * Bsum_xy - Bsum_x * Bsum_y) / ( 2 * Bsum_x2 - pow(Bsum_x, 2));
  Bb = (Bsum_x2 * Bsum_y - Bsum_xy * Bsum_x) / (2 * Bsum_x2 - pow(Bsum_x, 2));

  Ga = (2 * Gsum_xy - Gsum_x * Gsum_y) / (2 * Gsum_x2 - pow(Gsum_x, 2));
  Gb = (Gsum_x2 * Gsum_y - Gsum_xy * Gsum_x) / (2 * Gsum_x2 - pow(Gsum_x, 2));

  Ra = (2 * Rsum_xy - Rsum_x * Rsum_y) / (2 * Rsum_x2 - pow(Rsum_x, 2));
  Rb = (Rsum_x2 * Rsum_y - Rsum_xy * Rsum_x) / (2 * Rsum_x2 - pow(Rsum_x, 2));
/*
  printf("Ba = %f\n", Ba);
  printf("Bb = %f\n", Bb);

  printf("Ga = %f\n", Ga);
  printf("Gb = %f\n", Gb);

  printf("Ra = %f\n", Ra);
  printf("Rb = %f\n", Rb);
*/

  cv::Mat out(rows, cols, CV_8UC3);


  cv::Mat check(1,N,CV_64FC1);
    cv::Mat So(1,N,CV_64FC1);

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

	  check.at<double>(0,t) = sqrt(( ( B -img.data[t*3])*( B -img.data[t*3])+( G -img.data[t*3+1])*( G -img.data[t*3+1]) + (R -img.data[t*3+2])*( R -img.data[t*3+2]))/3.0);
  }
  cv::sort(check,So,CV_SORT_ASCENDING);
  int half = N/2;
  double Medi = (So.at<double>(0,half) + So.at<double>(0,half-1) )/2;

//  double Medi=  HistMedi(Sort);

  cout<<"Median  "<<j<<":"<<Medi<<endl;

  if(Medi<preMedi){
	  preMedi=Medi;
	outMedi=out;
		FILE *fp2;
	fp2 = fopen("outMedidata_5000_5.dat", "w");
			fprintf(fp2,"%d \n", j);
			fprintf(fp2,"%f \n", Medi);
			fprintf(fp2,"%f \n", Ba);
			fprintf(fp2,"%f \n", Bb);
			fprintf(fp2,"%f \n", Ga);
			fprintf(fp2,"%f \n", Gb);
			fprintf(fp2,"%f \n", Ra);
			fprintf(fp2,"%f \n", Rb);
			fprintf(fp2,"%d \n", b1);
			fprintf(fp2,"%d \n", b2);
			fprintf(fp2,"%d \n", b12);
			fprintf(fp2,"%d \n", b22);
	fclose(fp2);	

	//ファイルここまで

  }

  }


  cv::imwrite(outputfile,outMedi);


	cv::waitKey();
  return 0;


}


//基本形  
/*
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
*/
/*
//カーネル利用
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






	}


	fclose(fpR);
	fclose(fpG);
	fclose(fpB);
	system("wgnuplot -persist RGBdot.plt");//グラフの書式設定は testSali.plt に書きこんでおく


 

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
*/



//配列から中央値を出力する
double median(cv::Mat in){

	cv::Mat M=in;

	int n = M.cols*M.rows;      // データ数
   // int data[ MAX_SIZE ]; // データ配列
    double median;        // 中央値


    // データ配列を昇順に整列(バブルソート)
    for(int i = 0; i < n - 1; i++ ){
        for(int j = n - 1; j > i; j-- ){
            if( M.data[ j ] < M.data[ j - 1 ] ){
                double tmp = M.data[ j ];
                M.data[ j ] = M.data[ j - 1 ];
                M.data[ j - 1 ] = tmp;
            }
        }
    }

    // 中央値計算
    if( n % 2 == 1 ){
        median = M.data[ n / 2 ];
    }
    else{
        median = ( M.data[ n / 2 - 1 ] + M.data[ n / 2 ] ) / 2.0;
    }

return(median);
}

//ヒストグラムを利用して中央値を出力する
double HistMedi(cv::Mat In){

	// 256 分割レベルで量子化します
	int binNum = 256;
	int histSize[] = {binNum};//

	float value_ranges[] = { 0, 256 };
	const float* ranges[] = { value_ranges };
	cv::MatND hist;
	// 0 番目のチャンネルからヒストグラムを求めます．
	int channels[] = {0};
	int dims = 1; // 入力行列（画像）のチャンネル数

	cv::calcHist( &In, 1, channels, cv::Mat(), // マスクは利用しません
		hist, dims, histSize, ranges,
		true, // ヒストグラムは一様です
		false );

//	double maxVal=0;
//	minMaxLoc(histB, 0, &maxVal, 0, 0);


	//gnuplotでepsのグラフ作成するため、datファイルをオープン
	FILE *fp;
	fp = fopen("histMedi.dat", "w");
	for( int binId = 0; binId < binNum; binId++ ){
		float binVal = hist.at<float>(binId);
		fprintf(fp,"%d %d\n", binId, int(binVal) );
	}
	fclose(fp);	
//	system("wgnuplot -persist plot_hist.plt");//グラフの書式設定は testSali.plt に書きこんでおく
	//ファイルここまで

	double median=0.0;
	int count=0;
	int half =In.rows*In.cols/2;
	for(int i=0;i<binNum; i++){
		count += int(hist.at<float>(i));
		if(count == half){
			median= double(i) -0.5;
			break;
		}else if(count> half){
			median = double(i);
			break;
		}
	}
	return(median);
}