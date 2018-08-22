#pragma comment(lib, "ws2_32")
#include "헤더.h"
#define SERVERPORT 9000
#define BUFSIZE    512
extern GLuint texutre[];
GLfloat b_angle = 0.0f;
OBJECT EMEMY[EMY_NUM];
struct enemy
{
	OBJECT EMEMY[EMY_NUM];
};
enemy enemyPacket;
int curr_num = 7;
OBJECT button[4];
OBJECT ball[5];
OBJECT ball2[5];
int great = -1;
int atk = 1;
BOOL RectMap(GLfloat xPos, GLfloat zPos, int type);
OBJECT player;
OBJECT player2;
loginPacket login;
BOOL CheckWall(GLfloat *xPos, GLfloat *zPos, int *type, GLfloat *yPos);
DWORD WINAPI playerThread(LPVOID arg);
USHORT port1;
USHORT port2;
////함수들
void recvCrossLine(SOCKET client_sock);
char id[50] = "123";
char pw[10] = "123";
SOCKET UDPsock;
SOCKADDR_IN serveraddr;
CRITICAL_SECTION cs;
int num_temp = 0;
int timett = 0;
//타이머
GLvoid AITimer(int value);
GLvoid TimerFunction(int value);
GLvoid Attack(int value);
//충돌체크
bool CrashCheck(OBJECT obj1, OBJECT obj2);
bool CrashCheck2(OBJECT obj1, OBJECT obj2);
void ObjectCrash(OBJECT obj1, OBJECT *obj2);
void AI_Crash(OBJECT obj[], int num);
void Player_Crash(OBJECT *player, OBJECT obj[], int num);

//AI부분
void AI_FSM(OBJECT player, OBJECT *ai);
int follow(OBJECT object1, OBJECT *object2);
void AI_Attack(OBJECT *obj);
int AI_Filed(OBJECT obj1, OBJECT *obj2);
int AI_AREA(OBJECT obj1, OBJECT *obj2);

//프레임 조절
DWORD Get_Clock(void);
DWORD Start_Clock(void);
DWORD Wait_Clock(DWORD count);

//충돌체크
GLvoid ObjectCrash(OBJECT obj1, OBJECT *obj2);
bool CrashCheck(OBJECT obj1, OBJECT obj2);
bool CrashCheck2(OBJECT obj1, OBJECT obj2);


int Map_Type = 0;
int Map_Len = 150;
int Map_Width = 150;
int MapNum[4] = { 0, 1000, 2000, -1000 };
int OpenDoor1[3] = { 1, -1, -1 };
GLfloat Map_yPos[2] = { 150, 150 };
GLfloat door_rot = 0;
HANDLE Client1;
HANDLE Client2;
HANDLE EnemyUpdate;
// 소켓 함수 오류 출력 후 종료
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
void Init(){
	EMEMY[0].Init_Object(-100, CUBE_SCALE * 3, 0, 0, rect);
	EMEMY[1].Init_Object(100, CUBE_SCALE * 3, 0, 0, rect);
	EMEMY[2].Init_Object(0, CUBE_SCALE * 3, 100, 0, rect);
	for (int i = 3; i < EMY_NUM; ++i){
		EMEMY[i].Init_Object(1100 - rand() % 200, CUBE_SCALE * 3, i + 30, 1, rect);
	}

	button[0].Init_Object(1000 + 100, CUBE_SCALE * 3, 0, 1, rect);
	button[1].Init_Object(2000 - 100, CUBE_SCALE * 3, 0, 2, rect);
	button[2].Init_Object(0, CUBE_SCALE * 3, 50, 0, rect);
	button[3].Init_Object(0, CUBE_SCALE * 3, 0, 0, rect);
	for (int i = 0; i < 5; ++i) {
		ball[i].Init_Object(2000 + 100 - i * 40, CUBE_SCALE * 3, 0, 2, rect);
		ball2[i].Init_Object(2000 + 100 - i * 40, CUBE_SCALE * 3, 0, 2, rect);
	}

	player.Init_Object(0, CUBE_SCALE * 3, 0, 0, rect);
	player2.Init_Object(0, CUBE_SCALE * 3, 0, 0, rect);
	printf("init call");
}


DWORD WINAPI enemyThread(LPVOID arg)
{

	int retval;
	SOCKET enemy_sock = (SOCKET)arg;
	for (int i = 0; i < curr_num; i++)
	{
		retval = send(enemy_sock, (char*)&EMEMY[i], sizeof(EMEMY[i]), 0);
		if (retval == SOCKET_ERROR){
			err_display("send ememy()");
		}
	}

	return 0;

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
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	


	glutSwapBuffers();
	Wait_Clock(60);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;

	}
	//glutPostRedisplay(); // 화면 재출력을 위하여 디스플레이 콜백 함수 호출
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
	//glutPostRedisplay();
	glutTimerFunc(50, Attack, 2);
}

