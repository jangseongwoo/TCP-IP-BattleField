#include "object.h"

GLvoid OBJECT::Init_Object(GLfloat _xPos, GLfloat _yPos, GLfloat _zPos, GLint _mapPos, GLfloat _point[8][3])
{
	xPos=_xPos;
	yPos=_yPos;
	zPos=_zPos;
	save_y=_yPos;
	Attack_xPos=xPos;
	Attack_zPos=zPos;
	jump=0;
	combo=0;
	mapPos=_mapPos;
	Attack_Time;
	state=STATE_NORMAL;
	for(int i=0; i<8; ++i) 
		for(int j=0; j<3; ++j) {
			point[i][j]=_point[i][j];
		}
	double_rot=0;
	Init_Animation();
	time_num=0;
	HP = 10;
}
GLvoid OBJECT::SetPos(GLfloat _xPos, GLfloat _yPos, GLfloat _zPos, GLfloat _mapPos)
{
	xPos=_xPos;
	yPos=_yPos;
	zPos=_zPos;
	
	mapPos=_mapPos;
}
GLvoid OBJECT::SetRot()
{
	if(double_rot==1)
		rot=0;
	else if(double_rot==2)
		rot=180;
	else if(double_rot==4)
		rot=90;
	else if(double_rot==8)
		rot=270;
	else if(double_rot==5)
		rot=45;
	else if(double_rot==9)
		rot=315;
	else if(double_rot==6)
		rot=135;
	else if(double_rot==10)
		rot=225;
	glRotatef(rot, 0,1,0);
}
GLvoid OBJECT::SetVal(GLfloat _x, GLfloat _y, GLfloat _z)
{

	v_x = _x;

	v_y=_y;

	v_z=_z;

}
GLvoid OBJECT::Move()
{
		xPos+=v_x;
		zPos+=v_z;
		if(!RectMap(xPos, zPos, mapPos))
			v_y=-5;
		yPos+=v_y;
		if(CheckWall(&xPos, &zPos, &mapPos, &yPos) == TRUE) 
			glTranslatef(xPos, yPos, zPos);
		else {
			xPos-=v_x;
			zPos-=v_z;
			yPos-=v_y;
			glTranslatef(xPos, yPos, zPos);
		}
}
GLvoid OBJECT::JUMP()
{
	if(jump==1)
	{
		if(yPos<50)
			v_y=5;
		else
			jump=2;
	}
	if(jump==2)
	{
		if(yPos>save_y)
			v_y=-5;
		else{
			v_y=0;
			yPos=save_y;
			jump=0;
		}
	}
}
GLvoid OBJECT::MapMove()
{
}
GLvoid OBJECT:: DrawRect()
{
	glColor3f(1,1,1);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for(int i=0; i<8; ++i)
	{
		glVertex3f(point[i][0], point[i][1], point[i][2]);
	}
	glEnd();
	glColor3f(1.0f, 0,0);
	glPointSize(1.0f);
	glBegin(GL_QUADS);
	for(int i=0; i<4; ++i)
	{
		glVertex3f(point[i][0], point[i][1], point[i][2]);
	}

	for(int i=3; i>=0; --i)
	{
		glVertex3f(point[i][0], point[i][1], point[i][2]);
	}
	glEnd();
}
GLvoid OBJECT::DrawAtkPoint()
{
	glPointSize(10.0f);
	glPushMatrix();
	glTranslatef(Attack_xPos - xPos, 0, Attack_zPos - zPos);
	/*glBegin(GL_POINT);
	glVertex3f(0,0, 0);
	glEnd();*/
	glutSolidCube(10);
	glPopMatrix();
}
GLvoid OBJECT::SetAtkRect()
{
	Attack_xPos = xPos;
	Attack_zPos = zPos;
}
GLfloat b_angle=0.0f;
GLvoid OBJECT::Render(int rect, int atk, int shape)
{
	if(shape == 0) {
		if(state!=STATE_DEAD) {
			
			glPushMatrix();
			Move();
			Draw_HP();
			if(atk == 1)
				DrawAtkPoint();
			SetRot();
			if(rect == 1)
				DrawRect();

			Draw_Ani();
			Animation();
			glPopMatrix();
		}
		if(yPos<-20) {
			state=STATE_DEAD;
			Destroy();
		}
	}
	else if(shape == 1) {
		glPushMatrix();
		if(state!=STATE_DEAD)
			glColor3f(0,1,0);
		else
			glColor3f(1,0,0);
		
		Move();
		glRotatef(b_angle, 0,1,0);
		glutSolidCube(10);
		glPopMatrix();
	}
	else if(shape == 2) {
		glPushMatrix();
		glColor3f(1,0,1);
		Move();
		glRotatef(b_angle, 1,1,1);
		glutSolidCube(10);
		glPopMatrix();
	}
	

}

