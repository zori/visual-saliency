/*
�ŏ����@

11/02 �Ƃ肠�����A�^����ꂽ�f�[�^�ɑ΂���ŏ����𓮂����Ă݂�
11/04 input �� edit �摜����Aedit ��input �Ɋ񂹂�ׂ̌W���v�Z

	GauEdge Lay �ɑg�ݍ���ŁA�X�V����x�ɁAinput�摜�Ɋ񂹂�v�Z

11/05 HSV�ōŏ���悵�Ă݂� H�͌Œ�ASV�ōŏ����

11/09 RGB ��16�����̍ŏ����

11/16 L�����Č���(HSV��16�������O����)

11/20 RGB�œ��ꂵ�����K��ver (L���O����)

11/22 �K�E�V�A�������Ă��琳�K���̏��ŕۑ�
      �����āA�J�[�l����������֐��������B�ǂ��C�B�������Ă���K�E�V�A�����A�{��MiniMini�Ŏ����I�I


12/17 
	   edge_distance�Ɋւ��āA
	//�T���͈͂�臒l�ݒ�
	int  karnel = 7;//�T���͈̘͂r�̒�����臒l ����8�������Ƃ����7�ɕύX�@�����i�Ӗ����l�����āj

	���K���̍ۂ̃J�[�l����3*3�̒�������9��16�𗘗p����̂ł͂Ȃ��A
	�X�P�[���l���K�E�V�A���Őݒ肵���K�E�V�A����������͈͂𗘗p���Ă���Ă݂���
	�Ƃ����킯��bigloop ��������A
	MiniMiniGau�@�������@�܂��v�����ʂ�ɂ͓����Ă���ۂ�

*/
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>

#include <math.h> //�ŏ����Œǉ�

#pragma comment(lib,"cv200d.lib")
#pragma comment(lib,"cxcore200d.lib")
#pragma comment(lib,"highgui200d.lib")

#pragma comment(lib,"cv200.lib")
#pragma comment(lib,"cxcore200.lib")
#pragma comment(lib,"highgui200.lib")

using namespace std;

//#define N 131072 /* �f�[�^�� */


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

cv::Mat block_scale(cv::Mat, int scale); //�u���b�N�����Mat �̓O���[�o���ϐ��Ő錾���邱��
double bilinear_scale_ave(int i, int rows, int cols, int scale, int color_flag);

cv::Mat edge_distance(cv::Mat);

cv::Mat minScaleLevel(int , int );
cv::Mat MiniMini(const cv::Mat edit,int count);
cv::Mat MiniMiniHSV(const cv::Mat edit,int count);
cv::Mat MiniMiniRGB3(const cv::Mat edit,int count);
cv::Mat MiniMiniK(const cv::Mat edit,int count);
cv::Mat MiniMiniGau(const cv::Mat edit,int count,cv::Mat numK);


cv::Mat blockI5; //�u���b�N���ϒl������Ƃ�����
cv::Mat blockI6;
cv::Mat blockI7;
cv::Mat blockI8;
cv::Mat blockR5; //�u���b�N���ϒl������Ƃ�����
cv::Mat blockR6;
cv::Mat blockR7;
cv::Mat blockR8;
cv::Mat blockG5; //�u���b�N���ϒl������Ƃ�����
cv::Mat blockG6;
cv::Mat blockG7;
cv::Mat blockG8;
cv::Mat blockB5; //�u���b�N���ϒl������Ƃ�����
cv::Mat blockB6;
cv::Mat blockB7;
cv::Mat blockB8;
cv::Mat blockY5; //�u���b�N���ϒl������Ƃ�����
cv::Mat blockY6;
cv::Mat blockY7;
cv::Mat blockY8;

cv::Mat rgbbefore; //�ŏ��Ɍv�Z�����ω���
cv::Mat rgbgauss5; //�K�E�V�A����������̂���
cv::Mat rgbgauss6;
cv::Mat rgbgauss7;
cv::Mat rgbgauss8;

cv::Mat Mini16block(const cv::Mat edit,int count);
void MiniMini16ab(cv::Mat edit, cv::Mat img,int xb ,int yb);
//double bilinear_ave(cv::Mat block ,int i, int rows, int cols);

cv::Mat blockBa(4, 4, CV_64FC1); //16�����̍ŏ����̌��ʂ��O���[�o���ۊ�
cv::Mat blockGa(4, 4, CV_64FC1);
cv::Mat blockRa(4, 4, CV_64FC1);
cv::Mat blockBb(4, 4, CV_64FC1);
cv::Mat blockGb(4, 4, CV_64FC1);
cv::Mat blockRb(4, 4, CV_64FC1);

char inputfile[] = "Hiroshima_07.jpg"; //���͌��t�@�C����
char maskfile[] = "Hiroshima_07_mask1.bmp"; //���̓}�X�N�t�@�C����

char Infile[] = "Hiroshima_07.jpg"; //�ŏ����̗��z�t�@�C���� ���ɉ����Ȃ���Γ��͂Ɠ����ł悢

//char segfile[] = "109_seg.bmp"; //���̓}�X�N�t�@�C����

int testNO = 0; //�t�H���_�̖��O



int ikiti = 0;//臒l
int kaisuu = 0;//���x

int full = 1;//�S�摜���o�͂��邩�ǂ��������肷��t���O

int x1y1[100];//���W���Ƃ�Saliency��ۊ�
int x2y2[100];
int x3y3[100];
int x4y4[100];
int xy=0; //���W�Ǘ��p�̃O���[�o��

int X1=0,X2=0,X3=0,Y1=0,Y2=0,Y3=0,X4=0,Y4=0;//���W�@�[���������




int main(void){

	//��������
	//���W�A�񐔓���	
	cout<<"�t�H���_�ԍ�"<<endl;
	cout<<"testNO:";
	cin >> testNO;

	//�t�H���_�ԍ��̂����f�B���N�g���쐬
	char namedir[40];
	sprintf_s(namedir,"mkdir test%03d",testNO);
	system(namedir);

	cout<<"S�l�𑪂�T���v���_���W�����"<<endl;

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

	cout<<"������(k)�w��:";
	cin>>kaisuu;

	
	//�}�X�N�摜�ǂݍ���
	cv::Mat mask;
	mask = cv::imread(maskfile,0);//���m�N���Ń}�X�N�摜�ǂݍ���

	//�ꖇ�ڂ�before�摜�ǂݍ���
	cv::Mat before;
	before = cv::imread(inputfile);

	//���H����
	//�w��񐔂���
	for(int t=0 ; t<kaisuu ; t++){
		/*-------------------------------
		�@bigloop�̒��g
	�@�@	before��S�l�v�Z�A���W�ۊ�
		�@�@���x�W���}�b�v�A�F�̏d�ݕt���W������
		�@�@���H�������ʂ�after��Ԃ�
		----------------------------------*/
		cv::Mat after;
		//before��after�։��H
		after = bigloop(before,mask,t);
		//���ɔ����Č�after��before�ɃR�s�[
		before = after;
	}

	//�Ō�̈ꖇ��S�l�v�Z�A���W�ۊ�
	Saliency(before,kaisuu);

	//���W�y�уO���t�o��
	//�ۊǂ������W�̕\��
	 cout << "(" << X1 << "," << Y1 << "), (" << X2 << "," << Y2 << "), (" << X3 << "," << Y3 << "), (" <<X4 << "," << Y4 << ")" << endl;
	 for (int t = 0 ; t < kaisuu; t++) {
		  cout << t << "," << x1y1[t] << "," << x2y2[t] << "," << x3y3[t] << "," << x4y4[t] <<  endl;
	 }

	//gnuplot��eps�̃O���t�쐬
	FILE *fp1;
	FILE *fp2;
	FILE *fp3;
	FILE *fp4;

	fp1 = fopen("Sali1.dat", "w");//��̃T���v�����W�ɂ���̃t�@�C��
	fp2 = fopen("Sali2.dat", "w");
	fp3 = fopen("Sali3.dat", "w");
	fp4 = fopen("Sali4.dat", "w");

	//���W�̒l��dat�t�@�C���ɕۑ�
	FILE *zahyou;
	zahyou = fopen("Zahyou.dat","w");//���W�ۑ��p
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


		fprintf(fp1, "%d %d\n", t, S1);//k��Saliency�l����s���^����
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
	
	system("wgnuplot -persist petSali.plt");//�O���t�̏����ݒ�� testSali.plt �ɏ�������ł���

	cv::waitKey();
	return 0;
}



	



//�J���[���͉摜��I�ɐ��l��������֐� ����(����,�o��)
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

//R,G,B�ɐ��l��������֐��@����(����,�o��,��������,��������,��������)
cv::Mat makeBGR(const cv::Mat img,cv::Mat BGR,int tasu,int hiku1,int hiku2){

	//�����@0,b 1,g  2,r �Ȃ̂� 
	//R �Ȃ�@tasu=2 ����0,1��
	//G �Ȃ�@tasu=1     0,2
	//B �Ȃ�@tasu=0     1,2

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

//Y�ɐ��l��������֐��@����(����,�o��)
cv::Mat makeY(const cv::Mat img,cv::Mat Y){

	//�����@0,b 1,g  2,r

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



//��̔����摜�̉�f�l�̍��̐�Βl�����߂�֐� ����(�ЂƂ߂̉摜,���)
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

//double ���獷���Ƃ�
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

//��̔����摜�̉�f�l�̍������߂�֐� ����(�ЂƂ߂̉摜,���)
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

//��̔����摜�̉�f�l�� �a�����߂�֐� ����(�ЂƂ߂̉摜,���)
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

//�K�i������֐�

cv::Mat noramlization(cv::Mat img){

	int rows = img.rows;
	int cols = img.cols;

	//0-255�͈̔͂ɉ�������
	img = hirei(img);


	//�ő�l�����߂�
	int max = *std::max_element(&img.data[0], &img.data[rows * cols]);


	//�ɑ�l�̕��ς����߂�B
	//�㉺���E��4�_�Ɣ�r���A�ǂ������傫����΋ɑ�Ƃ݂Ȃ��@��ׂ�͉̂摜�͈͓̔�(������[�������)
	//�ɑ�ł���΂���𑫂��Ă���
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

				//�ɑ�l�ł��肩�ő�l�łȂ���Ή��Z
				if (now > upleft && now > up && now > upright &&
				now > left && now > right &&
				now > downleft && now > down && now > downright && now != max){
				wa = wa + now;
				kazu++;

				//	cout << "x:" << x << "y:" << y << "now:" << now << endl;
				}		
		}
	}



	//�ő�l�������āA�ɑ�l���ς����߂�
	if ( kazu==0){
		ave = 0;	
	} else {
		ave = wa / kazu ;
	}

	

	//M-m���
	double scale = 0.0;
	int sa =0;
	sa = (max - ave);
	if (sa ==0){
		scale = 1.0;
	}else {
		scale = double( sa * sa );
	}

	//cout <<"max:"<< max << ",wa:" <<wa<<",kazu:" <<kazu << ",ave:" <<  ave << ",scale:" << scale << endl;

	//�S�v�f�� M-n����������	
	cv::Mat n(rows,cols,CV_64FC1);
			
			for(int y=0 ; y<n.rows  ;y++){
					for(int x=0 ; x<n.cols ; x++){					
					n.at<double>(y,x) = double(img.data[(cols*y+x)] * scale) ;		
				}
			}	

	return(n);

}

