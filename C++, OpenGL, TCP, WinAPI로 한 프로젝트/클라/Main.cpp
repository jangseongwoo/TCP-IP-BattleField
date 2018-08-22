#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include "global.h"
#include "object.h"
#include "Map.h"
#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define WIDTH 800 
#define HEIGHT 600
#define EMY_NUM 8




struct enemy
{
	OBJECT EMEMY[EMY_NUM];
};
char serverip[15];
int curr_num = 7;
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
const GLfloat CUBE_SCALE = 10.0f;
OBJECT player;
OBJECT player2;
OBJECT EMEMY[EMY_NUM];
OBJECT button[4];
OBJECT ball[5];
OBJECT ball2[5];
DWORD WINAPI ClientMain(LPVOID arg);
DWORD WINAPI enemyThread(LPVOID arg);
GLfloat rect[][3] = {
	{ -CUBE_SCALE, -CUBE_SCALE, CUBE_SCALE }, // 0
	{ -CUBE_SCALE, CUBE_SCALE, CUBE_SCALE }, // 1
	{ CUBE_SCALE, CUBE_SCALE, CUBE_SCALE },  // 2
	{ CUBE_SCALE, -CUBE_SCALE, CUBE_SCALE }, // 3
	{ -CUBE_SCALE, -CUBE_SCALE, -CUBE_SCALE }, // 4
	{ -CUBE_SCALE, CUBE_SCALE, -CUBE_SCALE }, // 5
	{ CUBE_SCALE, CUBE_SCALE, -CUBE_SCALE }, // 6
	{ CUBE_SCALE, -CUBE_SCALE, -CUBE_SCALE }, // 7
};

struct SNOW
{
	float xPos;
	float yPos;
	float zPos;
};
SNOW snow[200];

struct loginPacket{
	char id[50];
	char pw[10];
};
loginPacket login;
SOCKADDR_IN serveraddr;
int addrlen;
GLvoid Init();
GLvoid DrawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid JoyStick(int value);
GLvoid AITimer(int value);
GLvoid Attack(int value);
GLvoid TimerFunction(int value);
//void SpecialKeyboard(int key, int x, int y);
void err_quit(char *msg);
void err_display(char *msg);
CRITICAL_SECTION cs;
int main()
{
	
	
	CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Points Drawing");
	Init();
	glutReshapeFunc(Reshape); // 다시 그리기 콜백 함수
	glutDisplayFunc(DrawScene); // 출력 콜백 함수
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백 함수
	//glutSpecialFunc(SpecialKeyboard);
	InitializeCriticalSection(&cs);
	glutTimerFunc(50, TimerFunction, 1); // 타이머 콜백 함수
	glutTimerFunc(50, JoyStick, 2); // 키입력부분
	//glutTimerFunc(50, AITimer, 3);  //AI  FSM
	glutTimerFunc(50, Attack, 4);  //AI  FSM
	glutMainLoop(); // 이벤트 루프 실행하기
	DeleteCriticalSection(&cs);
	return 0;
}

GLvoid Init()
{
	addrlen = sizeof(serveraddr);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST); //깊이테스트 활성화
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE); //뒷면은 계산안함
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //배경을 검은색으로 초기화

	player.Init_Object(0, CUBE_SCALE * 3, 0, 0, rect);
	player2.Init_Object(0, CUBE_SCALE * 3, 0, 0, rect);

	button[0].Init_Object(1000 + 100, CUBE_SCALE * 3, 0, 1, rect);
	button[1].Init_Object(2000 - 100, CUBE_SCALE * 3, 0, 2, rect);
	button[2].Init_Object(0, CUBE_SCALE * 3, 50, 0, rect);
	button[3].Init_Object(0, CUBE_SCALE * 3, 0, 0, rect);
	for (int i = 0; i < 5; ++i) {
		ball[i].Init_Object(2000 + 100 - i * 40, CUBE_SCALE * 3, 0, 2, rect);
		ball2[i].Init_Object(2000 + 100 - i * 40, CUBE_SCALE * 3, 0, 2, rect);
	}
	for (int i = 0; i < 200; ++i) {
		snow[i].xPos = -1300 + rand() % 600;
		snow[i].zPos = -300 + rand() % 600;
		snow[i].yPos = rand() % 200;
	}
	EMEMY[0].Init_Object(-100, CUBE_SCALE * 3, 0, 0, rect);
	EMEMY[1].Init_Object(100, CUBE_SCALE * 3, 0, 0, rect);
	EMEMY[2].Init_Object(0, CUBE_SCALE * 3, 100, 0, rect);
	for (int i = 3; i < 7; ++i){
		EMEMY[i].Init_Object(1100 - rand() % 200, CUBE_SCALE * 3, i + 30, 1, rect);
	}
	//EMEMY[1].Init_Object(-100,CUBE_SCALE*3,-100,0,rect);


}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, w / h, 50.0f, 3000.0f);
	//gluLookAt (0, 150, -300, 0.0, 0.0, 0.0, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

