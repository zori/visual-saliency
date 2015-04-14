/*
最小二乗法

11/02 とりあえず、与えられたデータに対する最小二乗を動かしてみる

11/08 16分割したときの最小二乗値を求める 16test.bmp
	  できたが、相変わらずブロックがひどいので、双線形補間にしてみた。できた！

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

cv::Mat Mini16block(const cv::Mat edit,int count);
void MiniMini16ab(cv::Mat edit, cv::Mat img,int xb ,int yb);
double bilinear_ave(cv::Mat block ,int i, int rows, int cols);

cv::Mat blockBa(4, 4, CV_64FC1); //16分割の最小二乗の結果をグローバル保管
cv::Mat blockGa(4, 4, CV_64FC1);
cv::Mat blockRa(4, 4, CV_64FC1);
cv::Mat blockBb(4, 4, CV_64FC1);
cv::Mat blockGb(4, 4, CV_64FC1);
cv::Mat blockRb(4, 4, CV_64FC1);


char Infile[] = "after_kaisuu001.bmp"; //入力元ファイル名
char editfile[]= "after_kaisuu036.bmp"; //更新後画像ファイル名
char outputfile[]="16test_bilinear.bmp";//規格化後出力ファイル名


//#define N 131072 /* データ数 */

int main(void){
	

	cv::Mat edit;
	edit = cv::imread(editfile);

	cv::Mat out(edit.rows, edit.cols, CV_8UC3);
	
	out = Mini16block( edit,1);
 

	cv::imwrite(outputfile,out);
 
	cv::waitKey();
	return 0;


}

//16ブロックにわける
cv::Mat Mini16block(const cv::Mat edit,int count){
	cv::Mat img;
	img = cv::imread(Infile);
	
	int rows=img.rows;
	int cols=img.cols;
	int i;

	int N = rows*cols;
		

	cv::Mat after(rows, cols, CV_8UC3);


	//元画像を 16分割
	int xstart = 0;
	int ystart = 0;

	for(int yb=0; yb<4 ; yb++){
		for(int xb=0; xb<4 ; xb++){
			xstart = xb* (cols/4);
			ystart = yb* (rows/4);

				cv::Mat pre16(rows/4, cols/4, CV_8UC3);
				cv::Mat edit16(rows/4, cols/4, CV_8UC3);
					cv::Mat Input16(rows/4, cols/4, CV_8UC3);


				for( int x=0 ; x < cols/4 ; x++ ){
					for( int y=0 ; y < rows/4 ; y++ ){
						int x2 = xstart + x;
						int y2 = ystart + y;

						edit16.data[ (y*cols/4+x)*3 ] = edit.data[ (y2 *cols+x2)*3 ];
						edit16.data[ (y*cols/4+x)*3 +1 ] = edit.data[ (y2 *cols+x2)*3 +1 ];
						edit16.data[ (y*cols/4+x)*3 +2 ] = edit.data[ (y2 *cols+x2)*3 +2 ];

						Input16.data[ (y*cols/4+x)*3 ] = img.data[ (y2 *cols+x2)*3 ];
						Input16.data[ (y*cols/4+x)*3 +1 ] = img.data[ (y2 *cols+x2)*3 +1 ];
						Input16.data[ (y*cols/4+x)*3 +2 ] = img.data[ (y2 *cols+x2)*3 +2 ];

					}
				}

			cout<<"(xb,yb)=("<<xb<<","<<yb<<")"<<endl;//確認用

			MiniMini16ab(edit16 , Input16,xb,yb);

/*
			for( int x=0 ; x < cols/4 ; x++ ){
				for( int y=0 ; y < rows/4 ; y++ ){
					int x2 = xstart + x;
					int y2 = ystart + y;

					after.data[ (y2*cols+x2)*3 ] = pre16.data[ (y*cols/4+x)*3 ];
					after.data[ (y2*cols+x2)*3 +1 ] = pre16.data[ (y*cols/4+x)*3 +1 ];
					after.data[ (y2*cols+x2)*3 +2 ] = pre16.data[ (y*cols/4+x)*3 +2 ];
					}
				}
*/

		}
	}

	for(int i=0; i<rows*cols ; i++){

		double Ba_ave = bilinear_ave(blockBa ,i, rows, cols);
		double Bb_ave = bilinear_ave(blockBb ,i, rows, cols);
		double Ga_ave = bilinear_ave(blockGa ,i, rows, cols);
		double Gb_ave = bilinear_ave(blockGb ,i, rows, cols);
		double Ra_ave = bilinear_ave(blockRa ,i, rows, cols);
		double Rb_ave = bilinear_ave(blockRb ,i, rows, cols);



		int B = int(Ba_ave*edit.data[i*3] + Bb_ave + 0.5);
		int G = int(Ga_ave*edit.data[i*3+1] + Gb_ave + 0.5);
		int R = int(Ra_ave*edit.data[i*3+2] + Rb_ave + 0.5);


		  if(B> 255)B=255;
	  if(G> 255)G=255;
	  if(R> 255)R=255;

	  if(B< 0)B=0;
	  if(G< 0)G=0;
	  if(R< 0)R=0;

	    after.data[i*3] = B;
	  after.data[i*3+1] = G;
	  after.data[i*3+2] = R;

	}

	return(after);

}


