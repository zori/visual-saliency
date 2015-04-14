/*
最小二乗法

11/02 とりあえず、与えられたデータに対する最小二乗を動かしてみる
11/04 input と edit 画像から、edit をinput に寄せる為の係数計算

	GauEdge Lay に組み込んで、更新する度に、input画像に寄せる計算

11/05 HSVで最小二乗してみた Hは固定、SVで最小二乗

11/09 RGB で16分割の最小二乗

11/16 Lを入れて見る(HSVも16分割も外した)

11/20 RGBで統一した正規化ver (Lを外した)

11/22 ガウシアンかけてから正規化の順で保存
      そして、カーネルをかける関数を実装。良さ気。これやってからガウシアンを、本家MiniMiniで試す！！


12/17 
	   edge_distanceに関して、
	//探索範囲の閾値設定
	int  karnel = 7;//探索範囲の腕の長さの閾値 元は8だったところを7に変更　した（意味を考慮して）

	正規化の際のカーネルを3*3の中だけで9や16を利用するのではなく、
	スケール考慮ガウシアンで設定したガウシアンをかける範囲を利用してやってみたい
	というわけでbigloop もいじりつつ、
	MiniMiniGau　を実装　まぁ思った通りには動いてるっぽい

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

//#define N 131072 /* データ数 */


cv::Mat makeI(const cv::Mat,cv::Mat);
cv::Mat makeBGR(const cv::Mat,cv::Mat,int,int,int);
cv::Mat makeY(const cv::Mat ,cv::Mat);

cv::Mat gabor( double , double ,char[9],double);

cv::Mat sabunnI(const cv::Mat, const cv::Mat);
cv::Mat sabunnD(const cv::Mat, const cv::Mat);
cv::Mat tadanosa(const cv::Mat, const cv::Mat);
cv::Mat wa(const cv::Mat , const cv::Mat );

cv::Mat noramlization(cv::Mat);
cv::Mat normali64(cv::Mat);
cv::Mat hirei(cv::Mat img);
cv::Mat sal(const cv::Mat , const cv::Mat);

cv::Mat DownSample(const cv::Mat ,  int );
cv::Mat UpSample(const cv::Mat ,  int );
void Markver2(cv::Mat ,const cv::Mat);
char nameMark[40];

void Marklocal(cv::Mat);

void changeoutput(cv::Mat , char[] );

cv::Mat DownSample8(const cv::Mat ,  int );
cv::Mat UpSample8(const cv::Mat ,  int );

cv::Mat Small(const cv::Mat ,  int );
cv::Mat Big(const cv::Mat ,  int );
cv::Mat Small8(const cv::Mat ,  int );
cv::Mat Big8(const cv::Mat ,  int );

void change2(cv::Mat , char[]);
void change3(cv::Mat  , char[],cv::Mat);
void changeSali(cv::Mat , char[]);

void changeBig(cv::Mat , char []);

cv::Mat adjust_bar(const cv::Mat , double );

int Hikaku(const cv::Mat, const cv::Mat , int , int);
void Saliency(const cv::Mat ,int);
cv::Mat trans(const cv::Mat,int,int ,int);
void adjust(int &, int & , int &);
void adjust5(int  &,int & ,int & ,int  &,int &);
int Hikaku_forfirst(const cv::Mat, const cv::Mat, int );

double Ave8(const cv::Mat);
double Ave64(const cv::Mat);

cv::Mat MakeNfile(const cv::Mat ,char[8] , char[9] );

int make1BGR(int tasu,int hiku1,int hiku2,int i);
int make1Y(int, int, int,int );

int mass9ave(cv::Mat Sali, int x, int y);
int mass9ave2(cv::Mat Sali, int x, int y);
cv::Mat Seikika(const cv::Mat in);
void Big88(cv::Mat in , char filename[]);


cv::Mat bigloop(cv::Mat before, cv::Mat mask, int k);

int ad(double );

int pm_select(double xy, double ave);

//cv::Mat AvepyrDown8UC3(cv::Mat, cv::Mat);
cv::Mat AvepyrDown8UC1(cv::Mat , cv::Mat );
cv::Mat AvepyrDown64FC1(cv::Mat , cv::Mat );

cv::Mat ScaleLevel(int testNO, int preNO);

cv::Mat block16(cv::Mat );
cv::Mat block_scale4(cv::Mat);

double bilinear_ave(cv::Mat ,int,int ,int);
double bilinear4_ave(cv::Mat block ,int i, int rows, int cols);

cv::Mat Kyoudo_Ikichimap(cv::Mat ,cv::Mat );

cv::Mat block_scale(cv::Mat, int scale); //ブロック入れるMat はグローバル変数で宣言すること
double bilinear_scale_ave(int i, int rows, int cols, int scale, int color_flag);

cv::Mat edge_distance(cv::Mat);

cv::Mat minScaleLevel(int , int );
cv::Mat MiniMini(const cv::Mat edit,int count);
cv::Mat MiniMiniHSV(const cv::Mat edit,int count);
cv::Mat MiniMiniRGB3(const cv::Mat edit,int count);
cv::Mat MiniMiniK(const cv::Mat edit,int count);
cv::Mat MiniMiniGau(const cv::Mat edit,int count,cv::Mat numK);


cv::Mat blockI5; //ブロック平均値をいれとくもの
cv::Mat blockI6;
cv::Mat blockI7;
cv::Mat blockI8;
cv::Mat blockR5; //ブロック平均値をいれとくもの
cv::Mat blockR6;
cv::Mat blockR7;
cv::Mat blockR8;
cv::Mat blockG5; //ブロック平均値をいれとくもの
cv::Mat blockG6;
cv::Mat blockG7;
cv::Mat blockG8;
cv::Mat blockB5; //ブロック平均値をいれとくもの
cv::Mat blockB6;
cv::Mat blockB7;
cv::Mat blockB8;
cv::Mat blockY5; //ブロック平均値をいれとくもの
cv::Mat blockY6;
cv::Mat blockY7;
cv::Mat blockY8;

cv::Mat rgbbefore; //最初に計算した変化量
cv::Mat rgbgauss5; //ガウシアンかけた後のもの
cv::Mat rgbgauss6;
cv::Mat rgbgauss7;
cv::Mat rgbgauss8;

cv::Mat Mini16block(const cv::Mat edit,int count);
void MiniMini16ab(cv::Mat edit, cv::Mat img,int xb ,int yb);
//double bilinear_ave(cv::Mat block ,int i, int rows, int cols);

cv::Mat blockBa(4, 4, CV_64FC1); //16分割の最小二乗の結果をグローバル保管
cv::Mat blockGa(4, 4, CV_64FC1);
cv::Mat blockRa(4, 4, CV_64FC1);
cv::Mat blockBb(4, 4, CV_64FC1);
cv::Mat blockGb(4, 4, CV_64FC1);
cv::Mat blockRb(4, 4, CV_64FC1);

char inputfile[] = "Hiroshima_07.jpg"; //入力元ファイル名
char maskfile[] = "Hiroshima_07_mask1.bmp"; //入力マスクファイル名

char Infile[] = "Hiroshima_07.jpg"; //最小二乗の理想ファイル名 特に何もなければ入力と同じでよい

//char segfile[] = "109_seg.bmp"; //入力マスクファイル名

int testNO = 0; //フォルダの名前



int ikiti = 0;//閾値
int kaisuu = 0;//強度

int full = 1;//全画像を出力するかどうかを決定するフラグ

int x1y1[100];//座標ごとにSaliencyを保管
int x2y2[100];
int x3y3[100];
int x4y4[100];
int xy=0; //座標管理用のグローバル

int X1=0,X2=0,X3=0,Y1=0,Y2=0,Y3=0,X4=0,Y4=0;//座標　端末から入力




int main(void){

	//初期準備
	//座標、回数入力	
	cout<<"フォルダ番号"<<endl;
	cout<<"testNO:";
	cin >> testNO;

	//フォルダ番号のついたディレクトリ作成
	char namedir[40];
	sprintf_s(namedir,"mkdir test%03d",testNO);
	system(namedir);

	cout<<"S値を測るサンプル点座標を入力"<<endl;

	cout<<"x1:";
	cin >> X1;
	cout<<"y1:";
	cin >> Y1;

	cout<<"x2:";
	cin >> X2;
	cout<<"y2:";
	cin >> Y2;

	cout<<"x3:";
	cin >> X3;
	cout<<"y3:";
	cin >> Y3;

	cout<<"x4:";
	cin >> X4;
	cout<<"y4:";
	cin >> Y4;

	cout<<"処理回数(k)指定:";
	cin>>kaisuu;

	
	//マスク画像読み込み
	cv::Mat mask;
	mask = cv::imread(maskfile,0);//モノクロでマスク画像読み込み

	//一枚目のbefore画像読み込み
	cv::Mat before;
	before = cv::imread(inputfile);

	//加工処理
	//指定回数だけ
	for(int t=0 ; t<kaisuu ; t++){
		/*-------------------------------
		　bigloopの中身
	　　	beforeのS値計算、座標保管
		　　強度係数マップ、色の重み付け係数決定
		　　加工した結果のafterを返す
		----------------------------------*/
		cv::Mat after;
		//beforeをafterへ加工
		after = bigloop(before,mask,t);
		//次に備えて現afterをbeforeにコピー
		before = after;
	}

	//最後の一枚のS値計算、座標保管
	Saliency(before,kaisuu);

	//座標及びグラフ出力
	//保管した座標の表示
	 cout << "(" << X1 << "," << Y1 << "), (" << X2 << "," << Y2 << "), (" << X3 << "," << Y3 << "), (" <<X4 << "," << Y4 << ")" << endl;
	 for (int t = 0 ; t < kaisuu; t++) {
		  cout << t << "," << x1y1[t] << "," << x2y2[t] << "," << x3y3[t] << "," << x4y4[t] <<  endl;
	 }

	//gnuplotでepsのグラフ作成
	FILE *fp1;
	FILE *fp2;
	FILE *fp3;
	FILE *fp4;

	fp1 = fopen("Sali1.dat", "w");//一つのサンプル座標につき一つのファイル
	fp2 = fopen("Sali2.dat", "w");
	fp3 = fopen("Sali3.dat", "w");
	fp4 = fopen("Sali4.dat", "w");

	//座標の値をdatファイルに保存
	FILE *zahyou;
	zahyou = fopen("Zahyou.dat","w");//座標保存用
	fprintf(zahyou,"1:(%d, %d)\n", X1, Y1);
	fprintf(zahyou,"2:(%d, %d)\n", X2, Y2);
	fprintf(zahyou,"3:(%d, %d)\n", X3, Y3);
	fclose(zahyou);

	int S1,S2,S3,S4=0;
	for (int t = 0 ; t <= kaisuu; t++) {
		S1=x1y1[t];
		S2=x2y2[t];
		S3=x3y3[t];
		S4=x4y4[t];


		fprintf(fp1, "%d %d\n", t, S1);//kとSaliency値を一行ずつ与える
		fprintf(fp2, "%d %d\n", t, S2);
		fprintf(fp3, "%d %d\n", t, S3);
		fprintf(fp4, "%d %d\n", t, S4);

		S1=0;
		S2=0;
		S3=0;
		S4=0;
	 }

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	
	system("wgnuplot -persist petSali.plt");//グラフの書式設定は testSali.plt に書きこんでおく

	cv::waitKey();
	return 0;
}



	



//カラー入力画像をIに数値処理する関数 引数(入力,出力)
cv::Mat makeI(const cv::Mat img,cv::Mat I){

	int rows = img.rows;
	int cols = img.cols;

	double pre =0.0;

	for(int i =0 ; i<(rows*cols) ; i++){
		pre = (img.data[i*3] + img.data[i*3+1] + img.data[i*3+2])/3;
		if(pre < 0) {
			I.data[i] =  0 ;
		}else if (pre>=255.0){
			I.data[i] = 255;
		}else {
			I.data[i] = pre;
		}
	}
	return(I);

}

//R,G,Bに数値処理する関数　引数(入力,出力,足すもの,引くもの,引くもの)
cv::Mat makeBGR(const cv::Mat img,cv::Mat BGR,int tasu,int hiku1,int hiku2){

	//メモ　0,b 1,g  2,r なので 
	//R なら　tasu=2 他を0,1に
	//G なら　tasu=1     0,2
	//B なら　tasu=0     1,2

	int rows = img.rows;
	int cols = img.cols;

	double pre = 0.0;

	int max = *std::max_element(&img.data[0], &img.data[rows * cols]);

	for(int i =0 ; i<(rows*cols) ; i++){
		pre = img.data[i*3+tasu] - (img.data[i*3+hiku1] + img.data[i*3+hiku2])/2+0.0;
		if(pre < 0 ) {
			BGR.data[i] =  0 ;
		}else if ((img.data[i*3] + img.data[i*3+1] + img.data[i*3+2])/3+0.0 < max/10+0.0){
			BGR.data[i]= 0;
		}else if(pre >255.0){
			BGR.data[i] = 255;
		}else{
			BGR.data[i] = pre;
		}
	}
	return(BGR);

}

//Yに数値処理する関数　引数(入力,出力)
cv::Mat makeY(const cv::Mat img,cv::Mat Y){

	//メモ　0,b 1,g  2,r

	int rows = img.rows;
	int cols = img.cols;

	int pre = 0;

	int max = *std::max_element(&img.data[0], &img.data[rows * cols]);

	for( int i =0 ; i<(rows*cols) ; i++){
		int r = i*3 +2;
		int g = i*3 +1;
		int b = i*3;

		pre = (img.data[r] + img.data[g])/2  -(abs(img.data[r] - img.data[g]))/2 - img.data[b];
		if(pre < 0  ){
			Y.data[i] = 0;
		}else if ((img.data[b] + img.data[g] + img.data[r])/3< max/10){
			Y.data[i] = 0;
		}else {
			Y.data[i] = pre;
		}
	}
	return(Y);
}



//二つの白黒画像の画素値の差の絶対値を求める関数 引数(ひとつめの画像,二つ目)
cv::Mat sabunnI(const cv::Mat a , const cv::Mat b){

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat c(rows,cols,CV_64FC1);

	double pre =0.0;

	for(int y=0 ; y<rows  ;y++){
		for(int x=0 ; x<cols ; x++){
			pre = a.data[y*cols+x] - b.data[y*cols+x] ;
			c.at<double>(y,x) = fabs(pre);
		}
	}

	return(c);

}

//double から差分とる
cv::Mat sabunnD(const cv::Mat a , const cv::Mat b){

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat c(rows,cols,CV_64FC1);

	double pre =0.0;

	for(int y=0 ; y<rows  ;y++){
		for(int x=0 ; x<cols ; x++){
			pre = a.at<double>(y,x) - b.at<double>(y,x) ;
			c.at<double>(y,x) = fabs(pre);
		}
	}

	return(c);

}

//二つの白黒画像の画素値の差を求める関数 引数(ひとつめの画像,二つ目)
cv::Mat tadanosa(const cv::Mat a , const cv::Mat b){

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat c(rows,cols,CV_64FC1);

	for(int y=0 ; y<a.rows  ;y++){
		for(int x=0 ; x<a.cols ; x++){
			c.at<double>(y,x) = a.data[y*cols+x] - b.data[y*cols+x] ;		
		}
	}

	return(c);


}

//二つの白黒画像の画素値の 和を求める関数 引数(ひとつめの画像,二つ目)
cv::Mat wa(const cv::Mat a , const cv::Mat b){

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat c(rows,cols,CV_64FC1);

	for(int y=0 ; y<a.rows  ;y++){
		for(int x=0 ; x<a.cols ; x++){
			c.at<double>(y,x) = a.at<double>(y,x) + b.at<double>(y,x) ;		
		}
	}

	return(c);

}

//規格化する関数

