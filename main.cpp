/*
* 开发日志
* 1.创建新项目（空项目模板）使用vs的任意版本
* 2.导入素材
* 3.实现最开始的游戏场景
* 4.实现游戏顶部的工具栏
* 5.实现工具栏中的植物卡牌
* 6.实现植物卡牌的拖动
* 7.实现植物卡牌的种植
* 8.实现植物卡牌的摇摆
* 9.优化游戏循环和渲染顺序
* 10.制作启动菜单
* 11.创建随机阳光
* 12.显示随机阳光
* 13.收集阳光，显示阳光值
* 14.创建僵尸
* 15.实现僵尸的行走动作
* 16.实现阳光球的飞跃
* 17.发射植物豌豆
* 18.渲染植物豌豆
* 19.实现子弹与僵尸的碰撞
* 20.实现僵尸与植物的碰撞
* 21.实现向日葵生成阳光
* 22.游戏动作速度的加速，以及代码的优化
* 23.片头巡场
* 24.增加开头动画
* 25.实现工具栏中的灰色卡牌
* 26.判断当前阳光能否种植植物
* 27.实现种植植物消耗阳光

*/

#include <stdio.h>
#include <graphics.h>  //easyx图形库的头文件，需要安装easyx图形库
#include<time.h>
#include<math.h>
#include "tools.h"
#include"vector2.h"

#include<Windows.h>
#include<mmsystem.h>	//音效播放的数据库
#pragma comment(lib,"winmm.lib")

#define WIN_WIDTH   900
#define WIN_HEIGHT  600
#define ZM_MAX      10

enum {WAN_DOU,XIANG_RI_KUI, ZHI_WU_COUNT};

  IMAGE imgBg; //表示背景图片
  IMAGE imgBar; //表示植物工具栏图片
  IMAGE imgCards[ZHI_WU_COUNT];  //表示植物卡牌图片
  IMAGE imgFCards[ZHI_WU_COUNT]; //表示不能使用的植物卡牌图片
  IMAGE imgFBea;	//表示灰色豌豆卡牌图片
  IMAGE imgFSunflower;//表示灰色向日葵卡牌图片
  IMAGE* imgZhiWu[ZHI_WU_COUNT][20];//通过二维指针数组来存放植物各个时刻的摇摆图片

  IMAGE imgReady;//表示倒计时图片
  IMAGE imgSet;//表示倒计时的图片
  IMAGE imgSetplans;//表示倒计时的图片

  int curX, curY;//当前选中的植物，在移动的过程中的位置
  int curZhiWu;  //表示选中的植物 0：没有选中，i：选择第i种植物

  enum{GOING,WIN,FAIL};
  int killCount;//已经杀掉的僵尸个数
  int zmCount;  //已经出现的僵尸个数
  int gameStatus;//当前游戏的状态

  struct zhiwu {
	  int type;		  //0：没有植物 1：1号植物
	  int frameIndex; //序列帧的序号

	  bool catched;	  //是否被僵尸吃
	  int deadtime;   //死亡计数器

	  int timer;      //计时器
	  int x, y;		  //植物的坐标

	  int shootTime;  //每颗豌豆的计数器

  };

  int PlantsCost[ZHI_WU_COUNT];

