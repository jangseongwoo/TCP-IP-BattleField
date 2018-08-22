#pragma once
#include "global.h"

#define PI 3.141592f
#define FILED 30
#define AREA 100

const char STATE_DEAD   = -1;
const char STATE_NORMAL =  0;
const char STATE_CHASE  =  1;
const char STATE_MOVE   =  1;
const char STATE_ATTACK =  2;

extern BOOL RectMap(float xPos, float zPos, int type);
extern BOOL CheckWall(float *xPos, float *zPos, int *type, float *yPos);

struct RobotBody {

	float BodySize;
	float ArmSize;
	float ArmSize2;						//팔꿈치아래
	float LegSize;
	float HandSize;
	float HandSize2;
	float FootSize;
	float HeadSize;

};


//////////////캐릭터 움직임 각도 및 스피드
struct RobotMove {
	float Punch1Rot_LEFT;
	float Punch1Rot_RIGHT;
	float Punch2Rot_LEFT;
	float Punch2Rot_RIGHT;
	float LegRot_LEFT;
	float LegRot_RIGHT;
	float LegRot2_LEFT;
	float LegRot2_RIGHT;
	float KickRot;
	float BodyRot;
	float BodyRot2;
	float LeftPunchSpeed;
	float LeftPunchSpeed2;
	float RightPunchSpeed;
	float RightPunchSpeed2;
	float KickSpeed;
	float WalkSpeed;
	float JumpSpeed;
	float FreezeRot;
	float FreezeSpeed;
};

class OBJECT {
private:
public:
	int playerNumber;
	float point[8][3];
	int   Attack_Time;
	int   combo;
	float xPos, yPos, zPos;
	float Attack_zPos, Attack_xPos;
	float save_y;
	int mapPos;
	float v_x, v_y, v_z;
	int state;
	float rot;
	int jump;
	char double_rot;
	
	void Init_Object(float _xPos, float _yPos, float _zPos, int _mapPos, float _point[8][3]);   //오브젝트 초기화
	void SetPos(float _xPos, float _yPos, float _zPos, float _mapPos);                              //좌표설정
	void SetRot(); //회전설정
	void SetVal(float _x, float _y, float _z); //속도설정
	void Move();  //이동
	void JUMP();  //점프
	void MapMove(); //맵이동
	void DrawRect();//충돌영역그리기

	void DrawAtkPoint();
	void SetAtkRect();

	void Render(int rect, int atk, int shape); //그리기
	int  Destroy();        //삭제(죽거나 땅에떨어지면)

	//공격판정
	void Attack_Timer();  //공격타이며
	int time_num;           //뻗고 당기고 설정위해
	void Draw_HP();
	//애니매이션부분
	RobotMove ROBOTMOVE;
	RobotBody ROBOTBODY;

	bool RobotWalk ;
	bool RobotLeftPunch ;
	bool RobotLeftPunch2;
	bool RobotRightPunch ;
	bool RobotRightPunch2;
	bool RobotKick ;
	bool RobotJump ;
	bool RobotFreeze;

	float rot_ani ;
	float FreezeTimer;
	int WalkCycle ;
	int PunchCycle ;
	int PunchCycle2 ;
	int JumpCycle ;
	int UpperCycle;
	int superCycle;
	int FreezCycle;

	float upper;
	float super; 
	float step;

	GLuint textures;
	//int RightPunchCombo;
	//int LeftPunchCombo;

	void drawAxis(float angle);
	void Init_Animation();
	void Draw_Ani();
	void Animation ();

	//스테이터스부분
	int HP;
};


//충돌체크
bool CrashCheck(OBJECT obj1, OBJECT obj2);
bool CrashCheck2(OBJECT obj1, OBJECT obj2);
void ObjectCrash(OBJECT obj1, OBJECT *obj2);
void AI_Crash(OBJECT obj[], int num);
void Player_Crash(OBJECT *player, OBJECT obj[], int num);

//AI부분
void AI_FSM(OBJECT player, OBJECT *ai);
int follow(OBJECT object1 , OBJECT *object2);
void AI_Attack(OBJECT *obj);
int AI_Filed(OBJECT obj1, OBJECT *obj2);
int AI_AREA(OBJECT obj1, OBJECT *obj2);

//프레임 조절
DWORD Get_Clock(void);
DWORD Start_Clock(void);
DWORD Wait_Clock(DWORD count);
