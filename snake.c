#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define ROW 22 //游戏区行数
#define COL 42 //游戏区列数

#define KONG 0 //标记空（什么也没有）
#define WALL 1 //标记墙
#define FOOD 2 //标记食物
#define HEAD 3 //标记蛇头
#define BODY 4 //标记蛇身

#define UP 72 //方向键：上
#define DOWN 80 //方向键：下
#define LEFT 75 //方向键：左
#define RIGHT 77 //方向键：右
#define SPACE 32 //暂停
#define ESC 27 //退出
#define SPDUP1 49
#define SPDDOWN2 50

int speedFactor = 100;  // 速度系数（百分比），100为正常速度
int baseScore = 10;     // 基础得分
const int BASE_INTERVAL = 6000; // 基准时间间隔（原3000的2倍）

//蛇头
struct Snake
{
	int len; //记录蛇身长度
	int x; //蛇头横坐标
	int y; //蛇头纵坐标
}snake;

//蛇身
struct Body
{
	int x; //蛇身横坐标
	int y; //蛇身纵坐标
}body[ROW * COL]; //开辟足以存储蛇身的结构体数组

int face[ROW][COL]; //标记游戏区各个位置的状态

int max, grade; //全局变量

// 函数声明
void WelcomeInterface();      // 欢迎界面
void GameInstructions();      // 游戏说明界面
void HideCursor();            // 隐藏光标
void CursorJump(int x, int y); // 光标跳转
void InitInterface();         // 初始化界面
void color(int c);            // 颜色设置
void ReadGrade();             // 从文件读取最高分
void WriteGrade();            // 更新最高分到文件
void InitSnake();             // 初始化蛇
void RandFood();              // 随机生成食物
void JudgeFunc(int x, int y); // 判断得分与结束
void DrawSnake(int flag);     // 打印蛇与覆盖蛇
void MoveSnake(int x, int y); // 移动蛇
void run(int x, int y);       // 执行按键
void Game();                  // 游戏主体逻辑函数
void speedup();				  // 加速函数
void speeddown();			  // 减速函数



int main()
{
#pragma warning (disable:4996) //消除警告
	max = 0, grade = 0; //初始化变量
	speedFactor = 100;   // 重置速度系数
	baseScore = 10;      // 重置基础得分
	// 显示欢迎界面
	WelcomeInterface();

	return 0;
}

// 隐藏光标
void HideCursor()
{
	CONSOLE_CURSOR_INFO curInfo; //定义光标信息的结构体变量
	curInfo.dwSize = 1; //如果没赋值的话，光标隐藏无效
	curInfo.bVisible = FALSE; //将光标设置为不可见
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //获取控制台句柄
	SetConsoleCursorInfo(handle, &curInfo); //设置光标信息
}

// 光标跳转
void CursorJump(int x, int y)
{
	COORD pos; //定义光标位置的结构体变量
	pos.X = x; //横坐标
	pos.Y = y; //纵坐标
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //获取控制台句柄
	SetConsoleCursorPosition(handle, pos); //设置光标位置
}