//     int costPea;		//豌豆射手的cost
//     int costSunflower;//向日葵的cost

  bool canUsePea;		//当前阳光能够种植豌豆
  bool canUseSunflower; //当前阳光能否种植向日葵

  struct zhiwu map[3][9]; //植物的布局用二维结构体数组表示

  enum{SUNSHINE_DOWN,SUNSHINE_GROUND,SUNSHINE_COLLECT,SUNSHINE_PRODUCT}; //表示阳光收集的不同状态

  //应用结构体表示阳光
  struct sunshineBall {   
	  int x, y;				     //阳光球在飘落过程中的坐标位置（x不变）
	  int frameIndex;			 //当前显示的图片帧的序号
	  int destY;				 //飘落的目标位置的y坐标
	  bool used;			     //阳光是否在使用
	  int timer;				 //计时器

	  float xoff;				 //实现阳光球的飞跃，x轴偏移量
	  float yoff;				 //实现阳光球的飞跃，y轴偏移量

	  float t;					 //贝塞尔曲线的时间点0-1
	  vector2 p1, p2, p3, p4;	 //贝塞尔曲线的四个控制点
	  vector2 pCur;				 //当前时刻阳光球的位置
	  float speed;				 //当前阳光的速度
	  int status;				 //当前阳光的状态

  }; 

  //应用结构体表示僵尸
  struct zm {
	  int x, y;			  //坐标x，y
	  int frameIndex;	  //每一帧的帧序号
	  bool used;		  //定义僵尸是否出场
	  int speed;		  //行走速度
	  int row;			  //僵尸的行数
	  int blood;	      //僵尸的血量
	  bool dead;		  //僵尸是否死亡
	  bool eating;		  //僵尸是否正在吃植物
	  
  };
  struct zm zms[10];      //僵尸池
  IMAGE imgZM[22];		  //加载每一帧的图片
  IMAGE imgZMDead[20];    //加载僵尸死亡的图片 
  IMAGE imgZMEat[21];     //加载僵尸吃植物的图片
  IMAGE imgZmStand[11];		//僵尸站立状态下的图片

  //应用结构体定义植物子弹的数据类型
  struct bullet {
	  int x, y;           //子弹的位置
	  int row;			  //子弹的行数
	  bool used;		  //子弹是否使用
	  int speed;		  //子弹的速度
	  bool blast;		  //子弹是否发生爆炸
	  int framIndex;	  //爆炸图片的帧序号 
  };

  struct bullet bullets[30];//定义子弹池
  IMAGE imgBulletNormal;    //正常状态下子弹的图片
  IMAGE imgBullBlast[4];    //爆炸状态下子弹的图片


  //池的概念——预先准备10个阳光，当需要生成阳光，从阳光池中取出一个能使用的，用完后归还池中
  struct sunshineBall balls[10]; //创建阳光池
  IMAGE imgSunshineBall[29];	 //创建植物每帧的图片
  int sunshine;                  //创建阳光值

  //bool类型判断文件是否存在
  bool fileExist(const char* name) {
	  FILE* fp = fopen(name, "r");
	  //如果文件为空则返回假，文件存在关闭后返回真
	  if (fp == NULL) {   
		  return false;
	  }
	  else {
		  fclose(fp);
		  return true;
	  }
  }

//初始化的函数
void gameInit() {
	//加载游戏背景图片
	//把字符集修改为“多字节字符集”
	loadimage(&imgBg, "res/bg.jpg");			//加载背景图片
	loadimage(&imgBar, "res/bar.png");			//加载植物栏图片

	loadimage(&imgReady, "res/res/ready.png");	//加载准备图片
	loadimage(&imgSet, "res/res/set.png");		//加载放置图片
	loadimage(&imgSetplans, "res/res/setplants.png");//加载植物图片

	loadimage(&imgFBea, "res/res/Cards_Black/card_1.png");		//加载灰色豌豆图片
	loadimage(&imgFSunflower, "res/res/Cards_Black/card_2.png");		//加载灰色向日葵图片


	memset(imgZhiWu, 0, sizeof(imgZhiWu));//memset函数将植物摆动数组内存连续设为一个值
	memset(map, 0, sizeof(map));		  //初始化植物布局数组

	killCount = 0;   //击杀僵尸数为0
	zmCount = 0;     //僵尸数为0
	gameStatus = GOING;//游戏状态 进行中
	canUsePea = false;       //初始化豌豆能否种植
	canUseSunflower = false; //初始化向日葵能否种植
	
	//初始化植物cost
	PlantsCost[0] = 100;
	PlantsCost[1] = 50;

	//初始化植物卡牌
	char name[64];
	for (int i = 0; i < ZHI_WU_COUNT; i++) {
		//生成植物卡牌的文件名
		sprintf_s(name, sizeof(name), "res/res/Cards/card_%d.png", i + 1);
			loadimage(&imgCards[i], name);
		
		sprintf_s(name, sizeof(name), "res/res/Cards_Black/card_%d.png", i + 1);
			loadimage(&imgFCards[i], name);

			for (int j = 0; j < 20; j++) {
				sprintf_s(name, sizeof(name), "res/res/zhiwu/%d/%d.png",i ,j + 1);
				//先判断这个文件是否存在 看能不能打开这个文件  文件存在就加载进来
				if (fileExist(name)) {
					imgZhiWu[i][j] = new IMAGE; //c++的方式分配内存
					loadimage(imgZhiWu[i][j], name);
				}
				else {
					break;
				}
			}
	}

	curZhiWu = 0;   //表示选中的植物
	sunshine = 50;  //初始化阳光值为50

	


	memset(balls, 0, sizeof(balls[10]));   //初始化阳光池全为0
	//for 循环加载阳光帧的图片
	for (int i = 0; i < 29; i++) {
		sprintf_s(name, sizeof(name), "res/res/sunshine/%d.png", i + 1);
		loadimage(&imgSunshineBall[i], name);
	}

	//配置随机种子
	srand(time(NULL));

	//创建游戏的图形窗口
	initgraph(WIN_WIDTH, WIN_HEIGHT);

	//设置字体
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWeight = 15;
	strcpy(f.lfFaceName, "Segoe UI Black");
	f.lfQuality = ANTIALIASED_QUALITY; //抗锯齿效果
	settextstyle(&f);				   //设置字体
	setbkmode(TRANSPARENT);			   //设置背景模式透明
	setcolor(BLACK);				   //设置字体颜色

	//初始化僵尸数据
	memset(zms, 0, sizeof(zms));	   //初始化僵尸为0
	//for循环加载僵尸每一帧的图片
	for (int i = 0; i < 22; i++) {
		sprintf_s(name, sizeof(name), "res/res/zm/%d.png", i + 1);
		loadimage(&imgZM[i], name);
	}

	//初始化子弹数据
	loadimage(&imgBulletNormal, "res/res/bullets/bullet_normal.png");
	memset(bullets, 0, sizeof(bullets));			//初始化子弹池

	//初始化豌豆子弹的帧图片数组 ,通过子弹爆炸图片的缩小展示动画
	loadimage(&imgBullBlast[3], "res/res/bullets/bullet_blast.png");
	for (int i = 0; i < 3; i++) {
		float k = (i + 1) * 0.2;
		loadimage(&imgBullBlast[i], "res/res/bullets/bullet_blast.png",
			imgBullBlast[3].getwidth() * k,
			imgBullBlast[3].getheight() * k, true);
	}

	//初始化僵尸死亡条件	
	for (int i = 0; i < 20; i++) {
		sprintf_s(name, sizeof(name), "res/res/zm_dead/%d.png", i + 1);
		loadimage(&imgZMDead[i], name);
	}

	for (int i = 0; i < 21; i++) {
		sprintf_s(name, "res/res/zm_eat/%d.png", i + 1);
		loadimage(&imgZMEat[i], name);
	}

	//加载僵尸站立图片
	for (int i = 0; i < 11; i++) {
		sprintf_s(name, "res/res/zm_stand/%d.png", i + 1);
		loadimage(&imgZmStand[i], name);
	}

}