void MiniMini16ab(cv::Mat edit, cv::Mat img,int xb ,int yb){
	
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
//		FILE *fpR;
//		FILE *fpG;
//		FILE *fpB;
//		fpR = fopen("R.dat", "w");
//		fpG = fopen("G.dat", "w");
//		fpB = fopen("B.dat", "w");

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

//			fprintf(fpR,"%d %d\n", img.data[i*3] , edit.data[i*3]  );
//			fprintf(fpG,"%d %d\n", img.data[i*3+1] , edit.data[i*3+1]  );
//			fprintf(fpB,"%d %d\n", img.data[i*3+2] , edit.data[i*3+2]  );
	}

//	fclose(fpR);
//	fclose(fpG);
//	fclose(fpB);
//	system("wgnuplot -persist RGBdot.plt");//グラフの書式設定は testSali.plt に書きこんでおく

 
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

  blockBa.at<double>(yb,xb)= Ba;
  blockBb.at<double>(yb,xb)= Bb;
	blockGa.at<double>(yb,xb)= Ga;
	blockGb.at<double>(yb,xb)= Gb;
	blockRa.at<double>(yb,xb)= Ra;
	blockRb.at<double>(yb,xb)= Rb;


/*
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
 

//	cv::imwrite(outputfile,out);

*/
  
//	cv::waitKey();


}


//16個の周辺の値を双線形補間してならす
double bilinear_ave(cv::Mat block ,int i, int rows, int cols){
	int x = i%cols;
	int y = i/cols;

	int yoko = cols/4;
	int tate = rows/4;

	int yokohalf = cols/8;
	int tatehalf = rows/8;

	int left =0;
	int right =0;
	int top =0;
	int bottom =0;


	if(x<=yokohalf){
		left = 0;
		right = 0;
	}
	
	if(x>= yokohalf*7){
		left = 3;
		right = 3;	
	}

	if(x>yokohalf && x< yokohalf*7){
		left = (x-yokohalf)/yoko;
		right = left+1;
	}

	if(y<=tatehalf){
		top = 0;
		bottom = 0;		
	}
	
	if(y>=tatehalf*7){
		top = 3;
		bottom = 3;
	}
	
	if(y>tatehalf && y< tatehalf*7){
		top = (y-tatehalf)/tate;
		bottom = top+1;
	}

	//各四隅
	double lt = block.at<double>(top,left);
	double rt = block.at<double>(top,right);
	double lb = block.at<double>(bottom,left);
	double rb = block.at<double>(bottom,right);

	//双線形のときの比

	int p =0;
	int q=0;

	if (x <yokohalf){
		 p = x;
	}else{
	 p = (x-yokohalf)%yoko;
	}

	if(y < tatehalf){
		 q = y;
	}else{
	 q = (y-tatehalf)%tate;
	}


	double p2 = double((p+0.0)/(yoko+0.0));
	double q2 = double((q+0.0)/(tate+0.0));

	double ave = 0.0;
	ave = (1-q2)*((1-p2)*lt + p2*rt) + q2*((1-p2)*lb + p2*rb);

	return(ave);
}