DWORD WINAPI ServerMain(LPVOID arg){

	int retval;
	char type = 0;
	HANDLE hThread;

	Client1 = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (Client1 == NULL) return 1;
	Client2 = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (Client2 == NULL) return 1;
	EnemyUpdate = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (EnemyUpdate == NULL) return 1;

	
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()

	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	////TCP
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;

	char check[14] = "login success";
	
	while (1){
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET){
			err_display("accept()");
			break;
		}
		bool optval = TRUE;
		setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (char *)&optval, sizeof(optval));
		/*
		retval = send(client_sock, (char*)&clientNumber, sizeof(clientNumber), 0);
		clientNumber++;
		*/
		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 클라이언트와 데이터 통신

		// 데이터 받기
		retval = recv(client_sock, (char*)&login, sizeof(login), 0);
		if (retval == SOCKET_ERROR){
			err_display("recv()");
		}

		// 받은 데이터 출력
		if (strcmp(login.id, id) == 0 && strcmp(login.pw, pw) == 0){


			printf("login 성공");
			retval = send(client_sock, check, sizeof(check), 0);
			if (retval == SOCKET_ERROR){
				err_display("sendCheck()");
			}

			if (port1 == 0)
				port1 = clientaddr.sin_port;
			else
			{
				port2 = clientaddr.sin_port;
			}

			hThread = CreateThread(NULL, 0, playerThread, (LPVOID)client_sock, 0, NULL);
		//	hThread1 = CreateThread(NULL, 0, enemyThread, (LPVOID)client_sock, 0, NULL);
			if (hThread == NULL){
				printf("[오류] 스레드 생성 실패!\n");
				closesocket(client_sock);
			}
			else
				CloseHandle(hThread);
		}




		//WaitForMultipleObjects(2, hThread, TRUE, INFINITE);


	}
	// closesocket()
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));


	// closesocket()
	closesocket(listen_sock);
	
	// 윈속 종료
	WSACleanup();
	return 0;
}
int main(int argc, char*argv[])
{
	
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Points Drawing");
	Init();
	glutReshapeFunc(Reshape); // 다시 그리기 콜백 함수
	glutDisplayFunc(DrawScene); // 출력 콜백 함수
	glutKeyboardFunc(Keyboard); // 키보드 입력 콜백 함수


	//InitializeCriticalSection(&cs);
	glutTimerFunc(50, AITimer, 1);  //AI  FSM
	//glutTimerFunc(50, TimerFunction, 2); //
	glutTimerFunc(50, Attack, 2);  //AI  FSM
	CreateThread(NULL, 0, ServerMain, NULL, 0, NULL);
	glutMainLoop(); // 이벤트 루프 실행하기
	//DeleteCriticalSection(&cs);
	return 0;
}


DWORD WINAPI playerThread(LPVOID arg){

	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	int addrlen;
	int retval = 0;
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);
	DWORD event1;
	DWORD event2;

	while (1){ 

	

		if (clientaddr.sin_port == port1)
		{
			event1 = WaitForSingleObject(EnemyUpdate, 1000);
			//if (event1 != WAIT_OBJECT_0) break;

			for (int i = 0; i < EMY_NUM; i++)
			{
				enemyPacket.EMEMY[i] = EMEMY[i];
				//printf("%d enemy %f %f %f\n", i, enemyPacket.EMEMY[i].xPos, enemyPacket.EMEMY[i].yPos, enemyPacket.EMEMY[i].zPos);
			}

			retval = recv(client_sock, (char*)&player, sizeof(player), 0);
			if (retval == SOCKET_ERROR){
				err_display("recvPlayer1");

			}
			retval = send(client_sock, (char*)&player2, sizeof(player2), 0);
			if (retval == SOCKET_ERROR){
				err_display("recvPlayer2()");
			}

			retval = send(client_sock, (char*)&enemyPacket, sizeof(enemyPacket), 0);
			if (retval == SOCKET_ERROR){
				err_display("send ememy()");
			}

			SetEvent(Client1);
		}

		if (clientaddr.sin_port == port2)
		{
			event2 = WaitForSingleObject(Client1, 1000);
			if (event2 != WAIT_OBJECT_0) break;

			for (int i = 0; i < EMY_NUM; i++)
			{
				enemyPacket.EMEMY[i] = EMEMY[i];
				//printf("%d enemy %f %f %f\n", i, enemyPacket.EMEMY[i].xPos, enemyPacket.EMEMY[i].yPos, enemyPacket.EMEMY[i].zPos);
			}

			retval = recv(client_sock, (char*)&player2, sizeof(player), 0);
			if (retval == SOCKET_ERROR){
				err_display("recvPlayer1");

			}
			retval = send(client_sock, (char*)&player, sizeof(player2), 0);
			if (retval == SOCKET_ERROR){
				err_display("recvPlayer2()");
			}

			retval = send(client_sock, (char*)&enemyPacket, sizeof(enemyPacket), 0);
			if (retval == SOCKET_ERROR){
				err_display("send ememy()");
			}
			SetEvent(Client2);
		}

	//		EnterCriticalSection(&cs);
		
		//LeaveCriticalSection(&cs);
		//Sleep(150);
	/*	for (int i = 0; i < EMY_NUM; i++)
		{
			
		}
		printf("player 1의 좌표 : %f , %f , %f\n", player.xPos, player.yPos, player.zPos);
		printf("player 2의 좌표 : %f , %f , %f\n", player2.xPos, player2.yPos, player2.zPos);*/


	}
	//closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;


}


GLvoid AI_Attack(OBJECT *obj)
{
	if (obj->Attack_Time != 0 ||
		obj->RobotLeftPunch == TRUE || obj->RobotRightPunch == TRUE ||
		obj->RobotLeftPunch2 == TRUE || obj->time_num != 0)
		return;
	switch (obj->combo)
	{
	case 0:
	case 1:
		if (obj->RobotLeftPunch == FALSE) {
			obj->SetAtkRect();
			obj->RobotLeftPunch = TRUE;
			obj->Attack_Time = 1;
		}
		break;
	case 2:
	case 3:
		if (obj->RobotRightPunch == FALSE) {
			obj->SetAtkRect();
			obj->RobotRightPunch = TRUE;
			obj->Attack_Time = 1;
		}
		break;
	}
}

GLint AI_Filed(OBJECT obj1, OBJECT *obj2)
{
	GLfloat d_x, d_z, len;
	d_x = obj1.xPos - obj2->xPos;
	d_z = obj1.zPos - obj2->zPos;
	len = sqrt(d_x*d_x + d_z*d_z);
	if (len<FILED)
		return 0;

	return 1;

}
GLint AI_AREA(OBJECT obj1, OBJECT *obj2)
{
	GLfloat d_x, d_z, len;
	d_x = obj1.xPos - obj2->xPos;
	d_z = obj1.zPos - obj2->zPos;
	len = sqrt(d_x*d_x + d_z*d_z);
	if (len<AREA)//AREA)
		return 1;
	return 0;
}

DWORD Get_Clock(void)
{
	return(GetTickCount());
}

DWORD start_clock_count;

DWORD Start_Clock(void)
{
	return (start_clock_count = Get_Clock());
}

