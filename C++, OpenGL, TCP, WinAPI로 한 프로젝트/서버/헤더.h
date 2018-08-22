#pragma once
//#include <winsock2.h>
#include <gl/glut.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define WIDTH 800 
#define HEIGHT 600
#define EMY_NUM 7
#define MOTION 1
#define STAGE 2

#define PI 3.141592f
#define FILED 30
#define AREA 100
const char STATE_DEAD = -1;
const char STATE_NORMAL = 0;
const char STATE_CHASE = 1;
const char STATE_MOVE = 1;
const char STATE_ATTACK = 2;
const GLfloat CUBE_SCALE = 10.0f;


struct RobotBody {

	float BodySize;
	float ArmSize;
	float ArmSize2;						//�Ȳ�ġ�Ʒ�
	float LegSize;
	float HandSize;
	float HandSize2;
	float FootSize;
	float HeadSize;

};


//////////////ĳ���� ������ ���� �� ���ǵ�
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

	void Init_Object(float _xPos, float _yPos, float _zPos, int _mapPos, float _point[8][3]);   //������Ʈ �ʱ�ȭ
	void SetPos(float _xPos, float _yPos, float _zPos, float _mapPos);                              //��ǥ����
	void SetRot(); //ȸ������
	void SetVal(float _x, float _y, float _z); //�ӵ�����
	void Move();  //�̵�
	void JUMP();  //����
	void MapMove(); //���̵�
	void DrawRect();//�浹�����׸���

	void DrawAtkPoint();
	void SetAtkRect();

	void Render(int rect, int atk, int shape); //�׸���
	int  Destroy();        //����(�װų� ������������)

	//��������
	void Attack_Timer();  //����Ÿ�̸�
	int time_num;           //���� ���� ��������
	void Draw_HP();
	//�ִϸ��̼Ǻκ�
	RobotMove ROBOTMOVE;
	RobotBody ROBOTBODY;

	bool RobotWalk;
	bool RobotLeftPunch;
	bool RobotLeftPunch2;
	bool RobotRightPunch;
	bool RobotRightPunch2;
	bool RobotKick;
	bool RobotJump;
	bool RobotFreeze;

	float rot_ani;
	float FreezeTimer;
	int WalkCycle;
	int PunchCycle;
	int PunchCycle2;
	int JumpCycle;
	int UpperCycle;
	int superCycle;
	int FreezCycle;

	float upper;
	float super;
	float step;

	int textures;
	//int RightPunchCombo;
	//int LeftPunchCombo;

	void drawAxis(float ane);
	void Init_Animation();
	void Draw_Ani();
	void Animation();

	//�������ͽ��κ�
	int HP;
};


struct loginPacket{
	char id[50];
	char pw[10];
};
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