GLint  OBJECT::Destroy()
{
	return (1);
}

GLvoid OBJECT::Attack_Timer()
{
	if(Attack_Time==1)
	{
		if(rot == 0) {
			Attack_xPos += 0;
			Attack_zPos += 8;
		}
		else if(rot>0 & rot<90) {
			Attack_xPos += 5.6;
			Attack_zPos += 5.6;
		}
		else if(rot == 90) {
			Attack_xPos += 8;
			Attack_zPos += 0;
		}
		else if(rot>90 && rot<180) {
			Attack_xPos += 5.6;
			Attack_zPos -= 5.6;
		}
		else if(rot == 180) {
			Attack_xPos += 0;
			Attack_zPos -= 8;
		}
		else if(rot>180 && rot<270) {
			Attack_xPos -= 5.6;
			Attack_zPos -= 5.6;
		}
		else if(rot == 270) {
			Attack_xPos -= 8;
			Attack_zPos += 0;
		}
		else if(rot>270 && rot<360) {
			Attack_xPos -= 5.6;
			Attack_zPos += 5.6;
		}
		time_num+=2;
		if(time_num>4) {
			time_num=0;
			Attack_Time=-1;
		}
	}
	else if(Attack_Time==-1)
	{
		if(rot == 0) {
			Attack_xPos -= 0;
			Attack_zPos -= 4;
		}
		else if(rot>0 & rot<90) {
			Attack_xPos -= 2.8;
			Attack_zPos -= 2.8;
		}
		else if(rot == 90) {
			Attack_xPos -= 4;
			Attack_zPos -= 0;
		}
		else if(rot>90 && rot<180) {
			Attack_xPos -= 2.8;
			Attack_zPos += 2.8;
		}
		else if(rot == 180) {
			Attack_xPos -= 0;
			Attack_zPos += 4;
		}
		else if(rot>180 && rot<270) {
			Attack_xPos += 2.8;
			Attack_zPos += 2.8;
		}
		else if(rot == 270) {
			Attack_xPos += 4;
			Attack_zPos -= 0;
		}
		else if(rot>270 && rot<360) {
			Attack_xPos += 2.8;
			Attack_zPos -= 2.8;
		}
		time_num+=2;
		if(time_num>8)
		{
			Attack_Time=0;
			time_num=0;
			combo+=1;
			if(combo>3)
				combo=0;
		}
	}
}
//애니메이션부분 --------------------------------------
//----------------------------------------------------
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
	ROBOTBODY.HandSize2= 20.0f;
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
	ROBOTMOVE.LegRot2_RIGHT =30;
	ROBOTMOVE.BodyRot = 0;
	ROBOTMOVE.BodyRot2 = 0;
	ROBOTMOVE.FreezeRot = 0;
	ROBOTMOVE.FreezeSpeed = 20;
	ROBOTMOVE.WalkSpeed = 5;
	ROBOTMOVE.LeftPunchSpeed = 20;
	ROBOTMOVE.LeftPunchSpeed2 = 80;
	ROBOTMOVE.RightPunchSpeed = 20;
	ROBOTMOVE.RightPunchSpeed2= 10;
	ROBOTMOVE.JumpSpeed = 5;
}