DWORD Wait_Clock(DWORD count)
{
	while ((Get_Clock() - start_clock_count) <count);
	return(Get_Clock());
}

GLvoid ObjectCrash(OBJECT obj1, OBJECT *obj2)
{
	if (obj2->jump != 0 || obj1.yPos != obj2->yPos)
		return;
	GLfloat d_x, d_z, len;
	d_x = obj1.xPos - obj2->xPos;
	d_z = obj1.zPos - obj2->zPos;
	len = sqrt(d_x*d_x + d_z*d_z);
	if (len<30) {
		if (d_x>0) {
			if (obj2->v_x>0)
				obj2->v_x = 0;
		}
		else {
			if (obj2->v_x<0)
				obj2->v_x = 0;
		}
		if (d_z>0) {
			if (obj2->v_z>0)
				obj2->v_z = 0;
		}
		else {
			if (obj2->v_z<0)
				obj2->v_z = 0;
		}
	}
}

GLvoid AI_Crash(OBJECT obj[], int num)
{
	for (int cur = 0; cur<num; ++cur) {
		for (int i = 0; i<num; ++i)
		{
			if (cur == i)
				continue;
			if (obj[cur].state == STATE_DEAD)
				continue;
			ObjectCrash(obj[i], &obj[cur]);
		}
	}
}

GLint follow(OBJECT obj1, OBJECT *obj2)
{
	GLfloat d_x, d_z, v_x, v_z;
	GLfloat _rot;  //실각도
	d_x = obj1.xPos - obj2->xPos;
	d_z = obj1.zPos - obj2->zPos;


	_rot = acos(d_z / sqrt(d_x*d_x + d_z*d_z)) / PI * 180;

	if (obj1.xPos<obj2->xPos)
		_rot = 360 - _rot;


	for (int i = 1; i<4; ++i)   //각도 보정
	{
		if (_rot> 90 * i - 10 && _rot<90 * i + 10)
			_rot = 90 * i;
	}
	if (_rot<10 || _rot>350)  //각도보정
		_rot = 0;

	obj2->rot = _rot;

	if (d_x>0)
		v_x = 5;
	else if (d_x<0)
		v_x = -5;
	else
		v_x = 0;
	if (d_z>0)
		v_z = 5;
	else if (d_z<0)
		v_z = -5;
	else
		v_z = 0;

	if (AI_Filed(obj1, obj2) == 0) {
		obj2->SetVal(0, 0, 0);
		printf("AI_Filed\n");
		return 0;
	}
	else {
		obj2->SetVal(v_x, 0, v_z);
		printf("Setval v_x : %f, v_z : %f \n",v_x,v_z);
		return 1;
	}
	return 1;
}

GLvoid Player_Crash(OBJECT *player, OBJECT obj[], int num)
{
	for (int cur = 0; cur<num; ++cur)
	{
		if (obj[cur].state == STATE_DEAD)
			continue;
		ObjectCrash(obj[cur], player);
	}
}


GLvoid AI_FSM(OBJECT player, OBJECT *ai)
{
	switch (ai->state)
	{
		//printf("ai state : %d\n", ai->state);
	case STATE_NORMAL:
		if (ai->RobotWalk != FALSE)
			ai->RobotWalk = FALSE;
		if (AI_AREA(player, ai) == 1)
			ai->state = STATE_CHASE;
		break;
	case STATE_CHASE:
		if (ai->RobotWalk != TRUE)
			ai->RobotWalk = TRUE;
		ai->SetAtkRect();
		if (follow(player, ai) != 1)
			ai->state = STATE_ATTACK;
		break;
	case STATE_ATTACK:
		if (ai->RobotWalk != FALSE)
			ai->RobotWalk = FALSE;
		if (ai->RobotFreeze != TRUE)
			AI_Attack(ai);
		if (AI_Filed(player, ai) != 0)
			ai->state = STATE_CHASE;
		break;
	case STATE_DEAD:
		if (ai->RobotWalk != FALSE)
			ai->RobotWalk = FALSE;
		ai->Destroy();
		break;
	}
	
	//printf("AI_FSM Call\n");
}
GLvoid OBJECT::Init_Object(GLfloat _xPos, GLfloat _yPos, GLfloat _zPos, GLint _mapPos, GLfloat _point[8][3])
{
	xPos = _xPos;
	yPos = _yPos;
	zPos = _zPos;
	save_y = _yPos;
	Attack_xPos = xPos;
	Attack_zPos = zPos;
	jump = 0;
	combo = 0;
	mapPos = _mapPos;
	Attack_Time;
	state = STATE_NORMAL;
	for (int i = 0; i<8; ++i)
		for (int j = 0; j<3; ++j) {
		point[i][j] = _point[i][j];
		}
	double_rot = 0;
	Init_Animation();
	time_num = 0;
	HP = 10;
}

GLvoid OBJECT::Init_Animation()
{
	RobotWalk = FALSE;
	RobotLeftPunch = FALSE;
	RobotLeftPunch2 = FALSE;
	RobotRightPunch = FALSE;
	RobotRightPunch2 = FALSE;
	RobotKick = FALSE;
	RobotJump = FALSE;
	RobotFreeze = FALSE;

	rot = 0;
	FreezeTimer = 0;
	WalkCycle = 0;
	PunchCycle = 0;
	PunchCycle2 = 0;
	JumpCycle = 0;
	FreezCycle = 0;
	UpperCycle = 0;
	superCycle = 0;
	upper = 0;
	super = 0;
	step = 0;
	//RightPunchCombo = 0;
	//LeftPunchCombo = 0;

	ROBOTBODY.HeadSize = 40.0f;
	ROBOTBODY.HandSize = 20.0f;
	ROBOTBODY.HandSize2 = 20.0f;
	ROBOTBODY.BodySize = 50.0f;
	ROBOTBODY.ArmSize = 50.0f;
	ROBOTBODY.LegSize = 35.0f;
	ROBOTMOVE.Punch1Rot_LEFT = 25.0f;
	ROBOTMOVE.Punch2Rot_LEFT = 150.0f;
	ROBOTMOVE.Punch1Rot_RIGHT = 25.0f;
	ROBOTMOVE.Punch2Rot_RIGHT = 150.0f;
	ROBOTMOVE.LegRot_LEFT = -15;
	ROBOTMOVE.LegRot2_LEFT = 0;
	ROBOTMOVE.LegRot_RIGHT = 15;
	ROBOTMOVE.LegRot2_RIGHT = 30;
	ROBOTMOVE.BodyRot = 0;
	ROBOTMOVE.BodyRot2 = 0;
	ROBOTMOVE.FreezeRot = 0;
	ROBOTMOVE.FreezeSpeed = 20;
	ROBOTMOVE.WalkSpeed = 5;
	ROBOTMOVE.LeftPunchSpeed = 20;
	ROBOTMOVE.LeftPunchSpeed2 = 80;
	ROBOTMOVE.RightPunchSpeed = 20;
	ROBOTMOVE.RightPunchSpeed2 = 10;
	ROBOTMOVE.JumpSpeed = 5;
}

