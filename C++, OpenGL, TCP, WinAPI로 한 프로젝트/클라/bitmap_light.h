#pragma once

#include <Windows.h>
#include <stdio.h>
#include "global.h"

#define W 24
#define H 24

extern BITMAPINFO *info; // ��Ʈ�� ��� ������ ����
extern GLubyte *pBytes; // �����͸� ����ų ������
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
	// ���̳ʸ� �б� ���� ������ ����
	if ( (fp = fopen (filename, "rb")) == NULL )
		return NULL;
	// ��Ʈ�� ���� ����� �д´�.
	if ( fread (&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1 ) {
		fclose(fp);
		return NULL;
	}
	// ������ BMP �������� Ȯ���Ѵ�.
	if ( header.bfType != 'MB' ) {
		fclose(fp);
		return NULL;
	}
	// BITMAPINFOHEADER ��ġ�� ����.
	infosize = header.bfOffBits - sizeof (BITMAPFILEHEADER);
	// ��Ʈ�� �̹��� �����͸� ���� �޸� �Ҵ��� �Ѵ�.
	if ( (*info = (BITMAPINFO *)malloc(infosize)) == NULL ) {
		fclose(fp);
		exit (0);
		return NULL;
	}
	// ��Ʈ�� ���� ����� �д´�.
	if ( fread (*info, 1, infosize, fp) < (unsigned int)infosize ) {
		free (*info);
		fclose(fp);
		return NULL;
	}
	// ��Ʈ���� ũ�� ����
	if ( (bitsize = (*info)->bmiHeader.biSizeImage) == 0 )
		bitsize = ( (*info)->bmiHeader.biWidth *
		(*info)->bmiHeader.biBitCount + 7 ) / 8.0 *
		abs((*info)->bmiHeader.biHeight);
	// ��Ʈ���� ũ�⸸ŭ �޸𸮸� �Ҵ��Ѵ�.
	if ( (bits = (unsigned char *)malloc(bitsize) ) == NULL ) {
		free (*info);
		fclose(fp);
		return NULL;
	}
	// ��Ʈ�� �����͸� bit(GLubyte Ÿ��)�� �����Ѵ�.
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