void OBJECT::drawAxis(GLfloat angle)
{
	glBegin(GL_LINES);
		
		glColor3f(1,0,0);
		glVertex3f(0,0,0);
		glVertex3f(angle,0,0);
		
		glColor3f(0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,angle,0);
		
		glColor3f(0,0,1);
		glVertex3f(0,0,0);
		glVertex3f(0,0,angle);
		
		glColor3f(0.3f,0.7f,1.0f);
		glEnd();
}
GLvoid OBJECT::Draw_HP()
{
	for(int i = 0 ; i < HP ; i++)
	{
		glPushMatrix();

		glTranslatef(-5,30,0);
		////HP그리기
		glColor3d(0,1,0);
		glTranslated(i,0,0);
		glutSolidCube(1.0);


		glPopMatrix();
	}
}
extern GLuint texutre[];
GLvoid OBJECT::Draw_Ani()
{
	GLUquadricObj *obj;

	obj = gluNewQuadric();
	

	////////////////전부그리기/////////////////////////////////

	glPushMatrix();	
	glScalef(0.1, 0.1, 0.1);
		glRotated(rot_ani,0,1,0);
	

	////텍스쳐 환경설정

	
	
	

	////////////////몸통그리기////////////////////////////////////////////
	glPushMatrix();
		if(RobotLeftPunch2 == TRUE )
		{
		glRotated(ROBOTMOVE.BodyRot2,1,0,0);
		}
	
		glRotated(ROBOTMOVE.BodyRot,0,1,0);
		
		glColor3d(1,1,0);
		glPushMatrix();
			
			
			if((RobotLeftPunch || RobotRightPunch) && RobotLeftPunch2 == FALSE ){
			glRotated(ROBOTMOVE.BodyRot2,1,0,0);
			}
			glRotated(270,1,0,0);
			
			//glEnable(GL_TEXTURE_GEN_Q);
			//glEnable(GL_TEXTURE_GEN_R);
			
			glColor3d(1,1,1);
			gluCylinder(obj,ROBOTBODY.BodySize,ROBOTBODY.BodySize,100,100,50);
			
	///////////////////머리그리기/////////////////////////////////////////
			
			glPushMatrix();
			glTranslated(0,0,150);
			glRotated(150,1,0,0);
			glEnable (GL_TEXTURE_GEN_S);
			glEnable (GL_TEXTURE_GEN_T);
			glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glBindTexture(GL_TEXTURE_2D, texutre[1]);
			gluSphere(obj,40,30,30);
			glDisable (GL_TEXTURE_GEN_S);
			glDisable (GL_TEXTURE_GEN_T);
			glColor3d(1.0,1.0,1.0);

			glPopMatrix();
		glPopMatrix();				


	////////////////////////////팔그리기//////////////////////////////
	glColor3d(1,245.0/255.0,237.0/255.0);
	//왼팔 윗부분
	//glPushMatrix();
	
	//glRotated(ROBOTMOVE.FreezeRot,
		glPushMatrix();
		if(RobotFreeze == TRUE)
		{
			glRotated(ROBOTMOVE.FreezeRot,0,-1,0);
		}
		
			glPushMatrix();

			glTranslated(-25,100,50);
			glColor3d(0,1,0);
			glRotated(ROBOTMOVE.Punch1Rot_LEFT,-1,0,0);
	
			glRotated(90,1,0,0);
			gluCylinder(obj,5,5,ROBOTBODY.ArmSize,100,50);
			glTranslated(0,0,ROBOTBODY.ArmSize);

	//왼팔 아랫부분
			glPushMatrix();
				glRotated(ROBOTMOVE.Punch2Rot_LEFT,-1,0,0);
				gluCylinder(obj,5,5,ROBOTBODY.ArmSize,100,50);
				//왼팔 주먹
				glTranslated(0,0,75);
				glutSolidSphere(ROBOTBODY.HandSize,50,50);	
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	//오른팔 윗부분
		
		glPushMatrix();

			glRotated(ROBOTMOVE.FreezeRot,0,1,0);				
			glTranslated(25,100,50);
			glColor3d(0,1,0);
			glRotated(ROBOTMOVE.Punch1Rot_RIGHT,-1,0,0);
			glRotated(90,1,0,0);
			gluCylinder(obj,5,5,ROBOTBODY.ArmSize,100,50);
			glTranslated(0,0,ROBOTBODY.ArmSize);
	//오른팔 아랫부분
			glPushMatrix();
				glRotated(ROBOTMOVE.Punch2Rot_RIGHT,-1,0,0);
				gluCylinder(obj,5,5,ROBOTBODY.ArmSize,100,50);
				//오른팔 주먹
				glTranslated(0,0,75);
				glutSolidSphere(ROBOTBODY.HandSize2,50,50);	
			glPopMatrix();
		glPopMatrix();
		glPopMatrix();
		
		glPushMatrix();

			///////////////////다리그리기////////////////////////////////////////
			//왼다리 허벅지

		glPushMatrix();
		glRotated(ROBOTMOVE.FreezeRot*0.3,1,0,0);
			glPushMatrix();
				glColor3d(0,0,1);
				glRotated(ROBOTMOVE.LegRot_LEFT-10,1,0,0);
				glTranslated(-25,-70,0);
				glRotated(270,1,0,0);
				
				gluCylinder(obj,10,10,ROBOTBODY.LegSize,100,50);

			//왼다리 종아리
				
				glRotated(ROBOTMOVE.LegRot2_LEFT,1,0,0);
				glTranslated(0,0,-(ROBOTBODY.LegSize+20));
			
				gluCylinder(obj,10,10,ROBOTBODY.LegSize,100,50);
			
			//왼발

				glTranslated(0,0,-50);
				glutSolidSphere(40,50,50);
			glPopMatrix();
			
			//오른다리 허벅지
			glPushMatrix();
				glColor3d(1,0,0);
				glRotated(ROBOTMOVE.LegRot_RIGHT,1,0,0);
				glTranslated(25,-70,0);
				glRotated(270,1,0,0);
				
				gluCylinder(obj,10,10,ROBOTBODY.LegSize,100,50);

			//오른다리 종아리
				
				glRotated(ROBOTMOVE.LegRot2_RIGHT,1,0,0);
				glTranslated(0,0,-(ROBOTBODY.LegSize+20));
			
				gluCylinder(obj,10,10,ROBOTBODY.LegSize,100,50);
			
			//오른발
				glTranslated(0,0,-50);
				glPushMatrix();
	
				glutSolidSphere(40,50,50);
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	
	glPopMatrix();
}