// 欢迎界面
void WelcomeInterface()
{
	system("title 贪吃蛇 - 欢迎界面");
	system("mode con cols=84 lines=25");
	HideCursor();

	// 设置颜色为青色
	color(11);

	// 绘制蛇形字符画
	const char* snakeArt[] = {
    "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■",
    "■  ■  ■  ■",
    "■  我真的有点服了■■■■■■■ ■■■■■■■■■■■■■■  ■",
    "■  这个东西怎么画■  ■  ■  ■  ■  ■  ■",
    "■  who can tell meee■■■■■■■ ■  ■  ■  ■",
    "■  总之这是■  ■■■■■■  ■",
    "■  蛇形字符画.jpg■  ■  ■  ■  ■  ■",
    "■  程序设计我讨厌你■  ■  ■  ■  ■",
    "■  ■■■■■■■■  ■■■■■■■■■  ■",
    "■  ■",
    "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■"
};

	// 显示蛇形字符画
	for (int i = 0; i < 11; i++) {
		CursorJump(10, i + 3);
		printf("%s", snakeArt[i]);
	}

	// 设置颜色为黄色，显示游戏标题
	color(14);
	CursorJump(28, 15);
	printf("==============================");
	CursorJump(28, 16);
	printf("      经 典 贪 吃 蛇        ");
	CursorJump(28, 17);
	printf("==============================");

	// 设置颜色为白色，显示菜单选项
	color(7);
	CursorJump(32, 20);
	printf("1. 开始游戏");
	CursorJump(32, 21);
	printf("2. 游戏说明");
	CursorJump(32, 22);
	printf("3. 退出游戏");
	CursorJump(32, 24);
	printf("请选择(1-3): ");

	// 读取最高分
	ReadGrade();

	// 处理用户输入
	char choice = getch();
	switch (choice) {
	case '1':
		system("cls");
		system("title 贪吃蛇");
		system("mode con cols=84 lines=23");
		InitInterface();
		InitSnake();
		srand((unsigned int)time(NULL));
		RandFood();
		DrawSnake(1);
		Game();
		break;
	case '2':
		system("cls");
		GameInstructions();
		break;
	case '3':
		system("cls");
		CursorJump(30, 10);
		printf("感谢使用，再见！");
		Sleep(1500);
		exit(0);
	default:
		CursorJump(32, 24);
		printf("选择无效，请重新选择(1-3): ");
		Sleep(1000);
		system("cls");
		WelcomeInterface();
		break;
	}
}

// 游戏说明界面
void GameInstructions()
{
	color(11);
	CursorJump(30, 3);
	printf("==============================");
	CursorJump(30, 4);
	printf("        游 戏 说 明        ");
	CursorJump(30, 5);
	printf("==============================");

	color(7);
	CursorJump(25, 7);
	printf("1. 游戏目标: 控制蛇吃到食物，使蛇身变长并获得分数");
	CursorJump(25, 8);
	printf("2. 操作说明:");
	CursorJump(28, 9);
	printf("   方向键(↑/↓/←/→): 控制蛇的移动方向");
	CursorJump(28, 10);
	printf("   空格键: 暂停游戏");
	CursorJump(28, 11);
	printf("   ESC键: 退出游戏");
	CursorJump(28, 12);
	printf("   1键: 加速前进(得分+2)");
	CursorJump(28, 13);
	printf("   2键: 减速前进(得分-2)");

	CursorJump(25, 15);
	printf("3. 游戏规则:");
	CursorJump(28, 16);
	printf("   - 蛇吃到食物后长度增加，基础得分10分");
	CursorJump(28, 17);
	printf("   - 速度越快得分越高，速度越慢得分越低");
	CursorJump(28, 18);
	printf("   - 蛇碰到墙壁或自己的身体时游戏结束");
	CursorJump(28, 19);
	printf("   - 游戏会记录历史最高得分");
	CursorJump(28, 20);
	printf("   - 速度范围: 40%%~250%%，得分范围: 4~20分");

	CursorJump(25, 22);
	printf("按任意键返回主菜单...");
	getch();
	system("cls");
	WelcomeInterface();
}
// 初始化界面
void InitInterface()
{
	color(25); //颜色设置为蓝色
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (j == 0 || j == COL - 1)
			{
				face[i][j] = WALL; //标记该位置为墙
				CursorJump(2 * j, i);
				printf("■");
			}
			else if (i == 0 || i == ROW - 1)
			{
				face[i][j] = WALL; //标记该位置为墙
				CursorJump(2 * j, i);
				printf("■");
			}
			else
			{
				face[i][j] = KONG; //标记该位置为空
			}
		}
	}
	color(7); //颜色设置为白色
	CursorJump(0, ROW);
	printf("当前得分:%d", grade);
	CursorJump(COL, ROW);
	printf("历史最高得分:%d", max);
	CursorJump(COL, ROW + 1);  // 在得分下方新增一行
	printf("当前速度: 100%%  (1加速, 2减速)");
}

// 颜色设置
void color(int c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); //颜色设置
	//注：SetConsoleTextAttribute是一个API（应用程序编程接口）
}