GLvoid OBJECT::SetPos(GLfloat _xPos, GLfloat _yPos, GLfloat _zPos, GLfloat _mapPos)
{
	xPos = _xPos;
	yPos = _yPos;
	zPos = _zPos;

	mapPos = _mapPos;
}


GLint  OBJECT::Destroy()
{
	return (1);
}

GLvoid OBJECT::SetVal(GLfloat _x, GLfloat _y, GLfloat _z)
{

	v_x = _x;

	v_y = _y;

	v_z = _z;

}

GLvoid OBJECT::SetAtkRect()
{
	Attack_xPos = xPos;
	Attack_zPos = zPos;
}

GLvoid AITimer(int value)
{
//	printf("AITimer call");
	DWORD event3;

	event3 = WaitForSingleObject(Client2, 1000);
	//if (event3 != WAIT_OBJECT_0) 

	for (int i = 0; i < curr_num; ++i) {
		if (EMEMY[i].HP < 0) {
			if (EMEMY[i].state != STATE_DEAD) {
				EMEMY[i].state = STATE_DEAD;
			}
			continue;
		}
		AI_FSM(player, &EMEMY[i]);
		AI_FSM(player2, &EMEMY[i]);
	}
	AI_Crash(EMEMY, curr_num);
	//glutPostRedisplay();
	for (int i = 0; i < curr_num; ++i)
		EMEMY[i].Render(0, 0, 0);
	SetEvent(EnemyUpdate);
	glutTimerFunc(50, AITimer, 1);
}
GLvoid TimerFunction(int value)
{
	/*
	if (player.mapPos == 3) {
		timett += 1;
		if (timett % 50 == 0) {
			EMEMY[curr_num].Init_Object(-1000, 10 * 3, 10 + rand() % 260, 3, rect);
			if (curr_num < 20)
				curr_num += 1;
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
	*/
	//printf("TimerFunction call");
	//glutPostRedisplay();
	
	glutTimerFunc(50, TimerFunction, 2);
}


GLvoid OBJECT::Attack_Timer()
{
	if (Attack_Time == 1)
	{
		if (rot == 0) {
			Attack_xPos += 0;
			Attack_zPos += 8;
		}
		else if (rot>0 & rot<90) {
			Attack_xPos += 5.6;
			Attack_zPos += 5.6;
		}
		else if (rot == 90) {
			Attack_xPos += 8;
			Attack_zPos += 0;
		}
		else if (rot>90 && rot<180) {
			Attack_xPos += 5.6;
			Attack_zPos -= 5.6;
		}
		else if (rot == 180) {
			Attack_xPos += 0;
			Attack_zPos -= 8;
		}
		else if (rot>180 && rot<270) {
			Attack_xPos -= 5.6;
			Attack_zPos -= 5.6;
		}
		else if (rot == 270) {
			Attack_xPos -= 8;
			Attack_zPos += 0;
		}
		else if (rot>270 && rot<360) {
			Attack_xPos -= 5.6;
			Attack_zPos += 5.6;
		}
		time_num += 2;
		if (time_num>4) {
			time_num = 0;
			Attack_Time = -1;
		}
	}
	else if (Attack_Time == -1)
	{
		if (rot == 0) {
			Attack_xPos -= 0;
			Attack_zPos -= 4;
		}
		else if (rot>0 & rot<90) {
			Attack_xPos -= 2.8;
			Attack_zPos -= 2.8;
		}
		else if (rot == 90) {
			Attack_xPos -= 4;
			Attack_zPos -= 0;
		}
		else if (rot>90 && rot<180) {
			Attack_xPos -= 2.8;
			Attack_zPos += 2.8;
		}
		else if (rot == 180) {
			Attack_xPos -= 0;
			Attack_zPos += 4;
		}
		else if (rot>180 && rot<270) {
			Attack_xPos += 2.8;
			Attack_zPos += 2.8;
		}
		else if (rot == 270) {
			Attack_xPos += 4;
			Attack_zPos -= 0;
		}
		else if (rot>270 && rot<360) {
			Attack_xPos += 2.8;
			Attack_zPos -= 2.8;
		}
		time_num += 2;
		if (time_num>8)
		{
			Attack_Time = 0;
			time_num = 0;
			combo += 1;
			if (combo>3)
				combo = 0;
		}
	}
}


bool CrashCheck(OBJECT obj1, OBJECT obj2)
{
	float len_x = obj1.Attack_xPos - obj2.xPos;
	float len_z = obj1.Attack_zPos - obj2.zPos;
	int len = sqrt((len_x * len_x) + (len_z * len_z));
	if (len<15)
		return true;
	return false;
}

bool CrashCheck2(OBJECT obj1, OBJECT obj2)
{
	float len_x = obj1.xPos - obj2.xPos;
	float len_z = obj1.zPos - obj2.zPos;
	int len = sqrt((len_x * len_x) + (len_z * len_z));
	if (len<30)
		return true;
	return false;
}


