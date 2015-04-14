/*
最小二乗法

11/02 とりあえず、与えられたデータに対する最小二乗を動かしてみる

11/13 上下左右の差分とも比較する　(16分割はしない)

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

char inputfile[] = "Hiroshima_07.jpg"; //入力元ファイル名
char editfile[]= "after_kaisuu036.bmp"; //更新後画像ファイル名
char outputfile[]="test.bmp";//規格化後出力ファイル名

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

	//上下左右の差分を格納
	cv::Mat imgUpB(rows, cols, CV_64FC1);
	cv::Mat editUpB(rows, cols, CV_64FC1);
	cv::Mat imgDownB(rows, cols, CV_64FC1);
	cv::Mat editDownB(rows, cols, CV_64FC1);
	cv::Mat imgLeftB(rows, cols, CV_64FC1);
	cv::Mat editLeftB(rows, cols, CV_64FC1);
	cv::Mat imgRightB(rows, cols, CV_64FC1);
	cv::Mat editRightB(rows, cols, CV_64FC1);

	cv::Mat imgUpG(rows, cols, CV_64FC1);
	cv::Mat editUpG(rows, cols, CV_64FC1);
	cv::Mat imgDownG(rows, cols, CV_64FC1);
	cv::Mat editDownG(rows, cols, CV_64FC1);
	cv::Mat imgLeftG(rows, cols, CV_64FC1);
	cv::Mat editLeftG(rows, cols, CV_64FC1);
	cv::Mat imgRightG(rows, cols, CV_64FC1);
	cv::Mat editRightG(rows, cols, CV_64FC1);

	cv::Mat imgUpR(rows, cols, CV_64FC1);
	cv::Mat editUpR(rows, cols, CV_64FC1);
	cv::Mat imgDownR(rows, cols, CV_64FC1);
	cv::Mat editDownR(rows, cols, CV_64FC1);
	cv::Mat imgLeftR(rows, cols, CV_64FC1);
	cv::Mat editLeftR(rows, cols, CV_64FC1);
	cv::Mat imgRightR(rows, cols, CV_64FC1);
	cv::Mat editRightR(rows, cols, CV_64FC1);



//傾斜をかける　
double kk = 1.0;

	for(int y =0 ; y<rows ; y++){
		for(int x =0 ; x<cols ; x++){
			int i = x + y*cols;

//cout<<y<<","<<x<<endl;

			if(y==0){//Up の端処理
				imgUpB.at<double>(y,x) =0.0;
				editUpB.at<double>(y,x) =0.0;
				imgDownB.at<double>(y,x) =kk* double( img.data[i*3] - img.data[(i+cols)*3] );
				editDownB.at<double>(y,x) = kk*double( edit.data[i*3] - edit.data[(i+cols)*3] );

				imgUpG.at<double>(y,x) =0.0;
				editUpG.at<double>(y,x) =0.0;
				imgDownG.at<double>(y,x) = kk* double( img.data[i*3+1] - img.data[(i+cols)*3+1] );
				editDownG.at<double>(y,x) = kk* double( edit.data[i*3+1] - edit.data[(i+cols)*3+1] );

				imgUpR.at<double>(y,x) =0.0;
				editUpR.at<double>(y,x) =0.0;
				imgDownR.at<double>(y,x) = kk* double( img.data[i*3+2] - img.data[(i+cols)*3+2] );
				editDownR.at<double>(y,x) = kk* double( edit.data[i*3+2] - edit.data[(i+cols)*3+2] );
			}else if(y==(rows-1)){//Down の端処理
				imgDownB.at<double>(y,x) = 0.0;
				editDownB.at<double>(y,x) = 0.0;
				imgUpB.at<double>(y,x) = kk* double( img.data[i*3] - img.data[(i-cols)*3] );
				editUpB.at<double>(y,x) = kk* double( edit.data[i*3] - edit.data[(i-cols)*3] );

				imgDownG.at<double>(y,x) = 0.0;
				editDownG.at<double>(y,x) = 0.0;
				imgUpG.at<double>(y,x) = kk* double( img.data[i*3+1] - img.data[(i-cols)*3+1] );
				editUpG.at<double>(y,x) = kk* double( edit.data[i*3+1] - edit.data[(i-cols)*3+1] );

								imgDownR.at<double>(y,x) = 0.0;
				editDownR.at<double>(y,x) = 0.0;
				imgUpR.at<double>(y,x) = kk* double( img.data[i*3+2] - img.data[(i-cols)*3+2] );
				editUpR.at<double>(y,x) = kk* double( edit.data[i*3+2] - edit.data[(i-cols)*3+2] );
			}else if(y>0 && y<(rows-1)){//中心部分
				imgUpB.at<double>(y,x) = kk* double( img.data[i*3] - img.data[(i-cols)*3] );
				editUpB.at<double>(y,x) = kk* double( edit.data[i*3] - edit.data[(i-cols)*3] );
				imgDownB.at<double>(y,x) = kk* double( img.data[i*3] - img.data[(i+cols)*3] );
				editDownB.at<double>(y,x) = kk* double( edit.data[i*3] - edit.data[(i+cols)*3] );

				imgUpG.at<double>(y,x) = kk* double( img.data[i*3+1] - img.data[(i-cols)*3+1] );
				editUpG.at<double>(y,x) = kk* double( edit.data[i*3+1] - edit.data[(i-cols)*3+1] );
				imgDownG.at<double>(y,x) = kk* double( img.data[i*3+1] - img.data[(i+cols)*3+1] );
				editDownG.at<double>(y,x) = kk* double( edit.data[i*3+1] - edit.data[(i+cols)*3+1] );

				imgUpR.at<double>(y,x) = kk* double( img.data[i*3+2] - img.data[(i-cols)*3+2] );
				editUpR.at<double>(y,x) = kk* double( edit.data[i*3+2] - edit.data[(i-cols)*3+2] );
				imgDownR.at<double>(y,x) = kk* double( img.data[i*3+2] - img.data[(i+cols)*3+2] );
				editDownR.at<double>(y,x) = kk* double( edit.data[i*3+2] - edit.data[(i+cols)*3+2] );
			}

			if(x==0){//Left の端処理
				imgLeftB.at<double>(y,x) =0.0;
				editLeftB.at<double>(y,x) =0.0;
				imgRightB.at<double>(y,x) = kk* double( img.data[i*3] - img.data[(i+1)*3] );
				editRightB.at<double>(y,x) = kk* double( edit.data[i*3] - edit.data[(i+1)*3] );

				imgLeftG.at<double>(y,x) =0.0;
				editLeftG.at<double>(y,x) =0.0;
				imgRightG.at<double>(y,x) = kk* double( img.data[i*3+1] - img.data[(i+1)*3+1] );
				editRightG.at<double>(y,x) = kk* double( edit.data[i*3+1] - edit.data[(i+1)*3+1] );

				imgLeftR.at<double>(y,x) =0.0;
				editLeftR.at<double>(y,x) =0.0;
				imgRightR.at<double>(y,x) = kk* double( img.data[i*3+2] - img.data[(i+1)*3+2] );
				editRightR.at<double>(y,x) = kk* double( edit.data[i*3+2] - edit.data[(i+1)*3+2] );
			}else if(x==(cols-1)){//Right の端処理
				imgRightB.at<double>(y,x) = 0.0;
				editRightB.at<double>(y,x) = 0.0;
				imgLeftB.at<double>(y,x) = kk* ( img.data[i*3] - img.data[(i-1)*3] );
				editLeftB.at<double>(y,x) = kk* ( edit.data[i*3] - edit.data[(i-1)*3] );

				imgRightG.at<double>(y,x) = 0.0;
				editRightG.at<double>(y,x) = 0.0;
				imgLeftG.at<double>(y,x) = kk* ( img.data[i*3+1] - img.data[(i-1)*3+1] );
				editLeftG.at<double>(y,x) = kk* ( edit.data[i*3+1] - edit.data[(i-1)*3+1] );

				imgRightR.at<double>(y,x) = 0.0;
				editRightR.at<double>(y,x) = 0.0;
				imgLeftR.at<double>(y,x) = kk* ( img.data[i*3+2] - img.data[(i-1)*3+2] );
				editLeftR.at<double>(y,x) = kk* ( edit.data[i*3+2] - edit.data[(i-1)*3+2] );
			}else if(x>0 && x<(cols-1)){//中心部分
				imgLeftB.at<double>(y,x) = kk* ( img.data[i*3] - img.data[(i-1)*3] );
				editLeftB.at<double>(y,x) = kk* ( edit.data[i*3] - edit.data[(i-1)*3] );
				imgRightB.at<double>(y,x) = kk* double( img.data[i*3] - img.data[(i+1)*3] );
				editRightB.at<double>(y,x) = kk* double( edit.data[i*3] - edit.data[(i+1)*3] );

				imgLeftG.at<double>(y,x) = kk* ( img.data[i*3+1] - img.data[(i-1)*3+1] );
				editLeftG.at<double>(y,x) = kk*( edit.data[i*3+1] - edit.data[(i-1)*3+1] );
				imgRightG.at<double>(y,x) = kk* double( img.data[i*3+1] - img.data[(i+1)*3+1] );
				editRightG.at<double>(y,x) = kk* double( edit.data[i*3+1] - edit.data[(i+1)*3+1] );

				imgLeftR.at<double>(y,x) = kk* ( img.data[i*3+2] - img.data[(i-1)*3+2] );
				editLeftR.at<double>(y,x) = kk*( edit.data[i*3+2] - edit.data[(i-1)*3+2] );
				imgRightR.at<double>(y,x) = kk* double( img.data[i*3+2] - img.data[(i+1)*3+2] );
				editRightR.at<double>(y,x) = kk* double( edit.data[i*3+2] - edit.data[(i+1)*3+2] );
			}
/*
			double pre = imgLeftG.at<double>(y,x);
			if(pre>0 &&y<30){
				cout<<"+"<<y<<","<<x<<endl;
				cout<<pre<<endl;
			}else if(pre<0 &&y<30){
								cout<<"-"<<y<<","<<x<<endl;
				cout<<pre<<endl;
			}else{
cout<<"0"<<y<<","<<x<<endl;
			}
		*/

		}
	}