GLvoid OBJECT::Animation ()
{
	//걷기
	if(RobotWalk == TRUE && RobotFreeze == FALSE)
	{

		if(ROBOTMOVE.LegRot_LEFT >= -15 && ROBOTMOVE.LegRot_LEFT <= 15){
		ROBOTMOVE.LegRot_LEFT+=ROBOTMOVE.WalkSpeed;
		ROBOTMOVE.LegRot_RIGHT-=ROBOTMOVE.WalkSpeed;
		ROBOTMOVE.LegRot2_LEFT+=ROBOTMOVE.WalkSpeed;
		ROBOTMOVE.LegRot2_RIGHT-=ROBOTMOVE.WalkSpeed;
		}
		else
		{
			ROBOTMOVE.WalkSpeed *= -1;
			ROBOTMOVE.LegRot_LEFT+=ROBOTMOVE.WalkSpeed;
			ROBOTMOVE.LegRot_RIGHT+=ROBOTMOVE.WalkSpeed;
		
			WalkCycle++;

		}
		if(WalkCycle == 2){
				RobotWalk = FALSE;
				WalkCycle = 0;
			}
	}
	//오른손 펀치
	if(RobotRightPunch == TRUE && RobotFreeze == FALSE && RobotLeftPunch2 == FALSE )
	{
		
		if(ROBOTMOVE.Punch1Rot_RIGHT <= 90 && ROBOTMOVE.Punch2Rot_RIGHT>=0 && ROBOTMOVE.Punch1Rot_RIGHT >= 25 && ROBOTMOVE.Punch2Rot_RIGHT<=150)
		{
			ROBOTMOVE.Punch1Rot_RIGHT+=ROBOTMOVE.RightPunchSpeed;
			ROBOTMOVE.Punch2Rot_RIGHT-=ROBOTMOVE.RightPunchSpeed*2;
			ROBOTMOVE.BodyRot -= (ROBOTMOVE.RightPunchSpeed*0.2);
			ROBOTMOVE.BodyRot2 += ROBOTMOVE.RightPunchSpeed*0.3;	
			ROBOTMOVE.LegRot2_RIGHT += ROBOTMOVE.RightPunchSpeed*0.3;	
			ROBOTBODY.HandSize2 += ROBOTMOVE.RightPunchSpeed*0.3;	
		
		}
		else  {
			ROBOTMOVE.RightPunchSpeed *= -1;
			ROBOTMOVE.Punch1Rot_RIGHT+=ROBOTMOVE.RightPunchSpeed;
			ROBOTMOVE.Punch2Rot_RIGHT-=ROBOTMOVE.RightPunchSpeed*2;
			ROBOTMOVE.BodyRot -= (ROBOTMOVE.RightPunchSpeed*0.2);
			ROBOTMOVE.BodyRot2 += ROBOTMOVE.RightPunchSpeed*0.5;
			PunchCycle2++;		
		}

		if(PunchCycle2==2){
				RobotRightPunch = FALSE;		
				PunchCycle2 = 0;
				
		}	
	}
	// 왼손펀치
	if(RobotLeftPunch == TRUE && RobotFreeze == FALSE && RobotLeftPunch2 == FALSE)
	{ 

		
		if(ROBOTMOVE.Punch1Rot_LEFT <= 90 && ROBOTMOVE.Punch2Rot_LEFT>=0 && ROBOTMOVE.Punch1Rot_LEFT >= 25 && ROBOTMOVE.Punch2Rot_LEFT<=150){
			ROBOTMOVE.Punch1Rot_LEFT+=ROBOTMOVE.LeftPunchSpeed;
			ROBOTMOVE.Punch2Rot_LEFT-=ROBOTMOVE.LeftPunchSpeed*2;
			ROBOTMOVE.BodyRot += (ROBOTMOVE.LeftPunchSpeed*0.2);
			ROBOTMOVE.BodyRot2 += ROBOTMOVE.LeftPunchSpeed*0.3;
			ROBOTBODY.HandSize += ROBOTMOVE.LeftPunchSpeed*0.3;	
		
	
		}
		else  {
			ROBOTMOVE.LeftPunchSpeed *= -1;
			ROBOTMOVE.Punch1Rot_LEFT+=ROBOTMOVE.LeftPunchSpeed;
			ROBOTMOVE.Punch2Rot_LEFT-=ROBOTMOVE.LeftPunchSpeed*2;
			ROBOTMOVE.BodyRot += (ROBOTMOVE.LeftPunchSpeed*0.2);
			ROBOTMOVE.BodyRot2 += ROBOTMOVE.LeftPunchSpeed*0.3;
			PunchCycle++;		
			
		}
		if(PunchCycle==2){
				RobotLeftPunch = FALSE;		
				PunchCycle = 0;
				
		}
			
	}
	
	//왼손 어퍼
	if(RobotLeftPunch2 == TRUE && RobotFreeze == FALSE )
	{

		RobotWalk = FALSE;
		RobotLeftPunch = FALSE;
		RobotRightPunch = FALSE;
		RobotRightPunch2 = FALSE;
		RobotKick = FALSE;
		RobotJump = FALSE;
		//RobotJump = FALSE;       점프하면서 어퍼는 할수 있게..
		
		
		if(upper <= 3){ 
			ROBOTMOVE.Punch2Rot_LEFT-=ROBOTMOVE.LeftPunchSpeed2*0.8;
			upper+=2;
			ROBOTMOVE.BodyRot2 += ROBOTMOVE.LeftPunchSpeed2*0.1;
			ROBOTBODY.HandSize += ROBOTMOVE.LeftPunchSpeed2*0.1;	
		}
		else if(3 < upper && upper <7){ 
			ROBOTMOVE.Punch1Rot_LEFT+=ROBOTMOVE.LeftPunchSpeed2*0.8;
			upper+=2;
			ROBOTMOVE.BodyRot2 -= ROBOTMOVE.LeftPunchSpeed2*0.1;
			ROBOTMOVE.BodyRot += (ROBOTMOVE.LeftPunchSpeed2*0.2);
		}
		else if(upper>=7 && upper <10){
			upper++;
		}
		else if(upper >= 10)
		{
			ROBOTMOVE.LeftPunchSpeed2 *= -1;
			UpperCycle++;
			upper = 0;
		}

		if(UpperCycle == 2)
		{
			RobotLeftPunch2 = FALSE ;
			UpperCycle = 0;
		}			
		
	}
	// 마지막 기모아서 때리기
	if(RobotRightPunch2 == TRUE )
	{
		RobotWalk = FALSE;
		RobotLeftPunch = FALSE;
		RobotRightPunch = FALSE;
		RobotLeftPunch2 = FALSE;
		RobotKick = FALSE;
		//RobotJump = FALSE;       점프하면서 어퍼는 할수 있게..

		/*static float super=0;
		if(super <= 3){ 
			rot_ani+=ROBOTMOVE.RightPunchSpeed2;

			super+=2;
		}
		else if(3 < super && super <7){ 
			super+=0.5;
		}
		else if(super>=7 && super <10){
			super+=2;
			rot_ani-=ROBOTMOVE.RightPunchSpeed2*5;
		}
		else if(super >= 10)
		{
			ROBOTMOVE.RightPunchSpeed2 *= -1;
			superCycle++;
			super = 0;
		}

		if(superCycle == 2)
		{
			RobotRightPunch2 = FALSE ;
			superCycle = 0;

		}			*/
		rot_ani+=100.0f;
		if(rot_ani>900) {
			rot_ani=0;
			RobotRightPunch2 = FALSE;
		}
	}

	//점프
	if(RobotJump == TRUE)
	{

		if((ROBOTMOVE.LegRot_LEFT <= -15 && ROBOTMOVE.LegRot_LEFT >= -60))
		{
			ROBOTMOVE.LegRot_LEFT -= ROBOTMOVE.JumpSpeed;
			ROBOTMOVE.LegRot_RIGHT += ROBOTMOVE.JumpSpeed;
			
		}
		else 
		{	
			ROBOTMOVE.JumpSpeed*= -1;
			ROBOTMOVE.LegRot_LEFT -= ROBOTMOVE.JumpSpeed;
			ROBOTMOVE.LegRot_RIGHT -= ROBOTMOVE.JumpSpeed;
			JumpCycle++;
		}
		if(JumpCycle ==2)
		{
			RobotJump = FALSE;
			JumpCycle = 0;
		}
	//경직
	}
		if(RobotFreeze == TRUE)
		{
			upper =0;
			super = 0;
			RobotWalk = FALSE;
			RobotLeftPunch = FALSE;
			RobotLeftPunch2 = FALSE;
			RobotRightPunch = FALSE;
			RobotRightPunch2 = FALSE;
			RobotKick = FALSE;
			RobotJump = FALSE;

			
			if(step <= 3){ 
				ROBOTMOVE.FreezeRot+=ROBOTMOVE.FreezeSpeed;
				step+=2;
			}
			else if(3 < step && step <10){ 
				ROBOTMOVE.FreezeRot+=0.01;
				step+=2;
				
			}
			else if(step>=10)
				{
					ROBOTMOVE.FreezeSpeed *= -1;
					FreezCycle++;
					step=0;
				}
			if(FreezCycle == 2)
			{
				RobotFreeze = FALSE ;
				FreezCycle = 0;
			}			
		}
		
}