cv::Mat noramlization(cv::Mat img){

	int rows = img.rows;
	int cols = img.cols;

	//0-255の範囲に押し込む
	img = hirei(img);


	//最大値を求める
	int max = *std::max_element(&img.data[0], &img.data[rows * cols]);


	//極大値の平均を求める。
	//上下左右の4点と比較し、どこよりも大きければ極大とみなす　比べるのは画像の範囲内(だから端より一つ内側)
	//極大であればそれを足していく
	int upleft=0,upright=0,up=0,left=0,right=0,downleft=0,down=0,downright=0,now = 0;
	int ave = 0;
	int wa = 0;
	int kazu = 0;

	for(int y=1 ; y <(rows-1)  ;y++){
			for(int x=1 ; x <(cols-1) ; x++){
				now = img.data[img.step*y+x];

				upleft = img.data[img.step*(y-1)+x-1];
				up = img.data[img.step*(y-1)+x];
				upright = img.data[img.step*(y-1)+x+1];

				left = img.data[img.step*y+x-1];
				right = img.data[img.step*y+x+1];

				downleft = img.data[img.step*(y+1)+x-1];
				down = img.data[img.step*(y+1)+x];
				downright = img.data[img.step*(y+1)+x+1];

				//極大値でありかつ最大値でなければ加算
				if (now > upleft && now > up && now > upright &&
				now > left && now > right &&
				now > downleft && now > down && now > downright && now != max){
				wa = wa + now;
				kazu++;

				//	cout << "x:" << x << "y:" << y << "now:" << now << endl;
				}		
		}
	}



	//最大値を引いて、極大値平均を求める
	if ( kazu==0){
		ave = 0;	
	} else {
		ave = wa / kazu ;
	}

	

	//M-m二乗
	double scale = 0.0;
	int sa =0;
	sa = (max - ave);
	if (sa ==0){
		scale = 1.0;
	}else {
		scale = double( sa * sa );
	}

	//cout <<"max:"<< max << ",wa:" <<wa<<",kazu:" <<kazu << ",ave:" <<  ave << ",scale:" << scale << endl;

	//全要素に M-n二乗をかける	
	cv::Mat n(rows,cols,CV_64FC1);
			
			for(int y=0 ; y<n.rows  ;y++){
					for(int x=0 ; x<n.cols ; x++){					
					n.at<double>(y,x) = double(img.data[(cols*y+x)] * scale) ;		
				}
			}	

	return(n);

}

//全画素値を比はそのままに0から255の範囲に押し込む関数
cv::Mat hirei(cv::Mat img){

	int rows = img.rows;
	int cols = img.cols;

	cv::Mat after(rows,cols,CV_8UC1);

	//最大・最小値を求める STL: 標準ライブラリよる　#include<algorithm>必須
	double max = *std::max_element(&img.data[0], &img.data[rows * cols]);
	double min = *std::min_element(&img.data[0], &img.data[rows * cols]);
	
	for(int i= 0;i<(rows*cols) ; i++){
			after.data[i] = unsigned char( 255*(img.data[i] - min) /(max-min) ) ;	//各画素に比をかける	
		}
	return(after);

}

//全画素値を比はそのままにdouble型で0から255の範囲に押し込む関数
cv::Mat hirei64(cv::Mat img){

	int rows = img.rows;
	int cols = img.cols;

	cv::Mat after(rows,cols,CV_64FC1);

	//最大・最小値を求める
	double minVal = 0.0, maxVal = 0.0;
	cv::Point minLoc = cv::Point(0,0);
	cv::Point maxLoc = cv::Point(0,0);

	cv::minMaxLoc(img, &minVal, &maxVal, &minLoc, &maxLoc);

	//printf("Min = %f, Loc(x, y) = (%d, %d)\n", minVal, minLoc.x, minLoc.y);
	//printf("Max = %f, Loc(x, y) = (%d, %d)\n", maxVal, maxLoc.x, maxLoc.y); 

		for(int y=0 ; y<rows  ;y++){			
				for(int x=0 ; x<cols ; x++){
						after.at<double>(y,x) = ((img.at<double>(y,x) - minVal) * 255.0) /(maxVal - minVal) ;
				}
			}

	return(after);

}

//２つの1/2の和を求める関数 引数(ひとつめの画像,二つ目、三つ目)
cv::Mat sal(const cv::Mat a , const cv::Mat b ){

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat Sali(rows,cols,CV_64FC1);

	double max = 0.0;
	double pre = 0.0;
	for(int y=0 ; y<Sali.rows  ;y++){			
		for(int x=0 ; x<Sali.cols ; x++){
			Sali.at<double>(y,x) = ( a.at<double>(y,x) + b.at<double>(y,x)  )/2.0 ;
			pre = Sali.at<double>(y,x);
			if( pre > max ){
				max = pre;
			}
		}
	}

	cout<< "Sal,max:" << max << endl;

	return(Sali);

}



//指定回数だけダウンスケールする関数　引数（入力画像、 ダウン回数）
cv::Mat DownSample(const cv::Mat a,  int down ){
	
	//書き込み用画像用意

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat in(rows,cols,CV_64FC1);	
	in = a;
	cv::Mat out;

	//down回数だけダウンサンプリング
	for (int i = 1 ; i<(down+1) ; i++){
		in = AvepyrDown64FC1(in,out);	
	}
	return(in);

}

//指定回数だけアップサンプリングする関数　引数（入力画像、アップ回数）
cv::Mat UpSample(const cv::Mat a,  int up){
	
	//書き込み用画像用意

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat in(rows,cols,CV_64FC1);	
	in = a;
	cv::Mat out;

	//up回数だけダウンサンプリング
	for (int i = 1 ; i<(up+1) ; i++){
		cv::pyrUp(in,out);
		in = out;	
	}
	return(in);

}



//入力された　32FU1 画像を　見えるような　8UC1 画像にして出力する関数

void changeoutput(cv::Mat in , char filename[]){

	//出力画像用意

	cv::Mat pre32(in.rows,in.cols,CV_32FC1);

	int rows = in.rows;
	int cols = in.cols;

	for(int i= 0;i<(rows*cols) ; i++){
			pre32.data[i] = in.data[i] ;		
		}


	cv::Mat pre8(rows,cols,CV_8UC1);

	float max = *std::max_element(&pre32.data[0], &pre32.data[rows * cols]);
	float min = *std::min_element(&pre32.data[0], &pre32.data[rows * cols]);
	
	for(int i= 0;i<(rows*cols) ; i++){
			pre8.data[i] = int( 255*pre32.data[i]/(max-min) ) ;	//各画素に比をかける	
		}

	cv::imwrite(filename, pre8);

	//cout << filename << endl;


}


//8ビット 8UC1　指定回数だけダウンスケールする関数　引数（入力画像、 ダウン回数）
cv::Mat DownSample8(const cv::Mat a,  int down ){
	
	//書き込み用画像用意

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat in(rows,cols,CV_8UC1);	
	in = a;
	//仮にサイズ指定外す
	cv::Mat out;

	//down回数だけダウンサンプリング
	for (int i = 1 ; i<(down+1) ; i++){
		in = AvepyrDown8UC1(in,out);
	}
	return(in);

}

//8ビット 8UC3　指定回数だけダウンスケールする関数　引数（入力画像、 ダウン回数）
/*cv::Mat DownSample8UC3(const cv::Mat a,  int down ){
	
	//書き込み用画像用意

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat in(rows,cols,CV_8UC3);	
	in = a;
	//仮にサイズ指定外す
	cv::Mat out;

	//down回数だけダウンサンプリング
	for (int i = 1 ; i<(down+1) ; i++){
		in = AvepyrDown8UC3(in,out);
	}
	return(in);

}
*/

//8ビット　指定回数だけアップサンプリングする関数　引数（入力画像、アップ回数）
cv::Mat UpSample8(const cv::Mat a,  int up){
	
	//書き込み用画像用意

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat in(rows,cols,CV_8UC1);

	in = a;

	cv::Mat out;

	//up回数だけアップサンプリング
	for (int i = 1 ; i<(up+1) ; i++){
		cv::pyrUp(in,out);
		in = out;	
	}
	return(in);

}


//Saliの高いところを元画像に重ねて出力する関数 改訂 （Sali(白黒) 64 1ch、 元画像（カラー）8 3ch）
void Markver2(cv::Mat Sali,const cv::Mat preimg){


	//書き込み用画像用意
	int rows = preimg.rows;
	int cols = preimg.cols;
	cv::Mat pre = preimg; //よくわからんが、仮においておく
	cv::Mat Markimg(rows/16,cols/16,CV_8UC3);	
	//元画像をスケール4まで縮小
	cv::resize(pre, Markimg, Markimg.size(),0,0);

	int row = Sali.rows;
	int col = Sali.cols;

	//最大とそこの座標を求める
	double minVal = 0.0, maxVal = 0.0;
	cv::Point minLoc = cv::Point(0,0);
	cv::Point maxLoc = cv::Point(0,0);

	cv::minMaxLoc(Sali, &minVal, &maxVal, &minLoc, &maxLoc);
	printf("SaliMax = %f, Loc(x, y) = (%d, %d)\n", maxVal, maxLoc.x, maxLoc.y);

	int MaxX = maxLoc.x;
	int MaxY = maxLoc.y;
	int step = Markimg.step;

	//最大値を赤く塗りつぶし
	Markimg.data[(step*MaxY+MaxX*3)] =  0;
	Markimg.data[(step*MaxY+MaxX*3+1)] =  0;
	Markimg.data[(step*MaxY+MaxX*3+2)] =  255;

	cv::Mat after(rows,cols,CV_8UC3);

	cv::resize(Markimg,after,after.size(),0,0);

	cv::imwrite(nameMark,after);

}

//Saliの高い座標出力する関数 （Sali(白黒) 64 1ch）
void Marklocal(cv::Mat Sali){

	//最大とそこの座標を求める
	double minVal = 0.0, maxVal = 0.0;
	cv::Point minLoc = cv::Point(0,0);
	cv::Point maxLoc = cv::Point(0,0);

	cv::minMaxLoc(Sali, &minVal, &maxVal, &minLoc, &maxLoc);
	printf("SaliMax = %f, Loc(x, y) = (%d, %d)\n", maxVal, maxLoc.x, maxLoc.y);

}

//入力された　64FC1 画像を　65025 で割って、目に見えるような　8UC1 画像にして出力する関数

void change2(cv::Mat in , char filename[]){

	//出力画像用意

	cv::Mat pre32(in.rows,in.cols,CV_64FC1);

	int rows = in.rows;
	int cols = in.cols;

	for(int y= 0; y<in.rows ; y++){
		for(int x =0; x<in.cols ; x++){
			pre32.at<double>(y,x) = in.at<double>(y,x) ;		
		}
	}

	cv::Mat pre8(rows,cols,CV_8UC1);

	
	double size = 0.0;
	for(int y= 0; y<in.rows ; y++){
		for(int x =0; x<in.cols ; x++){
			size = pre32.at<double>(y,x)/65025.0;
			if( size > 255.0 ){
				size = 255.0;
			}
			pre8.data[(cols*y+x)] =unsigned char(size)  ;	//各画素を65025で割って255段階に変換	
		
		}
	}
	cv::imwrite(filename, pre8);

//	cout << filename << endl;


}


//入力された　64FC1 画像を　65025 で割って、目に見えるような（大きさ元画像の）8UC1 画像にして出力する関数

void change3(cv::Mat in , char filename[],cv::Mat Imgsize){

	//出力画像用意

	cv::Mat pre32(in.rows,in.cols,CV_64FC1);

	int rows = in.rows;
	int cols = in.cols;

	for(int y= 0; y<in.rows ; y++){
		for(int x =0; x<in.cols ; x++){
			pre32.at<double>(y,x) = in.at<double>(y,x) ;		
		}
	}

	cv::Mat pre8(rows,cols,CV_8UC1);

	
	double size = 0.0;
	for(int y= 0; y<in.rows ; y++){
		for(int x =0; x<in.cols ; x++){
			size = pre32.at<double>(y,x)/65025.0;
			if( size > 255.0 ){
				size = 255.0;
			}
			pre8.data[(cols*y+x)] =unsigned char(size)  ;	//各画素を65025で割って255段階に変換	
		
		}
	}

	cv::Mat pre8Big;
	pre8Big = Imgsize;
	cv::resize(pre8, pre8Big, Imgsize.size(),0,0);
	cv::imwrite(filename, pre8Big);

//	cout << filename << endl;


}

//元が64ビットのものを規格化する関数
cv::Mat normali64(cv::Mat img){

	int rows = img.rows;
	int cols = img.cols;

	//大きさをそろえる
	img = hirei64(img);

	//最大値を求める
	//最大・最小値を求める
	double minVal = 0.0, maxVal = 0.0;
	cv::Point minLoc = cv::Point(0,0);
	cv::Point maxLoc = cv::Point(0,0);

	cv::minMaxLoc(img, &minVal, &maxVal, &minLoc, &maxLoc);

	//極大値の平均を求める。
	//上下左右の4点と比較し、どこよりも大きければ極大とみなす　比べるのは画像の範囲内(だから端より一つ内側)
	//極大であればそれを足していく
	double upleft=0.0,upright=0.0,up=0.0,left=0.0,right=0.0,downleft=0.0,down=0.0,downright=0.0,now = 0.0;
	double ave = 0.0;
	double wa = 0.0;
	double kazu = 0.0;

//	double max = 0.0;	

	for(int y=1 ; y<(img.rows-1)  ;y++){
			for(int x=1 ; x<(img.cols-1) ; x++){
				now = img.at<double>(y,x);


				upleft = img.at<double>(y-1,x-1);
				up = img.at<double>(y-1,x);
				upright = img.at<double>(y-1,x+1);

				left = img.at<double>(y,x-1);
				right = img.at<double>(y,x+1);

				downleft = img.at<double>(y+1,x-1);
				down = img.at<double>(y+1,x);
				downright = img.at<double>(y+1,x+1);

				//極大値であり、かつ最大値でなければ加算していく
				if (now > upleft && now > up && now > upright &&
				now > left && now > right &&
				now > downleft && now > down && now > downright && x != maxLoc.x && y != maxLoc.y){
				wa = wa + now;
				kazu++;
				}

		}
	}


	//極大値平均を求める
	if (kazu == 0){
		ave = 0.0;
	}else{
		ave = wa / kazu ;
	}

	//M-m二乗
	double scale = 0.0;
	double sa =0.0;
	sa = (maxVal - ave);
	if (sa ==0){
		scale = 1.0;
	}else {
		scale = double( sa * sa );
	}

	//cout <<"max:"<< maxVal << ",wa:" <<wa<<",kazu:" <<kazu << ",ave:" <<  ave << ",scale:" << scale << endl;

	//全要素に M-n二乗をかける	
	cv::Mat n(rows,cols,CV_64FC1);		
	
			for(int y=0 ; y<n.rows  ;y++){			
				for(int x=0 ; x<n.cols ; x++){					
					n.at<double>(y,x) = img.at<double>(y,x) * scale ;			
				}
			}

	return(n);

}



//画素値を指定回数で割る関数 引数(入力, 割る数)
cv::Mat adjust_bar(const cv::Mat in , double waru){

	int rows = in.rows;
	int cols = in.cols;

	cv::Mat After(rows,cols,CV_64FC1);

	for(int y=0 ; y<rows  ;y++){			
		for(int x=0 ; x<cols ; x++){
			After.at<double>(y,x) = (in.at<double>(y,x) / waru) ;
		}
	}

	return(After);

}




//ここから判定用に作成

//2画像の指定ピクセルを比較して、1番目が大きければ1を2番目が大きければ2を返す　引数(1枚目,2枚目,x座標,y座標)
int Hikaku(const cv::Mat a , const cv::Mat b , int x , int y){
	int flag = 1;

	double A = 0.0;
	double B = 0.0;

	A = a.at<double>(y,x);
	B = b.at<double>(y,x);

	if(A >= B){
		flag = 1;
	}else if (A< B){
		flag = 2;
	}

	return(flag);

}