//渲染僵尸的函数
void drawZM() {
	int zmCount = sizeof(zms) / sizeof(zms[0]);       //定义僵尸的最大值
	//循环加载每帧的僵尸图片
	for (int i = 0; i < zmCount; i++) {
		if (zms[i].used) {
			//IMAGE* img = (zms[i].dead) ? imgZMDead : imgZM; //定义指针，让对应帧的指针指向对应的图片
			//img += zms[i].frameIndex;
			IMAGE* img = NULL;
			if (zms[i].dead)img = imgZMDead;
			else if (zms[i].eating)img = imgZMEat;
			else img = imgZM;

			img += zms[i].frameIndex;

			putimagePNG(zms[i].x,
				zms[i].y - img->getheight(),		  //y坐标表示的是僵尸图片左下角的坐标，渲染是从图片左上角开始渲染，所以加上一个长度
				img);
		}
	}
}

//渲染子弹
void drawBullets() {
	int bulletMax = sizeof(bullets) / sizeof(bullets[0]);   //计算最大的子弹数
	//遍历每一个子弹渲染
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

//渲染阳光的函数
void drawSunshines() {
	int ballMax = sizeof(balls) / sizeof(balls[0]);		//计算最大的阳光数
	//遍历每一个阳光并渲染
	for (int i = 0; i < ballMax; i++) {
		//if (balls[i].used || balls[i].xoff) {
		if(balls[i].used){
			IMAGE* img = &imgSunshineBall[balls[i].frameIndex];
			//putimagePNG(balls[i].x, balls[i].y, img);
			putimagePNG(balls[i].pCur.x, balls[i].pCur.y, img);
		}
	}
	//渲染 阳光值
	char scoreText[8];				//把阳光值变成字符串
	sprintf_s(scoreText, sizeof(scoreText), "%d", sunshine);	//sprintf_s函数 把指定格式的字符串打印到数组中
	outtextxy(278, 67, scoreText); //输出阳光值
}

void drawZhiWu() {
	//双重循环渲染种植的植物
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type > 0) {
				//	int x = 256 + j * 81;				    //植物行坐标
				//	int y = 179 + i * 102 + 14;			    //植物列坐标

				int zhiWuType = map[i][j].type - 1;        //植物的类型
				int index = map[i][j].frameIndex;		   //植物当前的图片帧
				//putimagePNG(x, y, imgZhiWu[zhiWuType][index]);
				putimagePNG(map[i][j].x, map[i][j].y, imgZhiWu[zhiWuType][index]);
			}
		}
	}
	// 渲染 拖动过程中的植物
	if (curZhiWu > 0) {
		IMAGE* img = imgZhiWu[curZhiWu - 1][0];  //将植物放在光标的正中间
		putimagePNG(curX - img->getwidth() / 2, curY - img->getheight() / 2, img);
	}
}