//충돌체크부분 ----------------------------------------
//----------------------------------------------------

bool CrashCheck(OBJECT obj1, OBJECT obj2)
{
	float len_x = obj1.Attack_xPos - obj2.xPos;
	float len_z = obj1.Attack_zPos - obj2.zPos;
	int len = sqrt( (len_x * len_x) + (len_z * len_z) );
	if(len<15)
		return true;
	return false;
}

bool CrashCheck2(OBJECT obj1, OBJECT obj2)
{
	float len_x = obj1.xPos - obj2.xPos;
	float len_z = obj1.zPos - obj2.zPos;
	int len = sqrt( (len_x * len_x) + (len_z * len_z) );
	if(len<30)
		return true;
	return false;
}

GLvoid ObjectCrash(OBJECT obj1, OBJECT *obj2)
{
	if(obj2->jump!=0 || obj1.yPos!=obj2->yPos)
		return;
	GLfloat d_x, d_z, len;
	d_x=obj1.xPos-obj2->xPos;
	d_z=obj1.zPos-obj2->zPos;
	len=sqrt(d_x*d_x + d_z*d_z);
	if(len<30) {
		if(d_x>0) {
			if(obj2->v_x>0)
				obj2->v_x=0;
		}
		else {
			if(obj2->v_x<0)
				obj2->v_x=0;
		}
		if(d_z>0) {
			if(obj2->v_z>0)
				obj2->v_z=0;
		}
		else {
			if(obj2->v_z<0)
				obj2->v_z=0;
		}
	}
}