// 从文件读取最高分
void ReadGrade()
{
	FILE* pf = fopen("贪吃蛇最高得分记录.txt", "r"); //以只读的方式打开文件
	if (pf == NULL) //打开文件失败
	{
		pf = fopen("贪吃蛇最高得分记录.txt", "w"); //以只写的方式打开文件
		fwrite(&max, sizeof(int), 1, pf); //将max写入文件（此时max为0），即将最高得分初始化为0
	}
	fseek(pf, 0, SEEK_SET); //使文件指针pf指向文件开头
	fread(&max, sizeof(int), 1, pf); //读取文件当中的最高得分到max当中
	fclose(pf); //关闭文件
	pf = NULL; //文件指针及时置空
}

// 更新最高分到文件
void WriteGrade()
{
	FILE* pf = fopen("贪吃蛇最高得分记录.txt", "w"); //以只写的方式打开文件
	if (pf == NULL) //打开文件失败
	{
		printf("保存最高得分记录失败\n");
		exit(0);
	}
	fwrite(&grade, sizeof(int), 1, pf); //将本局游戏得分写入文件当中
	fclose(pf); //关闭文件
	pf = NULL; //文件指针及时置空
}

// 初始化蛇
void InitSnake()
{
	snake.len = 2; //蛇的身体长度初始化为2
	snake.x = COL / 2; //蛇头位置的横坐标
	snake.y = ROW / 2; //蛇头位置的纵坐标
	//蛇身坐标的初始化
	body[0].x = COL / 2 - 1;
	body[0].y = ROW / 2;
	body[1].x = COL / 2 - 2;
	body[1].y = ROW / 2;
	//将蛇头和蛇身位置进行标记
	face[snake.y][snake.x] = HEAD;
	face[body[0].y][body[0].x] = BODY;
	face[body[1].y][body[1].x] = BODY;
}

// 随机生成食物
void RandFood()
{
	int i, j;
	do
	{
		//随机生成食物的横纵坐标
		i = rand() % ROW;
		j = rand() % COL;
	} while (face[i][j] != KONG); //确保生成食物的位置为空，若不为空则重新生成
	face[i][j] = FOOD; //将食物位置进行标记
	color(12); //颜色设置为红色
	CursorJump(2 * j, i); //光标跳转到生成的随机位置处
	printf("●"); //打印食物
}

// 判断得分与结束
void JudgeFunc(int x, int y)
{
	//若蛇头即将到达的位置是食物，则得分
	if (face[snake.y + y][snake.x + x] == FOOD)
	{
		snake.len++;
		grade += baseScore;  // 使用baseScore计算得分
		color(7);
		CursorJump(0, ROW);
		printf("当前得分:%d", grade);
		speedup();  // 吃到食物加速
		RandFood();
	}
	//若蛇头即将到达的位置是墙或者蛇身，则游戏结束
	else if (face[snake.y + y][snake.x + x] == WALL || face[snake.y + y][snake.x + x] == BODY)
	{
		Sleep(1000); //留给玩家反应时间
		system("cls"); //清空屏幕
		color(7); //颜色设置为白色
		CursorJump(2 * (COL / 3), ROW / 2 - 3);
		if (grade > max)
		{
			printf("创纪录啦！最高分被你刷新啦，真棒！！！最高记录更新为%d", grade);
			WriteGrade();
		}
		else if (grade == max)
		{
			printf("与最高记录持平，加油再创佳绩", grade);
		}
		else
		{
			printf("继续努力吧~你离最高分还差:%d", max - grade);
		}
		CursorJump(2 * (COL / 3), ROW / 2);
		printf("GAME OVER");
		while (1) //询问玩家是否再来一局
		{
			char ch;
			CursorJump(2 * (COL / 3), ROW / 2 + 3);
			printf("再来一局?(1/2    1：重玩，2：退出游戏):");
			scanf("%c", &ch);
			while (getchar() != '\n'); // 清除缓冲区
			if (ch == '1')
			{
				system("cls");
				main();
			}
			else if (ch == '2')
			{
				CursorJump(2 * (COL / 3), ROW / 2 + 5);
				exit(0);
			}
			else
			{
				CursorJump(2 * (COL / 3), ROW / 2 + 5);
				printf("选择错误，请再次选择");
			}
		}
	}
}


