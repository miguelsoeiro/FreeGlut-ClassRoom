/*
 * Autores / Authors:
 * Miguel Soeiro, Nº 21502788
 * André Cerveira, Nº 21402048
 * João Batista, Nº 21404080
 *
 * Computação Grafica (CG)
 */

#include <cstdio>
#include <cstdarg>
#include <cmath>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

void display();
void specialKeys();
//Variáveis Globais
GLfloat rotateX = 0.0;
GLfloat rotateY = 0.0;
GLfloat rotateZ = 0.0;
GLfloat zoom = 0.0;
int var = 0;
int i = 0;
static float Angle = 0.0;
int varX = 0;
static unsigned int texture[15]; // Array of texture indices.

struct BitMapFile{
   int sizeX;
   int sizeY;
   unsigned char *data;
};

BitMapFile *getbmp(string filename)
{

    int offset, headerSize;

	// Initialize bitmap files for RGB (input) and RGBA (output).
	BitMapFile *bmpRGB = new BitMapFile;
	BitMapFile *bmpRGBA = new BitMapFile;

	// Read input bmp file name.
	ifstream infile(filename.c_str(), ios::binary);

	if (infile.fail())
		exit(1);


	// Get starting point of image data in bmp file.
	infile.seekg(10);
	infile.read((char *)&offset, 4);

	// Get header size of bmp file.
	infile.read((char *)&headerSize,4);

	// Get image width and height values from bmp file header.
	infile.seekg(18);
	infile.read((char *)&bmpRGB->sizeX, 4);
	infile.read((char *)&bmpRGB->sizeY, 4);

	// Determine the length of zero-byte padding of the scanlines
	// (each scanline of a bmp file is 4-byte aligned by padding with zeros).
	int padding = (3 * bmpRGB->sizeX) % 4 ? 4 - (3 * bmpRGB->sizeX) % 4 : 0;

	// Add the padding to determine size of each scanline.
	int sizeScanline = 3 * bmpRGB->sizeX + padding;

	// Allocate storage for image in input bitmap file.
	int sizeStorage = sizeScanline * bmpRGB->sizeY;
	bmpRGB->data = new unsigned char[sizeStorage];

	// Read bmp file image data into input bitmap file.
	infile.seekg(offset);
	infile.read((char *) bmpRGB->data , sizeStorage);

	// Reverse color values from BGR (bmp storage format) to RGB.
	int startScanline, endScanlineImageData, temp;
	for (int y = 0; y < bmpRGB->sizeY; y++)
	{
       startScanline = y * sizeScanline; // Start position of y'th scanline.
	   endScanlineImageData = startScanline + 3 * bmpRGB->sizeX; // Image data excludes padding.
       for (int x = startScanline; x < endScanlineImageData; x += 3)
	   {
	      temp = bmpRGB->data[x];
		  bmpRGB->data[x] = bmpRGB->data[x+2];
		  bmpRGB->data[x+2] = temp;
	   }
	}

	// Set image width and height values and allocate storage for image in output bitmap file.
	bmpRGBA->sizeX = bmpRGB->sizeX;
	bmpRGBA->sizeY = bmpRGB->sizeY;
	bmpRGBA->data = new unsigned char[4*bmpRGB->sizeX*bmpRGB->sizeY];

	// Copy RGB data from input to output bitmap files, set output A to 1.
	for(int j = 0; j < 4*bmpRGB->sizeY * bmpRGB->sizeX; j+=4)
	{
		bmpRGBA->data[j] = bmpRGB->data[(j/4)*3];
		bmpRGBA->data[j+1] = bmpRGB->data[(j/4)*3+1];
		bmpRGBA->data[j+2] = bmpRGB->data[(j/4)*3+2];
		bmpRGBA->data[j+3] = 0xFF;
	}

	return bmpRGBA;
}
// Load external textures.
void loadExternalTextures(){
   // Local storage for bmp image data.
    BitMapFile *image[10];

    // Load the texture.
    image[0] = getbmp("Textures/bluePlastic.bmp");
   // Activate texture index texture[0].
    glBindTexture(GL_TEXTURE_2D, texture[0]);
   // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   // Specify an image as the texture to be bound with the currently active texture index.
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);

   image[1] = getbmp("Textures/metal.bmp");
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0,
	            GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);

   image[2] = getbmp("Textures/table.bmp");
   glBindTexture(GL_TEXTURE_2D, texture[2]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[2]->sizeX, image[2]->sizeY, 0,
	            GL_RGBA, GL_UNSIGNED_BYTE, image[2]->data);

    image[3] = getbmp("Textures/quadroI.bmp");
   glBindTexture(GL_TEXTURE_2D, texture[3]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[3]->sizeX, image[3]->sizeY, 0,
	            GL_RGBA, GL_UNSIGNED_BYTE, image[3]->data);

    image[4] = getbmp("Textures/quadroII.bmp");
   glBindTexture(GL_TEXTURE_2D, texture[4]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[4]->sizeX, image[4]->sizeY, 0,
	            GL_RGBA, GL_UNSIGNED_BYTE, image[4]->data);

   image[5] = getbmp("Textures/floor.bmp");
   glBindTexture(GL_TEXTURE_2D, texture[5]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[5]->sizeX, image[5]->sizeY, 0,
	            GL_RGBA, GL_UNSIGNED_BYTE, image[5]->data);

     image[6] = getbmp("Textures/white_wall.bmp");
   glBindTexture(GL_TEXTURE_2D, texture[6]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[6]->sizeX, image[6]->sizeY, 0,
	            GL_RGBA, GL_UNSIGNED_BYTE, image[6]->data);

    image[7] = getbmp("Textures/greenwall.bmp");
   glBindTexture(GL_TEXTURE_2D, texture[7]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[7]->sizeX, image[7]->sizeY, 0,
	            GL_RGBA, GL_UNSIGNED_BYTE, image[7]->data);

    image[8] = getbmp("Textures/frote.bmp");
   glBindTexture(GL_TEXTURE_2D, texture[8]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[8]->sizeX, image[8]->sizeY, 0,
	            GL_RGBA, GL_UNSIGNED_BYTE, image[8]->data);

    image[9] = getbmp("Textures/black.bmp");
   glBindTexture(GL_TEXTURE_2D, texture[9]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[9]->sizeX, image[9]->sizeY, 0,
	            GL_RGBA, GL_UNSIGNED_BYTE, image[9]->data);

    image[10] = getbmp("Textures/pc.bmp");
   glBindTexture(GL_TEXTURE_2D, texture[10]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[10]->sizeX, image[10]->sizeY, 0,
	            GL_RGBA, GL_UNSIGNED_BYTE, image[10]->data);
}

