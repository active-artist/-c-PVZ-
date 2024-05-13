/*
* ������־
* 1.��������Ŀ������Ŀģ�壩ʹ��vs������汾
* 2.�����ز�
* 3.ʵ���ʼ����Ϸ����
* 4.ʵ����Ϸ�����Ĺ�����
* 5.ʵ�ֹ������е�ֲ�￨��
* 6.ʵ��ֲ�￨�Ƶ��϶�
* 7.ʵ��ֲ�￨�Ƶ���ֲ
* 8.ʵ��ֲ�￨�Ƶ�ҡ��
* 9.�Ż���Ϸѭ������Ⱦ˳��
* 10.���������˵�
* 11.�����������
* 12.��ʾ�������
* 13.�ռ����⣬��ʾ����ֵ
* 14.������ʬ
* 15.ʵ�ֽ�ʬ�����߶���
* 16.ʵ��������ķ�Ծ
* 17.����ֲ���㶹
* 18.��Ⱦֲ���㶹
* 19.ʵ���ӵ��뽩ʬ����ײ
* 20.ʵ�ֽ�ʬ��ֲ�����ײ
* 21.ʵ�����տ���������
* 22.��Ϸ�����ٶȵļ��٣��Լ�������Ż�
* 23.ƬͷѲ��
* 24.���ӿ�ͷ����
* 25.ʵ�ֹ������еĻ�ɫ����
* 26.�жϵ�ǰ�����ܷ���ֲֲ��
* 27.ʵ����ֲֲ����������

*/

#include <stdio.h>
#include <graphics.h>  //easyxͼ�ο��ͷ�ļ�����Ҫ��װeasyxͼ�ο�
#include<time.h>
#include<math.h>
#include "tools.h"
#include"vector2.h"

#include<Windows.h>
#include<mmsystem.h>	//��Ч���ŵ����ݿ�
#pragma comment(lib,"winmm.lib")

#define WIN_WIDTH   900
#define WIN_HEIGHT  600
#define ZM_MAX      10

enum {WAN_DOU,XIANG_RI_KUI, ZHI_WU_COUNT};

  IMAGE imgBg; //��ʾ����ͼƬ
  IMAGE imgBar; //��ʾֲ�﹤����ͼƬ
  IMAGE imgCards[ZHI_WU_COUNT];  //��ʾֲ�￨��ͼƬ
  IMAGE imgFCards[ZHI_WU_COUNT]; //��ʾ����ʹ�õ�ֲ�￨��ͼƬ
  IMAGE imgFBea;	//��ʾ��ɫ�㶹����ͼƬ
  IMAGE imgFSunflower;//��ʾ��ɫ���տ�����ͼƬ
  IMAGE* imgZhiWu[ZHI_WU_COUNT][20];//ͨ����άָ�����������ֲ�����ʱ�̵�ҡ��ͼƬ

  IMAGE imgReady;//��ʾ����ʱͼƬ
  IMAGE imgSet;//��ʾ����ʱ��ͼƬ
  IMAGE imgSetplans;//��ʾ����ʱ��ͼƬ

  int curX, curY;//��ǰѡ�е�ֲ����ƶ��Ĺ����е�λ��
  int curZhiWu;  //��ʾѡ�е�ֲ�� 0��û��ѡ�У�i��ѡ���i��ֲ��

  enum{GOING,WIN,FAIL};
  int killCount;//�Ѿ�ɱ���Ľ�ʬ����
  int zmCount;  //�Ѿ����ֵĽ�ʬ����
  int gameStatus;//��ǰ��Ϸ��״̬

  struct zhiwu {
	  int type;		  //0��û��ֲ�� 1��1��ֲ��
	  int frameIndex; //����֡�����

	  bool catched;	  //�Ƿ񱻽�ʬ��
	  int deadtime;   //����������

	  int timer;      //��ʱ��
	  int x, y;		  //ֲ�������

	  int shootTime;  //ÿ���㶹�ļ�����

  };

  int PlantsCost[ZHI_WU_COUNT];