//�S��f�l���͂��̂܂܂�0����255�͈̔͂ɉ������ފ֐�
cv::Mat hirei(cv::Mat img){

	int rows = img.rows;
	int cols = img.cols;

	cv::Mat after(rows,cols,CV_8UC1);

	//�ő�E�ŏ��l�����߂� STL: �W�����C�u�������@#include<algorithm>�K�{
	double max = *std::max_element(&img.data[0], &img.data[rows * cols]);
	double min = *std::min_element(&img.data[0], &img.data[rows * cols]);
	
	for(int i= 0;i<(rows*cols) ; i++){
			after.data[i] = unsigned char( 255*(img.data[i] - min) /(max-min) ) ;	//�e��f�ɔ��������	
		}
	return(after);

}

//�S��f�l���͂��̂܂܂�double�^��0����255�͈̔͂ɉ������ފ֐�
cv::Mat hirei64(cv::Mat img){

	int rows = img.rows;
	int cols = img.cols;

	cv::Mat after(rows,cols,CV_64FC1);

	//�ő�E�ŏ��l�����߂�
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

//�Q��1/2�̘a�����߂�֐� ����(�ЂƂ߂̉摜,��ځA�O��)
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



//�w��񐔂����_�E���X�P�[������֐��@�����i���͉摜�A �_�E���񐔁j
cv::Mat DownSample(const cv::Mat a,  int down ){
	
	//�������ݗp�摜�p��

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat in(rows,cols,CV_64FC1);	
	in = a;
	cv::Mat out;

	//down�񐔂����_�E���T���v�����O
	for (int i = 1 ; i<(down+1) ; i++){
		in = AvepyrDown64FC1(in,out);	
	}
	return(in);

}

//�w��񐔂����A�b�v�T���v�����O����֐��@�����i���͉摜�A�A�b�v�񐔁j
cv::Mat UpSample(const cv::Mat a,  int up){
	
	//�������ݗp�摜�p��

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat in(rows,cols,CV_64FC1);	
	in = a;
	cv::Mat out;

	//up�񐔂����_�E���T���v�����O
	for (int i = 1 ; i<(up+1) ; i++){
		cv::pyrUp(in,out);
		in = out;	
	}
	return(in);

}



//���͂��ꂽ�@32FU1 �摜���@������悤�ȁ@8UC1 �摜�ɂ��ďo�͂���֐�

void changeoutput(cv::Mat in , char filename[]){

	//�o�͉摜�p��

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
			pre8.data[i] = int( 255*pre32.data[i]/(max-min) ) ;	//�e��f�ɔ��������	
		}

	cv::imwrite(filename, pre8);

	//cout << filename << endl;


}


//8�r�b�g 8UC1�@�w��񐔂����_�E���X�P�[������֐��@�����i���͉摜�A �_�E���񐔁j
cv::Mat DownSample8(const cv::Mat a,  int down ){
	
	//�������ݗp�摜�p��

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat in(rows,cols,CV_8UC1);	
	in = a;
	//���ɃT�C�Y�w��O��
	cv::Mat out;

	//down�񐔂����_�E���T���v�����O
	for (int i = 1 ; i<(down+1) ; i++){
		in = AvepyrDown8UC1(in,out);
	}
	return(in);

}

//8�r�b�g 8UC3�@�w��񐔂����_�E���X�P�[������֐��@�����i���͉摜�A �_�E���񐔁j
/*cv::Mat DownSample8UC3(const cv::Mat a,  int down ){
	
	//�������ݗp�摜�p��

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat in(rows,cols,CV_8UC3);	
	in = a;
	//���ɃT�C�Y�w��O��
	cv::Mat out;

	//down�񐔂����_�E���T���v�����O
	for (int i = 1 ; i<(down+1) ; i++){
		in = AvepyrDown8UC3(in,out);
	}
	return(in);

}
*/

//8�r�b�g�@�w��񐔂����A�b�v�T���v�����O����֐��@�����i���͉摜�A�A�b�v�񐔁j
cv::Mat UpSample8(const cv::Mat a,  int up){
	
	//�������ݗp�摜�p��

	int rows = a.rows;
	int cols = a.cols;

	cv::Mat in(rows,cols,CV_8UC1);

	in = a;

	cv::Mat out;

	//up�񐔂����A�b�v�T���v�����O
	for (int i = 1 ; i<(up+1) ; i++){
		cv::pyrUp(in,out);
		in = out;	
	}
	return(in);

}


//Sali�̍����Ƃ�������摜�ɏd�˂ďo�͂���֐� ���� �iSali(����) 64 1ch�A ���摜�i�J���[�j8 3ch�j
void Markver2(cv::Mat Sali,const cv::Mat preimg){


	//�������ݗp�摜�p��
	int rows = preimg.rows;
	int cols = preimg.cols;
	cv::Mat pre = preimg; //�悭�킩��񂪁A���ɂ����Ă���
	cv::Mat Markimg(rows/16,cols/16,CV_8UC3);	
	//���摜���X�P�[��4�܂ŏk��
	cv::resize(pre, Markimg, Markimg.size(),0,0);

	int row = Sali.rows;
	int col = Sali.cols;

	//�ő�Ƃ����̍��W�����߂�
	double minVal = 0.0, maxVal = 0.0;
	cv::Point minLoc = cv::Point(0,0);
	cv::Point maxLoc = cv::Point(0,0);

	cv::minMaxLoc(Sali, &minVal, &maxVal, &minLoc, &maxLoc);
	printf("SaliMax = %f, Loc(x, y) = (%d, %d)\n", maxVal, maxLoc.x, maxLoc.y);

	int MaxX = maxLoc.x;
	int MaxY = maxLoc.y;
	int step = Markimg.step;

	//�ő�l��Ԃ��h��Ԃ�
	Markimg.data[(step*MaxY+MaxX*3)] =  0;
	Markimg.data[(step*MaxY+MaxX*3+1)] =  0;
	Markimg.data[(step*MaxY+MaxX*3+2)] =  255;

	cv::Mat after(rows,cols,CV_8UC3);

	cv::resize(Markimg,after,after.size(),0,0);

	cv::imwrite(nameMark,after);

}

//Sali�̍������W�o�͂���֐� �iSali(����) 64 1ch�j
void Marklocal(cv::Mat Sali){

	//�ő�Ƃ����̍��W�����߂�
	double minVal = 0.0, maxVal = 0.0;
	cv::Point minLoc = cv::Point(0,0);
	cv::Point maxLoc = cv::Point(0,0);

	cv::minMaxLoc(Sali, &minVal, &maxVal, &minLoc, &maxLoc);
	printf("SaliMax = %f, Loc(x, y) = (%d, %d)\n", maxVal, maxLoc.x, maxLoc.y);

}

//���͂��ꂽ�@64FC1 �摜���@65025 �Ŋ����āA�ڂɌ�����悤�ȁ@8UC1 �摜�ɂ��ďo�͂���֐�

void change2(cv::Mat in , char filename[]){

	//�o�͉摜�p��

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
			pre8.data[(cols*y+x)] =unsigned char(size)  ;	//�e��f��65025�Ŋ�����255�i�K�ɕϊ�	
		
		}
	}
	cv::imwrite(filename, pre8);

//	cout << filename << endl;


}


//���͂��ꂽ�@64FC1 �摜���@65025 �Ŋ����āA�ڂɌ�����悤�ȁi�傫�����摜�́j8UC1 �摜�ɂ��ďo�͂���֐�

void change3(cv::Mat in , char filename[],cv::Mat Imgsize){

	//�o�͉摜�p��

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
			pre8.data[(cols*y+x)] =unsigned char(size)  ;	//�e��f��65025�Ŋ�����255�i�K�ɕϊ�	
		
		}
	}

	cv::Mat pre8Big;
	pre8Big = Imgsize;
	cv::resize(pre8, pre8Big, Imgsize.size(),0,0);
	cv::imwrite(filename, pre8Big);

//	cout << filename << endl;


}