//渲染 卡牌
void drawCards() {
	int i = 0;					 //i=0是豌豆，i=1是向日葵
	int x = 338 + i * 65;    //卡牌位置坐标
	int y = 6;

	if (canUsePea) {
		putimage(x, y, &imgCards[0]); //渲染能使用的豌豆图片
	}
	if(canUsePea == false){
		putimage(x, y, &imgFCards[0]); //渲染灰色的豌豆图片
	}
	if (canUseSunflower) {
		putimage(x + 65, y, &imgCards[1]); //渲染能使用的向日葵图片
	}
	if(canUseSunflower == false) {
		putimage(x + 65, y, &imgFCards[1]); //渲染灰色的向日葵图片
	}
}

//渲染函数
void updateWindow() {
	BeginBatchDraw();//双缓冲——开始缓冲

	putimage(-112, 0, &imgBg);
	putimagePNG(250, 0, &imgBar);

	//渲染卡牌
	drawCards();

	//渲染植物
	drawZhiWu();

	// 渲染 阳光
	drawSunshines();

	//渲染 僵尸
	drawZM();

	//渲染 子弹
	drawBullets();

	EndBatchDraw();//结束双缓冲
}

//收集阳光操作的函数
void collectSunshine(ExMessage* msg) {
	int count = sizeof(balls) / sizeof(balls[0]);//计算当前有多少个阳光球
	int w = imgSunshineBall[0].getwidth();		 //计算第一帧阳光的宽度
	int h = imgSunshineBall[0].getheight();		 //计算第一帧阳光的高度
	//for循环判断点击位置是否有阳光球
	for (int i = 0; i < count; i++) {
		if (balls[i].used) {
			//int x = balls[i].x;
			//int y = balls[i].y;
			int x = balls[i].pCur.x;
			int y = balls[i].pCur.y;

			if (msg->x > x && msg->x < x + w &&
				msg->y > y && msg->y < y + h) {
				balls[i].status = SUNSHINE_COLLECT;
				PlaySound("res/res/sunshine.wav", NULL, SND_FILENAME | SND_ASYNC);   //阳光收集音效
				//计算贝塞尔曲线的始末位置
				balls[i].p1 = balls[i].pCur;
				balls[i].p4 = vector2(262, 0);
				balls[i].t = 0;
				float distance = dis(balls[i].p1 - balls[i].p4);   //c++运算符重载
				float off = 8;									   //每次移动的距离
				balls[i].speed = 1.0 / (distance / off);		   //计算速度
				break;
			}
		}
	}
}

//用户点击操作
void userClick() {
	ExMessage msg;  //消息类型参数
	static int status = 0;  //定义一个静态电压
	    //判断有没有鼠标点击消息  有消息返回真  无消息返回假)
		if (peekmessage(&msg)){
			//鼠标点击信息
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
					collectSunshine(&msg);  //收集阳光的函数
				}
		}
		//鼠标拖动信息
		else if (msg.message == WM_MOUSEMOVE && status == 1) {
				curX = msg.x;
				curY = msg.y;
		}
		else if (msg.message == WM_LBUTTONUP && status == 1/*鼠标抬起种植植物*/) {
				if (msg.x > 256-112 && msg.y > 179 && msg.y < 489) {
					int row = (msg.y - 179) / 102;  //行位置坐标
					int col = (msg.x - (256 - 112)) / 81;	//列位置坐标

					if (map[row][col].type == 0) {       /*判断当前格子是否有植物  为空则允许种植物*/
						mciSendString("play res/res/plant1.mp3", 0, 0, 0); //植物种植音效
						sunshine -= PlantsCost[curZhiWu - 1];
						map[row][col].type = curZhiWu;		//种植植物的种类
						map[row][col].frameIndex = 0;		//植物的序列帧
						map[row][col].shootTime = 0;		//每颗豌豆的计数器

						map[row][col].x = 256 - 112 + col * 81;
						map[row][col].y = 179 + row * 102 + 14;
					}
				}
				
				curZhiWu = 0;
				status = 0;
		}
	}
	
}

