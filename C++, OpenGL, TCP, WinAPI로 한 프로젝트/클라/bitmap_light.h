#pragma once

#include <Windows.h>
#include <stdio.h>
#include "global.h"

#define W 24
#define H 24

extern BITMAPINFO *info; // 비트맵 헤더 저장할 변수
extern GLubyte *pBytes; // 데이터를 가리킬 포인터
extern GLfloat Pos[];
extern GLfloat ambi[];

GLubyte * LoadDIBitmap (const char *filename, BITMAPINFO **info);
GLvoid mapping_image(GLuint texture[], int num_texture, const char *filename[]);

GLubyte * LoadDIBitmap (const char *filename, BITMAPINFO **info)
{
	FILE *fp;
	GLubyte *bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;
	// 바이너리 읽기 모드로 파일을 연다
	if ( (fp = fopen (filename, "rb")) == NULL )
		return NULL;
	// 비트맵 파일 헤더를 읽는다.
	if ( fread (&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1 ) {
		fclose(fp);
		return NULL;
	}
	// 파일이 BMP 파일인지 확인한다.
	if ( header.bfType != 'MB' ) {
		fclose(fp);
		return NULL;
	}
	// BITMAPINFOHEADER 위치로 간다.
	infosize = header.bfOffBits - sizeof (BITMAPFILEHEADER);
	// 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.
	if ( (*info = (BITMAPINFO *)malloc(infosize)) == NULL ) {
		fclose(fp);
		exit (0);
		return NULL;
	}
	// 비트맵 인포 헤더를 읽는다.
	if ( fread (*info, 1, infosize, fp) < (unsigned int)infosize ) {
		free (*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵의 크기 설정
	if ( (bitsize = (*info)->bmiHeader.biSizeImage) == 0 )
		bitsize = ( (*info)->bmiHeader.biWidth *
		(*info)->bmiHeader.biBitCount + 7 ) / 8.0 *
		abs((*info)->bmiHeader.biHeight);
	// 비트맵의 크기만큼 메모리를 할당한다.
	if ( (bits = (unsigned char *)malloc(bitsize) ) == NULL ) {
		free (*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵 데이터를 bit(GLubyte 타입)에 저장한다.
	if ( fread(bits, 1, bitsize, fp) < (unsigned int)bitsize ) {
		free (*info); free (bits);
		fclose(fp);
		return NULL;
	}
	fclose (fp);
	return bits;
}

GLvoid mapping_image(GLuint texture[], int num_texture, const char *filename[])
{
	glGenTextures(num_texture, texture);
	for(int i=0; i<num_texture; ++i) {
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		pBytes = LoadDIBitmap(filename[i], &info);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, W, H, 0, GL_BGR_EXT,
			GL_UNSIGNED_BYTE, pBytes);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
}

GLvoid SetLight(GLenum GL_num)
{
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambi);
	glLightfv(GL_num, GL_AMBIENT, ambi);
	glLightfv(GL_num, GL_DIFFUSE, ambi);
	glLightfv(GL_num, GL_SPECULAR, ambi);
	glLightfv(GL_num, GL_POSITION, Pos);
	glEnable(GL_num);
}