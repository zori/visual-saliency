/*
2枚の差を求める

4/21 点ごとにわかるように改良
7/



*/

//#include <cvgabor.h> //ガボール関数を使う為　http: //iplab.shinshu-u.ac.jp/index.php?Gabor_Filter より拝借
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <algorithm>

#include<math.h>

#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib,"cv200d.lib")
#pragma comment(lib,"cxcore200d.lib")
#pragma comment(lib,"highgui200d.lib")

#pragma comment(lib,"cv200.lib")
#pragma comment(lib,"cxcore200.lib")
#pragma comment(lib,"highgui200.lib")

using namespace std;


cv::Mat sa_AVG(const cv::Mat);

cv::Mat pointsa(const cv::Mat, const cv::Mat);

cv::Mat sa_point(const cv::Mat, const cv::Mat );





char basefileA[] = "after_kaisuu001.bmp"; //元ファイル名

char basefileB[] = "Hiroshima_07.jpg"; //元ファイル名


char A1[] = "RGBMini.bmp"; //比較ファイル名
char A2[] = "k16_g2.bmp"; //比較ファイル名
char A3[] = "k9G_g2.bmp"; //比較ファイル名
char A4[] = "k16G_g2.bmp"; //比較ファイル名

char outA1[] = "hikaku_W_H11.bmp";//出力ファイル名
char outA2[] = "hikaku_GL_H11.bmp";//出力ファイル名
char outA3[] = "hikaku_edge_goal2.bmp";//出力ファイル名


char B1[] = "k9_H07.bmp"; //比較ファイル名
char B2[] = "k16_H07.bmp"; //比較ファイル名
char B3[] = "k9G_H07.bmp"; //比較ファイル名
char B4[] = "k16G_H07.bmp"; //比較ファイル名

char outB1[] = "hikaku_W_H02.bmp";//出力ファイル名
char outB2[] = "hikaku_GL_H02.bmp";//出力ファイル名
char outB3[] = "hikaku_edge_H07.bmp";//出力ファイル名