//创建阳光的函数
void createSunshine() {
	static int count = 0;   //设置静态变量计数器
	static int fre = 400*2;   //fre为频率帧数，每过多少帧生产阳光
	count++;
	if (count >= fre) {
		fre = 200 + rand() % 200;
		count = 0;
		//从阳光池中取出一个可以使用的阳光
		int ballMax = sizeof(balls) / sizeof(balls[0]);

		int i;
		for (i = 0; i < ballMax && balls[i].used; i++);//for循环找到第i个没有使用的阳光
		if (i >= ballMax) return;     //没有可以使用的阳光直接结束

		//初始化阳光参数
		balls[i].used = true;
		balls[i].frameIndex = 0;
		balls[i].timer = 0;
		balls[i].status = SUNSHINE_DOWN;								//阳光处于降落状态
		balls[i].t = 0;
		balls[i].p1 = vector2(260-112 + rand() % (900 - (260-112)), 60);          //贝塞尔曲线初始位置
		balls[i].p4 = vector2(balls[i].p1.x, 200 + (rand() % 4) * 90);  //贝塞尔曲线最终位置
		int off = 2;
		float distance = balls[i].p4.y - balls[i].p1.y;
		balls[i].speed = 1.0 / (distance / off);

	}

	//向日葵生产阳光
	static int count2 = 0;			//定义静态变量 使阳光生产变慢
	if (++count2 < 2*2)return;
	count2 = 0;

	int ballMax = sizeof(balls) / sizeof(balls[0]);   //定义阳光球最大值
	//双重循环遍历整个地图植物，判断是否是向日葵
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type == XIANG_RI_KUI + 1) {
				map[i][j].timer++;
				if (map[i][j].timer > 200) {
					map[i][j].timer = 0;

					int k;							
					for (k = 0; k < ballMax && balls[k].used ; k++);		//如果阳光池的阳光已经使用了就跳过
					if (k >= ballMax) return;

					balls[k].used = true;									//如果没使用，就计算贝塞尔曲线的四个点，改变阳光球的状态
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

//更新阳光状态的函数
void updateSunshine() {
	int ballMax = sizeof(balls) / sizeof(balls[0]); //计算有多少个阳光球
	for (int i = 0; i < ballMax; i++) {
		//如果阳光没有使用，阳光帧就增加
		if (balls[i].used) {
			balls[i].frameIndex = (balls[i].frameIndex + 1) % 29;
			//从不同阳光的状态进行不同的操作
			//如果阳光下落完毕就变成地面状态
			if (balls[i].status == SUNSHINE_DOWN) {
				struct sunshineBall* sun = &balls[i];
				sun->t += sun->speed;
				sun->pCur = sun->p1 + sun->t * (sun->p4 - sun->p1);
				if (sun->t >= 1) {
					sun->status = SUNSHINE_GROUND;
					sun->timer = 0;
				}
			}
			//如果在地面时间超过200，阳光就失效
			else if (balls[i].status == SUNSHINE_GROUND) {
				balls[i].timer++;
				if (balls[i].timer > 200) {
					balls[i].used = false;
					balls[i].timer = 0;
				}
			}
			//如果阳光处于收集状态，就改变阳光的位置，达到后阳光值增加
			else if (balls[i].status == SUNSHINE_COLLECT) {
				struct sunshineBall* sun = &balls[i];
				sun->t += sun->speed;
				sun->pCur = sun->p1 + sun->t * (sun->p4 - sun->p1);
				if (sun->t > 1) {
					sun->used = false;
					sunshine += 25;
				}
			}
			//如果阳光处于向日葵的生产状态，就生成贝塞尔曲线
			else if (balls[i].status == SUNSHINE_PRODUCT) {
				struct sunshineBall* sun = &balls[i];
				sun->t += sun->speed;
				sun->pCur = calcBezierPoint(sun->t, sun->p1, sun->p2, sun->p3, sun->p4);	//贝塞尔函数生成贝塞尔曲线
				if (sun->t > 1) {
					sun->status = SUNSHINE_GROUND;
					sun->timer = 0;
				}
			}
		}
	}
}


//创建僵尸的函数
void createZM() {
	if (zmCount >= ZM_MAX) {
		return;
	}

	static int count2 = 0;
	if (++count2 < 2)return;
	count2 = 0;

	static int zmFre = 200;			//表示每隔多少创建一个僵尸
	static int count = 0;			// 表示僵尸生成循环调用的次数
	count++;
	if (count > zmFre) {
		count = 0;
		zmFre = rand() % 200 + 300;

		int i;
		int zmMax = sizeof(zms) / sizeof(zms[0]);   //僵尸最大数
		//遍历一边僵尸,找到可以使用的僵尸
		for (i = 0; i < zmMax && zms[i].used; i++);
		if (i < zmMax) {
			memset(&zms[i], 0, sizeof(zms[i]));     //初始化全部为0
			zms[i].used = true;
			zms[i].x = WIN_WIDTH;					//初始位置x为窗口宽度
			zms[i].row = rand() % 3;                //0-2行随机数
			zms[i].y = 172 + (1 + zms[i].row) * 100;
			zms[i].speed = 1;
			zms[i].blood = 100;
			zms[i].dead = false;					//初始化死亡状态为假
			zmCount++;
		}
	}
}


//更新僵尸状态的函数
void updateZM() {
	int zmMax = sizeof(zms) / sizeof(zms[0]);  //僵尸的个数

	static int count = 0;					  //定义计时器
	count++;								  
	//应用计时器减慢僵尸速度
	if (count > 2*2) {
		count = 0;
		// 更新僵尸的位置
		for (int i = 0; i < zmMax; i++) {
			if (zms[i].used) {
				zms[i].x -= zms[i].speed;
				//如果僵尸进入房子，游戏状态就改变
				if (zms[i].x < -112) {
					gameStatus = FAIL;
				}
			}
		}
	}

	static int count2;						//定义计时器来减慢僵尸图片帧加载速度
	count2++;
	if (count2 > 4*2) {
		count2 = 0;
		for (int i = 0; i < zmMax; i++) {
			if (zms[i].used) {
				//如果僵尸还在使用并且死亡，让僵尸死亡的帧序列增加，播放完毕后回收僵尸
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
					zms[i].frameIndex = (zms[i].frameIndex + 1) % 21;    //僵尸吃植物时的帧更新
				}
				else {
					zms[i].frameIndex = (zms[i].frameIndex + 1) % 22;	 //僵尸行走时的帧更新
				}
			}
		}
	}

}

//发射豌豆子弹的函数
void shoot() {
	static int count = 0;
	if (++count < 2 )return;
	count = 0;

	//判断有没有僵尸出现
	int lines[3] = { 0 };//有僵尸为1
	int zmCount = sizeof(zms) / sizeof(zms[0]);              //僵尸最大数量
	int bulletMax = sizeof(bullets) / sizeof(bullets[0]);    //子弹个数
	int dangerX = WIN_WIDTH - imgZM[0].getwidth()+50;           //僵尸走过多远植物开始攻击
	for (int i = 0; i < zmCount; i++) {
		if (zms[i].used && zms[i].x < dangerX) {
			lines[zms[i].row] = 1;				             //定义僵尸是哪行
		}
	}

	//双重循环检测草坪上豌豆射手
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type == WAN_DOU + 1 && lines[i]) {
				//static int count = 0;						//定义静态变量，控制子弹发射的频率
				//count++;
				map[i][j].shootTime++;
				//if (count > 70) {
				if (map[i][j].shootTime > 70)
				{
					//count = 0;
					map[i][j].shootTime = 0;
					int k;									//定义k为第k个能够使用的子弹数
					for (k = 0; k < bulletMax && bullets[k].used; k++); //找到能够使用的子弹
					if (k < bulletMax) {
						bullets[k].used = true;				//使用中
						bullets[k].row = i;					//第i行
						bullets[k].speed = 8;				//飞行速度

						bullets[k].blast = false;			//子弹是否发生爆炸
						bullets[k].framIndex = 0;			//帧序号

						int zwX = 256-112 + j * 81;				//植物的x轴位置
						int zwY = 179 + i * 102 + 14;		//植物的y轴位置
						bullets[k].x = zwX + imgZhiWu[map[i][j].type - 1][0]->getwidth() - 10; //子弹的初始位置为豌豆嘴巴处
						bullets[k].y = zwY + 5;
					}
				}
			}
		}
	}
}