GLvoid DrawScene()
{
	int len = 0;
	char text[] = "Player1";
	char text2[] = "Player2";
	Start_Clock();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	glPushMatrix();

	player.Render(0, 0, 0);
	player2.Render(0, 0, 0);
	glRasterPos3f(player.xPos + 20, player.yPos + 25, player.zPos);
	len = (int)strlen(text);
	for (int j = 0; j < len; j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[j]);

	glRasterPos3f(player2.xPos + 20, player2.yPos + 25, player2.zPos);
	len = (int)strlen(text2);
	for (int j = 0; j < len; j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text2[j]);



	button[0].Render(0, 0, 1);
	button[1].Render(0, 0, 1);
	if (button[0].state == STATE_DEAD && button[1].state == STATE_DEAD) {
		button[2].Render(0, 0, 1);
		button[3].Render(0, 0, 1);
	}
	for (int i = 0; i < 5; ++i) {
		ball[i].Render(0, 0, 2);
		ball2[i].Render(0, 0, 2);
	}
	if (player.mapPos == 3) {
		for (int i = 0; i < 200; ++i) {
			glPushMatrix();
			glColor3f(1, 1, 1);
			glTranslatef(snow[i].xPos, snow[i].yPos, snow[i].zPos);
			glutSolidSphere(1, 10, 10);
			glPopMatrix();
		}
	}
	for (int i = 0; i < curr_num; ++i)
		EMEMY[i].Render(0, 0, 0);
	DrawMap();
	DrawMap_2();
	DrawDoor();
	glPopMatrix();

	glLoadIdentity();


	gluLookAt(player.xPos, 100, -200, player.xPos, 0, 0, 0, 1, 0);
	//glPushMatrix();
	//glTranslated(player.xPos, player.yPos+5, player.zPos);
	//glutBitmapCharacter(GLUT_BITMAP_8_BY_13, printfNumber);
	//glPopMatrix();



	glutSwapBuffers();
	Wait_Clock(60);
}
int great = -1;
extern int OpenDoor1[3];
int atk = 1;
GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{

	case 'a':
		//player.rot_ani++;
		great *= -1;
		break;
	case 'z':
		if (player.Attack_Time != 0 ||
			(player.RobotLeftPunch == TRUE || player.RobotRightPunch == TRUE
			|| player.RobotLeftPunch2 == TRUE) || player.time_num != 0)
			return;
		if (player.RobotFreeze == TRUE)
			return;


		printf("%d\n", player.combo);
		//   playerMotion.SetAttack(TRUE);
		switch (player.combo)
		{
		case 0:
		case 1:

			player.SetAtkRect();
			player.RobotLeftPunch = TRUE;
			player.Attack_Time = 1;

			break;
		case 2:

			player.SetAtkRect();
			player.RobotRightPunch = TRUE;
			player.Attack_Time = 1;

			break;
		case 3:

			player.SetAtkRect();
			player.RobotLeftPunch2 = TRUE;
			player.Attack_Time = 1;

			break;
		}



		break;
	case 'c':
		//player.SetAtkRect();
		player.RobotRightPunch2 = TRUE;

		//player.Attack_Time=1;
		for (int i = 0; i < curr_num; ++i) {
			int lenX = player.xPos - EMEMY[i].xPos;
			int lenZ = player.zPos - EMEMY[i].zPos;
			int len = lenX*lenX + lenZ*lenZ;
			if (len < 900) {
				EMEMY[i].xPos += (EMEMY[i].xPos - player.xPos) * 3;
				EMEMY[i].zPos += (EMEMY[i].zPos - player.zPos) * 3;
				EMEMY[i].HP -= 10;
			}
		}
		if (player.RobotRightPunch2 == FALSE)
			player.rot_ani = 0;


		player2.RobotRightPunch2 = TRUE;

		//player.Attack_Time=1;
		for (int i = 0; i < curr_num; ++i) {
			int lenX = player2.xPos - EMEMY[i].xPos;
			int lenZ = player2.zPos - EMEMY[i].zPos;
			int len = lenX*lenX + lenZ*lenZ;
			if (len < 900) {
				EMEMY[i].xPos += (EMEMY[i].xPos - player2.xPos) * 3;
				EMEMY[i].zPos += (EMEMY[i].zPos - player2.zPos) * 3;
				EMEMY[i].HP -= 10;
			}
		}
		if (player2.RobotRightPunch2 == FALSE)
			player2.rot_ani = 0;


		break;
	case 'q':
		atk = 15;
		break;
	case 27:
		exit(0);
		break;

	}
	glutPostRedisplay(); // 화면 재출력을 위하여 디스플레이 콜백 함수 호출
}