//AI부분----------------------------------------------
//----------------------------------------------------
GLvoid AI_Crash(OBJECT obj[], int num)
{
	for(int cur=0; cur<num; ++cur) {
		for(int i=0; i<num; ++i)
		{
			if(cur==i)
				continue;
			if(obj[cur].state == STATE_DEAD)
				continue;
			ObjectCrash(obj[i], &obj[cur]);
		}
	}
}
GLvoid Player_Crash(OBJECT *player, OBJECT obj[], int num)
{
	for(int cur=0; cur<num; ++cur)
	{
		if(obj[cur].state == STATE_DEAD)
			continue;
		ObjectCrash(obj[cur], player);
	}
}
GLvoid AI_FSM(OBJECT player, OBJECT *ai)
{
	switch(ai->state)
	{
	case STATE_NORMAL:
		if(ai->RobotWalk != FALSE)
			ai->RobotWalk = FALSE;
		if(AI_AREA(player, ai) == 1)
			ai->state=STATE_CHASE;
		break;
	case STATE_CHASE:
		if(ai->RobotWalk != TRUE)
			ai->RobotWalk = TRUE;
		ai->SetAtkRect();
		if( follow(player, ai) != 1)
			ai->state=STATE_ATTACK;
		break;
	case STATE_ATTACK:
		if(ai->RobotWalk != FALSE)
			ai->RobotWalk = FALSE;
		if(ai->RobotFreeze != TRUE)
			AI_Attack(ai);
		if(AI_Filed(player, ai) != 0)
			ai->state=STATE_CHASE;
		break;
	case STATE_DEAD:
		if(ai->RobotWalk != FALSE)
			ai->RobotWalk = FALSE;
		ai->Destroy();
		break;
	}
}