//Saliencyを計算　（入力画像,回数を添え字で）
void Saliency(const cv::Mat In,int q){

	
	//SaliencyMap計算
	
	//入力画像読み込み
	cv::Mat img;
	
	img = In;	

	int rows = img.rows;
	int cols = img.cols;


	//書き込み用画像用意　8ビット
	cv::Mat I(rows,cols,CV_8UC1);
	cv::Mat R(rows,cols,CV_8UC1);
	cv::Mat G(rows,cols,CV_8UC1);
	cv::Mat B(rows,cols,CV_8UC1);
	cv::Mat Y(rows,cols,CV_8UC1);
	
	//カラー画像を数値に処理	
	I = makeI(img , I);	
	R = makeBGR(img, R, 2, 0 , 1);
	G = makeBGR(img, G, 1, 0 , 2);
	B = makeBGR(img, B, 0, 1 , 2);
	Y = makeY(img, Y);

	//c,s用に領域確保	8ビット	
	cv::Mat Ic(rows,cols,CV_8UC1);
	cv::Mat Is(rows,cols,CV_8UC1);
	cv::Mat Rc(rows,cols,CV_8UC1);
	cv::Mat Gc(rows,cols,CV_8UC1);
	cv::Mat Rs(rows,cols,CV_8UC1);
	cv::Mat Gs(rows,cols,CV_8UC1);
	cv::Mat Bc(rows,cols,CV_8UC1);
	cv::Mat Yc(rows,cols,CV_8UC1);
	cv::Mat Bs(rows,cols,CV_8UC1);
	cv::Mat Ys(rows,cols,CV_8UC1);

	
	//特徴マップの作成 　式(1)～(7)

	//I,RG,BY,θのマップ作成
	char nameI[40];  // mat型の名前用
	char nameRG[40];
	char nameBY[40];

	char name64I[40];
	char name64RG[40];
	char name64BY[40];

	//I,RG,BY,gabにまとめる為の(式　5,6,7)領域確保 64ビット
	cv::Mat Ibar(rows/(2*2*2*2),cols/(2*2*2*2),CV_64FC1);
	cv::Mat RGbar(rows/(2*2*2*2),cols/(2*2*2*2),CV_64FC1);
	cv::Mat BYbar(rows/(2*2*2*2),cols/(2*2*2*2),CV_64FC1);

	//初期化
	for(int y=0 ; y<Ibar.rows  ;y++){
		for(int x=0 ; x<Ibar.cols ; x++){
			Ibar.at<double>(y,x) = 0.0;
			RGbar.at<double>(y,x) = 0.0;
			BYbar.at<double>(y,x) = 0.0;
		}
	}


	CvSize k= cvSize(3,3); //3,3近傍でガウシアンフィルタをかけるという指定

		// c{2,3,4} s=c+δ　δ{3,4} をループで。
	// 一回の処理の中でやっていることは、（Iで例を挙げると）
	// 指定のスケールまでダウンサンプリングして、ガウシアンフィルタをかける　I(c),I(s)の作成
	// sをcのスケールまでアップサンプリングして大きさをそろえて、差をとる　　I(c,s) = |I(c)-I(s)|
	// 規格化する                                                            N(I(c,s))
	// スケールを4に調整し、統合していく　　　　　　　　　　　　　　　　　　 Ibar = Ibar + N(I(c,s))
	
	for(int c=2; c <5 ; c++){
		for(int i=3; i<5 ; i++){
		int s= c + i;

		//I
		
		//ダウンスケール処理
		cv::Mat Icsample(rows,cols,CV_8UC1);
		cv::Mat Issample(rows,cols,CV_8UC1);

		Icsample = DownSample8(I,  c);
		Issample = DownSample8(I,  s);

		Ic = Icsample;
		Is = Issample;

		//sをcのスケールまでアップ
		Is = UpSample8(Is ,  s-c );


		//差分をとる処理
		sprintf_s(nameI,"I_c%d_s%d",c,s);
		cv::Mat nameI(rows,cols,CV_64FC1);


		nameI = sabunnI(Ic ,Is);

		//規格化
		sprintf_s(name64I,"I_c%d_s%d_32",c,s);
		cv::Mat name64I(rows,cols,CV_64FC1);

		name64I = normali64(nameI);

		//スケールを4に固定
		name64I = DownSample(name64I, 4-c);

		//特徴マップに統合
		Ibar = wa(Ibar , name64I);

		//RG
		//ダウンスケール処理
		cv::Mat Rcsample(rows,cols,CV_8UC1);
		cv::Mat Rssample(rows,cols,CV_8UC1);
		Rcsample = DownSample8(R,  c );
		Rssample = DownSample8(R,  s );
		cv::Mat Gcsample(rows,cols,CV_8UC1);
		cv::Mat Gssample(rows,cols,CV_8UC1);
		Gcsample = DownSample8(G,  c );
		Gssample = DownSample8(G,  s );
	

		Rc = Rcsample;
		Rs = Rssample;
		Gc = Gcsample;
		Gs = Gssample;

		//差分をとる処理
		sprintf_s(nameRG,"RG_c%d_s%d",c,s);
		cv::Mat nameRG(rows,cols,CV_64FC1);

		cv::Mat Csa1(rows,cols,CV_64FC1);
		cv::Mat Ssa1(rows,cols,CV_64FC1);

		Csa1 = tadanosa(Rc , Gc);
		Ssa1 = tadanosa(Gs , Rs);

		//Ssa1をcのスケールまでUp
		Ssa1 = UpSample(Ssa1, s-c);
		nameRG = sabunnD(Csa1 , Ssa1);

		//規格化
		sprintf_s(name64RG,"RG_c%d_s%d_32",c,s);
		cv::Mat name64RG(rows,cols,CV_64FC1);

		name64RG = normali64(nameRG);
		//スケールを4に固定
		name64RG = DownSample(name64RG, 4-c);
		//特徴マップに統合
		RGbar = wa(RGbar , name64RG);

		//BY
		//ダウンスケール処理
		cv::Mat Bcsample(rows,cols,CV_8UC1);
		cv::Mat Bssample(rows,cols,CV_8UC1);
		Bcsample = DownSample8(B,  c );
		Bssample = DownSample8(B,  s );
		cv::Mat Ycsample(rows,cols,CV_8UC1);
		cv::Mat Yssample(rows,cols,CV_8UC1);
		Ycsample = DownSample8(Y,  c );
		Yssample = DownSample8(Y,  s );

		Bc = Bcsample;
		Bs = Bssample;
		Yc = Ycsample;
		Ys = Yssample;

		//差分をとる処理
		sprintf_s(nameBY,"BY_c%d_s%d",c,s);
		cv::Mat nameBY(rows,cols,CV_64FC1);

		cv::Mat Csa2(rows,cols,CV_64FC1);
		cv::Mat Ssa2(rows,cols,CV_64FC1);

		Csa2 = tadanosa(Bc , Yc);
		Ssa2 = tadanosa(Ys , Bs);
		//sをcのスケールまでアップ
		Ssa2 = UpSample(Ssa2,  s-c );
		nameBY = sabunnD(Csa2 , Ssa2);

		//規格化		
		sprintf_s(name64BY,"BY_c%d_s%d_32",c,s);
		cv::Mat name64BY(rows,cols,CV_32FC1);

		name64BY = normali64(nameBY);
		//スケールを4に固定
		name64BY = DownSample(name64BY,  4-c );
		//特徴マップに統合
		BYbar = wa(BYbar , name64BY);


		}
	}

	//式(8)の右辺にある　N(Ibar),N(Cbar)の作成
		
	//規格化済みの特徴マップを統合していく
	//I,gab はループの中で統合済み 統合されたものを規格化して、bmp保存

	cv::Mat Ibar2(rows,cols,CV_64FC1);
	Ibar2 = normali64(Ibar);

	//RG,BYをCに統合する
	cv::Mat Cbar(rows,cols,CV_64FC1);
	//初期化（不要？）
	for(int y=0 ; y<Cbar.rows  ;y++){
		for(int x=0 ; x<Cbar.cols ; x++){
			Cbar.at<double>(y,x) = 0.0;
			}
	}

	Cbar = wa(RGbar, BYbar);
	//統合されたCbarを規格化して、bmp保存
	cv::Mat Cbar2(rows,cols,CV_64FC1);
	Cbar2 = normali64(Cbar);
	//change2(Cbar2, "Cbar.bmp");

	//式(8) SaliencyMapの作成・保存
	
	//SaliencyMapの初期化
	cv::Mat Sali(rows,cols,CV_64FC1);
	for(int y=0 ; y<Sali.rows  ;y++){
		for(int x=0 ; x<Sali.cols ; x++){
			Sali.at<double>(y,x) = 0.0;
			}
	}
	
	//式(8)
	Sali = sal(Ibar2, Cbar2);



	//出力結果確認
	char nameSali[40];
	sprintf_s(nameSali,"test%03d/Sali_kaisuu%03d.bmp",testNO,q);

	cv::Mat Sali8(Sali.rows,Sali.cols,CV_64FC1);

	//Saliの積分値を正規化
	Sali8 = Seikika(Sali);

	//Saliencyの値を記録保持
	x1y1[q] = mass9ave2(Sali8, X1, Y1);
	x2y2[q] = mass9ave2(Sali8, X2, Y2);
	x3y3[q] = mass9ave2(Sali8, X3, Y3);
	x4y4[q] = mass9ave2(Sali8, X4, Y4);
//	x1y1[q] = Sali8.at<double>(Y1,X1);
//	x2y2[q] = Sali8.at<double>(Y2,X2);
//	x3y3[q] = Sali8.at<double>(Y3,X3);
//	x4y4[q] = Sali8.at<double>(Y4,X4);

	Big88(Sali8, nameSali);

	//Saliencyの高いところを印をつけて確認
//	cv::Mat Marked(rows,cols,CV_8UC3);
	//Saliで最も値が大きかった点にimgに重ねて点を打つ
	cout<<q<<":"<<endl;
	sprintf_s(nameMark,"test%03d/Mark_kaisuu%03d.bmp",testNO,q);
//	cv::Mat markpreimg = img; //imgに、Markが悪影響を与えていそうなので切り離す
	Markver2(Sali, img);
	
//	cout<<q<<":"<<endl;
//	Marklocal(Sali);
}



//変換するフィルタ (入力,基準の指定,座標x,y)
cv::Mat trans(const cv::Mat img,int basenum,int k,int i){


	int r2 =0;
	int g2 =0;
	int b2 =0;

	switch(basenum){
		case 1://Iを弱く
			r2 = -1;
			g2 = -1;
			b2 = -1;
			break;
		case 2://Rを弱く
			r2 = -1;
			g2 = +1;
			b2 = +1;
			break;
		case 3://Gを弱く
			r2 = +1;
			g2 = -1;
			b2 = +1;
			break;
		case 4://Bを弱く
			r2 = +1;
			g2 = +1;
			b2 = -1;
			break;
		case 5://Yを弱く
			r2 = -1;
			g2 = -1;
			b2 = 0;
			break;

	}
	int a=0;
	int b=0;
	int c=0;

	//符号ひっくり返し
	k = -1*k;

	

	//まずは入力画像をそのままコピーする
	cv::Mat out;
	out = img;

	//int step = img.step;

	a = img.data[(i*3 +0)] +b2*k;
	b = img.data[(i*3 +1)] +g2*k;
	c = img.data[(i*3 +2)] +r2*k;
						
	adjust(a, b, c);

	out.data[(i*3 +0)] = a;
	out.data[(i*3 +1)] = b;
	out.data[(i*3 +2)] = c;


	return(out);
}

void adjust(int &a , int &b , int &c){
		if( a > 255){
			a = 255;
		}else if( a < 0){
			a = 0;
		}

		if( b > 255){
			b = 255;
		}else if( b < 0){
			b = 0;
		}

		if( c > 255){
			c = 255;
		}else if( c < 0){
			c = 0;
		}

}

void adjust5(int &a ,int &b , int &c, int &d , int &e){
		if( a >= 255){
			a = 255;
		}else if( a <= 0){
			a = 0;
		}
		if( b >= 255){
			b = 255;
		}else if( b <= 0){
			b = 0;
		}

		if( c >= 255){
			c = 255;
		}else if( c <= 0){
			c = 0;
		}
		if( d >= 255){
			d = 255;
		}else if( d <= 0){
			d = 0;
		}

		if( e > 255){
			e = 255;
		}else if( e < 0){
			e = 0;
		}

}


//2画像の指定ピクセルを比較して、小さい方が大きいほうの1/2以上か　引数(NIbar,NCbar,x座標,y座標)
int Hikaku_forfirst(const cv::Mat I, const cv::Mat C, int i){
	int flag = 0;
	
	//I > 2C :1 Iのみ
	//2C > I または　2I > C :2　両方
	//C > 2I :3 Cのみ
	
	int Ixy = 0;
	int Cxy = 0;

	Ixy = I.data[i];
	Cxy = C.data[i];

	if(Ixy >= 2*Cxy){
		flag = 1;
	}else if (Cxy >= 2*Ixy){
		flag = 3;
	}else{
		flag = 2;
	}

	return(flag);

}

//入力画像の平均　引数(入力8ビット)
double Ave8(const cv::Mat in){
	double sum = 0.0;

	int rows = in.rows;
	int cols = in.cols;

	for(int i =0 ; i<(rows*cols) ; i++){
		sum = sum + in.data[i];
	}

	double ave=0.0;
	ave = sum / ((rows*cols)+0.0);

	return(ave);

}

//入力画像の平均　引数(入力64ビット)
double Ave64(const cv::Mat in){
	
	double sum = 0.0;

	int rows = in.rows;
	int cols = in.cols;

	
	for(int y=0 ; y<in.rows  ;y++){
	
		for(int x=0 ; x<in.cols ; x++){
			
			double pre = in.at<double>(y,x);
			
			sum = sum + pre;
			
		}
		
	}

	double ave=0.0;
	ave = sum / (rows*cols);

	return(ave);

}


//入力された　64FC1 画像を　65025 で割って、16倍に拡大して、目に見えるような　8UC1 画像にして出力する関数

void changeBig(cv::Mat in , char filename[]){

	//出力画像用意

	cv::Mat pre32(in.rows,in.cols,CV_64FC1);

	int rows = in.rows;
	int cols = in.cols;

	for(int y= 0; y<in.rows ; y++){
		for(int x =0; x<in.cols ; x++){
			pre32.at<double>(y,x) = in.at<double>(y,x) ;		
		}
	}

	cv::Mat pre8(rows,cols,CV_8UC1);

	
	double size = 0.0;
	for(int y= 0; y<in.rows ; y++){
		for(int x =0; x<in.cols ; x++){
			size = pre32.at<double>(y,x)/65025.0;
			if( size > 255.0 ){
				size = 255.0;
			}
			pre8.data[(cols*y+x)] =unsigned char(size)  ;	//各画素を65025で割って255段階に変換	
		
		}
	}

	cv::Mat Big8(rows*16,cols*16,CV_8UC1);

	cv::resize(pre8,Big8,Big8.size(),0,0);


	cv::imwrite(filename, Big8);


	//cout << filename << endl;


}


//R,G,B,YからSaliと同じ大きさのNR(2,5)を作り返す関数
cv::Mat MakeNfile(const cv::Mat in,char Colorfile[8], char ColorNfile[9]){

	int rows = in.rows;
	int cols = in.cols;

		//ダウンスケール処理
		cv::Mat Inc(rows,cols,CV_8UC1);
		cv::Mat Ins(rows,cols,CV_8UC1);

		Inc = DownSample8(in,  2);
		Ins = DownSample8(in,  5);

		//sをcのスケールまでアップ
		Ins = UpSample8(Ins ,  5-2 );

		//差分をとる処理
		//sprintf_s(nameI,"I_c%d_s%d",c,s);
		cv::Mat In25(rows,cols,CV_64FC1);

		In25 = sabunnI(Inc ,Ins);
		
		//bmpで結果確認
		cv::imwrite(Colorfile,In25);
		
		//規格化
		//sprintf_s(name64I,"I_c%d_s%d_32",c,s);
		cv::Mat In64(rows,cols,CV_64FC1);

		In64 = normali64(In25);

		//bmpで結果確認
		change2(In64, ColorNfile);


		//読み込みリサイズ
		cv::Mat bigNIn(rows,cols,CV_8UC1);
		cv::Mat reIn25(rows/4,cols/4,CV_8UC1);

		reIn25 = cv::imread(ColorNfile,0);
		cv::resize(reIn25,bigNIn,in.size(),0,0);

		return(bigNIn);

}