cout<<"a"<<endl;
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
		//差分入れの為
		int x = i%cols;
		int y = i/cols;

    Bsum_xy += double(img.data[i*3] * edit.data[i*3] + imgUpB.at<double>(y,x)* editUpB.at<double>(y,x)+ imgDownB.at<double>(y,x)* editDownB.at<double>(y,x)
		+ imgLeftB.at<double>(y,x)* editLeftB.at<double>(y,x) + imgRightB.at<double>(y,x)* editRightB.at<double>(y,x) );
    Bsum_x += double(edit.data[i*3] + editUpB.at<double>(y,x) + editDownB.at<double>(y,x) + editLeftB.at<double>(y,x) + editRightB.at<double>(y,x) );
    Bsum_y += double(img.data[i*3] + imgUpB.at<double>(y,x) + imgDownB.at<double>(y,x) + imgLeftB.at<double>(y,x) + imgRightB.at<double>(y,x) );
    Bsum_x2 += double( edit.data[i*3]*edit.data[i*3] + editUpB.at<double>(y,x)*editUpB.at<double>(y,x) + editDownB.at<double>(y,x)*editDownB.at<double>(y,x)
		+ editRightB.at<double>(y,x)*editRightB.at<double>(y,x) + editLeftB.at<double>(y,x)*editLeftB.at<double>(y,x) );

	Gsum_xy += double(img.data[i*3+1] * edit.data[i*3+1] + imgUpG.at<double>(y,x)* editUpG.at<double>(y,x)+ imgDownG.at<double>(y,x)* editDownG.at<double>(y,x)
		+ imgLeftG.at<double>(y,x)* editLeftG.at<double>(y,x) + imgRightG.at<double>(y,x)* editRightG.at<double>(y,x));
    Gsum_x += double(edit.data[i*3+1] + editUpG.at<double>(y,x) + editDownG.at<double>(y,x)  + editLeftG.at<double>(y,x) + editRightG.at<double>(y,x));
    Gsum_y += double(img.data[i*3+1]  + imgUpG.at<double>(y,x) + imgDownG.at<double>(y,x)  + imgLeftG.at<double>(y,x) + imgRightG.at<double>(y,x));
    Gsum_x2 +=double( edit.data[i*3+1]*edit.data[i*3+1] + editUpG.at<double>(y,x)*editUpG.at<double>(y,x) + editDownG.at<double>(y,x)*editDownG.at<double>(y,x)
		+ editRightG.at<double>(y,x)*editRightG.at<double>(y,x) + editLeftG.at<double>(y,x)*editLeftG.at<double>(y,x));

		Rsum_xy += double(img.data[i*3+2] * edit.data[i*3+2] + imgUpR.at<double>(y,x)* editUpR.at<double>(y,x)+ imgDownR.at<double>(y,x)* editDownR.at<double>(y,x)
		+ imgLeftR.at<double>(y,x)* editLeftR.at<double>(y,x) + imgRightR.at<double>(y,x)* editRightR.at<double>(y,x));
    Rsum_x += double(edit.data[i*3+2] + editUpR.at<double>(y,x) + editDownR.at<double>(y,x)  + editLeftR.at<double>(y,x) + editRightR.at<double>(y,x));
    Rsum_y += double(img.data[i*3+2]  + imgUpR.at<double>(y,x) + imgDownR.at<double>(y,x)  + imgLeftR.at<double>(y,x) + imgRightR.at<double>(y,x));
    Rsum_x2 +=double( edit.data[i*3+2]*edit.data[i*3+1] + editUpR.at<double>(y,x)*editUpR.at<double>(y,x) + editDownR.at<double>(y,x)*editDownR.at<double>(y,x)
		+ editRightR.at<double>(y,x)*editRightR.at<double>(y,x) + editLeftR.at<double>(y,x)*editLeftR.at<double>(y,x));


			fprintf(fpR,"%d %d\n", img.data[i*3] , edit.data[i*3]  );
			fprintf(fpG,"%d %d\n", img.data[i*3+1] , edit.data[i*3+1]  );
			fprintf(fpB,"%d %d\n", img.data[i*3+2] , edit.data[i*3+2]  );
	}

	fclose(fpR);
	fclose(fpG);
	fclose(fpB);