int main(void){

	//初期準備
	
	//比較する画像読み込み
	cv::Mat baseA;
	baseA = cv::imread(basefileA);
	cv::Mat baseB;
	baseB = cv::imread(basefileB);

	cv::Mat A1img;
	A1img = cv::imread(A1);
	cv::Mat A2img;
	A2img = cv::imread(A2);
	cv::Mat A3img;
	A3img = cv::imread(A3);
		cv::Mat A4img;
	A4img = cv::imread(A4);

	cv::Mat B1img;
	B1img = cv::imread(B1);
	cv::Mat B2img;
	B2img = cv::imread(B2);
	cv::Mat B3img;
	B3img = cv::imread(B3);
		cv::Mat B4img;
	B4img = cv::imread(B4);

	cv::Mat A1out;
	cv::Mat A2out;
	cv::Mat A3out;
		cv::Mat A4out;
	cv::Mat B1out;
	cv::Mat B2out;
	cv::Mat B3out;
		cv::Mat B4out;

//	cv::absdiff(base,A1img,A1out);
//	cv::imwrite(outA1,A1out);

	A1out = sa_point(baseA, A1img);
	cv::imwrite(outA1,A1out);
	cv::Mat A1AVG = sa_AVG(A1out);
	double A1ave=0.0;
	double A1bun=0.0;
	A1ave = A1AVG.at<double>(0,0);
	A1bun = A1AVG.at<double>(0,1);

	A2out = sa_point(baseA, A2img);
	cv::imwrite(outA2,A2out);
	cv::Mat A2AVG = sa_AVG(A2out);
	double A2ave=0.0;
	double A2bun=0.0;
	A2ave = A2AVG.at<double>(0,0);
	A2bun = A2AVG.at<double>(0,1);

	A3out = sa_point(baseA, A3img);
	cv::imwrite(outA3,A3out);
	cv::Mat A3AVG = sa_AVG(A3out);
	double A3ave=0.0;
	double A3bun=0.0;
	A3ave = A3AVG.at<double>(0,0);
	A3bun = A3AVG.at<double>(0,1);

	A4out = sa_point(baseA, A4img);
//	cv::imwrite(outA4,A4out);
	cv::Mat A4AVG = sa_AVG(A4out);
	double A4ave=0.0;
	double A4bun=0.0;
	A4ave = A4AVG.at<double>(0,0);
	A4bun = A4AVG.at<double>(0,1);

	B1out = sa_point(baseB, B1img);
	cv::imwrite(outB1,B1out);
	cv::Mat B1AVG = sa_AVG(B1out);
	double B1ave=0.0;
	double B1bun=0.0;
	B1ave = B1AVG.at<double>(0,0);
	B1bun = B1AVG.at<double>(0,1);

	B2out = sa_point(baseB, B2img);
	cv::imwrite(outB2,B2out);
	cv::Mat B2AVG = sa_AVG(B2out);
	double B2ave=0.0;
	double B2bun=0.0;
	B2ave = B2AVG.at<double>(0,0);
	B2bun = B2AVG.at<double>(0,1);

	B3out = sa_point(baseB, B3img);
	cv::imwrite(outB3,B3out);
	cv::Mat B3AVG = sa_AVG(B3out);
	double B3ave=0.0;
	double B3bun=0.0;
	B3ave = B3AVG.at<double>(0,0);
	B3bun = B3AVG.at<double>(0,1);

		B4out = sa_point(baseB, B4img);
//	cv::imwrite(outB4,B4out);
	cv::Mat B4AVG = sa_AVG(B4out);
	double B4ave=0.0;
	double B4bun=0.0;
	B4ave = B4AVG.at<double>(0,0);
	B4bun = B4AVG.at<double>(0,1);



	////A1 とbaseA の中央値の出力
	int rows=A1img.rows;
	int cols=A1img.cols;

	int N = rows*cols;

	cv::Mat check(1,N,CV_64FC1);
    cv::Mat So(1,N,CV_64FC1);

  for(int t=0; t<N; t++){
	  int B1 = baseA.data[t*3];
	  int B2 = A1img.data[t*3];
	  int G1 = baseA.data[t*3+1];
	  int G2 = A1img.data[t*3+1];
	  int R1 = baseA.data[t*3+2];
	  int R2 = A1img.data[t*3+2];

	  check.at<double>(0,t) = sqrt(( ( B1 - B2)*( B1 - B2)+( G1 - G2)*( G1 - G2) + (R1 - R2)*( R1 - R2))/3.0);
  }
  cv::sort(check,So,CV_SORT_ASCENDING);
  int half = N/2;
	double Medi = (So.at<double>(0,half) + So.at<double>(0,half-1) )/2;
	
	cout<<Medi<<endl;



	//分散の値をdatファイルに保存
	FILE *zahyou;
	zahyou = fopen("gaaa.dat","w");//座標保存用
	fprintf(zahyou,"A1: %06.3f %06.3f\n", A1ave, A1bun);
	fprintf(zahyou,"A2: %06.3f %06.3f\n", A2ave, A2bun);
	fprintf(zahyou,"A3: %06.3f %06.3f\n", A3ave, A3bun);
	fprintf(zahyou,"A4: %06.3f %06.3f\n", A4ave, A4bun);
	fprintf(zahyou,"B1: %06.3f %06.3f\n", B1ave, B1bun);
	fprintf(zahyou,"B2: %06.3f %06.3f\n", B2ave, B2bun);
	fprintf(zahyou,"B3: %06.3f %06.3f\n", B3ave, B3bun);
	fprintf(zahyou,"B4: %06.3f %06.3f\n", B4ave, B4bun);
	fclose(zahyou);





	
	
	cv::waitKey();
	return 0;
}



	



//全体の差分の分散を求める関数 引数(入力,出力)
cv::Mat sa_AVG(const cv::Mat hikaku){

	int rows = hikaku.rows;
	int cols = hikaku.cols;

	cv::Mat AVG(1,2,CV_64FC1);

	int sum = 0;

	for(int i =0 ; i<(rows*cols) ; i++){
		sum = sum + hikaku.data[i];
	}

	double ave = 0.0;
	ave = (sum+0.0)/(rows*cols+0.0);

	AVG.at<double>(0,0) = ave;

	double siguma = 0.0;
	for(int i =0 ; i<(rows*cols) ; i++){
		siguma = siguma + (ave -hikaku.data[i])*(ave - hikaku.data[i]);
	}

	double bunnsann =0.0;
	bunnsann = siguma/ (rows*cols - 1.0);

	AVG.at<double>(0,1) = bunnsann;


	return(AVG);
	

}



cv::Mat sa_point(const cv::Mat base, const cv::Mat hikaku){

	cv::Mat point(base.rows, base.cols, CV_8UC1, cv::Scalar(0));

	int rows = base.rows;
	int cols = base.cols;

	int pre = 0;
	double ave =0.0;

	for(int i = 0; i<(rows*cols) ; i++){
		pre = ( abs(base.data[i*3] - hikaku.data[i*3]) +abs(base.data[i*3+1] - hikaku.data[i*3+1])+abs(base.data[i*3+2] - hikaku.data[i*3+2]) )/3;
		
		if(pre>255){
			pre = 255;
		}
		point.data[i] = pre;
	}

return point;


}