//ただ1つのピクセルをRGBにする (tasu , hiku1, hiku2, 座標の番数i)
int make1BGR(int tasu,int hiku1,int hiku2,int i){

	//メモ　0,b 1,g  2,r なので 
	//R なら　tasu=r 他を0,1に
	//G なら　tasu=g     0,2
	//B なら　tasu=b     1,2

	
	int pre = 0;
	int BGR = 0;
	
		pre = tasu - (hiku1 + hiku2)/2;
		if(pre < 0 ) {
			BGR =  0 ;
		}else {
			BGR = pre;
		}

	return(BGR);

}

//1ピクセルだけYに数値処理する関数　引数(入力画像)
int make1Y(int r,int g, int b,int i){

	//メモ　0,b 1,g  2,r

	

	int pre = 0;
	int Y = 0;



		pre = (r + g)/2  -(abs(r - g))/2 - b;
		if(pre < 0  ){
			Y = 0;
		}else {
			Y = pre;
		}
	
	return(Y);
}

//指定座標を中心にした9マスで平均を出す

int mass9ave(cv::Mat Sali, int x, int y){
	
	double sum=0.0;
	int count = 0;
	int col = Sali.cols;
	int row = Sali.rows;
	for(int i=-1; i<2; i++){
		for(int t=-1; t<2; t++){
			if(0<=x+t && x+t<= col && 0 <= y+i && y+i<= row){  
			sum = sum + Sali.at<double>(y+i, x+t);
			count++;
			}
		}
	}

	double ave =0.0;
	ave = sum/(count*65025.0);

	int Ave =0;
	Ave = ave + 0.5;

	return(Ave);

}


//SaliencyMapを正規化する
cv::Mat Seikika(const cv::Mat in){

	//入力画像をいったんpreにそのままうつす

	cv::Mat pre32(in.rows,in.cols,CV_64FC1);

	int rows = in.rows;
	int cols = in.cols;

	for(int y= 0; y<in.rows ; y++){
		for(int x =0; x<in.cols ; x++){
			pre32.at<double>(y,x) = in.at<double>(y,x) ;		
		}
	}

	//比例定数を決定する為、総和を求める。　定数は　50*マス目の数/元の総和　50はてきとう
	double sum =0.0;
	for(int y= 0; y<in.rows ; y++){
		for(int x =0; x<in.cols ; x++){
			sum = sum + pre32.at<double>(y,x);
		}
	}
	double k =0.0;
	k = (50*rows*cols)/sum;


	//8ビットへ
	cv::Mat predata(rows,cols,CV_64FC1);

	double size = 0.0;
	for(int y= 0; y<in.rows ; y++){
		for(int x =0; x<in.cols ; x++){
			size = pre32.at<double>(y,x)*k;
			predata.at<double>(y,x)=size ;				
		}
	}

	return(predata);
}

//正規化されたSaliencyMapを出力255以上は255として出す
void Big88(cv::Mat in , char filename[]){
	int rows = in.rows;
	int cols = in.cols;

	cv::Mat real8(rows,cols,CV_8UC1);

	double size=0.0;
	for(int y= 0; y<in.rows ; y++){
		for(int x =0; x<in.cols ; x++){
			size = in.at<double>(y,x);
			if(size > 255.0){
				size =255.0;
			}
			real8.data[y*cols+x]=size ;
		
		}
	}

	cv::Mat Big8(rows*16,cols*16,CV_8UC1);

	cv::resize(real8,Big8,Big8.size(),0,0);
	cv::imwrite(filename, Big8);
//	cout << filename << endl;


}

//指定座標を中心にした9マスで平均を出す

int mass9ave2(cv::Mat Sali, int x, int y){
	
	double sum=0.0;
	int count = 0;
	int col = Sali.cols;
	int row = Sali.rows;
	for(int i=-1; i<2; i++){
		for(int t=-1; t<2; t++){
			int x2=0;
			int y2=0;
			x2=x+t;
			y2=y+i;
			if(0<=x+t && x+t< col && 0 <= y+i && y+i< row){  
				sum = sum + Sali.at<double>(y2,x2);
			count++;
			}
		}
	}

	double ave =0.0;
	ave = sum/count+0.0;

	int Ave2 =0;
	Ave2 = ave + 0.5;

	return(Ave2);

}