GLvoid OBJECT::SetRot()
{
	if (double_rot == 1)
		rot = 0;
	else if (double_rot == 2)
		rot = 180;
	else if (double_rot == 4)
		rot = 90;
	else if (double_rot == 8)
		rot = 270;
	else if (double_rot == 5)
		rot = 45;
	else if (double_rot == 9)
		rot = 315;
	else if (double_rot == 6)
		rot = 135;
	else if (double_rot == 10)
		rot = 225;
	glRotatef(rot, 0, 1, 0);
}


GLvoid OBJECT::Move()
{
	xPos += v_x;
	zPos += v_z;
	if (!RectMap(xPos, zPos, mapPos))
		v_y = -5;
	yPos += v_y;
	if (CheckWall(&xPos, &zPos, &mapPos, &yPos) == TRUE)
		glTranslatef(xPos, yPos, zPos);
	else {
		xPos -= v_x;
		zPos -= v_z;
		yPos -= v_y;
		glTranslatef(xPos, yPos, zPos);
	}
//	printf("xpos : %f", xPos);
}


GLvoid OBJECT::JUMP()
{
	if (jump == 1)
	{
		if (yPos<50)
			v_y = 5;
		else
			jump = 2;
	}
	if (jump == 2)
	{
		if (yPos>save_y)
			v_y = -5;
		else{
			v_y = 0;
			yPos = save_y;
			jump = 0;
		}
	}
}

BOOL CheckWall(GLfloat *xPos, GLfloat *zPos, int *type, GLfloat *yPos)
{
	int select = *type;
	switch (select)
	{
	case 0:
		if (*xPos>-Map_Width && *xPos<Map_Width)
			if (*zPos>-Map_Len && *zPos<Map_Len)
				return true;
		if (OpenDoor1[0] == 1) {
			if (*xPos >= Map_Width && (*zPos>-40 && *zPos<40)) {
				if (*xPos == Map_Width) {
					*type = 1;
					*xPos = (-Map_Width + MapNum[*type] + 30);
					*yPos = 10.0f * 3;
				}
				return true;
			}
		}
		if (OpenDoor1[1] == 1) {
			if (*xPos <= -Map_Width && (*zPos>-40 && *zPos<40)) {
				if (*xPos == -Map_Width) {
					*type = 2;
					*xPos = (Map_Width + MapNum[*type] - 30);
					*yPos = 10.0f * 3;
				}
				return true;
			}
		}
		if (*zPos <= -Map_Len)
			return true;
		if (door_rot >= 90) {
			if (OpenDoor1[2] == 1) {
				if (*zPos >= Map_Len && (*xPos>-40 && *xPos<40)) {
					if (*zPos == Map_Len) {
						*type = 3;
						*xPos = (MapNum[*type] + 10);
						*zPos *= -1;
						*yPos = 10.0f * 3;
					}
					return true;
				}
			}
		}
		return false;
	case 1:

		if (Map_yPos[0] <= 0) {
			if (*xPos>-Map_Width + MapNum[1] && *xPos<Map_Width + MapNum[1])
				if (*zPos>-Map_Len && *zPos<Map_Len)
					return true;
			if (*xPos >= Map_Width + MapNum[1] && (*zPos>-40 && *zPos<40)) {
				if (*xPos == Map_Width + MapNum[1]) {
					*type = 0;
					*xPos = 0;
					*yPos = 10.0f * 3;
					OpenDoor1[0] -= 1;
					OpenDoor1[1] += 1;
				}
				return true;
			}

		}
		else
			return true;
	case 2:
		if (*xPos>-Map_Width + MapNum[2] && *xPos<Map_Width + MapNum[2])
			if (*zPos>-Map_Len && *zPos<Map_Len)
				return true;
		if (Map_yPos[1] <= 0) {
			if (*xPos <= -Map_Width + MapNum[2] && (*zPos>-40 && *zPos<40)) {
				if (*xPos == -Map_Width + MapNum[2]) {
					*type = 0;
					*xPos = 0;
					*yPos = 10.0f * 3;
					OpenDoor1[1] += 1;
					OpenDoor1[2] *= -1;
				}
				return true;
			}

		}
		return false;
	case 3:
		return true;
	}
}


BOOL RectMap(GLfloat xPos, GLfloat zPos, int type)
{

	switch (type)
	{
	case 0:
		if (xPos>-Map_Width && xPos<Map_Width)
			if (zPos>-Map_Len && zPos<Map_Len)
				return true;

		return false;
	case 1:
		if (xPos>-Map_Width + MapNum[1] && xPos<Map_Width + MapNum[1])
			if (zPos>-Map_Len && zPos<Map_Len)
				return true;

		return false;
		break;
	case 2:
		if (xPos>-Map_Width + MapNum[2] && xPos<Map_Width + MapNum[2])
			if (zPos>-Map_Len && zPos<Map_Len)
				return true;
		return false;
		break;
	case 3:
		int lenX = xPos - MapNum[3];
		int lenZ = zPos - Map_Len;
		int len = lenX*lenX + lenZ*lenZ;
		if (len<(Map_Width * 2)*(Map_Width * 2))
			return true;
		return false;
		break;
	}
}