//Desenho
void computador(){

    //computador
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    //parte frente e tras pc para texturas
    glBegin(GL_POLYGON);//tras
        glTexCoord2f(0.0, 0.0);glVertex3f(-0.05, 0.00, -0.105);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.25, 0.00, -0.105);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.25, 0.05, -0.105);
        glTexCoord2f(1.0, 0.0);glVertex3f(-0.05, 0.05, -0.105);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glBegin(GL_POLYGON);//frente
        glTexCoord2f(0.0, 0.0);glVertex3f(-0.05, 0.00, 0.105);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.25, 0.00, 0.105);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.25, 0.05, 0.105);
        glTexCoord2f(1.0, 0.0);glVertex3f(-0.05, 0.05, 0.105);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[9]);
    //resto de computador para texturas
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(-0.05, 0.00, 0.10);
        glTexCoord2f(0.0, 1.0);glVertex3f(-0.05, 0.00, -0.10);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.25, 0.00, -0.10);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.25, 0.00, 0.10);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(-0.05, 0.05, 0.10);
        glTexCoord2f(0.0, 1.0);glVertex3f(-0.05, 0.05, -0.10);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.25, 0.05, -0.10);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.25, 0.05, 0.10);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(-0.05, 0.00, 0.10);
        glTexCoord2f(0.0, 1.0);glVertex3f(-0.05, 0.00, -0.10);
        glTexCoord2f(1.0, 1.0);glVertex3f(-0.05, 0.05, -0.10);
        glTexCoord2f(1.0, 0.0);glVertex3f(-0.05, 0.05, 0.10);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(0.25, 0.00, 0.10);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.25, 0.00, -0.10);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.25, 0.05, -0.10);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.25, 0.05, 0.10);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(-0.05, 0.00, -0.10);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.25, 0.00, -0.10);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.25, 0.05, -0.10);
        glTexCoord2f(1.0, 0.0);glVertex3f(-0.05, 0.05, -0.10);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(-0.05, 0.00, 0.10);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.25, 0.00, 0.10);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.25, 0.05, 0.10);
        glTexCoord2f(1.0, 0.0);glVertex3f(-0.05, 0.05, 0.10);
    glEnd();

    //ecra
        //base do ecra
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(0.00, 0.05, 0.05);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.00, 0.05, -0.08);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.20, 0.05, -0.08);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.20, 0.05, 0.05);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(0.00, 0.07, 0.05);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.00, 0.07, -0.08);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.20, 0.07, -0.08);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.20, 0.07, 0.05);;
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(0.00, 0.00, 0.05);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.00, 0.00, -0.08);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.00, 0.07, -0.08);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.00, 0.07, 0.05);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(0.20, 0.00, 0.05);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.20, 0.00, -0.08);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.20, 0.07, -0.08);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.20, 0.07, 0.05);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(0.00, 0.00, -0.08);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.20, 0.00, -0.08);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.20, 0.07, -0.08);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.00, 0.07, -0.08);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(0.00, 0.00, 0.05);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.20, 0.00, 0.05);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.20, 0.07, 0.05);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.00, 0.07, 0.05);
    glEnd();

        //suporte do ecra
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(0.05, 0.07, -0.05);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.05, 0.07, -0.04);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.15, 0.07, -0.04);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.15, 0.07, -0.05);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(0.05, 0.12, 0.00);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.05, 0.12, -0.02);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.15, 0.12, -0.02);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.15, 0.12, 0.00);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(0.05, 0.07, -0.04);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.15, 0.07, -0.04);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.15, 0.12, -0.02);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.05, 0.12, -0.02);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(0.05, 0.07, -0.02);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.15, 0.07, -0.02);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.15, 0.12, 0.00);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.05, 0.12, 0.00);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(0.05, 0.07, -0.04);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.05, 0.07, -0.02);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.05, 0.12, 0.00);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.05, 0.12, -0.02);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(0.15, 0.07, -0.04);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.15, 0.07, -0.02);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.15, 0.12, 0.00);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.15, 0.12, -0.02);
    glEnd();

        //monitor
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(-0.10, 0.09, -0.02);
        glTexCoord2f(0.0, 1.0);glVertex3f(-0.10, 0.24, -0.02);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.30, 0.24, -0.02);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.30, 0.09, -0.02);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(-0.10, 0.09, 0.00);
        glTexCoord2f(0.0, 1.0);glVertex3f(-0.10, 0.24, 0.00);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.30, 0.24, 0.00);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.30, 0.09, 0.00);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(-0.10, 0.09, -0.02);
        glTexCoord2f(0.0, 1.0);glVertex3f(-0.10, 0.24, -0.02);
        glTexCoord2f(1.0, 1.0);glVertex3f(-0.10, 0.24, 0.00);
        glTexCoord2f(1.0, 0.0);glVertex3f(-0.10, 0.09, 0.00);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(0.30, 0.09, -0.02);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.30, 0.24, -0.02);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.30, 0.24, 0.00);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.30, 0.09, 0.00);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerdablack
        glTexCoord2f(0.0, 0.0);glVertex3f(-0.10, 0.09, -0.02);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.30, 0.09, -0.02);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.30, 0.09, 0.00);
        glTexCoord2f(1.0, 0.0);glVertex3f(-0.10, 0.09, 0.00);
    glEnd();
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(-0.10, 0.24, -0.02);
        glTexCoord2f(0.0, 1.0);glVertex3f(0.30, 0.24, -0.02);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.30, 0.24, 0.00);
        glTexCoord2f(1.0, 0.0);glVertex3f(-0.10, 0.24, 0.00);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    //monitor para textura diferente
    glBegin(GL_POLYGON);//lateral esquerda
        glTexCoord2f(0.0, 0.0);glVertex3f(-0.08, 0.10, 0.005);
        glTexCoord2f(0.0, 1.0);glVertex3f(-0.08, 0.23, 0.005);
        glTexCoord2f(1.0, 1.0);glVertex3f(0.28, 0.23, 0.005);
        glTexCoord2f(1.0, 0.0);glVertex3f(0.28, 0.10, 0.005);
    glEnd();

}