//img画像とmask画像から加工後のafterを返す
cv::Mat bigloop(cv::Mat img, cv::Mat mask, int q){




	//beforeのSaliencyMap生成,座標保管

	// SaliencyMap計算

	int rows = img.rows;
	int cols = img.cols;

	//書き込み用画像用意　8ビット
	cv::Mat I(rows,cols,CV_8UC1);
	cv::Mat R(rows,cols,CV_8UC1);
	cv::Mat G(rows,cols,CV_8UC1);
	cv::Mat B(rows,cols,CV_8UC1);
	cv::Mat Y(rows,cols,CV_8UC1);
	
	//カラー画像を数値に処理	
	I = makeI(img , I);	
	R = makeBGR(img, R, 2, 0 , 1);
	G = makeBGR(img, G, 1, 0 , 2);
	B = makeBGR(img, B, 0, 1 , 2);
	Y = makeY(img, Y);
	
	cv::imwrite("I.bmp" , I );
	cv::imwrite("R.bmp" , R );
	cv::imwrite("G.bmp" , G );
	cv::imwrite("B.bmp" , B );
	cv::imwrite("Y.bmp" , Y );

	//c,s用に領域確保	8ビット	
	cv::Mat Ic(rows,cols,CV_8UC1);
	cv::Mat Is(rows,cols,CV_8UC1);
	cv::Mat Rc(rows,cols,CV_8UC1);
	cv::Mat Gc(rows,cols,CV_8UC1);
	cv::Mat Rs(rows,cols,CV_8UC1);
	cv::Mat Gs(rows,cols,CV_8UC1);
	cv::Mat Bc(rows,cols,CV_8UC1);
	cv::Mat Yc(rows,cols,CV_8UC1);
	cv::Mat Bs(rows,cols,CV_8UC1);
	cv::Mat Ys(rows,cols,CV_8UC1);


	//c,s用に領域確保	8ビット	
	cv::Mat NRG25;
	cv::Mat NBY25;

	//特徴マップの作成 　式(1)～(7)

	//I,RG,BY,θのマップ作成
	char nameI[40];  // mat型の名前用
	char nameI2[40]; // .bmp のファイル名用　それぞれ二つずつ必要
	char nameRG[40];
	char name2RG[40];
	char nameBY[40];
	char name2BY[40];

	char nameI3[40];

	char name64I[40];
	char name64RG[40];
	char name64BY[40];

	//仮に
	char name3RG[40];
	char name3BY[40];



	//I,RG,BY,gabにまとめる為の(式　5,6,7)領域確保 64ビット
	cv::Mat Ibar(rows/(2*2*2*2),cols/(2*2*2*2),CV_64FC1);
	cv::Mat RGbar(rows/(2*2*2*2),cols/(2*2*2*2),CV_64FC1);
	cv::Mat BYbar(rows/(2*2*2*2),cols/(2*2*2*2),CV_64FC1);

	//初期化
	for(int y=0 ; y<Ibar.rows  ;y++){
		for(int x=0 ; x<Ibar.cols ; x++){
			Ibar.at<double>(y,x) = 0.0;
			RGbar.at<double>(y,x) = 0.0;
			BYbar.at<double>(y,x) = 0.0;
		}
	}

	CvSize k= cvSize(3,3); //3,3近傍でガウシアンフィルタをかけるという指定

	//c{2,3,4} s=c+δ　δ{3,4} をループで。
	
	//一回の処理の中でやっていることは、（Iで例を挙げると）
	// 指定のスケールまでダウンサンプリングして、ガウシアンフィルタをかける　I(c),I(s)の作成
	// sをcのスケールまでアップサンプリングして大きさをそろえて、差をとる　　I(c,s) = |I(c)-I(s)|
	// 規格化する                                                            N(I(c,s))
	// スケールを4に調整し、統合していく　　　　　　　　　　　　　　　　　　 Ibar = Ibar + N(I(c,s))
	
	for(int c=2; c <5 ; c++){
		for(int i=3; i<5 ; i++){
		int s= c + i;

		//I
		
		//ダウンスケール処理
		cv::Mat Icsample(rows,cols,CV_8UC1);
		cv::Mat Issample(rows,cols,CV_8UC1);

		Icsample = DownSample8(I,  c);
		Issample = DownSample8(I,  s);

		Ic = Icsample;
		Is = Issample;

		//sをcのスケールまでアップ
		Is = UpSample8(Is ,  s-c );


		//差分をとる処理
		sprintf_s(nameI,"I_c%d_s%d",c,s);
		cv::Mat nameI(rows,cols,CV_64FC1);

		nameI = sabunnI(Ic ,Is);

		if (full ==1){
		//bmpで結果確認
		sprintf_s(nameI2,"I_c%d_s%d.bmp",c,s);
		change2(nameI,nameI2);
		}
		

		//規格化
		sprintf_s(name64I,"I_c%d_s%d_32",c,s);
		cv::Mat name64I(rows,cols,CV_64FC1);

		name64I = normali64(nameI);

		if (full ==1){
		//bmpで結果確認
		sprintf_s(nameI3,"NI_c%d_s%d.bmp",c,s);
//		cout<< "c:" << c << ",s:" << s << ", rows:" << name64I.rows << ", cols:" << name64I.cols << endl;
		change3(name64I , nameI3, img);
		}

		//スケールを4に固定

		name64I = DownSample(name64I, 4-c);

		//特徴マップに統合

		Ibar = wa(Ibar , name64I);




		//RG
		//ダウンスケール処理
		cv::Mat Rcsample(rows,cols,CV_8UC1);
		cv::Mat Rssample(rows,cols,CV_8UC1);
		Rcsample = DownSample8(R,  c );
		Rssample = DownSample8(R,  s );
		cv::Mat Gcsample(rows,cols,CV_8UC1);
		cv::Mat Gssample(rows,cols,CV_8UC1);
		Gcsample = DownSample8(G,  c );
		Gssample = DownSample8(G,  s );
	

		Rc = Rcsample;
		Rs = Rssample;
		Gc = Gcsample;
		Gs = Gssample;



		//差分をとる処理
		sprintf_s(nameRG,"RG_c%d_s%d",c,s);
		cv::Mat nameRG(rows,cols,CV_64FC1);

		cv::Mat Csa1(rows,cols,CV_64FC1);
		cv::Mat Ssa1(rows,cols,CV_64FC1);

		Csa1 = tadanosa(Rc , Gc);
		Ssa1 = tadanosa(Gs , Rs);


		//Ssa1をcのスケールまでUp

		Ssa1 = UpSample(Ssa1, s-c);
		nameRG = sabunnD(Csa1 , Ssa1);
		if (full ==1){
		//bmpで結果確認
		sprintf_s(name2RG,"RG_c%d_s%d.bmp",c,s);
		change2(nameRG,name2RG);
		}

		//規格化
		sprintf_s(name64RG,"RG_c%d_s%d_32",c,s);
		cv::Mat name64RG(rows,cols,CV_64FC1);

		name64RG = normali64(nameRG);



		if (full ==1){
		//bmp結果確認
		sprintf_s(name3RG,"NRG_c%d_s%d.bmp",c,s);
		change3(name64RG , name3RG,img);
		}


		//スケールを4に固定
		name64RG = DownSample(name64RG, 4-c);
		

		
		//特徴マップに統合
		RGbar = wa(RGbar , name64RG);
		

		//BY
		//ダウンスケール処理
		cv::Mat Bcsample(rows,cols,CV_8UC1);
		cv::Mat Bssample(rows,cols,CV_8UC1);
		Bcsample = DownSample8(B,  c );
		Bssample = DownSample8(B,  s );
		cv::Mat Ycsample(rows,cols,CV_8UC1);
		cv::Mat Yssample(rows,cols,CV_8UC1);
		Ycsample = DownSample8(Y,  c );
		Yssample = DownSample8(Y,  s );

		Bc = Bcsample;
		Bs = Bssample;
		Yc = Ycsample;
		Ys = Yssample;



		//差分をとる処理
		sprintf_s(nameBY,"BY_c%d_s%d",c,s);
		cv::Mat nameBY(rows,cols,CV_64FC1);

		cv::Mat Csa2(rows,cols,CV_64FC1);
		cv::Mat Ssa2(rows,cols,CV_64FC1);

		Csa2 = tadanosa(Bc , Yc);
		Ssa2 = tadanosa(Ys , Bs);
		//sをcのスケールまでアップ
		Ssa2 = UpSample(Ssa2,  s-c );
		nameBY = sabunnD(Csa2 , Ssa2);

		if (full ==1){
		//bmpで結果確認
		sprintf_s(name2BY,"BY_c%d_s%d.bmp",c,s);
		change2(nameBY,name2BY);
		}

		//規格化
		
		sprintf_s(name64BY,"BY_c%d_s%d_32",c,s);
		cv::Mat name64BY(rows,cols,CV_32FC1);

		name64BY = normali64(nameBY);

		if (full ==1){
		//bmp結果確認
		sprintf_s(name3BY,"NBY_c%d_s%d.bmp",c,s);
		change3(name64BY , name3BY, img);
		}

		//スケールを4に固定
		name64BY = DownSample(name64BY,  4-c );
		//特徴マップに統合
		BYbar = wa(BYbar , name64BY);



		}
	}

	//式(8)の右辺にある　N(Ibar),N(Cbar),N(gabbar)の作成
	
	//規格化済みの特徴マップを統合していく
	//I,gab はループの中で統合済み 統合されたものを規格化して、bmp保存

	cv::Mat Ibar2(rows,cols,CV_64FC1);
	Ibar2 = normali64(Ibar);
	change2(Ibar2, "Ibar.bmp");

	//RG,BYをCに統合する
	cv::Mat Cbar(rows,cols,CV_64FC1);
	//初期化（不要？）
	for(int y=0 ; y<Cbar.rows  ;y++){
		for(int x=0 ; x<Cbar.cols ; x++){
			Cbar.at<double>(y,x) = 0.0;
			}
	}

	Cbar = wa(RGbar, BYbar);
	//統合されたCbarを規格化して、bmp保存
	cv::Mat Cbar2(rows,cols,CV_64FC1);
	Cbar2 = normali64(Cbar);
	change2(Cbar2, "Cbar.bmp");

	//式(8) SaliencyMapの作成・保存
	
	//SaliencyMapの初期化
	cv::Mat Sali(rows,cols,CV_64FC1);
	for(int y=0 ; y<Sali.rows  ;y++){
		for(int x=0 ; x<Sali.cols ; x++){
			Sali.at<double>(y,x) = 0.0;
			}
	}
	
	//式(8)
	Sali = sal(Ibar2, Cbar2);
	//出力結果確認
//	cout<<q<<":"<<endl;
//	Marklocal(Sali);
	//Saliencyの高いところを印をつけて確認

	cout << q << ":" << endl;
	sprintf_s(nameMark,"test%03d/Mark_kaisuu%03d.bmp",testNO,q);
//	cv::Mat preimg = img;
	Markver2(Sali, img);


	//Saliの積分値を正規化
	cv::Mat Sali8(Sali.rows,Sali.cols,CV_64FC1);
	Sali8 = Seikika(Sali);

	//Saliencyの値を記録保持
	x1y1[q] = mass9ave2(Sali8, X1, Y1);
	x2y2[q] = mass9ave2(Sali8, X2, Y2);
	x3y3[q] = mass9ave2(Sali8, X3, Y3);
	x4y4[q] = mass9ave2(Sali8, X4, Y4);

	//正規化されたSaliencyMapを大きくして出力
	char nameSali[40];
	sprintf_s(nameSali,"test%03d/Sali_kaisuu%03d.bmp",testNO,q);
	Big88(Sali8, nameSali);


	//強度係数マップ生成
	//強度マップもそもそも8ビットでつくれる
	cv::Mat Kyoudo8(rows,cols,CV_8UC1);
	cv::Mat bigSali(img.rows,img.cols,CV_8UC1);
	Sali = cv::imread(nameSali,0);//モノクロ読み込み
	cv::resize(Sali,bigSali,img.size(),0,0);


	//閾値設定する場合ここから
//	Kyoudo8 = Kyoudo_Ikichimap(bigSali, mask);
	//ここまで

	//全体でやる場合ここから
	for(int i=0 ; i<rows*cols ; i++){
		if(mask.data[i]>200){
			Kyoudo8.data[i] = 128+(bigSali.data[i]/2);
		}else if(mask.data[i]<=200){
			Kyoudo8.data[i] = 128-(bigSali.data[i]/2);
		}
	}
	//ここまで


	 //強度係数ガウシアンでぼかさない場合
//	cv::Mat Kyoudo(rows,cols,CV_8UC1);
//	Kyoudo = Kyoudo8;
//	char namekyoudo[40];
//	sprintf_s(namekyoudo,"test%03d/kyoudo_notgau_kaisuu%03d.bmp",testNO,q);
//	cv::imwrite(namekyoudo,Kyoudo);
	//ここまで

	//ガウシアンでぼかす
	cv::Mat Kyoudo(rows,cols,CV_8UC1);
	CvSize ksize= cvSize(5,5);
	cv::GaussianBlur(Kyoudo8,Kyoudo,ksize,0,0,0);//ここの分散値はテキトー

	char namekyoudo[40];
	sprintf_s(namekyoudo,"test%03d/kyoudo_kaisuu%03d.bmp",testNO,q);
	cv::imwrite(namekyoudo,Kyoudo);
	//強度係数マップ作成ここまで



	//色の重み付け係数決定
	
	//NR,NG,NB,NYを作り、Saliと同じ大きさまで拡大する
	cv::Mat bigNR(I.rows,I.cols,CV_8UC1);
	cv::Mat bigNG(I.rows,I.cols,CV_8UC1);
	cv::Mat bigNB(I.rows,I.cols,CV_8UC1);
	cv::Mat bigNY(I.rows,I.cols,CV_8UC1);
	
	bigNR = MakeNfile(R,"R25.bmp","NR25.bmp");
	bigNG = MakeNfile(G,"G25.bmp","NG25.bmp");
	bigNB = MakeNfile(B,"B25.bmp","NB25.bmp");
	bigNY = MakeNfile(Y,"Y25.bmp","NY25.bmp");
	
	//Sali,Ibar2,Cbar2,NRG25,NBY25をimgと同じ大きさまで拡大する。
	//cv::Mat bigSali(img.rows,img.cols,CV_8UC1);
	cv::Mat bigNIbar(I.rows,I.cols,CV_8UC1);
	cv::Mat bigNCbar(I.rows,I.cols,CV_8UC1);
	cv::Mat bigNRG25(I.rows,I.cols,CV_8UC1);
	cv::Mat bigNBY25(I.rows,I.cols,CV_8UC1);

	//Sali = cv::imread(outputSalifile);
	//cv::resize(Sali,bigSali,img.size(),0,0);
	Ibar2 = cv::imread("Ibar.bmp",0);
	cv::resize(Ibar2,bigNIbar,I.size(),0,0);
	Cbar2 = cv::imread("Cbar.bmp",0);
	cv::resize(Cbar2,bigNCbar,I.size(),0,0);
	NRG25 = cv::imread("NRG_c2_s5.bmp",0);
	cv::resize(NRG25,bigNRG25,I.size(),0,0);
	NBY25 = cv::imread("NBY_c2_s5.bmp",0);
	cv::resize(NBY25,bigNBY25,I.size(),0,0);

//	cv::imwrite("bigSali.bmp",bigSali);
//	cv::imwrite("bigIbar.bmp",bigNIbar);
//	cv::imwrite("bigCbar.bmp",bigNCbar);
//	cv::imwrite("bigRG.bmp",bigNRG25);
//	cv::imwrite("bigBY.bmp",bigNBY25);


	//影響スケールの抽出
	cv::Mat ICscale;
	ICscale = ScaleLevel(testNO, q);


	cv::Mat after;//加工後の画像になる予定
	after = img; //初期化

	//比較に使うので、各マップの平均値をあらかじめ計算しておく
	double Iave =0.0;
	double Rave =0.0;
	double Bave =0.0;
	double Gave =0.0;
	double Yave =0.0;

/*	単純に画像全体平均の場合
	Iave = Ave8(I);
	Rave = Ave8(R);
	Gave = Ave8(G);
	Bave = Ave8(B);
	Yave = Ave8(Y);
	//ここまで
*/

//比較に使うので、各マップの16ブロックの平均値をあらかじめ計算しておく
/*	cv::Mat blockI(4, 4, CV_64FC1);
	cv::Mat blockR(4, 4, CV_64FC1);
	cv::Mat blockG(4, 4, CV_64FC1);
	cv::Mat blockB(4, 4, CV_64FC1);
	cv::Mat blockY(4, 4, CV_64FC1);

	blockI = block16(I);
	blockR = block16(R);
	blockG = block16(G);
	blockB = block16(B);
	blockY = block16(Y);
	//ここまで
*/

	//比較に使うので、scale4で見た各ブロックの平均値をあらかじめ計算しておく
/*	int tate = rows/16;
	int yoko = cols/16;

	cv::Mat blockI(tate, yoko, CV_64FC1);
	cv::Mat blockR(tate, yoko, CV_64FC1);
	cv::Mat blockG(tate, yoko, CV_64FC1);
	cv::Mat blockB(tate, yoko, CV_64FC1);
	cv::Mat blockY(tate, yoko, CV_64FC1);

	blockI = block_scale4(I);
	blockR = block_scale4(R);
	blockG = block_scale4(G);
	blockB = block_scale4(B);
	blockY = block_scale4(Y);
	//ここまで
*/

	//比較に使うので 5,6,7,8 スケールで見た各ブロックの平均値をあらかじめ計算しておく
	//ブロックはグローバルで宣言済み
	blockI5 = block_scale(I, 5);
	blockI6 = block_scale(I, 6);
	blockI7 = block_scale(I, 7);
	blockI8 = block_scale(I, 8);
	blockR5 = block_scale(R, 5);
	blockR6 = block_scale(R, 6);
	blockR7 = block_scale(R, 7);
	blockR8 = block_scale(R, 8);
	blockG5 = block_scale(G, 5);
	blockG6 = block_scale(G, 6);
	blockG7 = block_scale(G, 7);
	blockG8 = block_scale(G, 8);
	blockB5 = block_scale(B, 5);
	blockB6 = block_scale(B, 6);
	blockB7 = block_scale(B, 7);
	blockB8 = block_scale(B, 8);
	blockY5 = block_scale(Y, 5);
	blockY6 = block_scale(Y, 6);
	blockY7 = block_scale(Y, 7);
	blockY8 = block_scale(Y, 8);
	//ここまで


//	cout<<"Ave:"<<Iave<<","<<Rave<<","<<Gave<<","<<Bave<<","<<Yave<<","<<endl;

	cols = bigSali.cols;
	rows = bigSali.rows;
	int step = bigSali.step;
	int stepI = I.step;

	//色の重みづけ係数計算
	cv::Mat QI(rows,cols,CV_8UC1);
	cv::Mat QR(rows,cols,CV_8UC1);
	cv::Mat QG(rows,cols,CV_8UC1);
	cv::Mat QB(rows,cols,CV_8UC1);
	cv::Mat QY(rows,cols,CV_8UC1);

	//色の重みづけ係数計算の符号(plus,minus)
	cv::Mat QIpm(rows,cols,CV_8UC1);
	cv::Mat QRpm(rows,cols,CV_8UC1);
	cv::Mat QGpm(rows,cols,CV_8UC1);
	cv::Mat QBpm(rows,cols,CV_8UC1);
	cv::Mat QYpm(rows,cols,CV_8UC1);

	//テスト用
	cv::Mat predata(rows,cols,CV_8UC1);


//確認用
	cv::Mat QC(rows,cols,CV_8UC1);
	cv::Mat QRG(rows,cols,CV_8UC1);
	cv::Mat QBY(rows,cols,CV_8UC1);

	for(int i=0 ; i<(rows*cols)  ;i++){

			double qI=0.0,qC=0.0,qRG=0.0,qBY=0.0,qR=0.0,qG=0.0,qB=0.0,qY=0.0;

			if(bigNIbar.data[i]==0 && bigNCbar.data[i]==0){
				qI = 0;
				qC = 0;
			}else {
			qI = bigNIbar.data[i]/(bigNCbar.data[i] + bigNIbar.data[i] +0.0);
			qC = bigNCbar.data[i]/(bigNCbar.data[i] + bigNIbar.data[i] +0.0);
			}
			if(bigNRG25.data[i]==0 && bigNBY25.data[i]==0){
				qRG = 0;
				qBY = 0;
			}else {
			qRG = qC * bigNRG25.data[i]/(bigNRG25.data[i] + bigNBY25.data[i] +0.0);
			qBY = qC * bigNBY25.data[i]/(bigNRG25.data[i] + bigNBY25.data[i] +0.0);
			}
			if(bigNR.data[i]==0 && bigNG.data[i]==0){
				qR = 0;
				qG = 0;
			}else {
			qR = qRG * bigNR.data[i]/(bigNR.data[i]+bigNG.data[i] +0.0);
			qG = qRG * bigNG.data[i]/(bigNR.data[i]+bigNG.data[i] +0.0);
			}
			if(bigNB.data[i]==0 && bigNY.data[i]==0){
				qB = 0;
				qY = 0;
			}else {
			qB = qBY * bigNB.data[i]/(bigNB.data[i]+bigNY.data[i] +0.0);
			qY = qBY * bigNY.data[i]/(bigNB.data[i]+bigNY.data[i] +0.0);
			}

//			qI = qI/(qI+qR+qG+qB+qY)+0.0;
//			qR = qR/(qI+qR+qG+qB+qY)+0.0;
//			qG = qG/(qI+qR+qG+qB+qY)+0.0;
//			qB = qB/(qI+qR+qG+qB+qY)+0.0;
//			qY = qY/(qI+qR+qG+qB+qY)+0.0;


			//変化量の絶対値をマップに保管
			int qI2 = ad(qI);
			int qR2 = ad(qR);
			int qG2 = ad(qG);
			int qB2 = ad(qB);
			int qY2 = ad(qY);

			QI.data[i] = qI2;
			QR.data[i] = qR2;
			QG.data[i] = qG2;
			QB.data[i] = qB2;
			QY.data[i] = qY2;

			//変化量の符号をマップに保管
			//その点の色ごとの値と平均を比較
			//とりあえず、高くて目立つなら 2 低くて目立つなら 0　を与える。後で-1してかける

		

			//利用する背景ブロックを判定 双線形16ブロックのとき
/*			Iave = bilinear_ave(blockI,i,rows,cols);
			Rave = bilinear_ave(blockR,i,rows,cols);
			Gave = bilinear_ave(blockG,i,rows,cols);
			Bave = bilinear_ave(blockB,i,rows,cols);
			Yave = bilinear_ave(blockY,i,rows,cols);		
			//ここまで
*/
			//利用する背景ブロックを判定 scale4 のとき
/*			Iave = bilinear4_ave(blockI,i,rows,cols);
			Rave = bilinear4_ave(blockR,i,rows,cols);
			Gave = bilinear4_ave(blockG,i,rows,cols);
			Bave = bilinear4_ave(blockB,i,rows,cols);
			Yave = bilinear4_ave(blockY,i,rows,cols);
			//ここまで
*/

			//利用する背景ブロックを判定 スケールブロックのとき
			int Iscalenum = ICscale.data[3*i];
			int Cscalenum = ICscale.data[3*i+1];

			Iave = bilinear_scale_ave(i,rows,cols, Iscalenum ,1);
			Rave = bilinear_scale_ave(i,rows,cols, Cscalenum ,2);
			Gave = bilinear_scale_ave(i,rows,cols, Cscalenum ,3);
			Bave = bilinear_scale_ave(i,rows,cols, Cscalenum ,4);
			Yave = bilinear_scale_ave(i,rows,cols, Cscalenum ,5);
			//ここまで

			int qIpm = pm_select(I.data[i], Iave);
			int qRpm = pm_select(R.data[i], Rave);
			int qGpm = pm_select(G.data[i], Gave);
			int qBpm = pm_select(B.data[i], Bave);
			int qYpm = pm_select(Y.data[i], Yave);

			QIpm.data[i] = qIpm;
			QRpm.data[i] = qRpm;
			QGpm.data[i] = qGpm;
			QBpm.data[i] = qBpm;
			QYpm.data[i] = qYpm;
	}


	//加工


	//全画素についてループ
			
	//新手法のため変化量を一時保存
	cv::Mat beforepre(rows,cols,CV_8UC3);
	rgbbefore = beforepre;
	
		for(int i=0 ; i<(rows*cols)  ;i++){
	
			//変化量の絶対値
			int qI3 = QI.data[i];
			int qR3 = QR.data[i];
			int qG3 = QG.data[i];
			int qB3 = QB.data[i];
			int qY3 = QY.data[i];

			//変化量の符号
			int qI4 = QIpm.data[i]-1;
			int qR4 = QRpm.data[i]-1;
			int qG4 = QGpm.data[i]-1;
			int qB4 = QBpm.data[i]-1;
			int qY4 = QYpm.data[i]-1;

			qI3 = qI3*qI4;
			qR3 = qR3*qR4;
			qG3 = qG3*qG4;
			qB3 = qB3*qB4;
			qY3 = qY3*qY4;

			//画像画素値操作

			double chyousei = 0.0002; //0.0002 だったのを調節中

			double b1=0.0;
			double g1=0.0;
			double r1=0.0;

			b1 = (Kyoudo.data[i]-128)*chyousei*(qI3-qR3-qG3+qB3+0);
			g1 = (Kyoudo.data[i]-128)*chyousei*(qI3-qR3+qG3-qB3+qY3);
			r1 = (Kyoudo.data[i]-128)*chyousei*(qI3+qR3-qG3-qB3+qY3);

			rgbbefore.data[i*3]=b1+150.0;
			rgbbefore.data[i*3+1]=g1+150.0;
			rgbbefore.data[i*3+2]=r1+150.0;	

		}

		//全画素ループここまで


		//変化量にガウシアンかける操作

		cv::Mat mono(rows,cols,CV_8UC1);
		for(int t=0; t<rows*cols; t++){
			mono.data[t] = ( img.data[3*t] + img.data[3*t+1] + img.data[3*t+2] )/3;
		}


		//2,3,4のスケールの白黒画像を作成(実際に利用するのは2-4のみで1は仮)
		cv::Mat mono1(rows,cols,CV_8UC1);
		cv::Mat mono2(rows,cols,CV_8UC1);
		cv::Mat mono3(rows,cols,CV_8UC1);
		cv::Mat mono4(rows,cols,CV_8UC1);


		cv::GaussianBlur(mono, mono1, cv::Size(5,5) ,0,0);
		cv::GaussianBlur(mono1, mono2, cv::Size(5,5) ,0,0);
		cv::GaussianBlur(mono2, mono3, cv::Size(5,5) ,0,0);
		cv::GaussianBlur(mono3, mono4, cv::Size(5,5) ,0,0);

		cv::Mat edge2;
		cv::Mat edge3;
		cv::Mat edge4;
		cv::Mat depth2;
		cv::Mat depth3;
		cv::Mat depth4;

//		cv::Mat edge;
//		cv::Mat depth;

//		cv::Canny(mono, edge, 100.0, 350.0);

		//白黒画像からのエッジ検出
		cv::Canny(mono2, edge2, 50.0, 150.0);
		cv::Canny(mono3, edge3, 50.0, 150.0);
		cv::Canny(mono4, edge4, 50.0, 150.0);

		char nameedge2[40];
		sprintf_s(nameedge2,"test%03d/edge2_kaisuu%03d.bmp",testNO,q);
		cv::imwrite(nameedge2,edge2);
		char nameedge3[40];
		sprintf_s(nameedge3,"test%03d/edge3_kaisuu%03d.bmp",testNO,q);
		cv::imwrite(nameedge3,edge3);
		char nameedge4[40];
		sprintf_s(nameedge4,"test%03d/edge4_kaisuu%03d.bmp",testNO,q);
		cv::imwrite(nameedge4,edge4);

	
//		depth = edge_distance(edge);
		depth2 = edge_distance(edge2);
		depth3 = edge_distance(edge3);
		depth4 = edge_distance(edge4);

/*
		char namedepth[40];
    	sprintf_s(namedepth,"test%03d/depth_kaisuu%03d.bmp",testNO,q);
		cv::imwrite(namedepth,depth);
*/
		char namedepth2[40];
    	sprintf_s(namedepth2,"test%03d/depth2_kaisuu%03d.bmp",testNO,q);
		cv::imwrite(namedepth2,depth2);
		char namedepth3[40];
    	sprintf_s(namedepth3,"test%03d/depth3_kaisuu%03d.bmp",testNO,q);
		cv::imwrite(namedepth3,depth3);
		char namedepth4[40];
    	sprintf_s(namedepth4,"test%03d/depth4_kaisuu%03d.bmp",testNO,q);
		cv::imwrite(namedepth4,depth4);

/*
		char namedepth[40];
    	sprintf_s(namedepth,"test%03d/depth_kaisuu%03d.bmp",testNO,q);
		cv::imwrite(namedepth,depth);
*/
		//影響最小スケールの抽出
		cv::Mat minIC;
		minIC = minScaleLevel(testNO, q);
		

		//変化量にガウシアン　カーネルの大きさを3段階用意する
		//なお、カーネルの後の引数は0,0にしておくと、自動で分散が定義される．
		//分散の値はx,yそれぞれに関して　0.3+(ksize/2 -1)+0.8 で計算されているようである．
		//cv::GaussianBlur(rgbbefore, rgbgauss5, cv::Size(1,1) ,0,0);
		cv::GaussianBlur(rgbbefore, rgbgauss6, cv::Size(3,3) ,0,0);
		cv::GaussianBlur(rgbbefore, rgbgauss7, cv::Size(5,5) ,0,0);
		cv::GaussianBlur(rgbbefore, rgbgauss8, cv::Size(7,7) ,0,0);


		//後の最小化 MiniMiniGau で使う
		cv::Mat numK(rows,cols,CV_8UC1);

		for(int i=0 ; i<(rows*cols)  ;i++){
			int prey = i/cols;
			int prex = i%cols;


//			int Inum = depth.data[i];

	
			//影響を与えているスケールにおうじて、利用するdepthを決定
			int Inum =0;
			int scale = minIC.data[i];
			if (scale == 120){
				Inum = depth2.data[i];
			}else if (scale == 180){
				Inum = depth3.data[i];
			}else if (scale == 240){
				Inum = depth4.data[i];
			}

			cv::Mat rgbgauss;

			//depthにはエッジからの距離に応じて0,70,140,210,240 が格納

			if(Inum == 0) rgbgauss = rgbbefore;//エッジ上　ガウシアンかけない
			if (Inum == 70) rgbgauss = rgbbefore;//エッジから1-3マス以内　ガウシアンかけない
			if (Inum == 140) rgbgauss = rgbgauss6;//エッジから4,5マス　3*3ガウシアン
			if (Inum == 210) rgbgauss = rgbgauss7;//エッジから6,7マス　5*5ガウシアン
			if (Inum == 240) rgbgauss = rgbgauss8;//エッジから8マス以上 7*7ガウシアン

			int b = img.data[(i*3 +0)] + rgbgauss.data[(i*3 +0)]-150.0;
			int g = img.data[(i*3 +1)] +rgbgauss.data[(i*3 +1)]-150.0;
			int r = img.data[(i*3 +2)] +rgbgauss.data[(i*3 +2)]-150.0;

			adjust(b, g, r);

			after.data[(i*3 +0)] = b;
			after.data[(i*3 +1)] = g;
			after.data[(i*3 +2)] = r;

			numK.data[i] =Inum;//後のMiniGauで利用する
		}
		

		//加工後の画像を最小二乗で規格化
		//cv::Mat Mini = MiniMiniK(after,q+1);
		cv::Mat Mini = MiniMiniGau(after,q+1,numK);
		//加工後の画像出力
		char nameafter[40];
		sprintf_s(nameafter,"test%03d/after_kaisuu%03d.bmp",testNO,q+1);
		cv::imwrite(nameafter,Mini);

	return (Mini);
		
/*
		//加工後の画像出力
		char nameafter[40];
		sprintf_s(nameafter,"test%03d/after_kaisuu%03d.bmp",testNO,q+1);
		cv::imwrite(nameafter,after);

	return (after);
*/
}