GLvoid Attack(int value)
{
	player.Attack_Timer();
	player2.Attack_Timer();
	for (int i = 0; i < curr_num; ++i) {
		if (EMEMY[i].state == STATE_DEAD)
			continue;
		EMEMY[i].Attack_Timer();
		if (CrashCheck(player, EMEMY[i]) == true || CrashCheck(player2, EMEMY[i]) == true) {
			if (EMEMY[i].RobotFreeze != TRUE) {
				//EMEMY[i].Init_Animation();

				EMEMY[i].RobotFreeze = TRUE;
				EMEMY[i].xPos += (player.Attack_xPos - player.xPos);
				EMEMY[i].zPos += (player.Attack_zPos - player.zPos);
				EMEMY[i].xPos += (player2.Attack_xPos - player2.xPos);
				EMEMY[i].zPos += (player2.Attack_zPos - player2.zPos);


			}
			if (player.RobotLeftPunch2 == TRUE || player2.RobotLeftPunch2 == TRUE)
				EMEMY[i].HP -= atk * 3;
			EMEMY[i].HP -= atk;
		}
		if ((CrashCheck(EMEMY[i], player) == true || CrashCheck(EMEMY[i], player2) == true) && great == -1) {
			player.HP -= 1;
		}
	}
	for (int i = 0; i < 2; ++i) {
		if (button[i].state == STATE_DEAD)
			continue;
		if (CrashCheck(player, button[i]) == true || CrashCheck(player2, button[i]) == true) {
			button[i].state = STATE_DEAD;
	
		}
	}
	if (button[0].state == STATE_DEAD && button[1].state == STATE_DEAD) {

		if (CrashCheck(player, button[2]) == true || CrashCheck(player2, button[2]) == true)
			button[2].state = STATE_DEAD;
		if (button[3].state != STATE_DEAD){
			if (CrashCheck(player, button[3]) == true || CrashCheck(player2, button[3]) == true) {
				player.HP = 10;
				player2.HP = 10;
				button[3].state = STATE_DEAD;
			}
		}
	}
	for (int i = 0; i < 5; ++i) {
		if (CrashCheck2(player, ball[i]) == true || CrashCheck2(player2, ball[i]) == true) {
			player.HP -= 1;
			player2.HP -= 1;
			printf("%d\n", player.HP);
		}
		if (CrashCheck2(player, ball2[i]) == true || CrashCheck2(player2, ball2[i]) == true) {
			player.HP -= 1;
			printf("%d\n", player.HP);
		}
	}
	glutPostRedisplay();
	glutTimerFunc(50, Attack, 4);
}
int num = 0;

GLvoid JoyStick(int value)
{

	GLfloat _vx = 0, _vy = 0, _vz = 0;
	player.RobotWalk = FALSE;



	if ((KEY_DOWN(VK_UP)) || (KEY_DOWN(VK_DOWN)) || (KEY_DOWN(VK_LEFT)) || (KEY_DOWN(VK_RIGHT))) {


		player.double_rot = 0;
		player.SetAtkRect();
		printf("client1!!!\n");


	}
	if (KEY_DOWN(VK_UP)) {


		player.RobotWalk = TRUE;
		_vz = 5;
		player.double_rot += 1;


	}
	if (KEY_DOWN(VK_DOWN)) {

		player.RobotWalk = TRUE;
		_vz = -5;
		player.double_rot += 2;


	}
	if (KEY_DOWN(VK_LEFT)) {

		player.RobotWalk = TRUE;
		_vx = 5;
		player.double_rot += 4;


	}
	if (KEY_DOWN(VK_RIGHT)) {


		player.RobotWalk = TRUE;
		_vx = -5;
		player.double_rot += 8;


	}
	if (KEY_DOWN(VK_CONTROL)) {


		if (player.jump == 0)
			player.jump = 1;
	}
	player.SetVal(_vx, _vy, _vz);
	Player_Crash(&player, EMEMY, EMY_NUM);
	Player_Crash(&player2, EMEMY, EMY_NUM);
	player.JUMP();



	glutPostRedisplay();
	glutTimerFunc(50, JoyStick, 2);
}