//     int costPea;		//�㶹���ֵ�cost
//     int costSunflower;//���տ���cost

  bool canUsePea;		//��ǰ�����ܹ���ֲ�㶹
  bool canUseSunflower; //��ǰ�����ܷ���ֲ���տ�

  struct zhiwu map[3][9]; //ֲ��Ĳ����ö�ά�ṹ�������ʾ

  enum{SUNSHINE_DOWN,SUNSHINE_GROUND,SUNSHINE_COLLECT,SUNSHINE_PRODUCT}; //��ʾ�����ռ��Ĳ�ͬ״̬

  //Ӧ�ýṹ���ʾ����
  struct sunshineBall {   
	  int x, y;				     //��������Ʈ������е�����λ�ã�x���䣩
	  int frameIndex;			 //��ǰ��ʾ��ͼƬ֡�����
	  int destY;				 //Ʈ���Ŀ��λ�õ�y����
	  bool used;			     //�����Ƿ���ʹ��
	  int timer;				 //��ʱ��

	  float xoff;				 //ʵ��������ķ�Ծ��x��ƫ����
	  float yoff;				 //ʵ��������ķ�Ծ��y��ƫ����

	  float t;					 //���������ߵ�ʱ���0-1
	  vector2 p1, p2, p3, p4;	 //���������ߵ��ĸ����Ƶ�
	  vector2 pCur;				 //��ǰʱ���������λ��
	  float speed;				 //��ǰ������ٶ�
	  int status;				 //��ǰ�����״̬

  }; 

  //Ӧ�ýṹ���ʾ��ʬ
  struct zm {
	  int x, y;			  //����x��y
	  int frameIndex;	  //ÿһ֡��֡���
	  bool used;		  //���彩ʬ�Ƿ����
	  int speed;		  //�����ٶ�
	  int row;			  //��ʬ������
	  int blood;	      //��ʬ��Ѫ��
	  bool dead;		  //��ʬ�Ƿ�����
	  bool eating;		  //��ʬ�Ƿ����ڳ�ֲ��
	  
  };
  struct zm zms[10];      //��ʬ��
  IMAGE imgZM[22];		  //����ÿһ֡��ͼƬ
  IMAGE imgZMDead[20];    //���ؽ�ʬ������ͼƬ 
  IMAGE imgZMEat[21];     //���ؽ�ʬ��ֲ���ͼƬ
  IMAGE imgZmStand[11];		//��ʬվ��״̬�µ�ͼƬ

  //Ӧ�ýṹ�嶨��ֲ���ӵ�����������
  struct bullet {
	  int x, y;           //�ӵ���λ��
	  int row;			  //�ӵ�������
	  bool used;		  //�ӵ��Ƿ�ʹ��
	  int speed;		  //�ӵ����ٶ�
	  bool blast;		  //�ӵ��Ƿ�����ը
	  int framIndex;	  //��ըͼƬ��֡��� 
  };

  struct bullet bullets[30];//�����ӵ���
  IMAGE imgBulletNormal;    //����״̬���ӵ���ͼƬ
  IMAGE imgBullBlast[4];    //��ը״̬���ӵ���ͼƬ


  //�صĸ����Ԥ��׼��10�����⣬����Ҫ�������⣬���������ȡ��һ����ʹ�õģ������黹����
  struct sunshineBall balls[10]; //���������
  IMAGE imgSunshineBall[29];	 //����ֲ��ÿ֡��ͼƬ
  int sunshine;                  //��������ֵ

  //bool�����ж��ļ��Ƿ����
  bool fileExist(const char* name) {
	  FILE* fp = fopen(name, "r");
	  //����ļ�Ϊ���򷵻ؼ٣��ļ����ڹرպ󷵻���
	  if (fp == NULL) {   
		  return false;
	  }
	  else {
		  fclose(fp);
		  return true;
	  }
  }

//��ʼ���ĺ���
void gameInit() {
	//������Ϸ����ͼƬ
	//���ַ����޸�Ϊ�����ֽ��ַ�����
	loadimage(&imgBg, "res/bg.jpg");			//���ر���ͼƬ
	loadimage(&imgBar, "res/bar.png");			//����ֲ����ͼƬ

	loadimage(&imgReady, "res/res/ready.png");	//����׼��ͼƬ
	loadimage(&imgSet, "res/res/set.png");		//���ط���ͼƬ
	loadimage(&imgSetplans, "res/res/setplants.png");//����ֲ��ͼƬ

	loadimage(&imgFBea, "res/res/Cards_Black/card_1.png");		//���ػ�ɫ�㶹ͼƬ
	loadimage(&imgFSunflower, "res/res/Cards_Black/card_2.png");		//���ػ�ɫ���տ�ͼƬ


	memset(imgZhiWu, 0, sizeof(imgZhiWu));//memset������ֲ��ڶ������ڴ�������Ϊһ��ֵ
	memset(map, 0, sizeof(map));		  //��ʼ��ֲ�ﲼ������

	killCount = 0;   //��ɱ��ʬ��Ϊ0
	zmCount = 0;     //��ʬ��Ϊ0
	gameStatus = GOING;//��Ϸ״̬ ������
	canUsePea = false;       //��ʼ���㶹�ܷ���ֲ
	canUseSunflower = false; //��ʼ�����տ��ܷ���ֲ
	
	//��ʼ��ֲ��cost
	PlantsCost[0] = 100;
	PlantsCost[1] = 50;

	//��ʼ��ֲ�￨��
	char name[64];
	for (int i = 0; i < ZHI_WU_COUNT; i++) {
		//����ֲ�￨�Ƶ��ļ���
		sprintf_s(name, sizeof(name), "res/res/Cards/card_%d.png", i + 1);
			loadimage(&imgCards[i], name);
		
		sprintf_s(name, sizeof(name), "res/res/Cards_Black/card_%d.png", i + 1);
			loadimage(&imgFCards[i], name);

			for (int j = 0; j < 20; j++) {
				sprintf_s(name, sizeof(name), "res/res/zhiwu/%d/%d.png",i ,j + 1);
				//���ж�����ļ��Ƿ���� ���ܲ��ܴ�����ļ�  �ļ����ھͼ��ؽ���
				if (fileExist(name)) {
					imgZhiWu[i][j] = new IMAGE; //c++�ķ�ʽ�����ڴ�
					loadimage(imgZhiWu[i][j], name);
				}
				else {
					break;
				}
			}
	}

	curZhiWu = 0;   //��ʾѡ�е�ֲ��
	sunshine = 50;  //��ʼ������ֵΪ50

	


	memset(balls, 0, sizeof(balls[10]));   //��ʼ�������ȫΪ0
	//for ѭ����������֡��ͼƬ
	for (int i = 0; i < 29; i++) {
		sprintf_s(name, sizeof(name), "res/res/sunshine/%d.png", i + 1);
		loadimage(&imgSunshineBall[i], name);
	}

	//�����������
	srand(time(NULL));

	//������Ϸ��ͼ�δ���
	initgraph(WIN_WIDTH, WIN_HEIGHT);

	//��������
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWeight = 15;
	strcpy(f.lfFaceName, "Segoe UI Black");
	f.lfQuality = ANTIALIASED_QUALITY; //�����Ч��
	settextstyle(&f);				   //��������
	setbkmode(TRANSPARENT);			   //���ñ���ģʽ͸��
	setcolor(BLACK);				   //����������ɫ

	//��ʼ����ʬ����
	memset(zms, 0, sizeof(zms));	   //��ʼ����ʬΪ0
	//forѭ�����ؽ�ʬÿһ֡��ͼƬ
	for (int i = 0; i < 22; i++) {
		sprintf_s(name, sizeof(name), "res/res/zm/%d.png", i + 1);
		loadimage(&imgZM[i], name);
	}

	//��ʼ���ӵ�����
	loadimage(&imgBulletNormal, "res/res/bullets/bullet_normal.png");
	memset(bullets, 0, sizeof(bullets));			//��ʼ���ӵ���

	//��ʼ���㶹�ӵ���֡ͼƬ���� ,ͨ���ӵ���ըͼƬ����Сչʾ����
	loadimage(&imgBullBlast[3], "res/res/bullets/bullet_blast.png");
	for (int i = 0; i < 3; i++) {
		float k = (i + 1) * 0.2;
		loadimage(&imgBullBlast[i], "res/res/bullets/bullet_blast.png",
			imgBullBlast[3].getwidth() * k,
			imgBullBlast[3].getheight() * k, true);
	}

	//��ʼ����ʬ��������	
	for (int i = 0; i < 20; i++) {
		sprintf_s(name, sizeof(name), "res/res/zm_dead/%d.png", i + 1);
		loadimage(&imgZMDead[i], name);
	}

	for (int i = 0; i < 21; i++) {
		sprintf_s(name, "res/res/zm_eat/%d.png", i + 1);
		loadimage(&imgZMEat[i], name);
	}

	//���ؽ�ʬվ��ͼƬ
	for (int i = 0; i < 11; i++) {
		sprintf_s(name, "res/res/zm_stand/%d.png", i + 1);
		loadimage(&imgZmStand[i], name);
	}

}