//1文字だけアジャストする
int ad(double in){
	int after=0;
	after = in*255.0;

	if( after >= 255){
			after = 255;
		}else if( after <= 0){
			after = 0;
		}
return(after);

}

int pm_select(double xy, double ave){
	int pm;

	if (xy > ave){
		pm = 2;
	}else{
		pm = 0;
	}

	return(pm);
}

cv::Mat AvepyrDown8UC3(cv::Mat in, cv::Mat out){
	cv::Mat before = in;
	cv::Mat Gaus = in;

		cv::GaussianBlur(before, Gaus, cv::Size(5,5) ,0,0);

		int rows = before.rows/2;
		int cols = before.cols/2;
		cv::Mat Down(rows, cols, CV_8UC3);

			for(int y = 0; y< rows ; y++){
				for(int x = 0; x < cols ; x++){
					Down.data[(cols*y+x)*3] = 
						(Gaus.data[(cols*2*2*y+2*x)*3] + Gaus.data[(cols*2*2*y+2*x +1)*3] + Gaus.data[(cols*2*2*y+2*x + 2*cols)*3] + Gaus.data[(cols*2*2*y+2*x + 2*cols+1)*3])/4;
					Down.data[(cols*y+x)*3+1] = 
						(Gaus.data[(cols*2*2*y+2*x)*3+1] + Gaus.data[(cols*2*2*y+2*x +1)*3+1] + Gaus.data[(cols*2*2*y+2*x + 2*cols)*3+1] + Gaus.data[(cols*2*2*y+2*x + 2*cols+1)*3+1])/4;
					Down.data[(cols*y+x)*3+2] =
						(Gaus.data[(cols*2*2*y+2*x)*3+2] + Gaus.data[(cols*2*2*y+2*x +1)*3+2] + Gaus.data[(cols*2*2*y+2*x + 2*cols)*3+2] + Gaus.data[(cols*2*2*y+2*x + 2*cols+1)*3+2])/4;
				}
			}
				
		out = Down;
		return(out);
		
		
}


cv::Mat AvepyrDown8UC1(cv::Mat in, cv::Mat out){
	cv::Mat before = in;
	cv::Mat Gaus = in;
		cv::GaussianBlur(before, Gaus, cv::Size(5,5) ,0,0);
		int rows = before.rows/2;
		int cols = before.cols/2;
		cv::Mat Down(rows, cols, CV_8UC1);
			for(int y = 0; y< rows ; y++){
				for(int x = 0; x < cols ; x++){
					Down.data[cols*y+x] = 
						(Gaus.data[cols*2*2*y+2*x] + Gaus.data[cols*2*2*y+2*x +1] + Gaus.data[cols*2*2*y+2*x + 2*cols] + Gaus.data[cols*2*2*y+2*x + 2*cols+1])/4;
				}
			}

		out = Down;
	return(out);

}


cv::Mat AvepyrDown64FC1(cv::Mat in, cv::Mat out){
	cv::Mat before = in;
	cv::Mat Gaus = in;
		cv::GaussianBlur(before, Gaus, cv::Size(5,5) ,0,0);

		int rows = before.rows/2;
		int cols = before.cols/2;
		cv::Mat Down(rows, cols, CV_64FC1);

			for(int y = 0; y< rows ; y++){
				for(int x = 0; x < cols ; x++){
					Down.at<double>(y,x) = 
						(Gaus.at<double>(2*y,2*x)+ Gaus.at<double>(2*y,2*x+1) + Gaus.at<double>(2*y+1,2*x) + Gaus.at<double>(2*y+1,2*x+1))/4;
				}
			}				
		out = Down;
		return(out);
}

//元画像と同じ大きさの特性マップから、影響スケールを計算して、I,Cの情報を一枚にまとめて返す関数
//block法とくみ合わせるにあたり仕様変更 levelの値＝5,6,6,7,7,8 に
cv::Mat ScaleLevel(int testNO, int preNO){

	
	cv::Mat I25;
	cv::Mat I26;
	cv::Mat I36;
	cv::Mat I37;
	cv::Mat I47;
	cv::Mat I48;

	cv::Mat RG25;
	cv::Mat RG26;
	cv::Mat RG36;
	cv::Mat RG37;
	cv::Mat RG47;
	cv::Mat RG48;

	cv::Mat BY25;
	cv::Mat BY26;
	cv::Mat BY36;
	cv::Mat BY37;
	cv::Mat BY47;
	cv::Mat BY48;


	I25 = cv::imread("NI_c2_s5.bmp",0);   //0
	I26 = cv::imread("NI_c2_s6.bmp",0);  //50
	I36 = cv::imread("NI_c3_s6.bmp",0);  //100
	I37 = cv::imread("NI_c3_s7.bmp",0);  //150
	I47 = cv::imread("NI_c4_s7.bmp",0); //200
	I48 = cv::imread("NI_c4_s8.bmp",0); //255

	RG25 = cv::imread("NRG_c2_s5.bmp",0);
	RG26 = cv::imread("NRG_c2_s6.bmp",0);
	RG36 = cv::imread("NRG_c3_s6.bmp",0);
	RG37 = cv::imread("NRG_c3_s7.bmp",0);
	RG47 = cv::imread("NRG_c4_s7.bmp",0);
	RG48 = cv::imread("NRG_c4_s8.bmp",0);

	BY25 = cv::imread("NBY_c2_s5.bmp",0);
	BY26 = cv::imread("NBY_c2_s6.bmp",0);
	BY36 = cv::imread("NBY_c3_s6.bmp",0);
	BY37 = cv::imread("NBY_c3_s7.bmp",0);
	BY47 = cv::imread("NBY_c4_s7.bmp",0);
	BY48 = cv::imread("NBY_c4_s8.bmp",0);
	
	int rows = I25.rows;
	int cols = I25.cols;
	

	cv::Mat Idif(rows, cols, CV_8UC1);
	cv::Mat Cdif(rows, cols, CV_8UC1);


	cv::Mat IandCdif(rows,cols,CV_8UC3);

	//比較処理
	for(int t=0 ; t<(rows*cols) ; t++){
		double Ia;
		double Ib;
		int Ilevel = 5;

		double Ca;
		double Cb;
		int Clevel = 5;

		Ia = I25.data[t];
		Ca = RG25.data[t] + BY25.data[t];

		Ib = I26.data[t];
		Cb = RG26.data[t] + BY26.data[t];
		if(Ia<Ib){
			Ilevel = 6;
			Ia = Ib;
		}
		if(Ca<Cb){
			Clevel = 6;
			Ca = Cb;
		}

		Ib = I36.data[t];
		Cb = RG36.data[t] + BY36.data[t];
		if(Ia<Ib){
			Ilevel = 6;
			Ia = Ib;
		}
		if(Ca<Cb){
			Clevel = 6;
			Ca = Cb;
		}

		Ib = I37.data[t];
		Cb = RG37.data[t] + BY37.data[t];
		if(Ia<Ib){
			Ilevel = 7;
			Ia = Ib;
		}
		if(Ca<Cb){
			Clevel = 7;
			Ca = Cb;
		}

		Ib = I47.data[t];
		Cb = RG47.data[t] + BY47.data[t];
		if(Ia<Ib){
			Ilevel = 7;
			Ia = Ib;
		}
		if(Ca<Cb){
			Clevel = 7;
			Ca = Cb;
		}

		Ib = I48.data[t];
		Cb = RG48.data[t] + BY48.data[t];
		if(Ia<Ib){
			Ilevel = 8;
			Ia = Ib;
		}
		if(Ca<Cb){
			Clevel = 8;
			Ca = Cb;
		}
		Idif.data[t] = (Ilevel-5)*70;
		Cdif.data[t] = (Clevel-5)*70;

		//IandCdif の一枚に情報を集約
		IandCdif.data[3*t] = Ilevel;
		IandCdif.data[3*t+1] = Clevel;
		IandCdif.data[3*t+2] = 0; //一応使ってないとこはゼロで埋めとく

	}

	char nameCscale[40];
	sprintf_s(nameCscale,"test%03d/Cscale_kaisuu%03d.bmp",testNO,preNO);
	char nameIscale[40];
	sprintf_s(nameIscale,"test%03d/Iscale_kaisuu%03d.bmp",testNO,preNO);

	cv::imwrite(nameIscale,Idif);
	cv::imwrite(nameCscale,Cdif);

	return(IandCdif);
}

//背景の平均値を16ブロックで行う場合
cv::Mat block16(cv::Mat grayImg){

	//背景の平均値格納庫
	cv::Mat Ave(4, 4, CV_64FC1);

	int yoko = grayImg.cols/4;
	int tate = grayImg.rows/4;

	int y=0;
	int x=0;

	for(int m = 0 ; m < 4; m++){
		for(int n = 0 ; n < 4; n++){

			double sum =0.0;

			for(int q=0; q < tate ;q++){
				for(int p=0; p < yoko ;p++){
					x = yoko*n + p;
					y = tate*m + q;
					sum = sum + grayImg.data[grayImg.cols*y +x];
				}
			}

			Ave.at<double>(m,n) = sum/(tate*yoko+0.0); 
		}
	}
//確認用
/*
cout<<"ーーー確認用出力ーーー"<<endl;
	for(int y1 = 0; y1<4 ;y1++){
		for(int x1 = 0; x1<4 ;x1++){
		double pre = Ave.at<double>(y1,x1);
		cout<< pre <<endl;
		}
	}
*/
	return(Ave);		
};


cv::Mat block_scale4(cv::Mat grayImg){

	int yoko = grayImg.cols/16;
	int tate = grayImg.rows/16;

	//背景の平均値格納庫
	cv::Mat Ave(tate, yoko, CV_64FC1);

	int y=0;
	int x=0;

	for(int m = 0 ; m < tate; m++){
		for(int n = 0 ; n < yoko; n++){

			double sum =0.0;

			for(int q=0; q < 16 ;q++){
				for(int p=0; p < 16 ;p++){
					x = 16*n + p;
					y = 16*m + q;
					sum = sum + grayImg.data[grayImg.cols*y +x];
				}
			}

			Ave.at<double>(m,n) = sum/(16*16+0.0); 
		}
	}
//確認用
/*
cout<<"ーーー確認用出力ーーー"<<endl;
	for(int y1 = 0; y1<tate ;y1++){
		for(int x1 = 0; x1<yoko ;x1++){
		double pre = Ave.at<double>(y1,x1);
		cout<< pre <<endl;
		}
	}
*/
	return(Ave);		
};


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