//����64�r�b�g�̂��̂��K�i������֐�
cv::Mat normali64(cv::Mat img){

	int rows = img.rows;
	int cols = img.cols;

	//�傫�������낦��
	img = hirei64(img);

	//�ő�l�����߂�
	//�ő�E�ŏ��l�����߂�
	double minVal = 0.0, maxVal = 0.0;
	cv::Point minLoc = cv::Point(0,0);
	cv::Point maxLoc = cv::Point(0,0);

	cv::minMaxLoc(img, &minVal, &maxVal, &minLoc, &maxLoc);

	//�ɑ�l�̕��ς����߂�B
	//�㉺���E��4�_�Ɣ�r���A�ǂ������傫����΋ɑ�Ƃ݂Ȃ��@��ׂ�͉̂摜�͈͓̔�(������[�������)
	//�ɑ�ł���΂���𑫂��Ă���
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

				//�ɑ�l�ł���A���ő�l�łȂ���Ή��Z���Ă���
				if (now > upleft && now > up && now > upright &&
				now > left && now > right &&
				now > downleft && now > down && now > downright && x != maxLoc.x && y != maxLoc.y){
				wa = wa + now;
				kazu++;
				}

		}
	}


	//�ɑ�l���ς����߂�
	if (kazu == 0){
		ave = 0.0;
	}else{
		ave = wa / kazu ;
	}

	//M-m���
	double scale = 0.0;
	double sa =0.0;
	sa = (maxVal - ave);
	if (sa ==0){
		scale = 1.0;
	}else {
		scale = double( sa * sa );
	}

	//cout <<"max:"<< maxVal << ",wa:" <<wa<<",kazu:" <<kazu << ",ave:" <<  ave << ",scale:" << scale << endl;

	//�S�v�f�� M-n����������	
	cv::Mat n(rows,cols,CV_64FC1);		
	
			for(int y=0 ; y<n.rows  ;y++){			
				for(int x=0 ; x<n.cols ; x++){					
					n.at<double>(y,x) = img.at<double>(y,x) * scale ;			
				}
			}

	return(n);

}



//��f�l���w��񐔂Ŋ���֐� ����(����, ���鐔)
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




//�������画��p�ɍ쐬

//2�摜�̎w��s�N�Z�����r���āA1�Ԗڂ��傫�����1��2�Ԗڂ��傫�����2��Ԃ��@����(1����,2����,x���W,y���W)
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

//Saliency���v�Z�@�i���͉摜,�񐔂�Y�����Łj
void Saliency(const cv::Mat In,int q){

	
	//SaliencyMap�v�Z
	
	//���͉摜�ǂݍ���
	cv::Mat img;
	
	img = In;	

	int rows = img.rows;
	int cols = img.cols;


	//�������ݗp�摜�p�Ӂ@8�r�b�g
	cv::Mat I(rows,cols,CV_8UC1);
	cv::Mat R(rows,cols,CV_8UC1);
	cv::Mat G(rows,cols,CV_8UC1);
	cv::Mat B(rows,cols,CV_8UC1);
	cv::Mat Y(rows,cols,CV_8UC1);
	
	//�J���[�摜�𐔒l�ɏ���	
	I = makeI(img , I);	
	R = makeBGR(img, R, 2, 0 , 1);
	G = makeBGR(img, G, 1, 0 , 2);
	B = makeBGR(img, B, 0, 1 , 2);
	Y = makeY(img, Y);

	//c,s�p�ɗ̈�m��	8�r�b�g	
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

	
	//�����}�b�v�̍쐬 �@��(1)�`(7)

	//I,RG,BY,�Ƃ̃}�b�v�쐬
	char nameI[40];  // mat�^�̖��O�p
	char nameRG[40];
	char nameBY[40];

	char name64I[40];
	char name64RG[40];
	char name64BY[40];

	//I,RG,BY,gab�ɂ܂Ƃ߂�ׂ�(���@5,6,7)�̈�m�� 64�r�b�g
	cv::Mat Ibar(rows/(2*2*2*2),cols/(2*2*2*2),CV_64FC1);
	cv::Mat RGbar(rows/(2*2*2*2),cols/(2*2*2*2),CV_64FC1);
	cv::Mat BYbar(rows/(2*2*2*2),cols/(2*2*2*2),CV_64FC1);

	//������
	for(int y=0 ; y<Ibar.rows  ;y++){
		for(int x=0 ; x<Ibar.cols ; x++){
			Ibar.at<double>(y,x) = 0.0;
			RGbar.at<double>(y,x) = 0.0;
			BYbar.at<double>(y,x) = 0.0;
		}
	}


	CvSize k= cvSize(3,3); //3,3�ߖT�ŃK�E�V�A���t�B���^��������Ƃ����w��

		// c{2,3,4} s=c+�@��{3,4} �����[�v�ŁB
	// ���̏����̒��ł���Ă��邱�Ƃ́A�iI�ŗ��������Ɓj
	// �w��̃X�P�[���܂Ń_�E���T���v�����O���āA�K�E�V�A���t�B���^��������@I(c),I(s)�̍쐬
	// s��c�̃X�P�[���܂ŃA�b�v�T���v�����O���đ傫�������낦�āA�����Ƃ�@�@I(c,s) = |I(c)-I(s)|
	// �K�i������                                                            N(I(c,s))
	// �X�P�[����4�ɒ������A�������Ă����@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ Ibar = Ibar + N(I(c,s))
	
	for(int c=2; c <5 ; c++){
		for(int i=3; i<5 ; i++){
		int s= c + i;

		//I
		
		//�_�E���X�P�[������
		cv::Mat Icsample(rows,cols,CV_8UC1);
		cv::Mat Issample(rows,cols,CV_8UC1);

		Icsample = DownSample8(I,  c);
		Issample = DownSample8(I,  s);

		Ic = Icsample;
		Is = Issample;

		//s��c�̃X�P�[���܂ŃA�b�v
		Is = UpSample8(Is ,  s-c );


		//�������Ƃ鏈��
		sprintf_s(nameI,"I_c%d_s%d",c,s);
		cv::Mat nameI(rows,cols,CV_64FC1);


		nameI = sabunnI(Ic ,Is);

		//�K�i��
		sprintf_s(name64I,"I_c%d_s%d_32",c,s);
		cv::Mat name64I(rows,cols,CV_64FC1);

		name64I = normali64(nameI);

		//�X�P�[����4�ɌŒ�
		name64I = DownSample(name64I, 4-c);

		//�����}�b�v�ɓ���
		Ibar = wa(Ibar , name64I);

		//RG
		//�_�E���X�P�[������
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

		//�������Ƃ鏈��
		sprintf_s(nameRG,"RG_c%d_s%d",c,s);
		cv::Mat nameRG(rows,cols,CV_64FC1);

		cv::Mat Csa1(rows,cols,CV_64FC1);
		cv::Mat Ssa1(rows,cols,CV_64FC1);

		Csa1 = tadanosa(Rc , Gc);
		Ssa1 = tadanosa(Gs , Rs);

		//Ssa1��c�̃X�P�[���܂�Up
		Ssa1 = UpSample(Ssa1, s-c);
		nameRG = sabunnD(Csa1 , Ssa1);

		//�K�i��
		sprintf_s(name64RG,"RG_c%d_s%d_32",c,s);
		cv::Mat name64RG(rows,cols,CV_64FC1);

		name64RG = normali64(nameRG);
		//�X�P�[����4�ɌŒ�
		name64RG = DownSample(name64RG, 4-c);
		//�����}�b�v�ɓ���
		RGbar = wa(RGbar , name64RG);

		//BY
		//�_�E���X�P�[������
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

		//�������Ƃ鏈��
		sprintf_s(nameBY,"BY_c%d_s%d",c,s);
		cv::Mat nameBY(rows,cols,CV_64FC1);

		cv::Mat Csa2(rows,cols,CV_64FC1);
		cv::Mat Ssa2(rows,cols,CV_64FC1);

		Csa2 = tadanosa(Bc , Yc);
		Ssa2 = tadanosa(Ys , Bs);
		//s��c�̃X�P�[���܂ŃA�b�v
		Ssa2 = UpSample(Ssa2,  s-c );
		nameBY = sabunnD(Csa2 , Ssa2);

		//�K�i��		
		sprintf_s(name64BY,"BY_c%d_s%d_32",c,s);
		cv::Mat name64BY(rows,cols,CV_32FC1);

		name64BY = normali64(nameBY);
		//�X�P�[����4�ɌŒ�
		name64BY = DownSample(name64BY,  4-c );
		//�����}�b�v�ɓ���
		BYbar = wa(BYbar , name64BY);


		}
	}

	//��(8)�̉E�ӂɂ���@N(Ibar),N(Cbar)�̍쐬
		
	//�K�i���ς݂̓����}�b�v�𓝍����Ă���
	//I,gab �̓��[�v�̒��œ����ς� �������ꂽ���̂��K�i�����āAbmp�ۑ�

	cv::Mat Ibar2(rows,cols,CV_64FC1);
	Ibar2 = normali64(Ibar);

	//RG,BY��C�ɓ�������
	cv::Mat Cbar(rows,cols,CV_64FC1);
	//�������i�s�v�H�j
	for(int y=0 ; y<Cbar.rows  ;y++){
		for(int x=0 ; x<Cbar.cols ; x++){
			Cbar.at<double>(y,x) = 0.0;
			}
	}

	Cbar = wa(RGbar, BYbar);
	//�������ꂽCbar���K�i�����āAbmp�ۑ�
	cv::Mat Cbar2(rows,cols,CV_64FC1);
	Cbar2 = normali64(Cbar);
	//change2(Cbar2, "Cbar.bmp");

	//��(8) SaliencyMap�̍쐬�E�ۑ�
	
	//SaliencyMap�̏�����
	cv::Mat Sali(rows,cols,CV_64FC1);
	for(int y=0 ; y<Sali.rows  ;y++){
		for(int x=0 ; x<Sali.cols ; x++){
			Sali.at<double>(y,x) = 0.0;
			}
	}
	
	//��(8)
	Sali = sal(Ibar2, Cbar2);



	//�o�͌��ʊm�F
	char nameSali[40];
	sprintf_s(nameSali,"test%03d/Sali_kaisuu%03d.bmp",testNO,q);

	cv::Mat Sali8(Sali.rows,Sali.cols,CV_64FC1);

	//Sali�̐ϕ��l�𐳋K��
	Sali8 = Seikika(Sali);

	//Saliency�̒l���L�^�ێ�
	x1y1[q] = mass9ave2(Sali8, X1, Y1);
	x2y2[q] = mass9ave2(Sali8, X2, Y2);
	x3y3[q] = mass9ave2(Sali8, X3, Y3);
	x4y4[q] = mass9ave2(Sali8, X4, Y4);
//	x1y1[q] = Sali8.at<double>(Y1,X1);
//	x2y2[q] = Sali8.at<double>(Y2,X2);
//	x3y3[q] = Sali8.at<double>(Y3,X3);
//	x4y4[q] = Sali8.at<double>(Y4,X4);

	Big88(Sali8, nameSali);

	//Saliency�̍����Ƃ����������Ċm�F
//	cv::Mat Marked(rows,cols,CV_8UC3);
	//Sali�ōł��l���傫�������_��img�ɏd�˂ē_��ł�
	cout<<q<<":"<<endl;
	sprintf_s(nameMark,"test%03d/Mark_kaisuu%03d.bmp",testNO,q);
//	cv::Mat markpreimg = img; //img�ɁAMark�����e����^���Ă������Ȃ̂Ő؂藣��
	Markver2(Sali, img);
	
//	cout<<q<<":"<<endl;
//	Marklocal(Sali);
}