//��Ⱦ��ʬ�ĺ���
void drawZM() {
	int zmCount = sizeof(zms) / sizeof(zms[0]);       //���彩ʬ�����ֵ
	//ѭ������ÿ֡�Ľ�ʬͼƬ
	for (int i = 0; i < zmCount; i++) {
		if (zms[i].used) {
			//IMAGE* img = (zms[i].dead) ? imgZMDead : imgZM; //����ָ�룬�ö�Ӧ֡��ָ��ָ���Ӧ��ͼƬ
			//img += zms[i].frameIndex;
			IMAGE* img = NULL;
			if (zms[i].dead)img = imgZMDead;
			else if (zms[i].eating)img = imgZMEat;
			else img = imgZM;

			img += zms[i].frameIndex;

			putimagePNG(zms[i].x,
				zms[i].y - img->getheight(),		  //y�����ʾ���ǽ�ʬͼƬ���½ǵ����꣬��Ⱦ�Ǵ�ͼƬ���Ͻǿ�ʼ��Ⱦ�����Լ���һ������
				img);
		}
	}
}

//��Ⱦ�ӵ�
void drawBullets() {
	int bulletMax = sizeof(bullets) / sizeof(bullets[0]);   //���������ӵ���
	//����ÿһ���ӵ���Ⱦ
	for (int i = 0; i < bulletMax; i++) {
		if(bullets[i].used){
			if (bullets[i].blast) {
				IMAGE* img = &imgBullBlast[bullets[i].framIndex];
				putimagePNG(bullets[i].x, bullets[i].y, img);
			}
			else
			{
				putimagePNG(bullets[i].x, bullets[i].y, &imgBulletNormal);
			}
		}
	}
}

//��Ⱦ����ĺ���
void drawSunshines() {
	int ballMax = sizeof(balls) / sizeof(balls[0]);		//��������������
	//����ÿһ�����Ⲣ��Ⱦ
	for (int i = 0; i < ballMax; i++) {
		//if (balls[i].used || balls[i].xoff) {
		if(balls[i].used){
			IMAGE* img = &imgSunshineBall[balls[i].frameIndex];
			//putimagePNG(balls[i].x, balls[i].y, img);
			putimagePNG(balls[i].pCur.x, balls[i].pCur.y, img);
		}
	}
	//��Ⱦ ����ֵ
	char scoreText[8];				//������ֵ����ַ���
	sprintf_s(scoreText, sizeof(scoreText), "%d", sunshine);	//sprintf_s���� ��ָ����ʽ���ַ�����ӡ��������
	outtextxy(278, 67, scoreText); //�������ֵ
}

void drawZhiWu() {
	//˫��ѭ����Ⱦ��ֲ��ֲ��
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type > 0) {
				//	int x = 256 + j * 81;				    //ֲ��������
				//	int y = 179 + i * 102 + 14;			    //ֲ��������

				int zhiWuType = map[i][j].type - 1;        //ֲ�������
				int index = map[i][j].frameIndex;		   //ֲ�ﵱǰ��ͼƬ֡
				//putimagePNG(x, y, imgZhiWu[zhiWuType][index]);
				putimagePNG(map[i][j].x, map[i][j].y, imgZhiWu[zhiWuType][index]);
			}
		}
	}
	// ��Ⱦ �϶������е�ֲ��
	if (curZhiWu > 0) {
		IMAGE* img = imgZhiWu[curZhiWu - 1][0];  //��ֲ����ڹ������м�
		putimagePNG(curX - img->getwidth() / 2, curY - img->getheight() / 2, img);
	}
}