//	system("wgnuplot -persist RGBdot.plt");//グラフの書式設定は testSali.plt に書きこんでおく

 
  Ba = (N * Bsum_xy - Bsum_x * Bsum_y) / (N * Bsum_x2 - pow(Bsum_x, 2));
  Bb = (Bsum_x2 * Bsum_y - Bsum_xy * Bsum_x) / (N * Bsum_x2 - pow(Bsum_x, 2));

  Ga = (N * Gsum_xy - Gsum_x * Gsum_y) / (N * Gsum_x2 - pow(Gsum_x, 2));
  Gb = (Gsum_x2 * Gsum_y - Gsum_xy * Gsum_x) / (N * Gsum_x2 - pow(Gsum_x, 2));

  Ra = (N * Rsum_xy - Rsum_x * Rsum_y) / (N * Rsum_x2 - pow(Rsum_x, 2));
  Rb = (Rsum_x2 * Rsum_y - Rsum_xy * Rsum_x) / (N * Rsum_x2 - pow(Rsum_x, 2));
/*
   printf("Bsumxy = %f\n", Bsum_xy);
  printf("Bsumx = %f\n", Bsum_x);

     printf("Gsumxy = %f\n", Gsum_xy);
  printf("Gsumx = %f\n", Gsum_x);

       printf("Rsumxy = %f\n", Rsum_xy);
  printf("Rsumx = %f\n", Rsum_x);
*/
 

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