//�ϊ�����t�B���^ (����,��̎w��,���Wx,y)
cv::Mat trans(const cv::Mat img,int basenum,int k,int i){


	int r2 =0;
	int g2 =0;
	int b2 =0;

	switch(basenum){
		case 1://I���キ
			r2 = -1;
			g2 = -1;
			b2 = -1;
			break;
		case 2://R���キ
			r2 = -1;
			g2 = +1;
			b2 = +1;
			break;
		case 3://G���キ
			r2 = +1;
			g2 = -1;
			b2 = +1;
			break;
		case 4://B���キ
			r2 = +1;
			g2 = +1;
			b2 = -1;
			break;
		case 5://Y���キ
			r2 = -1;
			g2 = -1;
			b2 = 0;
			break;

	}
	int a=0;
	int b=0;
	int c=0;

	//�����Ђ�����Ԃ�
	k = -1*k;

	

	//�܂��͓��͉摜�����̂܂܃R�s�[����
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


//2�摜�̎w��s�N�Z�����r���āA�����������傫���ق���1/2�ȏォ�@����(NIbar,NCbar,x���W,y���W)
int Hikaku_forfirst(const cv::Mat I, const cv::Mat C, int i){
	int flag = 0;
	
	//I > 2C :1 I�̂�
	//2C > I �܂��́@2I > C :2�@����
	//C > 2I :3 C�̂�
	
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

//���͉摜�̕��ρ@����(����8�r�b�g)
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

//���͉摜�̕��ρ@����(����64�r�b�g)
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


//���͂��ꂽ�@64FC1 �摜���@65025 �Ŋ����āA16�{�Ɋg�債�āA�ڂɌ�����悤�ȁ@8UC1 �摜�ɂ��ďo�͂���֐�

void changeBig(cv::Mat in , char filename[]){

	//�o�͉摜�p��

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
			pre8.data[(cols*y+x)] =unsigned char(size)  ;	//�e��f��65025�Ŋ�����255�i�K�ɕϊ�	
		
		}
	}

	cv::Mat Big8(rows*16,cols*16,CV_8UC1);

	cv::resize(pre8,Big8,Big8.size(),0,0);


	cv::imwrite(filename, Big8);


	//cout << filename << endl;


}


//R,G,B,Y����Sali�Ɠ����傫����NR(2,5)�����Ԃ��֐�
cv::Mat MakeNfile(const cv::Mat in,char Colorfile[8], char ColorNfile[9]){

	int rows = in.rows;
	int cols = in.cols;

		//�_�E���X�P�[������
		cv::Mat Inc(rows,cols,CV_8UC1);
		cv::Mat Ins(rows,cols,CV_8UC1);

		Inc = DownSample8(in,  2);
		Ins = DownSample8(in,  5);

		//s��c�̃X�P�[���܂ŃA�b�v
		Ins = UpSample8(Ins ,  5-2 );

		//�������Ƃ鏈��
		//sprintf_s(nameI,"I_c%d_s%d",c,s);
		cv::Mat In25(rows,cols,CV_64FC1);

		In25 = sabunnI(Inc ,Ins);
		
		//bmp�Ō��ʊm�F
		cv::imwrite(Colorfile,In25);
		
		//�K�i��
		//sprintf_s(name64I,"I_c%d_s%d_32",c,s);
		cv::Mat In64(rows,cols,CV_64FC1);

		In64 = normali64(In25);

		//bmp�Ō��ʊm�F
		change2(In64, ColorNfile);


		//�ǂݍ��݃��T�C�Y
		cv::Mat bigNIn(rows,cols,CV_8UC1);
		cv::Mat reIn25(rows/4,cols/4,CV_8UC1);

		reIn25 = cv::imread(ColorNfile,0);
		cv::resize(reIn25,bigNIn,in.size(),0,0);

		return(bigNIn);

}