//��Ⱦ ����
void drawCards() {
	int i = 0;					 //i=0���㶹��i=1�����տ�
	int x = 338 + i * 65;    //����λ������
	int y = 6;

	if (canUsePea) {
		putimage(x, y, &imgCards[0]); //��Ⱦ��ʹ�õ��㶹ͼƬ
	}
	if(canUsePea == false){
		putimage(x, y, &imgFCards[0]); //��Ⱦ��ɫ���㶹ͼƬ
	}
	if (canUseSunflower) {
		putimage(x + 65, y, &imgCards[1]); //��Ⱦ��ʹ�õ����տ�ͼƬ
	}
	if(canUseSunflower == false) {
		putimage(x + 65, y, &imgFCards[1]); //��Ⱦ��ɫ�����տ�ͼƬ
	}
}

//��Ⱦ����
void updateWindow() {
	BeginBatchDraw();//˫���塪����ʼ����

	putimage(-112, 0, &imgBg);
	putimagePNG(250, 0, &imgBar);

	//��Ⱦ����
	drawCards();

	//��Ⱦֲ��
	drawZhiWu();

	// ��Ⱦ ����
	drawSunshines();

	//��Ⱦ ��ʬ
	drawZM();

	//��Ⱦ �ӵ�
	drawBullets();

	EndBatchDraw();//����˫����
}

//�ռ���������ĺ���
void collectSunshine(ExMessage* msg) {
	int count = sizeof(balls) / sizeof(balls[0]);//���㵱ǰ�ж��ٸ�������
	int w = imgSunshineBall[0].getwidth();		 //�����һ֡����Ŀ��
	int h = imgSunshineBall[0].getheight();		 //�����һ֡����ĸ߶�
	//forѭ���жϵ��λ���Ƿ���������
	for (int i = 0; i < count; i++) {
		if (balls[i].used) {
			//int x = balls[i].x;
			//int y = balls[i].y;
			int x = balls[i].pCur.x;
			int y = balls[i].pCur.y;

			if (msg->x > x && msg->x < x + w &&
				msg->y > y && msg->y < y + h) {
				balls[i].status = SUNSHINE_COLLECT;
				PlaySound("res/res/sunshine.wav", NULL, SND_FILENAME | SND_ASYNC);   //�����ռ���Ч
				//���㱴�������ߵ�ʼĩλ��
				balls[i].p1 = balls[i].pCur;
				balls[i].p4 = vector2(262, 0);
				balls[i].t = 0;
				float distance = dis(balls[i].p1 - balls[i].p4);   //c++���������
				float off = 8;									   //ÿ���ƶ��ľ���
				balls[i].speed = 1.0 / (distance / off);		   //�����ٶ�
				break;
			}
		}
	}
}

//�û��������
void userClick() {
	ExMessage msg;  //��Ϣ���Ͳ���
	static int status = 0;  //����һ����̬��ѹ
	    //�ж���û���������Ϣ  ����Ϣ������  ����Ϣ���ؼ�)
		if (peekmessage(&msg)){
			//�������Ϣ
		    if (msg.message == WM_LBUTTONDOWN) {
			    if (msg.x > 338 && msg.x < 338 + 65 * ZHI_WU_COUNT && msg.y < 96) {
					int index = (msg.x - 338) / 65;
					status = 1;
					if(canUsePea && index == 0)
						curZhiWu = index + 1;
					if(canUseSunflower && index ==1)
						curZhiWu = index + 1;
			}
				else {
					collectSunshine(&msg);  //�ռ�����ĺ���
				}
		}
		//����϶���Ϣ
		else if (msg.message == WM_MOUSEMOVE && status == 1) {
				curX = msg.x;
				curY = msg.y;
		}
		else if (msg.message == WM_LBUTTONUP && status == 1/*���̧����ֲֲ��*/) {
				if (msg.x > 256-112 && msg.y > 179 && msg.y < 489) {
					int row = (msg.y - 179) / 102;  //��λ������
					int col = (msg.x - (256 - 112)) / 81;	//��λ������

					if (map[row][col].type == 0) {       /*�жϵ�ǰ�����Ƿ���ֲ��  Ϊ����������ֲ��*/
						mciSendString("play res/res/plant1.mp3", 0, 0, 0); //ֲ����ֲ��Ч
						sunshine -= PlantsCost[curZhiWu - 1];
						map[row][col].type = curZhiWu;		//��ֲֲ�������
						map[row][col].frameIndex = 0;		//ֲ�������֡
						map[row][col].shootTime = 0;		//ÿ���㶹�ļ�����

						map[row][col].x = 256 - 112 + col * 81;
						map[row][col].y = 179 + row * 102 + 14;
					}
				}
				
				curZhiWu = 0;
				status = 0;
		}
	}
	
}