// 修改后的加速函数
void speedup() {
	if (speedFactor < 250) {  
		speedFactor += 10;   
		if (baseScore < 20) {
			baseScore += 2;  
		}    
	}
	// 更新控制台显示
	CursorJump(COL + 20, ROW);
	printf("当前速度: %d%%  (1加速, 2减速)", speedFactor);
}

// 修改后的减速函数
void speeddown() {
	if (speedFactor > 40) {   
		speedFactor -= 10;    // 每次减速10%
		if (baseScore > 5) {  // 最低得分为5
			baseScore -= 2;   // 得分-2
		}
	}
	// 更新控制台显示
	CursorJump(COL + 20, ROW);
	printf("当前速度: %d%%  (1加速, 2减速)", speedFactor);
}


// 打印蛇与覆盖蛇
void DrawSnake(int flag)
{
	if (flag == 1) //打印蛇
	{
		color(10); //颜色设置为绿色
		CursorJump(2 * snake.x, snake.y);
		printf("■"); //打印蛇头
		for (int i = 0; i < snake.len; i++)
		{
			CursorJump(2 * body[i].x, body[i].y);
			printf("□"); //打印蛇身
		}
	}
	else //覆盖蛇
	{
		if (body[snake.len - 1].x != 0) //防止len++后将(0, 0)位置的墙覆盖
		{
			//将蛇尾覆盖为空格即可
			CursorJump(2 * body[snake.len - 1].x, body[snake.len - 1].y);
			printf("  ");
		}
	}
}

// 移动蛇
void MoveSnake(int x, int y)
{
	DrawSnake(0); //先覆盖当前所显示的蛇
	face[body[snake.len - 1].y][body[snake.len - 1].x] = KONG; //蛇移动后蛇尾重新标记为空
	face[snake.y][snake.x] = BODY; //蛇移动后蛇头的位置变为蛇身
	//蛇移动后各个蛇身位置坐标需要更新
	for (int i = snake.len - 1; i > 0; i--)
	{
		body[i].x = body[i - 1].x;
		body[i].y = body[i - 1].y;
	}
	//蛇移动后蛇头位置信息变为第0个蛇身的位置信息
	body[0].x = snake.x;
	body[0].y = snake.y;
	//蛇头的位置更改
	snake.x = snake.x + x;
	snake.y = snake.y + y;
	DrawSnake(1); //打印移动后的蛇
}

// 执行按键
// 修改run函数
void run(int x, int y) {
	int t = 0;
	while (1) {
		if (t == 0)
			t = BASE_INTERVAL * 100 / speedFactor;  // 使用新基准

		while (--t) {
			if (kbhit() != 0) break;
		}

		if (t == 0) {
			JudgeFunc(x, y);
			MoveSnake(x, y);
		}
		else {
			break;
		}
	}
}
// 游戏主体逻辑函数
// 游戏主体逻辑函数（仅修改部分）
void Game()
{
	int n = RIGHT;
	int tmp = RIGHT; // 记录移动方向

	while (1)
	{
		if (_kbhit()) { // 非阻塞检测按键
			n = _getch();

			switch (n)
			{
			case UP: case DOWN: case LEFT: case RIGHT:
				tmp = n; // 更新方向
				break;

			case SPDUP1: // 按1加速
				speedup();
				break;

			case SPDDOWN2: // 按2减速
				speeddown();
				break;

			case SPACE: // 暂停
				system("pause>nul");
				break;

			case ESC: // 退出
				exit(0);
			}
		}

		// 持续移动（关键修改：不等待新按键）
		switch (tmp) {
		case UP:    run(0, -1); break;
		case DOWN:  run(0, 1);  break;
		case LEFT:  run(-1, 0); break;
		case RIGHT: run(1, 0);  break;
		}
	}
}
