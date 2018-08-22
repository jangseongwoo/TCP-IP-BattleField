#include "Map.h"
#include "object.h"
#include "bitmap_light.h"
int Map_Type=0;
int Map_Len=150;
int Map_Width=150;
int MapNum[4] = {0,1000, 2000, -1000};
int OpenDoor1[3] = {1,-1,-1};

extern OBJECT player;
extern OBJECT player2;
extern OBJECT button[3];
GLfloat Pos[]={0.0f, 150.0f, 0.0f, 1.0f};
GLubyte *pBytes; // 데이터를 가리킬 포인터
BITMAPINFO *info; // 비트맵 헤더 저장할 변수
GLfloat ambi[]={1.0f, 1.0f, 1.0f, 0.0f};

const char *filename[3] = {{"bottom.bmp"}, {"type.bmp"}, {"earth.bmp"}};
GLuint texutre[3];

GLvoid DrawMap()
{
	Pos[0] = MapNum[player.mapPos];
	mapping_image(texutre, 3, filename);
	glColor3f(0.5,0.5,0.5);
	glBindTexture(GL_TEXTURE_2D, texutre[1]);
	glBegin(GL_QUADS);
	glTexCoord2f (0.0, 0.0);
	glVertex3f(-Map_Width, 0, -Map_Len);
	glTexCoord2f (0.0, 1.0);
	glVertex3f(-Map_Width, 0, Map_Len);
	glTexCoord2f (1.0, 1.0);
	glVertex3f(Map_Width, 0, Map_Len);
	glTexCoord2f (1.0, 0.0);
	glVertex3f(Map_Width, 0, -Map_Len);
	glEnd();
	//
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D, texutre[0]);
	if(OpenDoor1[0] == 1) {
		glBegin(GL_QUADS);
			glTexCoord2f (0.0, 0.0);
			glVertex3f(Map_Width, 0, -Map_Len);
			glTexCoord2f (0.4, 0.0);
			glVertex3f(Map_Width, 0, -40);
			glTexCoord2f (0.4, 1.0);
			glVertex3f(Map_Width, Map_Width, -40);
			glTexCoord2f (0.0, 1.0);
			glVertex3f(Map_Width, Map_Width, -Map_Len);

			glTexCoord2f (0.6, 0.4);
			glVertex3f(Map_Width, 80, 40);
			glTexCoord2f (0.6, 1.0);
			glVertex3f(Map_Width, Map_Width, 40);
			glTexCoord2f (0.4, 1.0);
			glVertex3f(Map_Width, Map_Width, -40);
			glTexCoord2f (0.4, 0.4);
			glVertex3f(Map_Width, 80, -40);

			glTexCoord2f (0.6, 0.0);
			glVertex3f(Map_Width, 0, 40);
			glTexCoord2f (1.0, 0.0);
			glVertex3f(Map_Width, 0, Map_Len);
			glTexCoord2f (1.0, 1.0);
			glVertex3f(Map_Width, Map_Width, Map_Len);
			glTexCoord2f (0.6, 1.0);
			glVertex3f(Map_Width, Map_Width, 40);
		glEnd();
	}
	else {
		
		glBindTexture(GL_TEXTURE_2D, texutre[0]);
		glBegin(GL_QUADS);
		glTexCoord2f (0.0, 0.0);
		glVertex3f(Map_Width, 0, -Map_Len);
		glTexCoord2f (1.0, 0.0);
		glVertex3f(Map_Width, 0, Map_Len);
		glTexCoord2f (1.0, 1.0);
		glVertex3f(Map_Width, Map_Width, Map_Len);
		glTexCoord2f (0.0, 1.0);
		glVertex3f(Map_Width, Map_Width, -Map_Len);
		glEnd();
	}
	if(OpenDoor1[1] == 1) {
		//glBindTexture(GL_TEXTURE_2D, texutre[0]);
		glBegin(GL_QUADS);
			glTexCoord2f (0.0, 0.0);
			glVertex3f(-Map_Width, Map_Width, -Map_Len);
			glTexCoord2f (0.4, 0.0);
			glVertex3f(-Map_Width, Map_Width, -40);
			glTexCoord2f (0.4, 1.0);
			glVertex3f(-Map_Width, 0, -40);
			glTexCoord2f (0.0, 1.0);
			glVertex3f(-Map_Width, 0, -Map_Len);

			glTexCoord2f (0.6, 0.4);
			glVertex3f(-Map_Width, Map_Width, 40);
			glTexCoord2f (0.6, 1.0);
			glVertex3f(-Map_Width, 80, 40);
			glTexCoord2f (0.4, 1.0);
			glVertex3f(-Map_Width, 80, -40);
			glTexCoord2f (0.4, 0.4);
			glVertex3f(-Map_Width, Map_Width, -40);

			glTexCoord2f (0.6, 0.0);
			glVertex3f(-Map_Width, Map_Width, 40);
			glTexCoord2f (1.0, 0.0);
			glVertex3f(-Map_Width, Map_Width, Map_Len);
			glTexCoord2f (1.0, 1.0);
			glVertex3f(-Map_Width, 0, Map_Len);
			glTexCoord2f (0.6, 1.0);
			glVertex3f(-Map_Width, 0, 40);
		glEnd();
	}
	else {
		glBindTexture(GL_TEXTURE_2D, texutre[0]);
		glBegin(GL_QUADS);
		glTexCoord2f (0.0, 0.0);
		glVertex3f(-Map_Width, Map_Width, -Map_Len);
		glTexCoord2f (1.0, 0.0);
		glVertex3f(-Map_Width, Map_Width, Map_Len);
		glTexCoord2f (1.0, 1.0);
		glVertex3f(-Map_Width, 0, Map_Len);
		glTexCoord2f (0.0, 1.0);
		glVertex3f(-Map_Width, 0, -Map_Len);
		glEnd();
	}
	if(OpenDoor1[2] == 1) {
		glBindTexture(GL_TEXTURE_2D, texutre[0]);
		glBegin(GL_QUADS);
			glTexCoord2f (0.0, 0.0);
			glVertex3f(Map_Width, 0, Map_Len);
			glTexCoord2f (0.4, 0.0);
			glVertex3f(40, 0, Map_Len);
			glTexCoord2f (0.4, 1.0);
			glVertex3f(40, Map_Width, Map_Len);
			glTexCoord2f (0.0, 1.0);
			glVertex3f(Map_Width, Map_Width, Map_Len);

			glTexCoord2f (0.6, 0.4);
			glVertex3f(-40, 80, Map_Len);
			glTexCoord2f (0.6, 1.0);
			glVertex3f(-40, Map_Width, Map_Len);
			glTexCoord2f (0.4, 1.0);
			glVertex3f(40, Map_Width, Map_Len);
			glTexCoord2f (0.4, 0.4);
			glVertex3f(40, 80, Map_Len);

			glTexCoord2f (0.6, 0.0);
			glVertex3f(-40, 0, Map_Len);
			glTexCoord2f (1.0, 0.0);
			glVertex3f(-Map_Width, 0, Map_Len);
			glTexCoord2f (1.0, 1.0);
			glVertex3f(-Map_Width, Map_Width, Map_Len);
			glTexCoord2f (0.6, 1.0);
			glVertex3f(-40, Map_Width, Map_Len);
			glEnd();
	}
	else {
		glBegin(GL_QUADS);
		glTexCoord2f (0.0, 0.0);
		glVertex3f(Map_Width, 0, Map_Len);
		glTexCoord2f (1.0, 0.0);
		glVertex3f(-Map_Width, 0, Map_Len);
		glTexCoord2f (1.0, 1.0);
		glVertex3f(-Map_Width, Map_Width, Map_Len);
		glTexCoord2f (0.0, 1.0);
		glVertex3f(Map_Width, Map_Width, Map_Len);
		glEnd();
	}
	SetLight(GL_LIGHT0);
}
GLfloat Map_Color = 0.0f;
GLvoid DrawMap_2()
{
	glBindTexture(GL_TEXTURE_2D, texutre[1]);
	glColor3f(0.5,0.5,0.5);
	glBegin(GL_QUADS);
	glTexCoord2f (0.0, 0.0);
	glVertex3f(-Map_Width+MapNum[1], 0, -Map_Len);
	glTexCoord2f (1.0, 0.0);
	glVertex3f(-Map_Width+MapNum[1], 0, Map_Len);
	glTexCoord2f (1.0, 1.0);
	glVertex3f(Map_Width+MapNum[1], 0, Map_Len);
	glTexCoord2f (0.0, 1.0);
	glVertex3f(Map_Width+MapNum[1], 0, -Map_Len);
	glEnd();


	glColor3f(0.5,0.5,0.5);
	glBegin(GL_QUADS);
	glTexCoord2f (0.0, 0.0);
	glVertex3f(-Map_Width+MapNum[2], 0, -Map_Len);
	glTexCoord2f (1.0, 0.0);
	glVertex3f(-Map_Width+MapNum[2], 0, Map_Len);
	glTexCoord2f (1.0, 1.0);
	glVertex3f(Map_Width+MapNum[2], 0, Map_Len);
	glTexCoord2f (0.0, 1.0);
	glVertex3f(Map_Width+MapNum[2], 0, -Map_Len);
	glEnd();

	glColor3f(1,Map_Color,Map_Color);
	//glBegin(GL_LINE_LOOP);
	////glVertex3f(MapNum[3], 0, Map_Len);
	//for(float i=0; i<3.141592 * 2; i+=0.03) {
	//	glVertex3f(MapNum[3]+(Map_Width*2*cos(i)), 0, Map_Len+Map_Width*2*sin(i) );
	//}
	//glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(MapNum[3], 0, Map_Len);
	GLfloat x,y;
	for(float i=0; i<6.4; i+=0.1f) {
		x=cos(i)*Map_Width*2;
		y=sin(i)*Map_Width*2;
		glVertex3f(MapNum[3]+y, 0, Map_Len+x);
	}
	glEnd();
	glColor3f(0,0,0);
}
GLfloat Map_yPos[2]={150, 150};
GLfloat door_rot=0;
GLvoid DrawDoor()
{
	glBindTexture(GL_TEXTURE_2D, texutre[2]);
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
			

			glTexCoord2f (0.0, 0.1);
			glVertex3f(Map_Width+MapNum[1], Map_yPos[0], 40);
			glTexCoord2f (1.0, 1.0);
			glVertex3f(Map_Width+MapNum[1], 80+Map_yPos[0], 40);
			glTexCoord2f (1.0, 0.0);
			glVertex3f(Map_Width+MapNum[1], 80+Map_yPos[0], -40);
			glTexCoord2f (0.0, 0.0);
			glVertex3f(Map_Width+MapNum[1], Map_yPos[0], -40);

		
	glEnd();

	glBegin(GL_QUADS);
			

			glTexCoord2f (0.0, 1.0);
			glVertex3f(-Map_Width+ MapNum[2], 80+Map_yPos[1], 40);
			glTexCoord2f (1.0, 1.0);
			glVertex3f(-Map_Width+ MapNum[2], Map_yPos[1], 40);
			glTexCoord2f (1.0, 0.0);
			glVertex3f(-Map_Width+ MapNum[2], Map_yPos[1], -40);
			glTexCoord2f (0.0, 0.0);
			glVertex3f(-Map_Width+ MapNum[2], 80+Map_yPos[1], -40);

			
	glEnd();
	
	//glColor3f(0,0,1);
	glPushMatrix();
	
	glTranslatef(-40,0,Map_Len-5);
	glRotatef(door_rot, 0, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2f (0.0, 1.0);
	glVertex3f(0,80,0);
	glTexCoord2f (0.5, 1.0);
	glVertex3f(40,80,0);
	glTexCoord2f (0.5, 0.0);
	glVertex3f(40,0,0);
	glTexCoord2f (0.0, 0.0);
	glVertex3f(0,0,0);
	glTexCoord2f (0.0, 1.0);
	glVertex3f(40,80,0);
	glTexCoord2f (1.0, 1.0);
	glVertex3f(0,80,0);
	glTexCoord2f (1.0, 0.0);
	glVertex3f(0,0,0);
	glTexCoord2f (0.0, 0.0);
	glVertex3f(40,0,0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	
	glTranslatef(40,0,Map_Len-5);
	glRotatef(-door_rot, 0, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2f (0.5, 1.0);
	glVertex3f(-40,80,0);
	glTexCoord2f (1.0, 1.0);
	glVertex3f(0,80,0);
	glTexCoord2f (1.0, 0.0);
	glVertex3f(0,0,0);
	glTexCoord2f (0.5, 0.0);
	glVertex3f(-40,0,0);
	glTexCoord2f (0.0, 1.0);
	glVertex3f(0,80,0);
	glTexCoord2f (1.0, 1.0);
	glVertex3f(-40,80,0);
	glTexCoord2f (1.0, 0.0);
	glVertex3f(-40,0,0);
	glTexCoord2f (0.0, 0.0);
	glVertex3f(0,0,0);
	glEnd();
	glPopMatrix();
}

extern OBJECT ball[];
extern OBJECT ball2[];
int go[5] = {10,-23,29,-30,22};
GLvoid MapAni()
{
	if(OpenDoor1[2] == 1) {
		if(door_rot>=180)
			door_rot+=0.0f;
		else
			door_rot+=5.0f;
	}
	if(button[0].state == STATE_DEAD) {
		if(Map_yPos[0]>0)
			Map_yPos[0]-=10;
	}
	if(button[1].state == STATE_DEAD) {
		if(Map_yPos[1]>0)
			Map_yPos[1]-=10;
	}
	if(player.mapPos == 2) {
		for(int i=0; i<5; ++i) {
			if(ball[i].zPos>Map_Len)
				ball[i].zPos=-Map_Len;
			if(ball[i].zPos<-Map_Len)
				ball[i].zPos=Map_Len;

			if(ball2[i].zPos>Map_Len)
				ball2[i].zPos=-Map_Len;
			if(ball2[i].zPos<-Map_Len)
				ball2[i].zPos=Map_Len;

			ball[i].zPos+=go[i];
			ball[i].yPos=10.0f*3;

			ball2[i].zPos+=go[i]*-1;
			ball2[i].yPos=10.0f*3;
		}
	}
	if(button[2].state == STATE_DEAD) {
		OpenDoor1[2]=1;
	}

}

BOOL RectMap(GLfloat xPos, GLfloat zPos, int type)
{

	switch(type)
	{
	case 0:
		if(xPos>-Map_Width && xPos<Map_Width)
			if(zPos>-Map_Len && zPos<Map_Len)
				return true;

		return false;
	case 1:
		if(xPos>-Map_Width+MapNum[1] && xPos<Map_Width+MapNum[1])
			if(zPos>-Map_Len && zPos<Map_Len)
				return true;

		return false;
		break;
	case 2:
		if(xPos>-Map_Width+MapNum[2] && xPos<Map_Width+MapNum[2])
			if(zPos>-Map_Len && zPos<Map_Len)
				return true;
		return false;
		break;
	case 3:
		int lenX = xPos-MapNum[3];
		int lenZ = zPos-Map_Len;
		int len = lenX*lenX + lenZ*lenZ;
		if(len<(Map_Width*2)*(Map_Width*2))
			return true;
		return false;
		break;
	}
}
BOOL CheckWall(GLfloat *xPos, GLfloat *zPos, int *type, GLfloat *yPos)
{
	int select = *type;
	switch(select)
	{
	case 0:
		if(*xPos>-Map_Width && *xPos<Map_Width)
			if(*zPos>-Map_Len && *zPos<Map_Len)
				return true;
		if(OpenDoor1[0] == 1) {
			if(*xPos >= Map_Width  && ( *zPos>-40 && *zPos<40) ) {
				if(*xPos == Map_Width) {
					*type = 1;
					*xPos= (-Map_Width +MapNum[*type] +30);
					*yPos = 10.0f*3;
				}
				return true;
			}
		}
		if(OpenDoor1[1] == 1) {
			if(*xPos <= -Map_Width && ( *zPos>-40 && *zPos<40) ) {
				if(*xPos == -Map_Width) {
					*type = 2;
					*xPos= (Map_Width +MapNum[*type] -30);
					*yPos = 10.0f*3;
				}
				return true;
			}
		}
		if(*zPos <= -Map_Len  )
			return true;
		if(door_rot>=90) {
			if(OpenDoor1[2] == 1) {
				if(*zPos >= Map_Len  && ( *xPos>-40 && *xPos<40) ) {
					if(*zPos == Map_Len) {
						*type = 3;
						*xPos= (MapNum[*type] +10);
						*zPos *=-1;
						*yPos = 10.0f*3;
					}
					return true;
				}
			}
		}
		return false;
	case 1:
		
		if(Map_yPos[0] <=0) {
			if(*xPos>-Map_Width +MapNum[1] && *xPos<Map_Width +MapNum[1])
				if(*zPos>-Map_Len && *zPos<Map_Len)
					return true;
			if(*xPos >= Map_Width + MapNum[1]  && ( *zPos>-40 && *zPos<40) ) {
				if(*xPos == Map_Width + MapNum[1]) {
					*type = 0;
					*xPos= 0;
					*yPos = 10.0f*3;
					OpenDoor1[0]-=1;
					OpenDoor1[1]+=1;
				}
				return true;
			}

		}
		else
			return true;
	case 2:
		if(*xPos>-Map_Width +MapNum[2] && *xPos<Map_Width +MapNum[2])
			if(*zPos>-Map_Len && *zPos<Map_Len)
				return true;
		if(Map_yPos[1] <=0) {
			if(*xPos <= - Map_Width + MapNum[2]  && ( *zPos>-40 && *zPos<40) ) {
				if(*xPos == - Map_Width + MapNum[2]) {
					*type = 0;
					*xPos= 0;
					*yPos = 10.0f*3;
					OpenDoor1[1]+=1;
					OpenDoor1[2]*=-1;
				}
				return true;
			}
			
		}
		return false;
	case 3:
		return true;
	}
}