GLint follow(OBJECT obj1, OBJECT *obj2)
{
	GLfloat d_x, d_z, v_x, v_z;
	GLfloat _rot;  //실각도
	d_x=obj1.xPos-obj2->xPos;
	d_z=obj1.zPos-obj2->zPos;

	
	_rot=acos(d_z/sqrt(d_x*d_x + d_z*d_z)) / PI * 180;
	
	if(obj1.xPos<obj2->xPos)
		_rot=360-_rot;
	
	
	for(int i=1; i<4;++i)   //각도 보정
	{
		if(_rot> 90*i -10 && _rot<90 * i + 10)
			_rot = 90*i;
	}
	if(_rot<10 || _rot>350)  //각도보정
		_rot=0;

	obj2->rot=_rot;

	if(d_x>0)
		v_x=5;
	else if(d_x<0)
		v_x=-5;
	else
		v_x=0;
	if(d_z>0)
		v_z=5;
	else if(d_z<0)
		v_z=-5;
	else
		v_z=0;

	if( AI_Filed(obj1, obj2) == 0) {
		obj2->SetVal(0,0,0);
		return 0;
	}
	else {
		obj2->SetVal(v_x, 0, v_z);
		return 1;
	}
	return 1;
}

GLvoid AI_Attack(OBJECT *obj)
{
	if(obj->Attack_Time!=0 ||
		obj->RobotLeftPunch == TRUE || obj->RobotRightPunch == TRUE ||
		obj->RobotLeftPunch2 == TRUE || obj->time_num!=0)
		return;
	switch(obj->combo)
	{
	case 0:
	case 1:
		if(obj->RobotLeftPunch == FALSE) {
			obj->SetAtkRect();
			obj->RobotLeftPunch = TRUE;
			obj->Attack_Time=1;
		}
		break;
	case 2:
	case 3:
		if(obj->RobotRightPunch == FALSE) {
			obj->SetAtkRect();
			obj->RobotRightPunch = TRUE;
			obj->Attack_Time=1;
		}
		break;
	}
}
GLint AI_Filed(OBJECT obj1, OBJECT *obj2)
{
	GLfloat d_x, d_z, len;
	d_x=obj1.xPos-obj2->xPos;
	d_z=obj1.zPos-obj2->zPos;
	len=sqrt(d_x*d_x + d_z*d_z);
	if(len<FILED) 
		return 0;
		
	return 1;

}
GLint AI_AREA(OBJECT obj1, OBJECT *obj2)
{
	GLfloat d_x, d_z, len;
	d_x=obj1.xPos-obj2->xPos;
	d_z=obj1.zPos-obj2->zPos;
	len=sqrt(d_x*d_x + d_z*d_z);
	if(len<AREA) 
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
	while((Get_Clock() - start_clock_count) <count);
	return(Get_Clock());
}