//��������ĺ���
void createSunshine() {
	static int count = 0;   //���þ�̬����������
	static int fre = 400*2;   //freΪƵ��֡����ÿ������֡��������
	count++;
	if (count >= fre) {
		fre = 200 + rand() % 200;
		count = 0;
		//���������ȡ��һ������ʹ�õ�����
		int ballMax = sizeof(balls) / sizeof(balls[0]);

		int i;
		for (i = 0; i < ballMax && balls[i].used; i++);//forѭ���ҵ���i��û��ʹ�õ�����
		if (i >= ballMax) return;     //û�п���ʹ�õ�����ֱ�ӽ���

		//��ʼ���������
		balls[i].used = true;
		balls[i].frameIndex = 0;
		balls[i].timer = 0;
		balls[i].status = SUNSHINE_DOWN;								//���⴦�ڽ���״̬
		balls[i].t = 0;
		balls[i].p1 = vector2(260-112 + rand() % (900 - (260-112)), 60);          //���������߳�ʼλ��
		balls[i].p4 = vector2(balls[i].p1.x, 200 + (rand() % 4) * 90);  //��������������λ��
		int off = 2;
		float distance = balls[i].p4.y - balls[i].p1.y;
		balls[i].speed = 1.0 / (distance / off);

	}

	//���տ���������
	static int count2 = 0;			//���徲̬���� ʹ������������
	if (++count2 < 2*2)return;
	count2 = 0;

	int ballMax = sizeof(balls) / sizeof(balls[0]);   //�������������ֵ
	//˫��ѭ������������ͼֲ��ж��Ƿ������տ�
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type == XIANG_RI_KUI + 1) {
				map[i][j].timer++;
				if (map[i][j].timer > 200) {
					map[i][j].timer = 0;

					int k;							
					for (k = 0; k < ballMax && balls[k].used ; k++);		//�������ص������Ѿ�ʹ���˾�����
					if (k >= ballMax) return;

					balls[k].used = true;									//���ûʹ�ã��ͼ��㱴�������ߵ��ĸ��㣬�ı��������״̬
					balls[k].p1 = vector2(map[i][j].x, map[i][j].y);
					int w = (50 + rand() % 50) * (rand() % 2 ? 1 : -1);
					balls[k].p4 = vector2(map[i][j].x + w,
						map[i][j].y + imgZhiWu[XIANG_RI_KUI][0]->getheight() -
						imgSunshineBall[0].getheight());
					balls[k].p2 = vector2(balls[k].p1.x + w * 0.3, balls[k].p1.y - 100);
					balls[k].p3 = vector2(balls[k].p1.x + w * 0.7, balls[k].p1.y - 100);
					balls[k].status = SUNSHINE_PRODUCT;
					balls[k].speed = 0.05;
					balls[k].t = 0;
				}
			}
		}
	}

}

//��������״̬�ĺ���
void updateSunshine() {
	int ballMax = sizeof(balls) / sizeof(balls[0]); //�����ж��ٸ�������
	for (int i = 0; i < ballMax; i++) {
		//�������û��ʹ�ã�����֡������
		if (balls[i].used) {
			balls[i].frameIndex = (balls[i].frameIndex + 1) % 29;
			//�Ӳ�ͬ�����״̬���в�ͬ�Ĳ���
			//�������������Ͼͱ�ɵ���״̬
			if (balls[i].status == SUNSHINE_DOWN) {
				struct sunshineBall* sun = &balls[i];
				sun->t += sun->speed;
				sun->pCur = sun->p1 + sun->t * (sun->p4 - sun->p1);
				if (sun->t >= 1) {
					sun->status = SUNSHINE_GROUND;
					sun->timer = 0;
				}
			}
			//����ڵ���ʱ�䳬��200�������ʧЧ
			else if (balls[i].status == SUNSHINE_GROUND) {
				balls[i].timer++;
				if (balls[i].timer > 200) {
					balls[i].used = false;
					balls[i].timer = 0;
				}
			}
			//������⴦���ռ�״̬���͸ı������λ�ã��ﵽ������ֵ����
			else if (balls[i].status == SUNSHINE_COLLECT) {
				struct sunshineBall* sun = &balls[i];
				sun->t += sun->speed;
				sun->pCur = sun->p1 + sun->t * (sun->p4 - sun->p1);
				if (sun->t > 1) {
					sun->used = false;
					sunshine += 25;
				}
			}
			//������⴦�����տ�������״̬�������ɱ���������
			else if (balls[i].status == SUNSHINE_PRODUCT) {
				struct sunshineBall* sun = &balls[i];
				sun->t += sun->speed;
				sun->pCur = calcBezierPoint(sun->t, sun->p1, sun->p2, sun->p3, sun->p4);	//�������������ɱ���������
				if (sun->t > 1) {
					sun->status = SUNSHINE_GROUND;
					sun->timer = 0;
				}
			}
		}
	}
}


//������ʬ�ĺ���
void createZM() {
	if (zmCount >= ZM_MAX) {
		return;
	}

	static int count2 = 0;
	if (++count2 < 2)return;
	count2 = 0;

	static int zmFre = 200;			//��ʾÿ�����ٴ���һ����ʬ
	static int count = 0;			// ��ʾ��ʬ����ѭ�����õĴ���
	count++;
	if (count > zmFre) {
		count = 0;
		zmFre = rand() % 200 + 300;

		int i;
		int zmMax = sizeof(zms) / sizeof(zms[0]);   //��ʬ�����
		//����һ�߽�ʬ,�ҵ�����ʹ�õĽ�ʬ
		for (i = 0; i < zmMax && zms[i].used; i++);
		if (i < zmMax) {
			memset(&zms[i], 0, sizeof(zms[i]));     //��ʼ��ȫ��Ϊ0
			zms[i].used = true;
			zms[i].x = WIN_WIDTH;					//��ʼλ��xΪ���ڿ��
			zms[i].row = rand() % 3;                //0-2�������
			zms[i].y = 172 + (1 + zms[i].row) * 100;
			zms[i].speed = 1;
			zms[i].blood = 100;
			zms[i].dead = false;					//��ʼ������״̬Ϊ��
			zmCount++;
		}
	}
}