//更新发射子弹位置的函数
void updateBullets() {
	static int count = 0;
	if (++count < 2 )return;
	count = 0;

	int countMax = sizeof(bullets) / sizeof(bullets[0]);  //定义子弹池的最大数量
	//遍历每个子弹，更新位置，如果超出边界，子弹就回收
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

//豌豆子弹与僵尸的碰撞
void checkBullet2zm() {
	//每一颗子弹与每一个僵尸进行检测
	int bCount = sizeof(bullets) / sizeof(bullets[0]); //子弹的最大值
	int zCount = sizeof(zms) / sizeof(zms[0]);		   //僵尸的最大值
	for (int i = 0; i < bCount; i++) {
		//没使用和爆炸的子弹不用参加循环
		if (bullets[i].used == false || bullets[i].blast) continue;

		//没使用的僵尸不参与循环
		for (int k = 0; k < zCount; k++) {
			//if (zms[i].used == false) continue;
			if (zms[k].used == false) continue;
			int x1 = zms[k].x + 80;				//子弹与僵尸碰撞的左侧
			int x2 = zms[k].x + 110;			//子弹与僵尸碰撞的右侧
			int x = bullets[i].x;				//子弹的坐标
			//如果僵尸还没死亡,子弹与僵尸的行数相等，x轴坐标符合，就发生碰撞
			if (zms[k].dead == false && bullets[i].row == zms[k].row && x > x1 && x < x2) {
		//		PlaySound("res/res/splat.wav", NULL, SND_FILENAME | SND_ASYNC);   //子弹碰撞音效
				mciSendString("close res/res/splat1.mp3", 0, 0, 0);
				mciSendString("play res/res/splat1.mp3", 0, 0, 0);
				zms[k].blood -= 10;
				bullets[i].blast = true;
				bullets[i].speed = 0;
				//僵尸死亡时改变相关数据
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

//植物与僵尸的碰撞检测
void checkzm2ZhiWu() {
	int zCount = sizeof(zms) / sizeof(zms[0]);      //定义僵尸最大值
	//for循环遍历整个僵尸
	for (int i = 0; i < zCount; i++) {
		int row = zms[i].row;
		for (int k = 0; k < 9; k++) {
			if (map[row][k].type == 0) continue;	//如果这一行没有植物直接跳过循环

			//			x1   x2
			//	 植物    [   ]
			//	 僵尸    	[    ]
			//				x3   x4
			int zhiWuX = 256-112 + k * 81;
			int x1 = zhiWuX + 10;
			int x2 = zhiWuX + 60;
			int x3 = zms[i].x + 80;
			int x4 = zms[i].x + 135;
			//如果僵尸x3>x1而且x3<x2，说明僵尸可以开始吃了
			if (((x3 > x1 && x3 < x2 ) || (x4 > x1 && x4 < x2)) && zms[i].dead == false) {
				if (map[row][k].catched && zms[i].speed ==0) {
					map[row][k].deadtime++;                  //碰撞检测时植物开始死亡倒计时
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
					//僵尸碰到植物时改变相关数据
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

//实现碰撞检测
void collisionCheck() {
	checkBullet2zm();           //豌豆子弹与僵尸的碰撞检测
	checkzm2ZhiWu();			//僵尸与植物的碰撞检测

}

//渲染植物摇摆动作
void updateZhiWu() {
	static int count = 0;
	if (++count < 2*2)return;
	count = 0;
	// 双重循环渲染植物摇摆动作
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type > 0) {                 //判断有没有植物
				map[i][j].frameIndex++;				  //每次循环增加一次*imgZhiWu指针数值
				int zhiWuType = map[i][j].type - 1;
				int index = map[i][j].frameIndex;
				if (imgZhiWu[zhiWuType][index] == NULL) {/*判断指针是否指向空指针，如果是空指针就不加了 让渲染帧重新开始*/
					map[i][j].frameIndex = 0;
				}
			}
		}
	}
}

//判断植物是否能种植
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

//改变游戏相关数据的函数，更新游戏数据状态
void updateGame() {
	
	updateZhiWu();		//渲染植物摇摆动作

	createSunshine();   //创建阳光的函数
	updateSunshine();	//更新阳光状态的函数

	createZM();         //创建僵尸的函数
	updateZM();			//更新僵尸状态的函数

	shoot();			//发射豌豆子弹
	updateBullets();	//更新豌豆子弹的位置

	collisionCheck();   //碰撞检测的函数 
	updatePlantsState();//判断植物能否种植
}

//开始菜单界面UI
void starUI() {
	IMAGE imgBg, imgMenu1, imgMenu2;							//定义一个背景图片的变量
	loadimage(&imgBg, "res/res/menu.png");					    //加载图片
	loadimage(&imgMenu1, "res/res/menu1.png");
	loadimage(&imgMenu2, "res/res/menu2.png");

	int flag1 = 0;//判断鼠标是否按下的变量
	int flag2 = 0;//判断鼠标是否悬浮在图标上的变量
	while (1) {
		
		BeginBatchDraw();									//开始双缓冲
		putimage(0, 0, &imgBg);
		putimagePNG(474, 75, flag1 == 1 ? &imgMenu2 : &imgMenu1);  //三目运算符如果鼠标按下则为高亮，否则则为暗色
		putimagePNG(474, 75, flag2 == 1 ? &imgMenu2 : &imgMenu1);  //如果鼠标悬浮则为高亮，否则为暗色

		ExMessage msg;
		if (peekmessage(&msg)) {
			if (msg.message == WM_MOUSEMOVE && 			   //鼠标悬浮在菜单上时，flag2=1就加载高亮图片
				msg.x > 474 && msg.x < 474 + 300 &&
				msg.y > 75 && msg.y < 75 + 140) {
				flag2 = 1;
			}
			if (msg.message == WM_MOUSEMOVE && 			   //鼠标不悬浮在菜单上时，flag2=0就不加载高亮图片
				msg.x < 474 || msg.x  > 474 + 300 &&
				msg.y <  75 || msg.y  >  75 + 140) {
				flag2 = 0;
			}

			if (msg.message == WM_LBUTTONDOWN &&			//鼠标按下操作，flag1 = 1；
				msg.x > 474 && msg.x < 474 + 300 &&
				msg.y > 75 && msg.y < 75 + 140) {
				flag1 = 1;								 
			}
			else if (msg.message == WM_LBUTTONUP && flag1) {
				EndBatchDraw();
				break ;
			}
		}
		EndBatchDraw();											 //结束双缓冲	
	}
}

//字幕ready
void playReady(){
	BeginBatchDraw();
	for (int i = 0; i < 1000; i++) {
		putimagePNG(350, 250, &imgReady);
	}
	EndBatchDraw();
}

//字幕set
void playSet() {
	BeginBatchDraw();
	for (int i = 0; i < 1000; i++) {
		putimagePNG(354, 240, &imgSet);
	}
	EndBatchDraw();
}

//字幕plants
void playSetplants() {
	BeginBatchDraw();
	for (int i = 0; i < 1000; i++) {
		putimagePNG(330, 230, &imgSetplans);
	}
	EndBatchDraw();
}

//转场后的倒计时
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

//转场动画
void viewScence() {
	int xMin = WIN_WIDTH - imgBg.getwidth();  //900-1400=-500
	//转场马路上僵尸坐标
	vector2 points[9] = {
		{550, 80},{530,160},{630,170},
		{530,200},{515,270},{565,370},
		{605,340},{705,280},{690,340}
	};
	int index[9];  
	//初始化僵尸最初帧数
	for (int i = 0; i < 9; i++) {
		index[i] = rand() % 11;
	}

	//计时器减慢渲染速度
	static int count = 0;
	for (int x = 0; x >= xMin; x -= 3) {
		BeginBatchDraw();  //开始帧缓冲
		putimage(x, 0, &imgBg);
		count++;
		for (int k = 0; k < 9; k++) {
			//加载图片
			putimagePNG(points[k].x - xMin + x,  
				points[k].y,
				&imgZmStand[index[k]]);
			if (count >= 10) {
				index[k] = (index[k] + 1) % 11;
				}
		}
		if (count >= 10)count = 0;

		EndBatchDraw();    //结束帧缓冲
		Sleep(5);
	}

	//停留5s左右
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

	//画面往回移动
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

//工具栏的下降
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

//判断游戏是否结束
bool checkOver() {
	int ret = false;
	if (gameStatus == WIN) {
		Sleep(2000);

		mciSendString("close res/res/audio/bg.mp3", 0, 0, 0);//暂停背景音乐
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
	gameInit();			      //初始化

	starUI();				  //初始菜单界面

	viewScence();             //转场动画

	Countdown();			  //转场后倒计时

	barsDown();				  //工具栏的下降

	mciSendString("play res/res/audio/bg.mp3", 0, 0, 0);//播放背景音乐
	int timer = 0;            //进行记时的计时器
	int flag = true;		  //标记变量
	while (1) {
		userClick();          //用户点击操作
		timer += getDelay();  //getDelay获取延时时间的函数
		if (timer > 10) {
			flag = true;	  //标记为真说明进行一次渲染刷新
			timer = 0;        //延迟大于20ms计时器更新为0
		}

		if (flag) {
			flag = false;
			updateWindow();      //渲染函数
			updateGame();	     //改变游戏相关的数据

			if(checkOver()) break;		//表示游戏是否结束
		}

	}
	system("pause");
	return 0;
}