//����1�̃s�N�Z����RGB�ɂ��� (tasu , hiku1, hiku2, ���W�̔Ԑ�i)
int make1BGR(int tasu,int hiku1,int hiku2,int i){

	//�����@0,b 1,g  2,r �Ȃ̂� 
	//R �Ȃ�@tasu=r ����0,1��
	//G �Ȃ�@tasu=g     0,2
	//B �Ȃ�@tasu=b     1,2

	
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

//1�s�N�Z������Y�ɐ��l��������֐��@����(���͉摜)
int make1Y(int r,int g, int b,int i){

	//�����@0,b 1,g  2,r

	

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

//�w����W�𒆐S�ɂ���9�}�X�ŕ��ς��o��

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


//SaliencyMap�𐳋K������
cv::Mat Seikika(const cv::Mat in){

	//���͉摜����������pre�ɂ��̂܂܂���

	cv::Mat pre32(in.rows,in.cols,CV_64FC1);

	int rows = in.rows;
	int cols = in.cols;

	for(int y= 0; y<in.rows ; y++){
		for(int x =0; x<in.cols ; x++){
			pre32.at<double>(y,x) = in.at<double>(y,x) ;		
		}
	}

	//���萔�����肷��ׁA���a�����߂�B�@�萔�́@50*�}�X�ڂ̐�/���̑��a�@50�͂Ă��Ƃ�
	double sum =0.0;
	for(int y= 0; y<in.rows ; y++){
		for(int x =0; x<in.cols ; x++){
			sum = sum + pre32.at<double>(y,x);
		}
	}
	double k =0.0;
	k = (50*rows*cols)/sum;


	//8�r�b�g��
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

//���K�����ꂽSaliencyMap���o��255�ȏ��255�Ƃ��ďo��
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

//�w����W�𒆐S�ɂ���9�}�X�ŕ��ς��o��

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

//img�摜��mask�摜������H���after��Ԃ�
cv::Mat bigloop(cv::Mat img, cv::Mat mask, int q){




	//before��SaliencyMap����,���W�ۊ�

	// SaliencyMap�v�Z

	int rows = img.rows;
	int cols = img.cols;

	//�������ݗp�摜�p�Ӂ@8�r�b�g
	cv::Mat I(rows,cols,CV_8UC1);
	cv::Mat R(rows,cols,CV_8UC1);
	cv::Mat G(rows,cols,CV_8UC1);
	cv::Mat B(rows,cols,CV_8UC1);
	cv::Mat Y(rows,cols,CV_8UC1);
	
	//�J���[�摜�𐔒l�ɏ���	
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

	//c,s�p�ɗ̈�m��	8�r�b�g	
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


	//c,s�p�ɗ̈�m��	8�r�b�g	
	cv::Mat NRG25;
	cv::Mat NBY25;

	//�����}�b�v�̍쐬 �@��(1)�`(7)

	//I,RG,BY,�Ƃ̃}�b�v�쐬
	char nameI[40];  // mat�^�̖��O�p
	char nameI2[40]; // .bmp �̃t�@�C�����p�@���ꂼ�����K�v
	char nameRG[40];
	char name2RG[40];
	char nameBY[40];
	char name2BY[40];

	char nameI3[40];

	char name64I[40];
	char name64RG[40];
	char name64BY[40];

	//����
	char name3RG[40];
	char name3BY[40];



	//I,RG,BY,gab�ɂ܂Ƃ߂�ׂ�(���@5,6,7)�̈�m�� 64�r�b�g
	cv::Mat Ibar(rows/(2*2*2*2),cols/(2*2*2*2),CV_64FC1);
	cv::Mat RGbar(rows/(2*2*2*2),cols/(2*2*2*2),CV_64FC1);
	cv::Mat BYbar(rows/(2*2*2*2),cols/(2*2*2*2),CV_64FC1);

	//������
	for(int y=0 ; y<Ibar.rows  ;y++){
		for(int x=0 ; x<Ibar.cols ; x++){
			Ibar.at<double>(y,x) = 0.0;
			RGbar.at<double>(y,x) = 0.0;
			BYbar.at<double>(y,x) = 0.0;
		}
	}

	CvSize k= cvSize(3,3); //3,3�ߖT�ŃK�E�V�A���t�B���^��������Ƃ����w��

	//c{2,3,4} s=c+�@��{3,4} �����[�v�ŁB
	
	//���̏����̒��ł���Ă��邱�Ƃ́A�iI�ŗ��������Ɓj
	// �w��̃X�P�[���܂Ń_�E���T���v�����O���āA�K�E�V�A���t�B���^��������@I(c),I(s)�̍쐬
	// s��c�̃X�P�[���܂ŃA�b�v�T���v�����O���đ傫�������낦�āA�����Ƃ�@�@I(c,s) = |I(c)-I(s)|
	// �K�i������                                                            N(I(c,s))
	// �X�P�[����4�ɒ������A�������Ă����@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ Ibar = Ibar + N(I(c,s))
	
	for(int c=2; c <5 ; c++){
		for(int i=3; i<5 ; i++){
		int s= c + i;

		//I
		
		//�_�E���X�P�[������
		cv::Mat Icsample(rows,cols,CV_8UC1);
		cv::Mat Issample(rows,cols,CV_8UC1);

		Icsample = DownSample8(I,  c);
		Issample = DownSample8(I,  s);

		Ic = Icsample;
		Is = Issample;

		//s��c�̃X�P�[���܂ŃA�b�v
		Is = UpSample8(Is ,  s-c );


		//�������Ƃ鏈��
		sprintf_s(nameI,"I_c%d_s%d",c,s);
		cv::Mat nameI(rows,cols,CV_64FC1);

		nameI = sabunnI(Ic ,Is);

		if (full ==1){
		//bmp�Ō��ʊm�F
		sprintf_s(nameI2,"I_c%d_s%d.bmp",c,s);
		change2(nameI,nameI2);
		}
		

		//�K�i��
		sprintf_s(name64I,"I_c%d_s%d_32",c,s);
		cv::Mat name64I(rows,cols,CV_64FC1);

		name64I = normali64(nameI);

		if (full ==1){
		//bmp�Ō��ʊm�F
		sprintf_s(nameI3,"NI_c%d_s%d.bmp",c,s);
//		cout<< "c:" << c << ",s:" << s << ", rows:" << name64I.rows << ", cols:" << name64I.cols << endl;
		change3(name64I , nameI3, img);
		}

		//�X�P�[����4�ɌŒ�

		name64I = DownSample(name64I, 4-c);

		//�����}�b�v�ɓ���

		Ibar = wa(Ibar , name64I);




		//RG
		//�_�E���X�P�[������
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



		//�������Ƃ鏈��
		sprintf_s(nameRG,"RG_c%d_s%d",c,s);
		cv::Mat nameRG(rows,cols,CV_64FC1);

		cv::Mat Csa1(rows,cols,CV_64FC1);
		cv::Mat Ssa1(rows,cols,CV_64FC1);

		Csa1 = tadanosa(Rc , Gc);
		Ssa1 = tadanosa(Gs , Rs);


		//Ssa1��c�̃X�P�[���܂�Up

		Ssa1 = UpSample(Ssa1, s-c);
		nameRG = sabunnD(Csa1 , Ssa1);
		if (full ==1){
		//bmp�Ō��ʊm�F
		sprintf_s(name2RG,"RG_c%d_s%d.bmp",c,s);
		change2(nameRG,name2RG);
		}

		//�K�i��
		sprintf_s(name64RG,"RG_c%d_s%d_32",c,s);
		cv::Mat name64RG(rows,cols,CV_64FC1);

		name64RG = normali64(nameRG);



		if (full ==1){
		//bmp���ʊm�F
		sprintf_s(name3RG,"NRG_c%d_s%d.bmp",c,s);
		change3(name64RG , name3RG,img);
		}


		//�X�P�[����4�ɌŒ�
		name64RG = DownSample(name64RG, 4-c);
		

		
		//�����}�b�v�ɓ���
		RGbar = wa(RGbar , name64RG);
		

		//BY
		//�_�E���X�P�[������
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



		//�������Ƃ鏈��
		sprintf_s(nameBY,"BY_c%d_s%d",c,s);
		cv::Mat nameBY(rows,cols,CV_64FC1);

		cv::Mat Csa2(rows,cols,CV_64FC1);
		cv::Mat Ssa2(rows,cols,CV_64FC1);

		Csa2 = tadanosa(Bc , Yc);
		Ssa2 = tadanosa(Ys , Bs);
		//s��c�̃X�P�[���܂ŃA�b�v
		Ssa2 = UpSample(Ssa2,  s-c );
		nameBY = sabunnD(Csa2 , Ssa2);

		if (full ==1){
		//bmp�Ō��ʊm�F
		sprintf_s(name2BY,"BY_c%d_s%d.bmp",c,s);
		change2(nameBY,name2BY);
		}

		//�K�i��
		
		sprintf_s(name64BY,"BY_c%d_s%d_32",c,s);
		cv::Mat name64BY(rows,cols,CV_32FC1);

		name64BY = normali64(nameBY);

		if (full ==1){
		//bmp���ʊm�F
		sprintf_s(name3BY,"NBY_c%d_s%d.bmp",c,s);
		change3(name64BY , name3BY, img);
		}

		//�X�P�[����4�ɌŒ�
		name64BY = DownSample(name64BY,  4-c );
		//�����}�b�v�ɓ���
		BYbar = wa(BYbar , name64BY);



		}
	}

	//��(8)�̉E�ӂɂ���@N(Ibar),N(Cbar),N(gabbar)�̍쐬
	
	//�K�i���ς݂̓����}�b�v�𓝍����Ă���
	//I,gab �̓��[�v�̒��œ����ς� �������ꂽ���̂��K�i�����āAbmp�ۑ�

	cv::Mat Ibar2(rows,cols,CV_64FC1);
	Ibar2 = normali64(Ibar);
	change2(Ibar2, "Ibar.bmp");

	//RG,BY��C�ɓ�������
	cv::Mat Cbar(rows,cols,CV_64FC1);
	//�������i�s�v�H�j
	for(int y=0 ; y<Cbar.rows  ;y++){
		for(int x=0 ; x<Cbar.cols ; x++){
			Cbar.at<double>(y,x) = 0.0;
			}
	}

	Cbar = wa(RGbar, BYbar);
	//�������ꂽCbar���K�i�����āAbmp�ۑ�
	cv::Mat Cbar2(rows,cols,CV_64FC1);
	Cbar2 = normali64(Cbar);
	change2(Cbar2, "Cbar.bmp");

	//��(8) SaliencyMap�̍쐬�E�ۑ�
	
	//SaliencyMap�̏�����
	cv::Mat Sali(rows,cols,CV_64FC1);
	for(int y=0 ; y<Sali.rows  ;y++){
		for(int x=0 ; x<Sali.cols ; x++){
			Sali.at<double>(y,x) = 0.0;
			}
	}
	
	//��(8)
	Sali = sal(Ibar2, Cbar2);
	//�o�͌��ʊm�F
//	cout<<q<<":"<<endl;
//	Marklocal(Sali);
	//Saliency�̍����Ƃ����������Ċm�F

	cout << q << ":" << endl;
	sprintf_s(nameMark,"test%03d/Mark_kaisuu%03d.bmp",testNO,q);
//	cv::Mat preimg = img;
	Markver2(Sali, img);


	//Sali�̐ϕ��l�𐳋K��
	cv::Mat Sali8(Sali.rows,Sali.cols,CV_64FC1);
	Sali8 = Seikika(Sali);

	//Saliency�̒l���L�^�ێ�
	x1y1[q] = mass9ave2(Sali8, X1, Y1);
	x2y2[q] = mass9ave2(Sali8, X2, Y2);
	x3y3[q] = mass9ave2(Sali8, X3, Y3);
	x4y4[q] = mass9ave2(Sali8, X4, Y4);

	//���K�����ꂽSaliencyMap��傫�����ďo��
	char nameSali[40];
	sprintf_s(nameSali,"test%03d/Sali_kaisuu%03d.bmp",testNO,q);
	Big88(Sali8, nameSali);


	//���x�W���}�b�v����
	//���x�}�b�v����������8�r�b�g�ł����
	cv::Mat Kyoudo8(rows,cols,CV_8UC1);
	cv::Mat bigSali(img.rows,img.cols,CV_8UC1);
	Sali = cv::imread(nameSali,0);//���m�N���ǂݍ���
	cv::resize(Sali,bigSali,img.size(),0,0);


	//臒l�ݒ肷��ꍇ��������
//	Kyoudo8 = Kyoudo_Ikichimap(bigSali, mask);
	//�����܂�

	//�S�̂ł��ꍇ��������
	for(int i=0 ; i<rows*cols ; i++){
		if(mask.data[i]>200){
			Kyoudo8.data[i] = 128+(bigSali.data[i]/2);
		}else if(mask.data[i]<=200){
			Kyoudo8.data[i] = 128-(bigSali.data[i]/2);
		}
	}
	//�����܂�


	 //���x�W���K�E�V�A���łڂ����Ȃ��ꍇ
//	cv::Mat Kyoudo(rows,cols,CV_8UC1);
//	Kyoudo = Kyoudo8;
//	char namekyoudo[40];
//	sprintf_s(namekyoudo,"test%03d/kyoudo_notgau_kaisuu%03d.bmp",testNO,q);
//	cv::imwrite(namekyoudo,Kyoudo);
	//�����܂�

	//�K�E�V�A���łڂ���
	cv::Mat Kyoudo(rows,cols,CV_8UC1);
	CvSize ksize= cvSize(5,5);
	cv::GaussianBlur(Kyoudo8,Kyoudo,ksize,0,0,0);//�����̕��U�l�̓e�L�g�[

	char namekyoudo[40];
	sprintf_s(namekyoudo,"test%03d/kyoudo_kaisuu%03d.bmp",testNO,q);
	cv::imwrite(namekyoudo,Kyoudo);
	//���x�W���}�b�v�쐬�����܂�



	//�F�̏d�ݕt���W������
	
	//NR,NG,NB,NY�����ASali�Ɠ����傫���܂Ŋg�傷��
	cv::Mat bigNR(I.rows,I.cols,CV_8UC1);
	cv::Mat bigNG(I.rows,I.cols,CV_8UC1);
	cv::Mat bigNB(I.rows,I.cols,CV_8UC1);
	cv::Mat bigNY(I.rows,I.cols,CV_8UC1);
	
	bigNR = MakeNfile(R,"R25.bmp","NR25.bmp");
	bigNG = MakeNfile(G,"G25.bmp","NG25.bmp");
	bigNB = MakeNfile(B,"B25.bmp","NB25.bmp");
	bigNY = MakeNfile(Y,"Y25.bmp","NY25.bmp");
	
	//Sali,Ibar2,Cbar2,NRG25,NBY25��img�Ɠ����傫���܂Ŋg�傷��B
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


	//�e���X�P�[���̒��o
	cv::Mat ICscale;
	ICscale = ScaleLevel(testNO, q);


	cv::Mat after;//���H��̉摜�ɂȂ�\��
	after = img; //������

	//��r�Ɏg���̂ŁA�e�}�b�v�̕��ϒl�����炩���ߌv�Z���Ă���
	double Iave =0.0;
	double Rave =0.0;
	double Bave =0.0;
	double Gave =0.0;
	double Yave =0.0;

/*	�P���ɉ摜�S�̕��ς̏ꍇ
	Iave = Ave8(I);
	Rave = Ave8(R);
	Gave = Ave8(G);
	Bave = Ave8(B);
	Yave = Ave8(Y);
	//�����܂�
*/

//��r�Ɏg���̂ŁA�e�}�b�v��16�u���b�N�̕��ϒl�����炩���ߌv�Z���Ă���
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
	//�����܂�
*/

	//��r�Ɏg���̂ŁAscale4�Ō����e�u���b�N�̕��ϒl�����炩���ߌv�Z���Ă���
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
	//�����܂�
*/

	//��r�Ɏg���̂� 5,6,7,8 �X�P�[���Ō����e�u���b�N�̕��ϒl�����炩���ߌv�Z���Ă���
	//�u���b�N�̓O���[�o���Ő錾�ς�
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
	//�����܂�


//	cout<<"Ave:"<<Iave<<","<<Rave<<","<<Gave<<","<<Bave<<","<<Yave<<","<<endl;

	cols = bigSali.cols;
	rows = bigSali.rows;
	int step = bigSali.step;
	int stepI = I.step;

	//�F�̏d�݂Â��W���v�Z
	cv::Mat QI(rows,cols,CV_8UC1);
	cv::Mat QR(rows,cols,CV_8UC1);
	cv::Mat QG(rows,cols,CV_8UC1);
	cv::Mat QB(rows,cols,CV_8UC1);
	cv::Mat QY(rows,cols,CV_8UC1);

	//�F�̏d�݂Â��W���v�Z�̕���(plus,minus)
	cv::Mat QIpm(rows,cols,CV_8UC1);
	cv::Mat QRpm(rows,cols,CV_8UC1);
	cv::Mat QGpm(rows,cols,CV_8UC1);
	cv::Mat QBpm(rows,cols,CV_8UC1);
	cv::Mat QYpm(rows,cols,CV_8UC1);

	//�e�X�g�p
	cv::Mat predata(rows,cols,CV_8UC1);


//�m�F�p
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


			//�ω��ʂ̐�Βl���}�b�v�ɕۊ�
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

			//�ω��ʂ̕������}�b�v�ɕۊ�
			//���̓_�̐F���Ƃ̒l�ƕ��ς��r
			//�Ƃ肠�����A�����Ėڗ��Ȃ� 2 �Ⴍ�Ėڗ��Ȃ� 0�@��^����B���-1���Ă�����

		

			//���p����w�i�u���b�N�𔻒� �o���`16�u���b�N�̂Ƃ�
/*			Iave = bilinear_ave(blockI,i,rows,cols);
			Rave = bilinear_ave(blockR,i,rows,cols);
			Gave = bilinear_ave(blockG,i,rows,cols);
			Bave = bilinear_ave(blockB,i,rows,cols);
			Yave = bilinear_ave(blockY,i,rows,cols);		
			//�����܂�
*/
			//���p����w�i�u���b�N�𔻒� scale4 �̂Ƃ�
/*			Iave = bilinear4_ave(blockI,i,rows,cols);
			Rave = bilinear4_ave(blockR,i,rows,cols);
			Gave = bilinear4_ave(blockG,i,rows,cols);
			Bave = bilinear4_ave(blockB,i,rows,cols);
			Yave = bilinear4_ave(blockY,i,rows,cols);
			//�����܂�
*/

			//���p����w�i�u���b�N�𔻒� �X�P�[���u���b�N�̂Ƃ�
			int Iscalenum = ICscale.data[3*i];
			int Cscalenum = ICscale.data[3*i+1];

			Iave = bilinear_scale_ave(i,rows,cols, Iscalenum ,1);
			Rave = bilinear_scale_ave(i,rows,cols, Cscalenum ,2);
			Gave = bilinear_scale_ave(i,rows,cols, Cscalenum ,3);
			Bave = bilinear_scale_ave(i,rows,cols, Cscalenum ,4);
			Yave = bilinear_scale_ave(i,rows,cols, Cscalenum ,5);
			//�����܂�

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


	//���H


	//�S��f�ɂ��ă��[�v
			
	//�V��@�̂��ߕω��ʂ��ꎞ�ۑ�
	cv::Mat beforepre(rows,cols,CV_8UC3);
	rgbbefore = beforepre;
	
		for(int i=0 ; i<(rows*cols)  ;i++){
	
			//�ω��ʂ̐�Βl
			int qI3 = QI.data[i];
			int qR3 = QR.data[i];
			int qG3 = QG.data[i];
			int qB3 = QB.data[i];
			int qY3 = QY.data[i];

			//�ω��ʂ̕���
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

			//�摜��f�l����

			double chyousei = 0.0002; //0.0002 �������̂𒲐ߒ�

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

		//�S��f���[�v�����܂�


		//�ω��ʂɃK�E�V�A�������鑀��

		cv::Mat mono(rows,cols,CV_8UC1);
		for(int t=0; t<rows*cols; t++){
			mono.data[t] = ( img.data[3*t] + img.data[3*t+1] + img.data[3*t+2] )/3;
		}


		//2,3,4�̃X�P�[���̔����摜���쐬(���ۂɗ��p����̂�2-4�݂̂�1�͉�)
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

		//�����摜����̃G�b�W���o
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
		//�e���ŏ��X�P�[���̒��o
		cv::Mat minIC;
		minIC = minScaleLevel(testNO, q);
		

		//�ω��ʂɃK�E�V�A���@�J�[�l���̑傫����3�i�K�p�ӂ���
		//�Ȃ��A�J�[�l���̌�̈�����0,0�ɂ��Ă����ƁA�����ŕ��U����`�����D
		//���U�̒l��x,y���ꂼ��Ɋւ��ā@0.3+(ksize/2 -1)+0.8 �Ōv�Z����Ă���悤�ł���D
		//cv::GaussianBlur(rgbbefore, rgbgauss5, cv::Size(1,1) ,0,0);
		cv::GaussianBlur(rgbbefore, rgbgauss6, cv::Size(3,3) ,0,0);
		cv::GaussianBlur(rgbbefore, rgbgauss7, cv::Size(5,5) ,0,0);
		cv::GaussianBlur(rgbbefore, rgbgauss8, cv::Size(7,7) ,0,0);


		//��̍ŏ��� MiniMiniGau �Ŏg��
		cv::Mat numK(rows,cols,CV_8UC1);

		for(int i=0 ; i<(rows*cols)  ;i++){
			int prey = i/cols;
			int prex = i%cols;


//			int Inum = depth.data[i];

	
			//�e����^���Ă���X�P�[���ɂ������āA���p����depth������
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

			//depth�ɂ̓G�b�W����̋����ɉ�����0,70,140,210,240 ���i�[

			if(Inum == 0) rgbgauss = rgbbefore;//�G�b�W��@�K�E�V�A�������Ȃ�
			if (Inum == 70) rgbgauss = rgbbefore;//�G�b�W����1-3�}�X�ȓ��@�K�E�V�A�������Ȃ�
			if (Inum == 140) rgbgauss = rgbgauss6;//�G�b�W����4,5�}�X�@3*3�K�E�V�A��
			if (Inum == 210) rgbgauss = rgbgauss7;//�G�b�W����6,7�}�X�@5*5�K�E�V�A��
			if (Inum == 240) rgbgauss = rgbgauss8;//�G�b�W����8�}�X�ȏ� 7*7�K�E�V�A��

			int b = img.data[(i*3 +0)] + rgbgauss.data[(i*3 +0)]-150.0;
			int g = img.data[(i*3 +1)] +rgbgauss.data[(i*3 +1)]-150.0;
			int r = img.data[(i*3 +2)] +rgbgauss.data[(i*3 +2)]-150.0;

			adjust(b, g, r);

			after.data[(i*3 +0)] = b;
			after.data[(i*3 +1)] = g;
			after.data[(i*3 +2)] = r;

			numK.data[i] =Inum;//���MiniGau�ŗ��p����
		}
		

		//���H��̉摜���ŏ����ŋK�i��
		//cv::Mat Mini = MiniMiniK(after,q+1);
		cv::Mat Mini = MiniMiniGau(after,q+1,numK);
		//���H��̉摜�o��
		char nameafter[40];
		sprintf_s(nameafter,"test%03d/after_kaisuu%03d.bmp",testNO,q+1);
		cv::imwrite(nameafter,Mini);

	return (Mini);
		
/*
		//���H��̉摜�o��
		char nameafter[40];
		sprintf_s(nameafter,"test%03d/after_kaisuu%03d.bmp",testNO,q+1);
		cv::imwrite(nameafter,after);

	return (after);
*/
}

//1���������A�W���X�g����
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

//���摜�Ɠ����傫���̓����}�b�v����A�e���X�P�[�����v�Z���āAI,C�̏����ꖇ�ɂ܂Ƃ߂ĕԂ��֐�
//block�@�Ƃ��ݍ��킹��ɂ�����d�l�ύX level�̒l��5,6,6,7,7,8 ��
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

	//��r����
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

		//IandCdif �̈ꖇ�ɏ����W��
		IandCdif.data[3*t] = Ilevel;
		IandCdif.data[3*t+1] = Clevel;
		IandCdif.data[3*t+2] = 0; //�ꉞ�g���ĂȂ��Ƃ��̓[���Ŗ��߂Ƃ�

	}

	char nameCscale[40];
	sprintf_s(nameCscale,"test%03d/Cscale_kaisuu%03d.bmp",testNO,preNO);
	char nameIscale[40];
	sprintf_s(nameIscale,"test%03d/Iscale_kaisuu%03d.bmp",testNO,preNO);

	cv::imwrite(nameIscale,Idif);
	cv::imwrite(nameCscale,Cdif);

	return(IandCdif);
}

//�w�i�̕��ϒl��16�u���b�N�ōs���ꍇ
cv::Mat block16(cv::Mat grayImg){

	//�w�i�̕��ϒl�i�[��
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
//�m�F�p
/*
cout<<"�[�[�[�m�F�p�o�́[�[�["<<endl;
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

	//�w�i�̕��ϒl�i�[��
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
//�m�F�p
/*
cout<<"�[�[�[�m�F�p�o�́[�[�["<<endl;
	for(int y1 = 0; y1<tate ;y1++){
		for(int x1 = 0; x1<yoko ;x1++){
		double pre = Ave.at<double>(y1,x1);
		cout<< pre <<endl;
		}
	}
*/
	return(Ave);		
};


//16�̎��ӂ̒l��o���`��Ԃ��ĂȂ炷
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

	//�e�l��
	double lt = block.at<double>(top,left);
	double rt = block.at<double>(top,right);
	double lb = block.at<double>(bottom,left);
	double rb = block.at<double>(bottom,right);

	//�o���`�̂Ƃ��̔�

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


//�X�P�[��4�̃u���b�N�̎��ӂ̒l��o���`��Ԃ��ĂȂ炷
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

	//�e�l��

	double lt = block.at<double>(top,left);
	double rt = block.at<double>(top,right);
	double lb = block.at<double>(bottom,left);
	double rb = block.at<double>(bottom,right);

	//�o���`�̂Ƃ��̔�

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





//臒l�ݒ�t���̋��x�W���}�b�v
cv::Mat Kyoudo_Ikichimap(cv::Mat bigSali ,cv::Mat mask){
	//�}�X�N������Sali���ς�臒l�ɐݒ�

	int level = 178; //�̈�����̉��H���x���@�������ׂ� (128~255)

	int rows = bigSali.rows;
	int cols = bigSali.cols;

	
	int sum=0;
	int count=0;

	cv::Mat Kyoudo8(rows, cols, CV_8UC1);

	//�S�̂�128���S�ɔ����ɂ��āA��グ
	for(int i=0 ; i<rows*cols ; i++){
			Kyoudo8.data[i] = 128+(bigSali.data[i]/2);
			if(mask.data[i]>200){
				//�̈�����ł����
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



//�X�P�[���ɉ������}�X�̑傫���Ńu���b�N�̕��ϋ��߂�
cv::Mat block_scale(cv::Mat grayImg, int scale){

	int masu =0;
	masu = int( pow(2.0, double(scale)) );

	int yoko = grayImg.cols/masu;
	int tate = grayImg.rows/masu;

	//�w�i�̕��ϒl�i�[��
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
//�m�F�p
/*
cout<<"�[�[�[�m�F�p�o�́[�[�["<<endl;
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

	//�e����^���Ă���X�P�[���ɍ��킹�āC�����`
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


//�X�P�[��7�̃u���b�N�̎��ӂ̒l��o���`��Ԃ��ĂȂ炷
double bilinear_scale_ave(int i, int rows, int cols, int scale, int color_flag){

	//�X�P�[���ƐF�t���O�ɉ����� I,R,G,B,Y �� �X�P�[�����߂�
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


	//�e�l��
	double lt = block.at<double>(top,left);
	double rt = block.at<double>(top,right);
	double lb = block.at<double>(bottom,left);
	double rb = block.at<double>(bottom,right);


	//�o���`�̂Ƃ��̔�

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
	cv::Mat dis(edge.rows, edge.cols,CV_8UC1,cv::Scalar(240)); //�������ŋ�����S��255��


	//�T���͈͂�臒l�ݒ�
	int  karnel = 7;//�T���͈̘͂r�̒�����臒l ����8�������Ƃ����7�ɕύX
	int  gain = 70; //�u���b�N���Ƃɑ��₷��

	for(int i=0; i<rows*cols ; i++){

		if( edge.data[i]>100){//edge�̂�����W�Ɋւ���
			//���ڔ�����Ƃ���� dis=0�Ƃ���
			dis.data[i]=0;

			int pre=255;//pre�̒l���Ⴏ��΂��������
			int x = i%cols;
			int y = i/cols;

			for(int k=1; k <=karnel ; k++){
				
				if(k<4){
					pre= gain;//k=1,2,3�̂Ƃ� dis=70
				}else if(k>=4 && k<6){
					pre = 2*gain;//k=4,5�̂Ƃ� dis=140
				}else if(k>=6){//k=6,7,8�̂Ƃ��@dis=210
					pre = 3*gain;
				}


				//x,y �𒆐S�Ƃ������a k �̃N���X�̐���
				for(int y2=y-k; y2 <= y+k ; y2++){
					for(int x2=x-k; x2 <= x+k ; x2++){

						//�΂߂̐����
						if( ( abs(y2-y) + abs(x2-x)) == k){
						//�摜�͈͓̔��ł����
						if(y2>=0 && y2<rows && x2>=0 && x2 <cols){

							if(dis.data[y2*cols+x2] > pre){
								dis.data[y2*cols+x2] = pre;
							}

						}//if����̏I���
						}

					}
				}//x2,y2���[�v�̏I���

			}//k��1���甼�a��臒l�܂ŉ񂷏I���
		}//edge ���G�b�W�ɍڂ��Ă邩�̔���
	}//�S��f�̃��[�v�I���


	return(dis);
}


//�e����^���Ă���X�P�[����IC�̂��������������o��
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

	//��r����
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
		
		//IandCdif �̈ꖇ�ɏ����W��
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


		//(a^2 + b^2) ������������Ƃ��������������B�����ł́@L �͉��̒l�Ƃ���
	int L = N/30;

  Ba = ( (N-L) * Bsum_xy - Bsum_x * Bsum_y) / ( (N-L) * (Bsum_x2-L) - pow(Bsum_x, 2));
  Bb = ((Bsum_x2-L) * Bsum_y - Bsum_xy * Bsum_x) / ((N-L) * (Bsum_x2-L) - pow(Bsum_x, 2));

  Ga = ( (N-L) * Gsum_xy - Gsum_x * Gsum_y) / ( (N-L) * (Gsum_x2-L) - pow(Gsum_x, 2));
  Gb = ((Gsum_x2-L) * Gsum_y - Gsum_xy * Gsum_x) / ((N-L) * (Gsum_x2-L) - pow(Gsum_x, 2));

  Ra = ( (N-L) * Rsum_xy - Rsum_x * Rsum_y) / ( (N-L) * (Rsum_x2-L) - pow(Rsum_x, 2));
  Rb = ((Rsum_x2-L) * Rsum_y - Rsum_xy * Rsum_x) / ((N-L) * (Rsum_x2-L) - pow(Rsum_x, 2));
//L ���p�����܂�

  //L���p���Ȃ��ꍇ
/*  Ba = (N * Bsum_xy - Bsum_x * Bsum_y) / (N * Bsum_x2 - pow(Bsum_x, 2));
  Bb = (Bsum_x2 * Bsum_y - Bsum_xy * Bsum_x) / (N * Bsum_x2 - pow(Bsum_x, 2));

  Ga = (N * Gsum_xy - Gsum_x * Gsum_y) / (N * Gsum_x2 - pow(Gsum_x, 2));
  Gb = (Gsum_x2 * Gsum_y - Gsum_xy * Gsum_x) / (N * Gsum_x2 - pow(Gsum_x, 2));

  Ra = (N * Rsum_xy - Rsum_x * Rsum_y) / (N * Rsum_x2 - pow(Rsum_x, 2));
  Rb = (Rsum_x2 * Rsum_y - Rsum_xy * Rsum_x) / (N * Rsum_x2 - pow(Rsum_x, 2));
*/
  cout<<"--�ŏ����--"<<count<<endl;
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

//HSV�ɂ�����SV�̍ŏ����Ő��K������
cv::Mat MiniMiniHSV(const cv::Mat edit,int count){

	cv::Mat img;
	img = cv::imread(Infile);
	
	int rows=img.rows;
	int cols=img.cols;
	int i;

	int N = rows*cols;

	//HSV�ɕ������邽�߂̉摜�̈�m��
	cv::Mat imgHSV(rows,cols,CV_8UC3);
	cv::Mat editHSV(rows,cols,CV_8UC3);
	//cv::Mat Vimage2(rows, cols,CV_8UC1);
	//cv::Mat Simage2(rows, cols,CV_8UC1);
	//cv::Mat Himage2(rows, cols,CV_8UC1);

	//�ϊ�
	cv::cvtColor(img, imgHSV, CV_BGR2HSV);
	cv::cvtColor(edit, editHSV, CV_BGR2HSV);


	//G�Ə����Ă��邪�AS
		double Ga = 0.0;
	double Gb = 0.0;
	double Gsum_xy = 0.0, Gsum_x = 0.0, Gsum_y = 0.0, Gsum_x2 = 0.0;

	//R�Ə����Ă��邪�AV
	double Ra = 0.0;
	double Rb = 0.0;
	double Rsum_xy = 0.0, Rsum_x = 0.0, Rsum_y = 0.0, Rsum_x2 = 0.0;

//�v���b�g�p
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

	//	fprintf(fpS,"%d %d\n", imgHSV.data[i*3+1] , editHSV.data[i*3+1]  );//�v���b�g�p
	//	fprintf(fpV,"%d %d\n", imgHSV.data[i*3+2] , editHSV.data[i*3+2]  );//�v���b�g�p
	}

//	fclose(fpS);
//	fclose(fpV);
//	system("wgnuplot -persist SVdot.plt");//�O���t�̏����ݒ�� .plt �ɏ�������ł���


  Ga = (N * Gsum_xy - Gsum_x * Gsum_y) / (N * Gsum_x2 - pow(Gsum_x, 2));
  Gb = (Gsum_x2 * Gsum_y - Gsum_xy * Gsum_x) / (N * Gsum_x2 - pow(Gsum_x, 2));

  Ra = (N * Rsum_xy - Rsum_x * Rsum_y) / (N * Rsum_x2 - pow(Rsum_x, 2));
  Rb = (Rsum_x2 * Rsum_y - Rsum_xy * Rsum_x) / (N * Rsum_x2 - pow(Rsum_x, 2));

  cout<<"--HSV�ŏ����--"<<count<<endl;
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

//16�u���b�N�ɂ킯��
cv::Mat Mini16block(const cv::Mat edit,int count){
	cv::Mat img;
	img = cv::imread(Infile);
	
	int rows=img.rows;
	int cols=img.cols;
	int i;

	int N = rows*cols;
		

	cv::Mat after(rows, cols, CV_8UC3);


	//���摜�� 16����
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

			cout<<"(xb,yb)=("<<xb<<","<<yb<<")"<<endl;//�m�F�p

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




	//gnuplot��eps�̃O���t�쐬���邽�߁Adat�t�@�C�����I�[�v��
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
//	system("wgnuplot -persist RGBdot.plt");//�O���t�̏����ݒ�� testSali.plt �ɏ�������ł���

 
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

//R,G,B�o���o���ł͂Ȃ� (R+G+B)/3 �𗘗p���Ĉ�̒l�ōŏ����
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

//�J�[�l���������Ă���ŏ����
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


	//�J�[�l�����ꎞ�ۊǂ���
	cv::Mat img2(rows, cols, CV_8UC3); 
	cv::Mat edit2(rows, cols, CV_8UC3);
		for (int y=0; y<rows; y++){
		for (int x=0; x<cols; x++){

			//�J�[�l��16�̂Ƃ�
			double center = 4.0;
			double next = 8.0;
			double cross = 16.0;

			//�J�[�l��9�̂Ƃ�
			//double center = 9.0;
			//double next = 9.0;
			//double cross = 9.0;


			//���S
			double preiB  = img.data[(y*cols+x)*3] /center;
			double preiG  = img.data[(y*cols+x)*3+1] /center;
			double preiR  = img.data[(y*cols+x)*3+2] /center;

			double preEB  = edit.data[(y*cols+x)*3] /center;
			double preEG  = edit.data[(y*cols+x)*3+1] /center;
			double preER  = edit.data[(y*cols+x)*3+2] /center;

			if( x-1>=0 && y-1>=0 ){//����
				preiB = preiB + img.data[((y-1)*cols+x-1)*3] /cross;
				preiG = preiG + img.data[((y-1)*cols+x-1)*3+1] /cross;
				preiR = preiR + img.data[((y-1)*cols+x-1)*3+2] /cross;

				preEB = preEB + edit.data[((y-1)*cols+x-1)*3] /cross;
				preEG = preEG + edit.data[((y-1)*cols+x-1)*3+1] /cross;
				preER = preER + edit.data[((y-1)*cols+x-1)*3+2] /cross;
			}

			if( y-1>=0 ){//��
				preiB = preiB + img.data[((y-1)*cols+x)*3] /next;
				preiG = preiG + img.data[((y-1)*cols+x)*3+1] /next;
				preiR = preiR + img.data[((y-1)*cols+x)*3+2] /next;

				preEB = preEB + edit.data[((y-1)*cols+x)*3] /next;
				preEG = preEG + edit.data[((y-1)*cols+x)*3+1] /next;
				preER = preER + edit.data[((y-1)*cols+x)*3+2] /next;
			}

			if( x+1<=cols && y-1>=0 ){//�E��
				preiB = preiB + img.data[((y-1)*cols+x+1)*3] /cross;
				preiG = preiG + img.data[((y-1)*cols+x+1)*3+1] /cross;
				preiR = preiR + img.data[((y-1)*cols+x+1)*3+2] /cross;

				preEB = preEB + edit.data[((y-1)*cols+x+1)*3] /cross;
				preEG = preEG + edit.data[((y-1)*cols+x+1)*3+1] /cross;
				preER = preER + edit.data[((y-1)*cols+x+1)*3+2] /cross;

			}

			if( x-1>=0 ){//��
				preiB = preiB + img.data[(y*cols+x)*3] /next;
				preiG = preiG + img.data[(y*cols+x)*3+1] /next;
				preiR = preiR + img.data[(y*cols+x)*3+2] /next;

				preEB = preEB + edit.data[(y*cols+x)*3] /next;
				preEG = preEG + edit.data[(y*cols+x)*3+1] /next;
				preER = preER + edit.data[(y*cols+x)*3+2] /next;
			}
			
			if( x+1<=cols ){//�E
				preiB = preiB + img.data[(y*cols+x+1)*3] /next;
				preiG = preiG + img.data[(y*cols+x+1)*3+1] /next;
				preiR = preiR + img.data[(y*cols+x+1)*3+2] /next;

				preEB = preEB + edit.data[(y*cols+x+1)*3] /next;
				preEG = preEG + edit.data[(y*cols+x+1)*3+1] /next;
				preER = preER + edit.data[(y*cols+x+1)*3+2] /next;

			}

			if( x-1>=0 && y+1<=rows ){//����
				preiB = preiB + img.data[((y+1)*cols+x-1)*3] /cross;
				preiG = preiG + img.data[((y+1)*cols+x-1)*3+1] /cross;
				preiR = preiR + img.data[((y+1)*cols+x-1)*3+2] /cross;

				preEB = preEB + edit.data[((y+1)*cols+x-1)*3] /cross;
				preEG = preEG + edit.data[((y+1)*cols+x-1)*3+1] /cross;
				preER = preER + edit.data[((y+1)*cols+x-1)*3+2] /cross;
			}

			if( y+1<=rows ){//��
				preiB = preiB + img.data[((y+1)*cols+x)*3] /next;
				preiG = preiG + img.data[((y+1)*cols+x)*3+1] /next;
				preiR = preiR + img.data[((y+1)*cols+x)*3+2] /next;

				preEB = preEB + edit.data[((y+1)*cols+x)*3] /next;
				preEG = preEG + edit.data[((y+1)*cols+x)*3+1] /next;
				preER = preER + edit.data[((y+1)*cols+x)*3+2] /next;
			}

			if( x+1<=cols && y+1<=rows ){//�E��
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

  cout<<"--�ŏ����--"<<count<<endl;
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

//�K�E�V�A���������Ă���ŏ����(���H��摜�A�X�V�񐔁A�K�E�V�A��������J�[�l���̑傫���i�[�摜)
cv::Mat MiniMiniGau(const cv::Mat edit,int count,cv::Mat numK){
	cv::Mat img;
	img = cv::imread(Infile);
	
	int rows=img.rows;
	int cols=img.cols;
	int i;

	int N = rows*cols;

	//���H��摜���K�E�V�A��������
	cv::Mat edit3(rows,cols,CV_8UC3);
	cv::Mat edit5(rows,cols,CV_8UC3);
	cv::Mat edit7(rows,cols,CV_8UC3);

	cv::Mat img3(rows,cols,CV_8UC3);
	cv::Mat img5(rows,cols,CV_8UC3);
	cv::Mat img7(rows,cols,CV_8UC3);
/*
	//�J�[�l���̑傫���͎O�i�K
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

	//���W���ƂɓK�������������������edit
	cv::Mat editG(rows,cols,CV_8UC3);
	cv::Mat imgG(rows,cols,CV_8UC3);
/*
	for(int i=0;i<rows*cols;i++){
		int preK = numK.data[i];

		if(preK<=70){//�G�b�W����0-3�}�X�͕������Ȃ�
			editG.data[i*3]=edit.data[i*3];
			editG.data[i*3+1]=edit.data[i*3+1];
			editG.data[i*3+2]=edit.data[i*3+2];
			imgG.data[i*3]=img.data[i*3];
			imgG.data[i*3+1]=img.data[i*3+1];
			imgG.data[i*3+2]=img.data[i*3+2];
		}else if(preK==140){//�G�b�W����4,5�}�X��3*3�ŕ�����
			editG.data[i*3]=edit3.data[i*3];
			editG.data[i*3+1]=edit3.data[i*3+1];
			editG.data[i*3+2]=edit3.data[i*3+2];
			imgG.data[i*3]=img3.data[i*3];
			imgG.data[i*3+1]=img3.data[i*3+1];
			imgG.data[i*3+2]=img3.data[i*3+2];
		}else if(preK==210){//�G�b�W����6,7�}�X��5*5�ŕ�����
			editG.data[i*3]=edit5.data[i*3];
			editG.data[i*3+1]=edit5.data[i*3+1];
			editG.data[i*3+2]=edit5.data[i*3+2];
				imgG.data[i*3]=img5.data[i*3];
			imgG.data[i*3+1]=img5.data[i*3+1];
			imgG.data[i*3+2]=img5.data[i*3+2];
		}else if(preK==240){//�G�b�W����8�}�X�ȏ��7*7�ŕ�����
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

		if(preK<=70){//�G�b�W����0-3�}�X��3*3�ŕ�����
			editG.data[i*3]=edit3.data[i*3];
			editG.data[i*3+1]=edit3.data[i*3+1];
			editG.data[i*3+2]=edit3.data[i*3+2];
			imgG.data[i*3]=img3.data[i*3];
			imgG.data[i*3+1]=img3.data[i*3+1];
			imgG.data[i*3+2]=img3.data[i*3+2];
		}else if(preK==140){//�G�b�W����4,5�}�X��5*5�ŕ�����
			editG.data[i*3]=edit5.data[i*3];
			editG.data[i*3+1]=edit5.data[i*3+1];
			editG.data[i*3+2]=edit5.data[i*3+2];
			imgG.data[i*3]=img5.data[i*3];
			imgG.data[i*3+1]=img5.data[i*3+1];
			imgG.data[i*3+2]=img5.data[i*3+2];
		}else if(preK==210){//�G�b�W����6�}�X�ȏ�͂�7*7�ŕ�����
			editG.data[i*3]=edit7.data[i*3];
			editG.data[i*3+1]=edit7.data[i*3+1];
			editG.data[i*3+2]=edit7.data[i*3+2];
			imgG.data[i*3]=img7.data[i*3];
			imgG.data[i*3+1]=img7.data[i*3+1];
			imgG.data[i*3+2]=img7.data[i*3+2];
		}else if(preK==240){//�G�b�W����8�}�X�ȏ��7*7�ŕ�����
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
	editG=edit3;//�S���J�[�l��3�ɂ����Ⴄ
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

  cout<<"--�ŏ����--"<<count<<endl;
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