//���½�ʬ״̬�ĺ���
void updateZM() {
	int zmMax = sizeof(zms) / sizeof(zms[0]);  //��ʬ�ĸ���

	static int count = 0;					  //�����ʱ��
	count++;								  
	//Ӧ�ü�ʱ��������ʬ�ٶ�
	if (count > 2*2) {
		count = 0;
		// ���½�ʬ��λ��
		for (int i = 0; i < zmMax; i++) {
			if (zms[i].used) {
				zms[i].x -= zms[i].speed;
				//�����ʬ���뷿�ӣ���Ϸ״̬�͸ı�
				if (zms[i].x < -112) {
					gameStatus = FAIL;
				}
			}
		}
	}

	static int count2;						//�����ʱ����������ʬͼƬ֡�����ٶ�
	count2++;
	if (count2 > 4*2) {
		count2 = 0;
		for (int i = 0; i < zmMax; i++) {
			if (zms[i].used) {
				//�����ʬ����ʹ�ò����������ý�ʬ������֡�������ӣ�������Ϻ���ս�ʬ
				if (zms[i].dead) {
					zms[i].frameIndex++;
					if (zms[i].frameIndex >= 20) {
						zms[i].used = false;
						killCount++;
						if (killCount == ZM_MAX) {
							gameStatus = WIN;
						}
					}
				}
				else if (zms[i].eating) {
					zms[i].frameIndex = (zms[i].frameIndex + 1) % 21;    //��ʬ��ֲ��ʱ��֡����
				}
				else {
					zms[i].frameIndex = (zms[i].frameIndex + 1) % 22;	 //��ʬ����ʱ��֡����
				}
			}
		}
	}

}

//�����㶹�ӵ��ĺ���
void shoot() {
	static int count = 0;
	if (++count < 2 )return;
	count = 0;

	//�ж���û�н�ʬ����
	int lines[3] = { 0 };//�н�ʬΪ1
	int zmCount = sizeof(zms) / sizeof(zms[0]);              //��ʬ�������
	int bulletMax = sizeof(bullets) / sizeof(bullets[0]);    //�ӵ�����
	int dangerX = WIN_WIDTH - imgZM[0].getwidth()+50;           //��ʬ�߹���Զֲ�￪ʼ����
	for (int i = 0; i < zmCount; i++) {
		if (zms[i].used && zms[i].x < dangerX) {
			lines[zms[i].row] = 1;				             //���彩ʬ������
		}
	}

	//˫��ѭ������ƺ���㶹����
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type == WAN_DOU + 1 && lines[i]) {
				//static int count = 0;						//���徲̬�����������ӵ������Ƶ��
				//count++;
				map[i][j].shootTime++;
				//if (count > 70) {
				if (map[i][j].shootTime > 70)
				{
					//count = 0;
					map[i][j].shootTime = 0;
					int k;									//����kΪ��k���ܹ�ʹ�õ��ӵ���
					for (k = 0; k < bulletMax && bullets[k].used; k++); //�ҵ��ܹ�ʹ�õ��ӵ�
					if (k < bulletMax) {
						bullets[k].used = true;				//ʹ����
						bullets[k].row = i;					//��i��
						bullets[k].speed = 8;				//�����ٶ�

						bullets[k].blast = false;			//�ӵ��Ƿ�����ը
						bullets[k].framIndex = 0;			//֡���

						int zwX = 256-112 + j * 81;				//ֲ���x��λ��
						int zwY = 179 + i * 102 + 14;		//ֲ���y��λ��
						bullets[k].x = zwX + imgZhiWu[map[i][j].type - 1][0]->getwidth() - 10; //�ӵ��ĳ�ʼλ��Ϊ�㶹��ʹ�
						bullets[k].y = zwY + 5;
					}
				}
			}
		}
	}
}

//���·����ӵ�λ�õĺ���
void updateBullets() {
	static int count = 0;
	if (++count < 2 )return;
	count = 0;

	int countMax = sizeof(bullets) / sizeof(bullets[0]);  //�����ӵ��ص��������
	//����ÿ���ӵ�������λ�ã���������߽磬�ӵ��ͻ���
	for (int i = 0; i < countMax; i++) {
		if (bullets[i].used) {
			bullets[i].x += bullets[i].speed;
			if (bullets[i].x > WIN_WIDTH) {
				bullets[i].used = false;
			}

			
			if (bullets[i].blast) {
				bullets[i].framIndex++;
				if (bullets[i].framIndex >= 4) {
					bullets[i].used = false;
				}
			}
		}
	}
}

//�㶹�ӵ��뽩ʬ����ײ
void checkBullet2zm() {
	//ÿһ���ӵ���ÿһ����ʬ���м��
	int bCount = sizeof(bullets) / sizeof(bullets[0]); //�ӵ������ֵ
	int zCount = sizeof(zms) / sizeof(zms[0]);		   //��ʬ�����ֵ
	for (int i = 0; i < bCount; i++) {
		//ûʹ�úͱ�ը���ӵ����òμ�ѭ��
		if (bullets[i].used == false || bullets[i].blast) continue;

		//ûʹ�õĽ�ʬ������ѭ��
		for (int k = 0; k < zCount; k++) {
			//if (zms[i].used == false) continue;
			if (zms[k].used == false) continue;
			int x1 = zms[k].x + 80;				//�ӵ��뽩ʬ��ײ�����
			int x2 = zms[k].x + 110;			//�ӵ��뽩ʬ��ײ���Ҳ�
			int x = bullets[i].x;				//�ӵ�������
			//�����ʬ��û����,�ӵ��뽩ʬ��������ȣ�x��������ϣ��ͷ�����ײ
			if (zms[k].dead == false && bullets[i].row == zms[k].row && x > x1 && x < x2) {
		//		PlaySound("res/res/splat.wav", NULL, SND_FILENAME | SND_ASYNC);   //�ӵ���ײ��Ч
				mciSendString("close res/res/splat1.mp3", 0, 0, 0);
				mciSendString("play res/res/splat1.mp3", 0, 0, 0);
				zms[k].blood -= 10;
				bullets[i].blast = true;
				bullets[i].speed = 0;
				//��ʬ����ʱ�ı��������
				if (zms[k].blood <= 0) {
					zms[k].dead = true;
					zms[k].speed = 0;
					zms[k].frameIndex = 0;
				}
				break;
			}
		}
	}
}