GLvoid OBJECT::Render(int rect, int atk, int shape)
{
	if (shape == 0) {
		if (state != STATE_DEAD) {

			glPushMatrix();
			Move();
		//	Draw_HP();
			if (atk == 1)
		//		DrawAtkPoint();
			SetRot();
			if (rect == 1)
		//		DrawRect();

			//Draw_Ani();
			//Animation();
			glPopMatrix();
		}
		if (yPos<-20) {
			state = STATE_DEAD;
			Destroy();
		}
	}
	else if (shape == 1) {
		glPushMatrix();
		if (state != STATE_DEAD)
			glColor3f(0, 1, 0);
		else
			glColor3f(1, 0, 0);

		Move();
		glRotatef(b_angle, 0, 1, 0);
		glutSolidCube(10);
		glPopMatrix();
	}
	else if (shape == 2) {
		glPushMatrix();
		glColor3f(1, 0, 1);
		Move();
		glRotatef(b_angle, 1, 1, 1);
		glutSolidCube(10);
		glPopMatrix();
	}


}
//
//GLvoid OBJECT::DrawRect()
//{
//	glColor3f(1, 1, 1);
//	glPointSize(5.0f);
//	glBegin(GL_POINTS);
//	for (int i = 0; i<8; ++i)
//	{
//		glVertex3f(point[i][0], point[i][1], point[i][2]);
//	}
//	glEnd();
//	glColor3f(1.0f, 0, 0);
//	glPointSize(1.0f);
//	glBegin(GL_QUADS);
//	for (int i = 0; i<4; ++i)
//	{
//		glVertex3f(point[i][0], point[i][1], point[i][2]);
//	}
//
//	for (int i = 3; i >= 0; --i)
//	{
//		glVertex3f(point[i][0], point[i][1], point[i][2]);
//	}
//	glEnd();
//}
//
//GLvoid OBJECT::DrawAtkPoint()
//{
//	glPointSize(10.0f);
//	glPushMatrix();
//	glTranslatef(Attack_xPos - xPos, 0, Attack_zPos - zPos);
//	/*glBegin(GL_POINT);
//	glVertex3f(0,0, 0);
//	glEnd();*/
//	glutSolidCube(10);
//	glPopMatrix();
//}
//
//GLvoid OBJECT::Animation()
//{
//	//걷기
//	if (RobotWalk == TRUE && RobotFreeze == FALSE)
//	{
//
//		if (ROBOTMOVE.LegRot_LEFT >= -15 && ROBOTMOVE.LegRot_LEFT <= 15){
//			ROBOTMOVE.LegRot_LEFT += ROBOTMOVE.WalkSpeed;
//			ROBOTMOVE.LegRot_RIGHT -= ROBOTMOVE.WalkSpeed;
//			ROBOTMOVE.LegRot2_LEFT += ROBOTMOVE.WalkSpeed;
//			ROBOTMOVE.LegRot2_RIGHT -= ROBOTMOVE.WalkSpeed;
//		}
//		else
//		{
//			ROBOTMOVE.WalkSpeed *= -1;
//			ROBOTMOVE.LegRot_LEFT += ROBOTMOVE.WalkSpeed;
//			ROBOTMOVE.LegRot_RIGHT += ROBOTMOVE.WalkSpeed;
//
//			WalkCycle++;
//
//		}
//		if (WalkCycle == 2){
//			RobotWalk = FALSE;
//			WalkCycle = 0;
//		}
//	}
//	//오른손 펀치
//	if (RobotRightPunch == TRUE && RobotFreeze == FALSE && RobotLeftPunch2 == FALSE)
//	{
//
//		if (ROBOTMOVE.Punch1Rot_RIGHT <= 90 && ROBOTMOVE.Punch2Rot_RIGHT >= 0 && ROBOTMOVE.Punch1Rot_RIGHT >= 25 && ROBOTMOVE.Punch2Rot_RIGHT <= 150)
//		{
//			ROBOTMOVE.Punch1Rot_RIGHT += ROBOTMOVE.RightPunchSpeed;
//			ROBOTMOVE.Punch2Rot_RIGHT -= ROBOTMOVE.RightPunchSpeed * 2;
//			ROBOTMOVE.BodyRot -= (ROBOTMOVE.RightPunchSpeed*0.2);
//			ROBOTMOVE.BodyRot2 += ROBOTMOVE.RightPunchSpeed*0.3;
//			ROBOTMOVE.LegRot2_RIGHT += ROBOTMOVE.RightPunchSpeed*0.3;
//			ROBOTBODY.HandSize2 += ROBOTMOVE.RightPunchSpeed*0.3;
//
//		}
//		else  {
//			ROBOTMOVE.RightPunchSpeed *= -1;
//			ROBOTMOVE.Punch1Rot_RIGHT += ROBOTMOVE.RightPunchSpeed;
//			ROBOTMOVE.Punch2Rot_RIGHT -= ROBOTMOVE.RightPunchSpeed * 2;
//			ROBOTMOVE.BodyRot -= (ROBOTMOVE.RightPunchSpeed*0.2);
//			ROBOTMOVE.BodyRot2 += ROBOTMOVE.RightPunchSpeed*0.5;
//			PunchCycle2++;
//		}
//
//		if (PunchCycle2 == 2){
//			RobotRightPunch = FALSE;
//			PunchCycle2 = 0;
//
//		}
//	}
//	// 왼손펀치
//	if (RobotLeftPunch == TRUE && RobotFreeze == FALSE && RobotLeftPunch2 == FALSE)
//	{
//
//
//		if (ROBOTMOVE.Punch1Rot_LEFT <= 90 && ROBOTMOVE.Punch2Rot_LEFT >= 0 && ROBOTMOVE.Punch1Rot_LEFT >= 25 && ROBOTMOVE.Punch2Rot_LEFT <= 150){
//			ROBOTMOVE.Punch1Rot_LEFT += ROBOTMOVE.LeftPunchSpeed;
//			ROBOTMOVE.Punch2Rot_LEFT -= ROBOTMOVE.LeftPunchSpeed * 2;
//			ROBOTMOVE.BodyRot += (ROBOTMOVE.LeftPunchSpeed*0.2);
//			ROBOTMOVE.BodyRot2 += ROBOTMOVE.LeftPunchSpeed*0.3;
//			ROBOTBODY.HandSize += ROBOTMOVE.LeftPunchSpeed*0.3;
//
//
//		}
//		else  {
//			ROBOTMOVE.LeftPunchSpeed *= -1;
//			ROBOTMOVE.Punch1Rot_LEFT += ROBOTMOVE.LeftPunchSpeed;
//			ROBOTMOVE.Punch2Rot_LEFT -= ROBOTMOVE.LeftPunchSpeed * 2;
//			ROBOTMOVE.BodyRot += (ROBOTMOVE.LeftPunchSpeed*0.2);
//			ROBOTMOVE.BodyRot2 += ROBOTMOVE.LeftPunchSpeed*0.3;
//			PunchCycle++;
//
//		}
//		if (PunchCycle == 2){
//			RobotLeftPunch = FALSE;
//			PunchCycle = 0;
//
//		}
//
//	}
//
//	//왼손 어퍼
//	if (RobotLeftPunch2 == TRUE && RobotFreeze == FALSE)
//	{
//
//		RobotWalk = FALSE;
//		RobotLeftPunch = FALSE;
//		RobotRightPunch = FALSE;
//		RobotRightPunch2 = FALSE;
//		RobotKick = FALSE;
//		RobotJump = FALSE;
//		//RobotJump = FALSE;       점프하면서 어퍼는 할수 있게..
//
//
//		if (upper <= 3){
//			ROBOTMOVE.Punch2Rot_LEFT -= ROBOTMOVE.LeftPunchSpeed2*0.8;
//			upper += 2;
//			ROBOTMOVE.BodyRot2 += ROBOTMOVE.LeftPunchSpeed2*0.1;
//			ROBOTBODY.HandSize += ROBOTMOVE.LeftPunchSpeed2*0.1;
//		}
//		else if (3 < upper && upper <7){
//			ROBOTMOVE.Punch1Rot_LEFT += ROBOTMOVE.LeftPunchSpeed2*0.8;
//			upper += 2;
//			ROBOTMOVE.BodyRot2 -= ROBOTMOVE.LeftPunchSpeed2*0.1;
//			ROBOTMOVE.BodyRot += (ROBOTMOVE.LeftPunchSpeed2*0.2);
//		}
//		else if (upper >= 7 && upper <10){
//			upper++;
//		}
//		else if (upper >= 10)
//		{
//			ROBOTMOVE.LeftPunchSpeed2 *= -1;
//			UpperCycle++;
//			upper = 0;
//		}
//
//		if (UpperCycle == 2)
//		{
//			RobotLeftPunch2 = FALSE;
//			UpperCycle = 0;
//		}
//
//	}
//	// 마지막 기모아서 때리기
//	if (RobotRightPunch2 == TRUE)
//	{
//		RobotWalk = FALSE;
//		RobotLeftPunch = FALSE;
//		RobotRightPunch = FALSE;
//		RobotLeftPunch2 = FALSE;
//		RobotKick = FALSE;
//		//RobotJump = FALSE;       점프하면서 어퍼는 할수 있게..
//
//		/*static float super=0;
//		if(super <= 3){
//		rot_ani+=ROBOTMOVE.RightPunchSpeed2;
//
//		super+=2;
//		}
//		else if(3 < super && super <7){
//		super+=0.5;
//		}
//		else if(super>=7 && super <10){
//		super+=2;
//		rot_ani-=ROBOTMOVE.RightPunchSpeed2*5;
//		}
//		else if(super >= 10)
//		{
//		ROBOTMOVE.RightPunchSpeed2 *= -1;
//		superCycle++;
//		super = 0;
//		}
//
//		if(superCycle == 2)
//		{
//		RobotRightPunch2 = FALSE ;
//		superCycle = 0;
//
//		}			*/
//		rot_ani += 100.0f;
//		if (rot_ani>900) {
//			rot_ani = 0;
//			RobotRightPunch2 = FALSE;
//		}
//	}
//
//	//점프
//	if (RobotJump == TRUE)
//	{
//
//		if ((ROBOTMOVE.LegRot_LEFT <= -15 && ROBOTMOVE.LegRot_LEFT >= -60))
//		{
//			ROBOTMOVE.LegRot_LEFT -= ROBOTMOVE.JumpSpeed;
//			ROBOTMOVE.LegRot_RIGHT += ROBOTMOVE.JumpSpeed;
//
//		}
//		else
//		{
//			ROBOTMOVE.JumpSpeed *= -1;
//			ROBOTMOVE.LegRot_LEFT -= ROBOTMOVE.JumpSpeed;
//			ROBOTMOVE.LegRot_RIGHT -= ROBOTMOVE.JumpSpeed;
//			JumpCycle++;
//		}
//		if (JumpCycle == 2)
//		{
//			RobotJump = FALSE;
//			JumpCycle = 0;
//		}
//		//경직
//	}
//	if (RobotFreeze == TRUE)
//	{
//		upper = 0;
//		super = 0;
//		RobotWalk = FALSE;
//		RobotLeftPunch = FALSE;
//		RobotLeftPunch2 = FALSE;
//		RobotRightPunch = FALSE;
//		RobotRightPunch2 = FALSE;
//		RobotKick = FALSE;
//		RobotJump = FALSE;
//
//
//		if (step <= 3){
//			ROBOTMOVE.FreezeRot += ROBOTMOVE.FreezeSpeed;
//			step += 2;
//		}
//		else if (3 < step && step <10){
//			ROBOTMOVE.FreezeRot += 0.01;
//			step += 2;
//
//		}
//		else if (step >= 10)
//		{
//			ROBOTMOVE.FreezeSpeed *= -1;
//			FreezCycle++;
//			step = 0;
//		}
//		if (FreezCycle == 2)
//		{
//			RobotFreeze = FALSE;
//			FreezCycle = 0;
//		}
//	}
//
//}
//
//
//GLvoid OBJECT::Draw_HP()
//{
//	for (int i = 0; i < HP; i++)
//	{
//		glPushMatrix();
//
//		glTranslatef(-5, 30, 0);
//		////HP그리기
//		glColor3d(0, 1, 0);
//		glTranslated(i, 0, 0);
//		glutSolidCube(1.0);
//
//
//		glPopMatrix();
//	}
//}
//
//GLvoid OBJECT::Draw_Ani()
//{
//	GLUquadricObj *obj;
//
//	obj = gluNewQuadric();
//
//
//	////////////////전부그리기/////////////////////////////////
//
//	glPushMatrix();
//	glScalef(0.1, 0.1, 0.1);
//	glRotated(rot_ani, 0, 1, 0);
//
//
//	////텍스쳐 환경설정
//
//
//
//
//
//	////////////////몸통그리기////////////////////////////////////////////
//	glPushMatrix();
//	if (RobotLeftPunch2 == TRUE)
//	{
//		glRotated(ROBOTMOVE.BodyRot2, 1, 0, 0);
//	}
//
//	glRotated(ROBOTMOVE.BodyRot, 0, 1, 0);
//
//	glColor3d(1, 1, 0);
//	glPushMatrix();
//
//
//	if ((RobotLeftPunch || RobotRightPunch) && RobotLeftPunch2 == FALSE){
//		glRotated(ROBOTMOVE.BodyRot2, 1, 0, 0);
//	}
//	glRotated(270, 1, 0, 0);
//
//	//glEnable(GL_TEXTURE_GEN_Q);
//	//glEnable(GL_TEXTURE_GEN_R);
//
//	glColor3d(1, 1, 1);
//	gluCylinder(obj, ROBOTBODY.BodySize, ROBOTBODY.BodySize, 100, 100, 50);
//
//	///////////////////머리그리기/////////////////////////////////////////
//
//	glPushMatrix();
//	glTranslated(0, 0, 150);
//	glRotated(150, 1, 0, 0);
//	glEnable(GL_TEXTURE_GEN_S);
//	glEnable(GL_TEXTURE_GEN_T);
//	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
//	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
//	glBindTexture(GL_TEXTURE_2D, texutre[1]);
//	gluSphere(obj, 40, 30, 30);
//	glDisable(GL_TEXTURE_GEN_S);
//	glDisable(GL_TEXTURE_GEN_T);
//	glColor3d(1.0, 1.0, 1.0);
//
//	glPopMatrix();
//	glPopMatrix();
//
//
//	////////////////////////////팔그리기//////////////////////////////
//	glColor3d(1, 245.0 / 255.0, 237.0 / 255.0);
//	//왼팔 윗부분
//	//glPushMatrix();
//
//	//glRotated(ROBOTMOVE.FreezeRot,
//	glPushMatrix();
//	if (RobotFreeze == TRUE)
//	{
//		glRotated(ROBOTMOVE.FreezeRot, 0, -1, 0);
//	}
//
//	glPushMatrix();
//
//	glTranslated(-25, 100, 50);
//	glColor3d(0, 1, 0);
//	glRotated(ROBOTMOVE.Punch1Rot_LEFT, -1, 0, 0);
//
//	glRotated(90, 1, 0, 0);
//	gluCylinder(obj, 5, 5, ROBOTBODY.ArmSize, 100, 50);
//	glTranslated(0, 0, ROBOTBODY.ArmSize);
//
//	//왼팔 아랫부분
//	glPushMatrix();
//	glRotated(ROBOTMOVE.Punch2Rot_LEFT, -1, 0, 0);
//	gluCylinder(obj, 5, 5, ROBOTBODY.ArmSize, 100, 50);
//	//왼팔 주먹
//	glTranslated(0, 0, 75);
//	glutSolidSphere(ROBOTBODY.HandSize, 50, 50);
//	glPopMatrix();
//	glPopMatrix();
//	glPopMatrix();
//	//오른팔 윗부분
//
//	glPushMatrix();
//
//	glRotated(ROBOTMOVE.FreezeRot, 0, 1, 0);
//	glTranslated(25, 100, 50);
//	glColor3d(0, 1, 0);
//	glRotated(ROBOTMOVE.Punch1Rot_RIGHT, -1, 0, 0);
//	glRotated(90, 1, 0, 0);
//	gluCylinder(obj, 5, 5, ROBOTBODY.ArmSize, 100, 50);
//	glTranslated(0, 0, ROBOTBODY.ArmSize);
//	//오른팔 아랫부분
//	glPushMatrix();
//	glRotated(ROBOTMOVE.Punch2Rot_RIGHT, -1, 0, 0);
//	gluCylinder(obj, 5, 5, ROBOTBODY.ArmSize, 100, 50);
//	//오른팔 주먹
//	glTranslated(0, 0, 75);
//	glutSolidSphere(ROBOTBODY.HandSize2, 50, 50);
//	glPopMatrix();
//	glPopMatrix();
//	glPopMatrix();
//
//	glPushMatrix();
//
//	///////////////////다리그리기////////////////////////////////////////
//	//왼다리 허벅지
//
//	glPushMatrix();
//	glRotated(ROBOTMOVE.FreezeRot*0.3, 1, 0, 0);
//	glPushMatrix();
//	glColor3d(0, 0, 1);
//	glRotated(ROBOTMOVE.LegRot_LEFT - 10, 1, 0, 0);
//	glTranslated(-25, -70, 0);
//	glRotated(270, 1, 0, 0);
//
//	gluCylinder(obj, 10, 10, ROBOTBODY.LegSize, 100, 50);
//
//	//왼다리 종아리
//
//	glRotated(ROBOTMOVE.LegRot2_LEFT, 1, 0, 0);
//	glTranslated(0, 0, -(ROBOTBODY.LegSize + 20));
//
//	gluCylinder(obj, 10, 10, ROBOTBODY.LegSize, 100, 50);
//
//	//왼발
//
//	glTranslated(0, 0, -50);
//	glutSolidSphere(40, 50, 50);
//	glPopMatrix();
//
//	//오른다리 허벅지
//	glPushMatrix();
//	glColor3d(1, 0, 0);
//	glRotated(ROBOTMOVE.LegRot_RIGHT, 1, 0, 0);
//	glTranslated(25, -70, 0);
//	glRotated(270, 1, 0, 0);
//
//	gluCylinder(obj, 10, 10, ROBOTBODY.LegSize, 100, 50);
//
//	//오른다리 종아리
//
//	glRotated(ROBOTMOVE.LegRot2_RIGHT, 1, 0, 0);
//	glTranslated(0, 0, -(ROBOTBODY.LegSize + 20));
//
//	gluCylinder(obj, 10, 10, ROBOTBODY.LegSize, 100, 50);
//
//	//오른발
//	glTranslated(0, 0, -50);
//	glPushMatrix();
//
//	glutSolidSphere(40, 50, 50);
//	glPopMatrix();
//	glPopMatrix();
//	glPopMatrix();
//	glPopMatrix();
//
//	glPopMatrix();
//}
//