void quadroCube()
{
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glBegin(GL_QUADS);
        //glColor3f (0.36, 0.36, 0.36);
      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, 1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, 1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, 1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, 1.0f,  1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, -1.0f, -1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f,  1.0f, 1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f,  1.0f, 1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, 1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, -1.0f, 1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f,  1.0f, -1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f,  1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f(1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();

}

void quadroCubeII()
{
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glBegin(GL_QUADS);
        //glColor3f (0.36, 0.36, 0.36);
      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, 1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, 1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, 1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, 1.0f,  1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, -1.0f, -1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f,  1.0f, 1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f,  1.0f, 1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, 1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, -1.0f, 1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f,  1.0f, -1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f,  1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f(1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();

}

// CADEIRA
void tamposCadeiraCube()
{
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glBegin(GL_QUADS);
        //glColor3f (0, 0, 255);
      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f, 1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, 1.0f, -1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, 1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f( 1.0f, 1.0f,  1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f( 1.0f, -1.0f, -1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f,  1.0f, 1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f,  1.0f, 1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, -1.0f, 1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f( 1.0f, -1.0f, 1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f( 1.0f,  1.0f, -1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f,  1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f,  1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f(1.0f,  1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(1.0f,  1.0f,  1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();

}

void pilaresCadeiraCube()
{
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
        //glColor3f (0.36, 0.36, 0.36);
      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f, 1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, 1.0f, -1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, 1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f( 1.0f, 1.0f,  1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f( 1.0f, -1.0f, -1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f,  1.0f, 1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f,  1.0f, 1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, -1.0f, 1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f( 1.0f, -1.0f, 1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f( 1.0f,  1.0f, -1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f,  1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f,  1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f(1.0f,  1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(1.0f,  1.0f,  1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();

}

// MESAS
void tamposMesaCube()
{
    glBindTexture(GL_TEXTURE_2D, texture[2]);

    glBegin(GL_QUADS);
        //glColor3f (128, 0, 128);
      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f, 1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, 1.0f, -1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, 1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f( 1.0f, 1.0f,  1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f( 1.0f, -1.0f, -1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f,  1.0f, 1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f,  1.0f, 1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, -1.0f, 1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, -1.0f, 1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f( 1.0f,  1.0f, -1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f,  1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f,  1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f(1.0f,  1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(1.0f,  1.0f,  1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();

}

void pilaresMesaCube()
{
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glBegin(GL_QUADS);
        //glColor3f (0.36, 0.36, 0.36);
      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f, 1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, 1.0f, -1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, 1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f( 1.0f, 1.0f,  1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f( 1.0f, -1.0f, -1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f,  1.0f, 1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f,  1.0f, 1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, -1.0f, 1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f( 1.0f, -1.0f, 1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f( 1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f( 1.0f,  1.0f, -1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f,  1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f,  1.0f);

      glTexCoord2f(0.0, 0.0);glVertex3f(1.0f,  1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(1.0f,  1.0f,  1.0f);
      glTexCoord2f(1.0, 1.0);glVertex3f(1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();

}

void chaoCube()
{
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glBegin(GL_QUADS);
      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, 1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, 1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, 1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, 1.0f,  1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, -1.0f, -1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f,  1.0f, 1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f,  1.0f, 1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, 1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, -1.0f, 1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f,  1.0f, -1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f,  1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f(1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();

}

void paredeCube()
{
    glBindTexture(GL_TEXTURE_2D, texture[6]);

    glBegin(GL_QUADS);
      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, 1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, 1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, 1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, 1.0f,  1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, -1.0f, -1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f,  1.0f, 1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f,  1.0f, 1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, 1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, -1.0f, 1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f,  1.0f, -1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f,  1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f(1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();

}

void paredeCubeVerde()
{
    glBindTexture(GL_TEXTURE_2D, texture[7]);

    glBegin(GL_QUADS);
      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, 1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, 1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, 1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, 1.0f,  1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, -1.0f, -1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f,  1.0f, 1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f,  1.0f, 1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, 1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, -1.0f, 1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f,  1.0f, -1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f,  1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f(1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();

}

void fronteCube()
{
    glBindTexture(GL_TEXTURE_2D, texture[8]);

    glBegin(GL_QUADS);
      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, 1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, 1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, 1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, 1.0f,  1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f,  1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, -1.0f, -1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f,  1.0f, 1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f,  1.0f, 1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, 1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, -1.0f, 1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f,  1.0f, -1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f,  1.0f);

      glTexCoord2f(1.0, 1.0);glVertex3f(1.0f,  1.0f, -1.0f);
      glTexCoord2f(0.0, 1.0);glVertex3f(1.0f,  1.0f,  1.0f);
      glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, -1.0f,  1.0f);
      glTexCoord2f(1.0, 0.0);glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();
}

//Esta função serve para desenhar uma cadeira.
void chair(float x, float y, float z)
{
    //PERNAS GRANDES DA CADEIRA
    glPushMatrix();
    glTranslatef( varX+x-2.0f, y-0.1f, z+0.0f);
    glScalef (0.05, 0.9, 0.05);
    pilaresCadeiraCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef( varX+x-2.0f, y-0.1f, z-0.5f);
    glScalef (0.05, 0.9, 0.05);
    pilaresCadeiraCube();
    glPopMatrix();

    // PERNAS PEQUENAS DA CADEIRA
    glPushMatrix();
    glTranslatef( varX+x-1.5f, y-0.5f, z+0.0f);
    glScalef (0.05, 0.5, 0.05);
    pilaresCadeiraCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef( varX+x-1.5f, y-0.5f, z-0.5f);
    glScalef (0.05, 0.5, 0.05);
    pilaresCadeiraCube();
    glPopMatrix();

    // TAMPO DA CADEIRA
    glPushMatrix();
    glTranslatef( varX+x-1.75f, y-0.1f, z-0.25f);
    glScalef (0.35, 0.125, 0.35);
    tamposCadeiraCube();
    glPopMatrix();

    // COSTAS DA CADEIRA
    glPushMatrix();
    glTranslatef( varX+x-2.0f, y+0.55f, z-0.25f);
    glScalef (0.1, 0.3, 0.4);
    tamposCadeiraCube();
    glPopMatrix();
}

void chairProf(float x, float y, float z)
{
    float i;
    //PERNAS GRANDES DA CADEIRA
    glPushMatrix();
    glTranslatef( varX+x-1.5f, y-0.1f, i+z+0.0f);
    glScalef (0.05, 0.9, 0.05);
    pilaresCadeiraCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef( varX+x-1.5f, y-0.1f, i+z-0.5f);
    glScalef (0.05, 0.9, 0.05);
    pilaresCadeiraCube();
    glPopMatrix();

    // PERNAS PEQUENAS DA CADEIRA
    glPushMatrix();
    glTranslatef( varX+x-2.0f, y-0.5f, i+z+0.0f);
    glScalef (0.05, 0.5, 0.05);
    pilaresCadeiraCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef( varX+x-2.0f, y-0.5f, i+z-0.5f);
    glScalef (0.05, 0.5, 0.05);
    pilaresCadeiraCube();
    glPopMatrix();

    // TAMPO DA CADEIRA
    glPushMatrix();
    glTranslatef( varX+x-1.75f, y-0.1f, i+z-0.25f);
    glScalef (0.35, 0.125, 0.4);
    tamposCadeiraCube();
    glPopMatrix();

    // COSTAS DA CADEIRA
    glPushMatrix();
    glTranslatef( varX+x-1.5f, y+0.55f, i+z-0.25f);
    glScalef (0.1, 0.3, 0.325);
    tamposCadeiraCube();
    glPopMatrix();
}

void table(float x, float y, float z)
{
    // TAMPO DA MESA
    glPushMatrix();
    glTranslatef( x-0.0f, y+0.4f, z+0.2f);
    glScalef (0.65, 0.125, 1.25);
    tamposMesaCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef( x-0.0f, y+0.5f, z+0.2f);
    glScalef (2.5, 2.5, 2.5);
    glRotatef(270, 0.0, 1.0, 0.0);
    computador();
    glPopMatrix();

    //PERNAS ESQUERDA DIANTEIRA DA MESA
    glPushMatrix();
    glTranslatef( x-0.5f, y-0.3f, z+1.3f);
    glScalef (0.075, 0.75, 0.075);
    pilaresMesaCube();
    glPopMatrix();

    //PERNAS DIREITA DIANTEIRA DA MESA
    glPushMatrix();
    glTranslatef( x+0.5f, y-0.3f, z+1.3f);
    glScalef (0.075, 0.75, 0.075);
    pilaresMesaCube();
    glPopMatrix();

    //PERNAS DIREITA TRASEIRA DA MESA
    glPushMatrix();
    glTranslatef( x+0.5f, y-0.3f, z-0.9f);
    glScalef (0.075, 0.75, 0.075);
    pilaresMesaCube();
    glPopMatrix();

    //PERNAS ESQUERDA TRASEIRA DA MESA
    glPushMatrix();
    glTranslatef( x-0.5f, y-0.3f, z-0.9f);
    glScalef (0.075, 0.75, 0.075);
    pilaresMesaCube();
    glPopMatrix();
}

void board(float x, float y, float z)
{
    glPushMatrix();
    glTranslatef( x-0.5f, y-0.7f, z-1.5f);
    glScalef (0.1, 0.6, 1.3);
    quadroCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef( x-0.5f, y-0.7f, z+1.5f);
    glScalef (0.1, 0.6, 1.3);
    quadroCubeII();
    glPopMatrix();
}

void chao(float x, float y, float z)
{
    glPushMatrix();
    glTranslatef( x+0.1f, y-2.0f, z-1.5f);
    glScalef (15.1, 0.05, 7.0);
    chaoCube();
    glPopMatrix();
}

void tecto(float x, float y, float z)
{
    glPushMatrix();
    glTranslatef( x+0.05f, y+2.0f, z-1.5f);
    glScalef (15.1, 0.05, 7.0);
    paredeCube();
    glPopMatrix();
}

void paredeVerde(float x, float y, float z)
{
    glPushMatrix();
    glTranslatef( x+0.1f, y+0.0f, z-8.5f);
    glScalef (15.03, 2.0, 0.05);
    paredeCubeVerde();
    glPopMatrix();
}

void fronte(float x, float y, float z)
{
    glPushMatrix();
    glTranslatef( var+x-15.0f, y+0.0f, z-1.5f);
    glScalef (0.15, 2.0, 7.0);
    fronteCube();
    glPopMatrix();
}

void parede(float x, float y, float z)
{
    paredeVerde(var+1 , 1, 1);

    //trazeira
    glPushMatrix();
    glTranslatef( var+x+15.2f, y+0.0f, z-1.5f);
    glScalef (0.05, 2.0, 7.0);
    paredeCube();
    glPopMatrix();

    //frente
    glPushMatrix();
    glTranslatef( var+x-15.0f, y+0.0f, z-1.5f);
    glScalef (0.05, 2.0, 7.0);
    paredeCube();
    glPopMatrix();

    tecto( var+1, 1, 1);

    //lados
    glPushMatrix();
    glTranslatef( var+x+0.1f, y+0.0f, z+5.5f);
    glScalef (15.03, 2.0, 0.05);
    paredeCube();
    glPopMatrix();

}

void display() {

	glMatrixMode (GL_MODELVIEW);
	// Limpa o ecrã e o Z (|GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 4.0, 0.0);
	glScalef(1.0, 2.0, 1.0);
	glFlush();


	glTranslatef(0.0, 0.0, zoom);
	glRotatef(rotateX, 2.0, 0.0, 0.0);
	glRotatef(rotateY, 0.0, 2.0, 0.0);
	glRotatef(rotateZ, 0.0, 0.0, 2.0);

    // Ultima Fila
	chair(-8, 0, 0); // CADEIRA TRASEIRA
    chair(-8, 0, 1); // CADEIRA DIANTEIRA
    table(-8, 0, 0.05); // MESA

    chair(-8, 0, 2.5); // CADEIRA TRASEIRA
    chair(-8, 0, 3.5); // CADEIRA DIANTEIRA
    table(-8, 0, 2.5); // MESA

    chair(-8, 0, -2.8); // CADEIRA TRASEIRA
    chair(-8, 0, -3.8); // CADEIRA DIANTEIRA
    table(-8, 0, -3.65); // MESA


    // Outra Fila
	chair(-4, 0, 0); // CADEIRA TRASEIRA
    chair(-4, 0, 1); // CADEIRA DIANTEIRA
    table(-4, 0, 0.05); // MESA

    chair(-4, 0, 2.5); // CADEIRA TRASEIRA
    chair(-4, 0, 3.5); // CADEIRA DIANTEIRA
    table(-4, 0, 2.5); // MESA

    chair(-4, 0, -2.8); // CADEIRA TRASEIRA
    chair(-4, 0, -3.8); // CADEIRA DIANTEIRA
    table(-4, 0, -3.65); // MESA

    // Outra Fila
	chair(0, 0, 0); // CADEIRA TRASEIRA
    chair(0, 0, 1); // CADEIRA DIANTEIRA
    table(0, 0, 0.05); // MESA

    chair(0, 0, 2.5); // CADEIRA TRASEIRA
    chair(0, 0, 3.5); // CADEIRA DIANTEIRA
    table(0, 0, 2.5); // MESA

    chair(0, 0, -2.8); // CADEIRA TRASEIRA
    chair(0, 0, -3.8); // CADEIRA DIANTEIRA
    table(0, 0, -3.65); // MESA

    // Outra Fila
    chair(4, 0, 0); // CADEIRA TRASEIRA
    chair(4, 0, 1); // CADEIRA DIANTEIRA
    table(4, 0, 0.05); // MESA

    chair(4, 0, 2.5); // CADEIRA TRASEIRA
    chair(4, 0, 3.5); // CADEIRA DIANTEIRA
    table(4, 0, 2.5); // MESA

    chair(4, 0, -2.8); // CADEIRA TRASEIRA
    chair(4, 0, -3.8); // CADEIRA DIANTEIRA
    table(4, 0, -3.65); // MESA

    // Outra Fila
    chair(8, 0, 0); // CADEIRA TRASEIRA
    chair(8, 0, 1); // CADEIRA DIANTEIRA
    table(8, 0, 0.05); // MESA

    chair(8, 0, 2.5); // CADEIRA TRASEIRA
    chair(8, 0, 3.5); // CADEIRA DIANTEIRA
    table(8, 0, 2.5); // MESA

    chair(8, 0, -2.8); // CADEIRA TRASEIRA
    chair(8, 0, -3.8); // CADEIRA DIANTEIRA
    table(8, 0, -3.65); // MESA

    // Primeira Fila
    chair(12, 0, 0); // CADEIRA TRASEIRA
    chair(12, 0, 1); // CADEIRA DIANTEIRA
    table(12, 0, 0.05); // MESA

    chair(12, 0, 2.5); // CADEIRA TRASEIRA
    chair(12, 0, 3.5); // CADEIRA DIANTEIRA
    table(12, 0, 2.5); // MESA

    chair(12, 0, -2.8); // CADEIRA TRASEIRA
    chair(12, 0, -3.8); // CADEIRA DIANTEIRA
    table(12, 0, -3.65); // MESA

    // Sitio do Professor
    table(13.3, 0, -3.65);
    chairProf(16, 0, -2.8);
    board( 16.5, 2, 0);

    //Sala
    chao(1,1,1);

    parede(1,1, 1);
    fronte(1,1,1);

	glFlush();
	glutSwapBuffers();
}

void specialKeys(int key, int x, int y) {
	// VISAO PARA A DIREITA
	if (key == GLUT_KEY_LEFT) rotateY += 2.5;
	// VISAO PARA A ESQUERDA
	if (key == GLUT_KEY_RIGHT) rotateY -= 2.5;
	// VISAO PARA CIMA
	if (key == GLUT_KEY_UP) rotateX += 2.5;
	// VISAO PARA BAIXO
	if (key == GLUT_KEY_DOWN) rotateX -= 2.5;
	// ZOOM
	if (key == GLUT_KEY_PAGE_UP && zoom <= 40.0) zoom += 0.25;
	if (key == GLUT_KEY_PAGE_DOWN&& zoom >= -40.0) zoom -= 0.25;

	if (key == GLUT_KEY_F3) var = 100000000; // para poder sair da vista
	if (key == GLUT_KEY_F4) var = 0;


	// ACTUALIZAR O ECRAN
	glutPostRedisplay();

}

void reshape (int w, int h){

	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 100.0);
	glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}

void setup(void){

    glClearColor(1.0,1.0,1.0,1.0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_AUTO_NORMAL); // Enable automatic normal calculation.
    glGenTextures(12, texture);

    loadExternalTextures();

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

}

void keyInput(unsigned char key, int x, int y){

   switch(key){
		case 'r': rotateX = 0.0;
                 rotateY = 0.0;
                 Angle = 0.0;
                 glutPostRedisplay();
                 break;
		// TECLA ESC CORRESPONDE AO 27 E TEM A FUNCIONALIDADE DE FECHAR A JANELA NESTE CASO
        case 24: exit(0); //http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
				 break;
        default: break;
   }
}

void prints(void){

    cout << "Instrucoes por Parte do Utilizador:\n" << endl;
    cout << "- Setas do Teclado para Rodar a Imagem." << endl;
    cout << "- Lado Direito do Rato para Poder Abrir o Menu." << endl;
    cout << "- Tecla R para fazer Reset a Imagem." << endl;
    cout << "- F3 para Remover paredes e tecto." << endl;
    cout << "- F4 para Voltar a ter Paredes e tecto." << endl;
    cout << "- PageUp e PageDown para ZoomIn e ZoomOut" << endl;
    cout << "Esc para poder Sair!" << endl;
}

void exit(int id){

	if (id==7){
        exit(0);
	}
}

void menu(int id){

	switch(id){
		case 0: rotateX = 0.0; //Trás
				rotateY= 0.0;
				rotateZ = 0.0;
				Angle = 0.0;
				break;
		case 1: rotateX = 0.0; //Frente
				rotateY = 180.0;
				rotateZ = 0.0;
				Angle = 0.0;
				break;
		case 2: rotateX = 90.0; //Cima
				rotateY = 0.0;
				rotateZ = 0.0;
				Angle = 0.0;
				break;
		case 3: rotateX = -90.0; //Baixo
				rotateY = 0.0;
				rotateZ = 0.0;
				Angle = 0.0;
				break;
		case 4: rotateX = 0.0;  //Esquerda
				rotateY = 90.0;
				rotateZ = 0.0;
				Angle = 0.0;
				break;
		case 5: rotateX = 0.0;  //Direita
				rotateY = -90.0;
				rotateZ = 0.0;
				Angle = 0.0;
				break;
        case 6: exit(0);

	}
	glutPostRedisplay();

}

void makeMenu(void){

	// The sub-menu is created first (because it should be visible when the top
	// menu is created): its callback function is registered and menu entries added.
	int vmenu;
	vmenu = glutCreateMenu(menu);
	glutAddMenuEntry("Vista da Parte Traseira.", 0);
	glutAddMenuEntry("Vista da Parte Dianteira.", 1);
	glutAddMenuEntry("Vita da Parte Superior.", 2);
	glutAddMenuEntry("Vista da Parte Inferior.",3);
	glutAddMenuEntry("Vista da Parte Esquerda.",4);
	glutAddMenuEntry("Vista da Parte Direita.",5);
	glutCreateMenu(exit);
	glutAddSubMenu("Vistas", vmenu);
	glutAddMenuEntry("Sair",6);

	//É atribuido o menu à tecla direita do rato
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char* argv []) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sala de Aula");

	setup();
	prints();
	// Função callback
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeys);
	makeMenu();
	// Chama função principal -- permite interação com o usuário
	glutMainLoop();

	return 0;
}