//ֲ���뽩ʬ����ײ���
void checkzm2ZhiWu() {
	int zCount = sizeof(zms) / sizeof(zms[0]);      //���彩ʬ���ֵ
	//forѭ������������ʬ
	for (int i = 0; i < zCount; i++) {
		int row = zms[i].row;
		for (int k = 0; k < 9; k++) {
			if (map[row][k].type == 0) continue;	//�����һ��û��ֲ��ֱ������ѭ��

			//			x1   x2
			//	 ֲ��    [   ]
			//	 ��ʬ    	[    ]
			//				x3   x4
			int zhiWuX = 256-112 + k * 81;
			int x1 = zhiWuX + 10;
			int x2 = zhiWuX + 60;
			int x3 = zms[i].x + 80;
			int x4 = zms[i].x + 135;
			//�����ʬx3>x1����x3<x2��˵����ʬ���Կ�ʼ����
			if (((x3 > x1 && x3 < x2 ) || (x4 > x1 && x4 < x2)) && zms[i].dead == false) {
				if (map[row][k].catched && zms[i].speed ==0) {
					map[row][k].deadtime++;                  //��ײ���ʱֲ�￪ʼ��������ʱ
					if (map[row][k].deadtime > 350) {
						map[row][k].deadtime = 0;
						map[row][k].type = 0;
						zms[i].eating = false;
						zms[i].frameIndex = 0;
						zms[i].speed = 1;
					}
				}
				else 
				{
					//��ʬ����ֲ��ʱ�ı��������
					map[row][k].catched = true;				
					map[row][k].deadtime = 0;
					zms[i].eating = true;
					zms[i].speed = 0;
					zms[i].frameIndex = 0; 
				}
			}
		}
	}
}

//ʵ����ײ���
void collisionCheck() {
	checkBullet2zm();           //�㶹�ӵ��뽩ʬ����ײ���
	checkzm2ZhiWu();			//��ʬ��ֲ�����ײ���

}

//��Ⱦֲ��ҡ�ڶ���
void updateZhiWu() {
	static int count = 0;
	if (++count < 2*2)return;
	count = 0;
	// ˫��ѭ����Ⱦֲ��ҡ�ڶ���
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type > 0) {                 //�ж���û��ֲ��
				map[i][j].frameIndex++;				  //ÿ��ѭ������һ��*imgZhiWuָ����ֵ
				int zhiWuType = map[i][j].type - 1;
				int index = map[i][j].frameIndex;
				if (imgZhiWu[zhiWuType][index] == NULL) {/*�ж�ָ���Ƿ�ָ���ָ�룬����ǿ�ָ��Ͳ����� ����Ⱦ֡���¿�ʼ*/
					map[i][j].frameIndex = 0;
				}
			}
		}
	}
}

//�ж�ֲ���Ƿ�����ֲ
void updatePlantsState() {
	if (sunshine >= 50) {
		canUseSunflower = true;
	}
	else {
		canUseSunflower = false;
	}

	if (sunshine >= 100) {
		canUsePea = true;
	}
	else {
		canUsePea = false;
	}
	
}

//�ı���Ϸ������ݵĺ�����������Ϸ����״̬
void updateGame() {
	
	updateZhiWu();		//��Ⱦֲ��ҡ�ڶ���

	createSunshine();   //��������ĺ���
	updateSunshine();	//��������״̬�ĺ���

	createZM();         //������ʬ�ĺ���
	updateZM();			//���½�ʬ״̬�ĺ���

	shoot();			//�����㶹�ӵ�
	updateBullets();	//�����㶹�ӵ���λ��

	collisionCheck();   //��ײ���ĺ��� 
	updatePlantsState();//�ж�ֲ���ܷ���ֲ
}

//��ʼ�˵�����UI
void starUI() {
	IMAGE imgBg, imgMenu1, imgMenu2;							//����һ������ͼƬ�ı���
	loadimage(&imgBg, "res/res/menu.png");					    //����ͼƬ
	loadimage(&imgMenu1, "res/res/menu1.png");
	loadimage(&imgMenu2, "res/res/menu2.png");

	int flag1 = 0;//�ж�����Ƿ��µı���
	int flag2 = 0;//�ж�����Ƿ�������ͼ���ϵı���
	while (1) {
		
		BeginBatchDraw();									//��ʼ˫����
		putimage(0, 0, &imgBg);
		putimagePNG(474, 75, flag1 == 1 ? &imgMenu2 : &imgMenu1);  //��Ŀ����������갴����Ϊ������������Ϊ��ɫ
		putimagePNG(474, 75, flag2 == 1 ? &imgMenu2 : &imgMenu1);  //������������Ϊ����������Ϊ��ɫ

		ExMessage msg;
		if (peekmessage(&msg)) {
			if (msg.message == WM_MOUSEMOVE && 			   //��������ڲ˵���ʱ��flag2=1�ͼ��ظ���ͼƬ
				msg.x > 474 && msg.x < 474 + 300 &&
				msg.y > 75 && msg.y < 75 + 140) {
				flag2 = 1;
			}
			if (msg.message == WM_MOUSEMOVE && 			   //��겻�����ڲ˵���ʱ��flag2=0�Ͳ����ظ���ͼƬ
				msg.x < 474 || msg.x  > 474 + 300 &&
				msg.y <  75 || msg.y  >  75 + 140) {
				flag2 = 0;
			}

			if (msg.message == WM_LBUTTONDOWN &&			//��갴�²�����flag1 = 1��
				msg.x > 474 && msg.x < 474 + 300 &&
				msg.y > 75 && msg.y < 75 + 140) {
				flag1 = 1;								 
			}
			else if (msg.message == WM_LBUTTONUP && flag1) {
				EndBatchDraw();
				break ;
			}
		}
		EndBatchDraw();											 //����˫����	
	}
}