//スケール4のブロックの周辺の値を双線形補間してならす
double bilinear4_ave(cv::Mat block ,int i, int rows, int cols){

	int x = i%cols;
	int y = i/cols;

	int n = cols/16;
	int m = rows/16;

	int left =0;
	int right =0;
	int top =0;
	int bottom =0;


	if(x<=8){
		left = 0;
		right = 0;
	}
	
	if(x>= (cols-8) ){
		left = n-1;
		right = n-1;	
	}

	if(x>8 && x< (cols-8) ){
		left = (x-8)/16;
		right = left+1;
	}

	if(y<=8){
		top = 0;
		bottom = 0;		
	}
	
	if(y>= (rows-8) ){
		top = m-1;
		bottom = m-1;
	}
	
	if(y>8 && y< (rows-8) ){
		top = (y-8)/16;
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

	if (x < 8){
		 p = x+8;
	}else{
	 p = (x-8)%16;
	}

	if(y <= 8){
		 q = y+8;
	}else{
	 q = (y-8)%16;
	}


	double p2 = double((p+0.0)/(16+0.0));
	double q2 = double((q+0.0)/(16+0.0));

	double ave = 0.0;
	ave = (1-q2)*((1-p2)*lt + p2*rt) + q2*((1-p2)*lb + p2*rb);

	return(ave);
}





//閾値設定付きの強度係数マップ
cv::Mat Kyoudo_Ikichimap(cv::Mat bigSali ,cv::Mat mask){
	//マスク内部のSali平均を閾値に設定

	int level = 178; //領域内部の加工レベル　調整すべし (128~255)

	int rows = bigSali.rows;
	int cols = bigSali.cols;

	
	int sum=0;
	int count=0;

	cv::Mat Kyoudo8(rows, cols, CV_8UC1);

	//全体を128中心に半分にして、底上げ
	for(int i=0 ; i<rows*cols ; i++){
			Kyoudo8.data[i] = 128+(bigSali.data[i]/2);
			if(mask.data[i]>200){
				//領域内部であれば
				sum = sum + Kyoudo8.data[i];
				count++;
			}
	}

	int Ikichi = 0;
	Ikichi = sum/count +0.5 - 10;
	if(Ikichi < 128){
		Ikichi = 128;
	}


	for(int i=0 ; i<rows*cols ; i++){
		if(mask.data[i]>200){
			Kyoudo8.data[i] = level;
		}else if(Kyoudo8.data[i] > Ikichi){
			Kyoudo8.data[i] = 128 - (Kyoudo8.data[i] - Ikichi);
		}else{
			Kyoudo8.data[i] = 128;
		}
	}

	return(Kyoudo8);

	
}



//スケールに応じたマスの大きさでブロックの平均求める
cv::Mat block_scale(cv::Mat grayImg, int scale){

	int masu =0;
	masu = int( pow(2.0, double(scale)) );

	int yoko = grayImg.cols/masu;
	int tate = grayImg.rows/masu;

	//背景の平均値格納庫
	cv::Mat Ave(tate, yoko, CV_64FC1);

	int y=0;
	int x=0;

	for(int m = 0 ; m < tate; m++){
		for(int n = 0 ; n < yoko; n++){

			double sum =0.0;

			for(int q=0; q < masu ;q++){
				for(int p=0; p < masu ;p++){
					x = masu*n + p;
					y = masu*m + q;
					sum = sum + grayImg.data[grayImg.cols*y +x];
				}
			}

			Ave.at<double>(m,n) = sum/(masu*masu+0.0); 
		}
	}
//確認用
/*
cout<<"ーーー確認用出力ーーー"<<endl;
	for(int y1 = 0; y1<tate ;y1++){
		for(int x1 = 0; x1<yoko ;x1++){
		double pre = Ave.at<double>(y1,x1);
		cout<< pre <<endl;
		}
	}
*/
	return(Ave);		
};


/*


int level = ICscale.data[3*t];
	int haba =0;

	//影響を与えているスケールに合わせて，幅を定義
	if (level < 10){
		haba = 16;
	}else if(level>10 && level < 110){
		haba = 32;
	}else if(level>110 && level < 210){
		haba = 64;
	}else if(level>210 ){
		haba = 128;
	}
*/


//スケール7のブロックの周辺の値を双線形補間してならす
double bilinear_scale_ave(int i, int rows, int cols, int scale, int color_flag){

	//スケールと色フラグに応じて I,R,G,B,Y と スケール決める
	cv::Mat block;
	if(scale==8 && color_flag==1) block = blockI8;
	if(scale==7 && color_flag==1) block = blockI7;
	if(scale==6 && color_flag==1) block = blockI6;
	if(scale==5 && color_flag==1) block = blockI5;

	if(scale==8 && color_flag==2) block = blockR8;
	if(scale==7 && color_flag==2) block = blockR7;
	if(scale==6 && color_flag==2) block = blockR6;
	if(scale==5 && color_flag==2) block = blockR5;

	if(scale==8 && color_flag==3) block = blockG8;
	if(scale==7 && color_flag==3) block = blockG7;
	if(scale==6 && color_flag==3) block = blockG6;
	if(scale==5 && color_flag==3) block = blockG5;

	if(scale==8 && color_flag==4) block = blockB8;
	if(scale==7 && color_flag==4) block = blockB7;
	if(scale==6 && color_flag==4) block = blockB6;
	if(scale==5 && color_flag==4) block = blockB5;

	if(scale==8 && color_flag==5) block = blockY8;
	if(scale==7 && color_flag==5) block = blockY7;
	if(scale==6 && color_flag==5) block = blockY6;
	if(scale==5 && color_flag==5) block = blockY5;

	int masu =0;
	masu = int( pow(2.0, double(scale)) );

	int halfmasu = masu/2;

	int x = i%cols;
	int y = i/cols;

	int n = cols/ masu;
	int m = rows/ masu;

	int left =0;
	int right =0;
	int top =0;
	int bottom =0;



	if(x<=halfmasu){
		left = 0;
		right = 0;
	}
	
	if(x>= (cols-halfmasu) ){
		left = n-1;
		right = n-1;	
	}

	if(x>halfmasu && x< (cols-halfmasu) ){
		left = (x-halfmasu)/masu;
		right = left+1;
	}

	if(y<=halfmasu){
		top = 0;
		bottom = 0;		
	}
	
	if(y>= (rows-halfmasu) ){
		top = m-1;
		bottom = m-1;
	}
	
	if(y>halfmasu && y< (rows-halfmasu) ){
		top = (y-halfmasu)/masu;
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

	if (x < halfmasu){
		 p = x+halfmasu;
	}else{
	 p = (x-halfmasu)%masu;
	}

	if(y <= halfmasu){
		 q = y+halfmasu;
	}else{
	 q = (y-halfmasu)%masu;
	}


	double p2 = double((p+0.0)/(masu+0.0));
	double q2 = double((q+0.0)/(masu+0.0));

	double ave = 0.0;
	ave = (1-q2)*((1-p2)*lt + p2*rt) + q2*((1-p2)*lb + p2*rb);


	return(ave);
}

cv::Mat edge_distance(cv::Mat edge){

//	cout<<"a"<<endl;

	int rows = edge.rows;
	int cols = edge.cols;
	cv::Mat dis(edge.rows, edge.cols,CV_8UC1,cv::Scalar(240)); //初期化で距離を全て255に


	//探索範囲の閾値設定
	int  karnel = 7;//探索範囲の腕の長さの閾値 元は8だったところを7に変更
	int  gain = 70; //ブロックごとに増やす分

	for(int i=0; i<rows*cols ; i++){

		if( edge.data[i]>100){//edgeのある座標に関して
			//直接被ったところは dis=0とする
			dis.data[i]=0;

			int pre=255;//preの値が低ければそれを入れる
			int x = i%cols;
			int y = i/cols;

			for(int k=1; k <=karnel ; k++){
				
				if(k<4){
					pre= gain;//k=1,2,3のとき dis=70
				}else if(k>=4 && k<6){
					pre = 2*gain;//k=4,5のとき dis=140
				}else if(k>=6){//k=6,7,8のとき　dis=210
					pre = 3*gain;
				}


				//x,y を中心とした半径 k のクロスの線上
				for(int y2=y-k; y2 <= y+k ; y2++){
					for(int x2=x-k; x2 <= x+k ; x2++){

						//斜めの線上で
						if( ( abs(y2-y) + abs(x2-x)) == k){
						//画像の範囲内であれば
						if(y2>=0 && y2<rows && x2>=0 && x2 <cols){

							if(dis.data[y2*cols+x2] > pre){
								dis.data[y2*cols+x2] = pre;
							}

						}//if二つ分の終わり
						}

					}
				}//x2,y2ループの終わり

			}//kを1から半径の閾値まで回す終わり
		}//edge がエッジに載ってるかの判定
	}//全画素のループ終わり


	return(dis);
}


//影響を与えているスケールのICのうち小さい方を出す
cv::Mat minScaleLevel(int testNO, int preNO){
	
	cv::Mat I25;
	cv::Mat I26;
	cv::Mat I36;
	cv::Mat I37;
	cv::Mat I47;
	cv::Mat I48;

	cv::Mat RG25;
	cv::Mat RG26;
	cv::Mat RG36;
	cv::Mat RG37;
	cv::Mat RG47;
	cv::Mat RG48;

	cv::Mat BY25;
	cv::Mat BY26;
	cv::Mat BY36;
	cv::Mat BY37;
	cv::Mat BY47;
	cv::Mat BY48;

	I25 = cv::imread("NI_c2_s5.bmp",0);   //0
	I26 = cv::imread("NI_c2_s6.bmp",0);  //50
	I36 = cv::imread("NI_c3_s6.bmp",0);  //100
	I37 = cv::imread("NI_c3_s7.bmp",0);  //150
	I47 = cv::imread("NI_c4_s7.bmp",0); //200
	I48 = cv::imread("NI_c4_s8.bmp",0); //255

	RG25 = cv::imread("NRG_c2_s5.bmp",0);
	RG26 = cv::imread("NRG_c2_s6.bmp",0);
	RG36 = cv::imread("NRG_c3_s6.bmp",0);
	RG37 = cv::imread("NRG_c3_s7.bmp",0);
	RG47 = cv::imread("NRG_c4_s7.bmp",0);
	RG48 = cv::imread("NRG_c4_s8.bmp",0);

	BY25 = cv::imread("NBY_c2_s5.bmp",0);
	BY26 = cv::imread("NBY_c2_s6.bmp",0);
	BY36 = cv::imread("NBY_c3_s6.bmp",0);
	BY37 = cv::imread("NBY_c3_s7.bmp",0);
	BY47 = cv::imread("NBY_c4_s7.bmp",0);
	BY48 = cv::imread("NBY_c4_s8.bmp",0);
	
	int rows = I25.rows;
	int cols = I25.cols;

	cv::Mat minICdif(rows,cols,CV_8UC1);

	//比較処理
	for(int t=0 ; t<(rows*cols) ; t++){
		double Ia;
		double Ib;
		int Ilevel = 2;

		double Ca;
		double Cb;
		int Clevel = 2;

		Ia = I25.data[t];
		Ca = RG25.data[t] + BY25.data[t];

		Ib = I26.data[t];
		Cb = RG26.data[t] + BY26.data[t];
		if(Ia<Ib){
			Ilevel = 2;
			Ia = Ib;
		}
		if(Ca<Cb){
			Clevel = 2;
			Ca = Cb;
		}

		Ib = I36.data[t];
		Cb = RG36.data[t] + BY36.data[t];
		if(Ia<Ib){
			Ilevel = 3;
			Ia = Ib;
		}
		if(Ca<Cb){
			Clevel = 3;
			Ca = Cb;
		}

		Ib = I37.data[t];
		Cb = RG37.data[t] + BY37.data[t];
		if(Ia<Ib){
			Ilevel = 3;
			Ia = Ib;
		}
		if(Ca<Cb){
			Clevel = 3;
			Ca = Cb;
		}

		Ib = I47.data[t];
		Cb = RG47.data[t] + BY47.data[t];
		if(Ia<Ib){
			Ilevel = 4;
			Ia = Ib;
		}
		if(Ca<Cb){
			Clevel = 4;
			Ca = Cb;
		}

		Ib = I48.data[t];
		Cb = RG48.data[t] + BY48.data[t];
		if(Ia<Ib){
			Ilevel = 4;
			Ia = Ib;
		}
		if(Ca<Cb){
			Clevel = 4;
			Ca = Cb;
		}

		int minlevel=0;
		if(Ilevel < Clevel){
			minlevel = Ilevel;
		}else{
			minlevel = Clevel;
		}
		
		//IandCdif の一枚に情報を集約
		minICdif.data[t] = minlevel*60;
	}

	char nameICscalemin[40];
	sprintf_s(nameICscalemin,"test%03d/minICscale_kaisuu%03d.bmp",testNO,preNO);
	cv::imwrite(nameICscalemin,minICdif);

	return(minICdif);
}

cv::Mat MiniMini(const cv::Mat edit,int count){
	cv::Mat img;
	img = cv::imread(Infile);
	
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
	}


		//(a^2 + b^2) を小さくするという制限もいれる。ここでの　L は仮の値として
	int L = N/30;

  Ba = ( (N-L) * Bsum_xy - Bsum_x * Bsum_y) / ( (N-L) * (Bsum_x2-L) - pow(Bsum_x, 2));
  Bb = ((Bsum_x2-L) * Bsum_y - Bsum_xy * Bsum_x) / ((N-L) * (Bsum_x2-L) - pow(Bsum_x, 2));

  Ga = ( (N-L) * Gsum_xy - Gsum_x * Gsum_y) / ( (N-L) * (Gsum_x2-L) - pow(Gsum_x, 2));
  Gb = ((Gsum_x2-L) * Gsum_y - Gsum_xy * Gsum_x) / ((N-L) * (Gsum_x2-L) - pow(Gsum_x, 2));

  Ra = ( (N-L) * Rsum_xy - Rsum_x * Rsum_y) / ( (N-L) * (Rsum_x2-L) - pow(Rsum_x, 2));
  Rb = ((Rsum_x2-L) * Rsum_y - Rsum_xy * Rsum_x) / ((N-L) * (Rsum_x2-L) - pow(Rsum_x, 2));
//L 利用ここまで

  //L利用しない場合
/*  Ba = (N * Bsum_xy - Bsum_x * Bsum_y) / (N * Bsum_x2 - pow(Bsum_x, 2));
  Bb = (Bsum_x2 * Bsum_y - Bsum_xy * Bsum_x) / (N * Bsum_x2 - pow(Bsum_x, 2));

  Ga = (N * Gsum_xy - Gsum_x * Gsum_y) / (N * Gsum_x2 - pow(Gsum_x, 2));
  Gb = (Gsum_x2 * Gsum_y - Gsum_xy * Gsum_x) / (N * Gsum_x2 - pow(Gsum_x, 2));

  Ra = (N * Rsum_xy - Rsum_x * Rsum_y) / (N * Rsum_x2 - pow(Rsum_x, 2));
  Rb = (Rsum_x2 * Rsum_y - Rsum_xy * Rsum_x) / (N * Rsum_x2 - pow(Rsum_x, 2));
*/
  cout<<"--最小二乗--"<<count<<endl;
  printf("Ba = %f\n", Ba);
  printf("Bb = %f\n", Bb);

    printf("Ga = %f\n", Ga);
  printf("Gb = %f\n", Gb);

    printf("Ra = %f\n", Ra);
  printf("Rb = %f\n", Rb);



  cv::Mat out(rows, cols, CV_8UC3);
  for(int t=0; t<N; t++){
	 // cout<<t<<endl;
/*
	  if(t>65000){
		  //int pre = out.data[t*3];
		  int pre2 = edit.data[t*3];
		  cout<<t<<":::"<<pre2<<","<<Ba <<","<<Bb<<endl;
	  }
  */
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


/*
	  if(t>66800){
		  int pre = out.data[t*3];
		  int pre2 = edit.data[t*3];
		  cout<<t<<":"<<pre<<","<<pre2<<","<<Ba <<","<<Bb<<endl;
	  }
  */
  }
 
  return (out);


}

//HSVにおいてSVの最小二乗で正規化する
cv::Mat MiniMiniHSV(const cv::Mat edit,int count){

	cv::Mat img;
	img = cv::imread(Infile);
	
	int rows=img.rows;
	int cols=img.cols;
	int i;

	int N = rows*cols;

	//HSVに分解するための画像領域確保
	cv::Mat imgHSV(rows,cols,CV_8UC3);
	cv::Mat editHSV(rows,cols,CV_8UC3);
	//cv::Mat Vimage2(rows, cols,CV_8UC1);
	//cv::Mat Simage2(rows, cols,CV_8UC1);
	//cv::Mat Himage2(rows, cols,CV_8UC1);

	//変換
	cv::cvtColor(img, imgHSV, CV_BGR2HSV);
	cv::cvtColor(edit, editHSV, CV_BGR2HSV);


	//Gと書いてあるが、S
		double Ga = 0.0;
	double Gb = 0.0;
	double Gsum_xy = 0.0, Gsum_x = 0.0, Gsum_y = 0.0, Gsum_x2 = 0.0;

	//Rと書いてあるが、V
	double Ra = 0.0;
	double Rb = 0.0;
	double Rsum_xy = 0.0, Rsum_x = 0.0, Rsum_y = 0.0, Rsum_x2 = 0.0;

//プロット用
//		FILE *fpS;
//		FILE *fpV;
//		fpS = fopen("S.dat", "w");
//		fpV = fopen("V.dat", "w");

	for (i=0; i<N; i++){
	Gsum_xy += double(imgHSV.data[i*3+1] * editHSV.data[i*3+1]);
    Gsum_x += double(editHSV.data[i*3+1]);
    Gsum_y += double(imgHSV.data[i*3+1]);
    Gsum_x2 += pow(double(editHSV.data[i*3+1]), 2);

	Rsum_xy += double(imgHSV.data[i*3+2] * editHSV.data[i*3+2]);
   Rsum_x += double(editHSV.data[i*3+2]);
    Rsum_y += double(imgHSV.data[i*3+2]);
    Rsum_x2 += pow(double(editHSV.data[i*3+2]), 2);

	//	fprintf(fpS,"%d %d\n", imgHSV.data[i*3+1] , editHSV.data[i*3+1]  );//プロット用
	//	fprintf(fpV,"%d %d\n", imgHSV.data[i*3+2] , editHSV.data[i*3+2]  );//プロット用
	}

//	fclose(fpS);
//	fclose(fpV);
//	system("wgnuplot -persist SVdot.plt");//グラフの書式設定は .plt に書きこんでおく


  Ga = (N * Gsum_xy - Gsum_x * Gsum_y) / (N * Gsum_x2 - pow(Gsum_x, 2));
  Gb = (Gsum_x2 * Gsum_y - Gsum_xy * Gsum_x) / (N * Gsum_x2 - pow(Gsum_x, 2));

  Ra = (N * Rsum_xy - Rsum_x * Rsum_y) / (N * Rsum_x2 - pow(Rsum_x, 2));
  Rb = (Rsum_x2 * Rsum_y - Rsum_xy * Rsum_x) / (N * Rsum_x2 - pow(Rsum_x, 2));

  cout<<"--HSV最小二乗--"<<count<<endl;
  printf("Sa = %f\n", Ga);
  printf("Sb = %f\n", Gb);

    printf("Va = %f\n", Ra);
  printf("Vb = %f\n", Rb);

  cv::Mat outHSV(rows, cols, CV_8UC3);
  for(int t=0; t<N; t++){

	  int R=0;
	  int G =0;
	  int B =0;
	  B = editHSV.data[t*3];
	  G = int(editHSV.data[t*3+1] * Ga + Gb+0.5);
	  R = int(editHSV.data[t*3+2] * Ra + Rb+0.5);
 
	  if(G> 255)G=255;
	  if(R> 255)R=255;

	  	  if(G< 0)G=0;
		  	  if(R< 0)R=0;

	  outHSV.data[t*3] = B;
	  outHSV.data[t*3+1] = G;
	  outHSV.data[t*3+2] = R;

  }

	cv::Mat out(rows,cols,CV_8UC3);
	cv::cvtColor(outHSV,out,CV_HSV2BGR);
 
  return (out);


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

//R,G,Bバラバラではなく (R+G+B)/3 を利用して一つの値で最小二乗
cv::Mat MiniMiniRGB3(const cv::Mat edit,int count){

	cv::Mat img;
	img = cv::imread(Infile);
	
	int rows=img.rows;
	int cols=img.cols;
	int i;

	int N = rows*cols;

	double RGBa = 0.0;
	double RGBb = 0.0;
	double RGBsum_xy = 0.0, RGBsum_x = 0.0, RGBsum_y = 0.0, RGBsum_x2 = 0.0;

	for (i=0; i<N; i++){

	RGBsum_xy += double( ((img.data[i*3] + img.data[i*3+1] + img.data[i*3+2])/3) * ((edit.data[i*3] + edit.data[i*3+1] + edit.data[i*3+2])/3) );
    RGBsum_x += double((edit.data[i*3] + edit.data[i*3+1] + edit.data[i*3+2])/3);
    RGBsum_y += double((img.data[i*3] + img.data[i*3+1] + img.data[i*3+2])/3);
    RGBsum_x2 += pow(double((edit.data[i*3] + edit.data[i*3+1] + edit.data[i*3+2])/3), 2);
	}

	RGBa = (N * RGBsum_xy - RGBsum_x * RGBsum_y) / (N * RGBsum_x2 - pow(RGBsum_x, 2));
	RGBb = (RGBsum_x2 * RGBsum_y - RGBsum_xy * RGBsum_x) / (N * RGBsum_x2 - pow(RGBsum_x, 2));

	  cv::Mat out(rows, cols, CV_8UC3);


    for(int t=0; t<3*N; t++){

	  int after = int(edit.data[t] * RGBa + RGBb +0.5);
	  if(after> 255)after=255;
	  if(after< 0)after=0;
		out.data[t] = after;
  }

  return (out);

}

//カーネルをかけてから最小二乗
cv::Mat MiniMiniK(const cv::Mat edit,int count){
	cv::Mat img;
	img = cv::imread(Infile);
	
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


	//カーネルを一時保管する
	cv::Mat img2(rows, cols, CV_8UC3); 
	cv::Mat edit2(rows, cols, CV_8UC3);
		for (int y=0; y<rows; y++){
		for (int x=0; x<cols; x++){

			//カーネル16のとき
			double center = 4.0;
			double next = 8.0;
			double cross = 16.0;

			//カーネル9のとき
			//double center = 9.0;
			//double next = 9.0;
			//double cross = 9.0;


			//中心
			double preiB  = img.data[(y*cols+x)*3] /center;
			double preiG  = img.data[(y*cols+x)*3+1] /center;
			double preiR  = img.data[(y*cols+x)*3+2] /center;

			double preEB  = edit.data[(y*cols+x)*3] /center;
			double preEG  = edit.data[(y*cols+x)*3+1] /center;
			double preER  = edit.data[(y*cols+x)*3+2] /center;

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


	for (i=0; i<N; i++){
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
	}


 
  Ba = (N * Bsum_xy - Bsum_x * Bsum_y) / (N * Bsum_x2 - pow(Bsum_x, 2));
  Bb = (Bsum_x2 * Bsum_y - Bsum_xy * Bsum_x) / (N * Bsum_x2 - pow(Bsum_x, 2));

  Ga = (N * Gsum_xy - Gsum_x * Gsum_y) / (N * Gsum_x2 - pow(Gsum_x, 2));
  Gb = (Gsum_x2 * Gsum_y - Gsum_xy * Gsum_x) / (N * Gsum_x2 - pow(Gsum_x, 2));

  Ra = (N * Rsum_xy - Rsum_x * Rsum_y) / (N * Rsum_x2 - pow(Rsum_x, 2));
  Rb = (Rsum_x2 * Rsum_y - Rsum_xy * Rsum_x) / (N * Rsum_x2 - pow(Rsum_x, 2));

  cout<<"--最小二乗--"<<count<<endl;
  printf("Ba = %f\n", Ba);
  printf("Bb = %f\n", Bb);

    printf("Ga = %f\n", Ga);
  printf("Gb = %f\n", Gb);

    printf("Ra = %f\n", Ra);
  printf("Rb = %f\n", Rb);



  cv::Mat out(rows, cols, CV_8UC3);
  for(int t=0; t<N; t++){
	 // cout<<t<<endl;
/*
	  if(t>65000){
		  //int pre = out.data[t*3];
		  int pre2 = edit.data[t*3];
		  cout<<t<<":::"<<pre2<<","<<Ba <<","<<Bb<<endl;
	  }
  */
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


/*
	  if(t>66800){
		  int pre = out.data[t*3];
		  int pre2 = edit.data[t*3];
		  cout<<t<<":"<<pre<<","<<pre2<<","<<Ba <<","<<Bb<<endl;
	  }
  */
  }
 
  return (out);


}

//ガウシアンをかけてから最小二乗(加工後画像、更新回数、ガウシアンかけるカーネルの大きさ格納画像)
cv::Mat MiniMiniGau(const cv::Mat edit,int count,cv::Mat numK){
	cv::Mat img;
	img = cv::imread(Infile);
	
	int rows=img.rows;
	int cols=img.cols;
	int i;

	int N = rows*cols;

	//加工後画像をガウシアン平滑化
	cv::Mat edit3(rows,cols,CV_8UC3);
	cv::Mat edit5(rows,cols,CV_8UC3);
	cv::Mat edit7(rows,cols,CV_8UC3);

	cv::Mat img3(rows,cols,CV_8UC3);
	cv::Mat img5(rows,cols,CV_8UC3);
	cv::Mat img7(rows,cols,CV_8UC3);
/*
	//カーネルの大きさは三段階
	cv::GaussianBlur(edit,edit3, cv::Size(3,3) ,0.8,0.8);
	cv::GaussianBlur(edit,edit5,cv::Size(5,5) ,1.0,1.0);
	cv::GaussianBlur(edit,edit7,cv::Size(7,7) ,1.3,1.3);

		cv::GaussianBlur(img,img3, cv::Size(3,3) ,0.8,0.8);
	cv::GaussianBlur(img,img5,cv::Size(5,5) ,1.0,1.0);
	cv::GaussianBlur(img,img7,cv::Size(7,7) ,1.3,1.3);
*/
	cv::blur(img,img3,cv::Size(3,3));
cv::blur(img,img5,cv::Size(5,5));
cv::blur(img,img7,cv::Size(7,7));

	cv::blur(edit,edit3,cv::Size(3,3));
cv::blur(edit,edit5,cv::Size(5,5));
cv::blur(edit,edit7,cv::Size(7,7));

	/*
	if(count==1){
		cv::imwrite("Gau3_sig08.bmp",edit3);
		cv::imwrite("Gau5_sig10.bmp",edit5);
		cv::imwrite("Gau7_sig13.bmp",edit7);
	}
*/

	//座標ごとに適した平滑化をした後のedit
	cv::Mat editG(rows,cols,CV_8UC3);
	cv::Mat imgG(rows,cols,CV_8UC3);
/*
	for(int i=0;i<rows*cols;i++){
		int preK = numK.data[i];

		if(preK<=70){//エッジから0-3マスは平滑化なし
			editG.data[i*3]=edit.data[i*3];
			editG.data[i*3+1]=edit.data[i*3+1];
			editG.data[i*3+2]=edit.data[i*3+2];
			imgG.data[i*3]=img.data[i*3];
			imgG.data[i*3+1]=img.data[i*3+1];
			imgG.data[i*3+2]=img.data[i*3+2];
		}else if(preK==140){//エッジから4,5マスは3*3で平滑化
			editG.data[i*3]=edit3.data[i*3];
			editG.data[i*3+1]=edit3.data[i*3+1];
			editG.data[i*3+2]=edit3.data[i*3+2];
			imgG.data[i*3]=img3.data[i*3];
			imgG.data[i*3+1]=img3.data[i*3+1];
			imgG.data[i*3+2]=img3.data[i*3+2];
		}else if(preK==210){//エッジから6,7マスは5*5で平滑化
			editG.data[i*3]=edit5.data[i*3];
			editG.data[i*3+1]=edit5.data[i*3+1];
			editG.data[i*3+2]=edit5.data[i*3+2];
				imgG.data[i*3]=img5.data[i*3];
			imgG.data[i*3+1]=img5.data[i*3+1];
			imgG.data[i*3+2]=img5.data[i*3+2];
		}else if(preK==240){//エッジから8マス以上は7*7で平滑化
			editG.data[i*3]=edit7.data[i*3];
			editG.data[i*3+1]=edit7.data[i*3+1];
			editG.data[i*3+2]=edit7.data[i*3+2];
				imgG.data[i*3]=img7.data[i*3];
			imgG.data[i*3+1]=img7.data[i*3+1];
			imgG.data[i*3+2]=img7.data[i*3+2];
		}else{
			cout<<"====error!=====MiniMiniGau==="<<i<<endl;
		}
	}
*/

		for(int i=0;i<rows*cols;i++){
		int preK = numK.data[i];

		if(preK<=70){//エッジから0-3マスは3*3で平滑化
			editG.data[i*3]=edit3.data[i*3];
			editG.data[i*3+1]=edit3.data[i*3+1];
			editG.data[i*3+2]=edit3.data[i*3+2];
			imgG.data[i*3]=img3.data[i*3];
			imgG.data[i*3+1]=img3.data[i*3+1];
			imgG.data[i*3+2]=img3.data[i*3+2];
		}else if(preK==140){//エッジから4,5マスは5*5で平滑化
			editG.data[i*3]=edit5.data[i*3];
			editG.data[i*3+1]=edit5.data[i*3+1];
			editG.data[i*3+2]=edit5.data[i*3+2];
			imgG.data[i*3]=img5.data[i*3];
			imgG.data[i*3+1]=img5.data[i*3+1];
			imgG.data[i*3+2]=img5.data[i*3+2];
		}else if(preK==210){//エッジから6マス以上はは7*7で平滑化
			editG.data[i*3]=edit7.data[i*3];
			editG.data[i*3+1]=edit7.data[i*3+1];
			editG.data[i*3+2]=edit7.data[i*3+2];
			imgG.data[i*3]=img7.data[i*3];
			imgG.data[i*3+1]=img7.data[i*3+1];
			imgG.data[i*3+2]=img7.data[i*3+2];
		}else if(preK==240){//エッジから8マス以上は7*7で平滑化
			editG.data[i*3]=edit7.data[i*3];
			editG.data[i*3+1]=edit7.data[i*3+1];
			editG.data[i*3+2]=edit7.data[i*3+2];
			imgG.data[i*3]=img7.data[i*3];
			imgG.data[i*3+1]=img7.data[i*3+1];
			imgG.data[i*3+2]=img7.data[i*3+2];
		}else{
			cout<<"====error!=====MiniMiniGau==="<<i<<endl;
		}
		}

/*
	editG=edit3;//全部カーネル3にしちゃう
	imgG=img3;
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


	


	for (i=0; i<N; i++){
	Bsum_xy += double(imgG.data[i*3] * editG.data[i*3]);
    Bsum_x += double(editG.data[i*3]);
    Bsum_y += double(imgG.data[i*3]);
    Bsum_x2 += pow(double(editG.data[i*3]), 2);

	Gsum_xy += double(imgG.data[i*3+1] * editG.data[i*3+1]);
    Gsum_x += double(editG.data[i*3+1]);
    Gsum_y += double(imgG.data[i*3+1]);
    Gsum_x2 += pow(double(editG.data[i*3+1]), 2);

	Rsum_xy += double(imgG.data[i*3+2] * editG.data[i*3+2]);
    Rsum_x += double(editG.data[i*3+2]);
    Rsum_y += double(imgG.data[i*3+2]);
    Rsum_x2 += pow(double(editG.data[i*3+2]), 2);
	}


 
  Ba = (N * Bsum_xy - Bsum_x * Bsum_y) / (N * Bsum_x2 - pow(Bsum_x, 2));
  Bb = (Bsum_x2 * Bsum_y - Bsum_xy * Bsum_x) / (N * Bsum_x2 - pow(Bsum_x, 2));

  Ga = (N * Gsum_xy - Gsum_x * Gsum_y) / (N * Gsum_x2 - pow(Gsum_x, 2));
  Gb = (Gsum_x2 * Gsum_y - Gsum_xy * Gsum_x) / (N * Gsum_x2 - pow(Gsum_x, 2));

  Ra = (N * Rsum_xy - Rsum_x * Rsum_y) / (N * Rsum_x2 - pow(Rsum_x, 2));
  Rb = (Rsum_x2 * Rsum_y - Rsum_xy * Rsum_x) / (N * Rsum_x2 - pow(Rsum_x, 2));

  cout<<"--最小二乗--"<<count<<endl;
  printf("Ba = %f\n", Ba);
  printf("Bb = %f\n", Bb);

    printf("Ga = %f\n", Ga);
  printf("Gb = %f\n", Gb);

    printf("Ra = %f\n", Ra);
  printf("Rb = %f\n", Rb);



  cv::Mat out(rows, cols, CV_8UC3);
  for(int t=0; t<N; t++){
	 // cout<<t<<endl;
/*
	  if(t>65000){
		  //int pre = out.data[t*3];
		  int pre2 = edit.data[t*3];
		  cout<<t<<":::"<<pre2<<","<<Ba <<","<<Bb<<endl;
	  }
  */
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


/*
	  if(t>66800){
		  int pre = out.data[t*3];
		  int pre2 = edit.data[t*3];
		  cout<<t<<":"<<pre<<","<<pre2<<","<<Ba <<","<<Bb<<endl;
	  }
  */
  }
 
  return (out);


}