GLvoid AITimer(int value)
{
	for (int i = 0; i < curr_num; ++i) {
		if (EMEMY[i].HP < 0) {
			if (EMEMY[i].state != STATE_DEAD) {
				EMEMY[i].state = STATE_DEAD;
				player.Init_Animation();
				player2.Init_Animation();
			}
			continue;
		}
		AI_FSM(player, &EMEMY[i]);
		AI_FSM(player2, &EMEMY[i]);
	}
	AI_Crash(EMEMY, curr_num);

	glutPostRedisplay();
	glutTimerFunc(50, AITimer, 3);
}
extern GLfloat b_angle;
extern GLfloat Map_Color;
int num_temp = 0;
int timett = 0;
GLvoid CreateEmemy()
{
	timett += 1;
	if (timett % 50 == 0) {
		EMEMY[curr_num].Init_Object(-1000, 10 * 3, 10 + rand() % 260, 3, rect);
	/*	if (curr_num < 20)
			curr_num += 1;*/
		for (int i = 0; i < 20; ++i) {
			if (EMEMY[i].state != STATE_DEAD)
				return;
			EMEMY[i].Init_Object(-1000, 10 * 3, 150, 3, rect);
		}
		for (int i = 0; i < 5; ++i)
			curr_num = 5;
		timett = 0;
	}
}
GLvoid TimerFunction(int value)
{
	MapAni();
	/*if(timett>0)
	timett+=1;
	if(timett>=10)
	timett=0;*/
	b_angle += 10.0f;
	if (player.mapPos == 3) {
		for (int i = 0; i < 200; ++i) {
			if (snow[i].yPos < 0) {
				snow[i].yPos = 200;
				num_temp += 1;
			}
			snow[i].yPos -= 10;
		}
		if (num_temp % 1000 == 0)
			Map_Color += 0.1f;
	//	CreateEmemy();
	}

	
	glutPostRedisplay(); // 화면 재출력을 위하여 디스플레이 콜백 함수 호
	glutTimerFunc(50, TimerFunction, 1);
}


DWORD WINAPI ClientMain(LPVOID arg){
	int retval;
	char type = 1;
	HANDLE hThread;
	enemy enemyPacket;
	double start;
	double end;
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	//udp

	printf("접속하려는 서버 ip를 입력해주세요 \n");
	scanf("%s", serverip);
	printf("접속하려는 서버 ip :%s \n", serverip);
	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(serverip);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	char check[14];
	printf("login을 위한 id와 pw를 입력해주세요 \n");
	scanf("%s", login.id);
	scanf("%s", login.pw);
	retval = send(sock, (char*)&login, sizeof(login), 0);
	if (retval == SOCKET_ERROR){
		err_display("send()");
	}
	retval = recv(sock, check, sizeof(check), 0);
	if (retval == SOCKET_ERROR){
		err_display("recv()");
	}
	printf("%s", check);




	start = GetTickCount();
	end = GetTickCount();
	//   OBJECT p1;
	//   OBJECT p2;

	//hThread = CreateThread(NULL, 0, PlayerThread, (LPVOID)sock, 0, NULL);



	while (1){
		//EnterCriticalSection(&cs);





		retval = send(sock, (char*)&player, sizeof(player), 0);
		if (retval == SOCKET_ERROR){
			err_display("sendPlayer()");
		}





		retval = recv(sock, (char*)&player2, sizeof(player2), 0);
		if (retval == SOCKET_ERROR)
			err_display("recv2()");



		retval = recv(sock, (char*)&enemyPacket, sizeof(enemyPacket), 0);
		if (retval == SOCKET_ERROR)
			err_display("recv3()");

		//CreateThread(NULL, 0, enemyThread, (LPVOID)sock, 0, NULL);
		//i printf("player(%d)의 좌표 : %f , %f , %f\n", clientNumber, p1.xPos, p1.yPos, p1.zPos);
		//printf("player(%d)의 좌표 : %f , %f , %f\n", clientNumber, player.xPos, player.yPos, player.zPos);
		//   printf("player(2)의 좌표 : %f , %f , %f\n", clientNumber, player2.xPos, player2.yPos, player2.zPos);


		//   LeaveCriticalSection(&cs);
	//	EnterCriticalSection(&cs);
		for (int i = 0; i < EMY_NUM; i++)
		{
			EMEMY[i] = enemyPacket.EMEMY[i];
		}
	//	LeaveCriticalSection(&cs);
		//Sleep(320);
	}
	
	closesocket(sock);
	return 0;

}

DWORD WINAPI enemyThread(LPVOID arg)
{

	int retval;
	SOCKET enemy_sock = (SOCKET)arg;
	for (int i = 0; i < curr_num; i++)
	{
		retval = recv(enemy_sock, (char*)&EMEMY[i], sizeof(EMEMY[i]), 0);
		if (retval == SOCKET_ERROR){
			err_display("send ememy()");
		}
	}

	return 0;

}

void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