//��Ļready
void playReady(){
	BeginBatchDraw();
	for (int i = 0; i < 1000; i++) {
		putimagePNG(350, 250, &imgReady);
	}
	EndBatchDraw();
}

//��Ļset
void playSet() {
	BeginBatchDraw();
	for (int i = 0; i < 1000; i++) {
		putimagePNG(354, 240, &imgSet);
	}
	EndBatchDraw();
}

//��Ļplants
void playSetplants() {
	BeginBatchDraw();
	for (int i = 0; i < 1000; i++) {
		putimagePNG(330, 230, &imgSetplans);
	}
	EndBatchDraw();
}

//ת����ĵ���ʱ
void Countdown() {
	Sleep(50);
	playReady();
	EndBatchDraw();

	playSet();
	playSetplants();
	Sleep(200);

	mciSendString("play res/res/zombiescoming.mp3", 0, 0, 0);
	Sleep(3500);
}

//ת������
void viewScence() {
	int xMin = WIN_WIDTH - imgBg.getwidth();  //900-1400=-500
	//ת����·�Ͻ�ʬ����
	vector2 points[9] = {
		{550, 80},{530,160},{630,170},
		{530,200},{515,270},{565,370},
		{605,340},{705,280},{690,340}
	};
	int index[9];  
	//��ʼ����ʬ���֡��
	for (int i = 0; i < 9; i++) {
		index[i] = rand() % 11;
	}

	//��ʱ��������Ⱦ�ٶ�
	static int count = 0;
	for (int x = 0; x >= xMin; x -= 3) {
		BeginBatchDraw();  //��ʼ֡����
		putimage(x, 0, &imgBg);
		count++;
		for (int k = 0; k < 9; k++) {
			//����ͼƬ
			putimagePNG(points[k].x - xMin + x,  
				points[k].y,
				&imgZmStand[index[k]]);
			if (count >= 10) {
				index[k] = (index[k] + 1) % 11;
				}
		}
		if (count >= 10)count = 0;

		EndBatchDraw();    //����֡����
		Sleep(5);
	}

	//ͣ��5s����
	for (int i = 0; i < 100; i++) {
		BeginBatchDraw();

		putimage(xMin, 0, &imgBg);
		for (int k = 0; k < 9; k++) {
			putimagePNG(points[k].x, points[k].y, &imgZmStand[index[k]]);
			index[k] = (index[k] + 1) % 11;
		}

		EndBatchDraw();
		Sleep(30);
	}

	//���������ƶ�
	for (int x = xMin; x <= -112; x += 3) {
		BeginBatchDraw();

		putimage(x, 0, &imgBg);

		count++;
		for (int k = 0; k < 9; k++) {
			putimagePNG(points[k].x - xMin + x, points[k].y, &imgZmStand[index[k]]);
			if (count >= 10) {
				index[k] = (index[k] + 1) % 11;
			}
			if (count >= 10) count = 0;
		}

		EndBatchDraw();
		Sleep(5);
	}

//���������½�
}void barsDown() {
	int height = imgBar.getheight();
	for (int y = -height; y <= 0; y++) {
		BeginBatchDraw();

		putimage(-112, 0, &imgBg);
		putimagePNG(250, y, &imgBar);

		for (int i = 0; i < ZHI_WU_COUNT; i++) {
			int x = 338 + i * 65;
			putimage(x, y+6, &imgCards[i]);
		}

		EndBatchDraw();
		Sleep(2);
	}
}

//�ж���Ϸ�Ƿ����
bool checkOver() {
	int ret = false;
	if (gameStatus == WIN) {
		Sleep(2000);

		mciSendString("close res/res/audio/bg.mp3", 0, 0, 0);//��ͣ��������
		mciSendString("play res/res/win.mp3", 0, 0, 0);//
		loadimage(0, "res/res/win2.png");
		ret = true;
	}
	else if (gameStatus == FAIL) {
		Sleep(2000);

		mciSendString("close res/res/audio/bg.mp3", 0, 0, 0);
		mciSendString("play res/res/lose.mp3", 0, 0, 0);
		loadimage(0, "res/res/fail2.png");
		ret = true;
	}

	return ret;
}

int main(void) {
	gameInit();			      //��ʼ��

	starUI();				  //��ʼ�˵�����

	viewScence();             //ת������

	Countdown();			  //ת���󵹼�ʱ

	barsDown();				  //���������½�

	mciSendString("play res/res/audio/bg.mp3", 0, 0, 0);//���ű�������
	int timer = 0;            //���м�ʱ�ļ�ʱ��
	int flag = true;		  //��Ǳ���
	while (1) {
		userClick();          //�û��������
		timer += getDelay();  //getDelay��ȡ��ʱʱ��ĺ���
		if (timer > 10) {
			flag = true;	  //���Ϊ��˵������һ����Ⱦˢ��
			timer = 0;        //�ӳٴ���20ms��ʱ������Ϊ0
		}

		if (flag) {
			flag = false;
			updateWindow();      //��Ⱦ����
			updateGame();	     //�ı���Ϸ��ص�����

			if(checkOver()) break;		//��ʾ��Ϸ�Ƿ����
		}

	}
	system("pause");
	return 0;
}