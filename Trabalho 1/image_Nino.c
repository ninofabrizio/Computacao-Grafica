/*  Departamento de Informatica, PUC-Rio, INF1761 Computer Graphhics
*    
*   @file image.c TAD: digital image (implementation).
*   @author Marcelo Gattass and others
*
*   @date
*         Last version:     09/2015.
*
*   @version 3.1
* 
*   @Copyright/License
*   DI PUC-Rio Educational Software
*   All the products under this license are free software: they can be used for both academic and commercial purposes at absolutely no cost. 
*   There are no paperwork, no royalties, no GNU-like "copyleft" restrictions, either. Just download and use it. 
*   They are licensed under the terms of the MIT license reproduced below, and so are compatible with GPL and also qualifies as Open Source software. 
*   They are not in the public domain, PUC-Rio keeps their copyright. The legal details are below. 
* 
*   The spirit of this license is that you are free to use the libraries for any purpose at no cost without having to ask us. 
*   The only requirement is that if you do use them, then you should give us credit by including the copyright notice below somewhere in your product or its documentation. 
*   
*   Copyright � 2010-2015 DI PUC-Rio Educational Software
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software 
*   without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sub license, and/or sell copies of the Software, and to permit 
*   persons to whom the Software is furnished to do so, subject to the following conditions: 
*   
*   The above copyright notice and this permission notice shall be included in all copies or suavlantial portions of the Software. 
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
*   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
*
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <memory.h>
#include <iup/iup.h>

#include "image_Nino.h"

#define ROUND(_) (int)floor( (_) + 0.5 )
#define N_CORES   256
#define MAIOR_COR 255



struct Image_imp {
   int dcs;        /* define a dim do espaco de cor (dimension of the color space): 3=RGB, 1=luminancia */
   int width;      /* numero de pixels na direcao horizontal da imagem */
   int height;     /* numero de pixels na direcao vertical da imagem   */
   float *buf;      /* vetor de dimensao dcs*width*height que armazena consecutivamente as componentes de cor */
                   /* de cada pixel a partir do canto inferior esquerdo da imagem.  */
                   /* A posicao das componentes de cor do pixel (x,y) fica armazenada */
                   /* a partir da posicao: (y*width*dcs) + (x*dcs)  */
};


/************************************************************************/
/* Definicao das Funcoes Privadas                                       */
/************************************************************************/

/*  getuint e putuint:
* Funcoes auxiliares para ler e escrever inteiros na ordem (lo-hi)
* Note  que  no Windows as variaveis tipo "unsigned short int" sao 
* armazenadas  no disco em dois bytes na ordem inversa. Ou seja, o 
* numero  400, por exemplo, que  pode ser escrito como 0x190, fica 
* armazenado  em dois bytes consecutivos 0x90 e 0x01. Nos sistemas 
* UNIX e Mac este mesmo inteiro seria armazenado  na  ordem 0x01 e 
* 0x90. O armazenamento do Windows e' chamado  de  "little endian"  
* (i.e., lowest-order byte stored first), e  no  sitemas  Unix sao
* "big-endian" (i.e., highest-order byte stored first). 
*/

/***************************************************************************
* Reads an unsigned integer from input                                     *
***************************************************************************/
static int getuint(unsigned short *uint, FILE *input)
{
   int got;
   unsigned char temp[2];
   unsigned short tempuint;

   got = (int) fread(&temp, 1, 2, input);
   if (got != 2) return 0;

   tempuint = ((unsigned short)(temp[1])<<8) | ((unsigned short)(temp[0]));

   *uint = tempuint;

   return 1;
}

/***************************************************************************
* Writes an unsigned integer in output                                     *
***************************************************************************/
static int putuint(unsigned short uint, FILE *output)
{
   int put;
   unsigned char temp[2];

   temp[0] = uint & 0xff;
   temp[1] = (uint >> 8) & 0xff;

   put = (int) fwrite(&temp, 1, 2, output);
   if (put != 2) return 0;

   return 1;
}

/***************************************************************************
* Reads a long integer from input                                          *
***************************************************************************/ 
static int getlong(FILE *input, long int *longint)
{
   int got;
   unsigned char temp[4];
   long int templongint;

   got = (int)fread(&temp, 1, 4, input);
   if (got != 4) return 0;

   templongint = ((long int)(temp[3])<<24) | ((long int)(temp[2])<<16) 
      | ((long int)(temp[1])<<8) | ((long int)(temp[0]));

   *longint = templongint;

   return 1;
}

/***************************************************************************
* Writes a long integer in output                                          *
***************************************************************************/ 
static int putlong(FILE *output, long int longint)
{
   int put;
   unsigned char temp[4];

   temp[0] = (unsigned char)longint & 0xff;
   temp[1] = (unsigned char)(longint >> 8) & 0xff;
   temp[2] = (unsigned char)(longint >> 16) & 0xff;
   temp[3] = (unsigned char)(longint >> 24) & 0xff;

   put = (int)fwrite(&temp, 1, 4, output);

   if (put != 4) return 0;

   return 1;
}

/***************************************************************************
* Reads a word from input                                                  *
***************************************************************************/ 
static int getword(FILE *input, unsigned short int *word)
{
   int got;
   unsigned char temp[2];
   unsigned short int tempword;

   got = (int)fread(&temp, 1, 2, input);
   if (got != 2) return 0;

   tempword = ((unsigned short int)(temp[1])<<8) | ((unsigned short int)(temp[0]));

   *word = tempword;

   return 1;
}

/***************************************************************************
* Writes a word in output                                                  *
***************************************************************************/ 
static int putword(FILE *output, unsigned short int word)
{
   int put;
   unsigned char temp[2];

   temp[0] = word & 0xff;
   temp[1] = (word >> 8) & 0xff;

   put = (int)fwrite(&temp, 1, 2, output);
   if (put != 2) return 0;

   return 1;
}

/***************************************************************************
* Reads a double word from input                                           *
***************************************************************************/ 
static int getdword(FILE *input, unsigned long int *dword)
{
   int got;
   unsigned char temp[4];
   unsigned long int tempdword;

   got = (int)fread(&temp, 1, 4, input);
   if (got != 4) return 0;

   tempdword = ((unsigned long int)(temp[3])<<24) | ((unsigned long int)(temp[2])<<16) 
      | ((unsigned long int)(temp[1])<<8) | ((unsigned long int)(temp[0]));

   *dword = tempdword;

   return 1;
}

/***************************************************************************
* Writes a double word in output                                           *
***************************************************************************/ 
static int putdword(FILE *output, unsigned long int dword)
{
   int put;
   unsigned char temp[4];

   temp[0] = (unsigned char) (dword & 0xff);
   temp[1] = (unsigned char) ((dword >>  8) & 0xff);
   temp[2] = (unsigned char) ((dword >> 16) & 0xff);
   temp[3] = (unsigned char) ((dword >> 24) & 0xff);

   put = (int)fwrite(&temp, 1, 4, output);

   if (put != 4) return 0;

   return 1;
}

static float luminance(float red, float green, float blue)
{
 return 0.2126f*red +0.7152f*green+0.0722f*blue;
}

/***************************************************************************
* Sees if area is hit by the mask for erotion and gives new area	TODO   *
***************************************************************************/ 
void hitErotion(int *area, int *new) {
	
	if(area[0] == 1 && area[1] == 0 && area[2] == 1 ||
		area[0] == 0 && area[1] == 1 && area[2] == 1 ||
		area[0] == 1 && area[1] == 1 && area[2] == 0) {

		new[0] = 1;
		new[1] = 1;
		new[2] = 1;
	}
	else if(area[0] == 1 && area[1] == 0 && area[2] == 0) {

		new[0] = 1;
		new[1] = 1;
		new[2] = 0;
	}
	else if(area[0] == 0 && area[1] == 0 && area[2] == 1) {

		new[0] = 0;
		new[1] = 1;
		new[2] = 1;
	}
	else {

		new[0] = area[0];
		new[1] = area[1];
		new[2] = area[2];
	}
}

/***************************************************************************
* Sees if area is hit by the mask for dilatation and gives new area	 TODO  *
***************************************************************************/ 
void hitDilatation(int *area, int *new) {
	
	if(area[0] == 0 && area[1] == 0 && area[2] == 1 ||
		area[0] == 1 && area[1] == 0 && area[2] == 1 ||
		area[0] == 1 && area[1] == 0 && area[2] == 0) {

		new[0] = 0;
		new[1] = 0;
		new[2] = 0;
	}
	else if(area[0] == 0 && area[1] == 1 && area[2] == 1) {

		new[0] = 0;
		new[1] = 0;
		new[2] = 1;
	}
	else if(area[0] == 1 && area[1] == 1 && area[2] == 0) {

		new[0] = 1;
		new[1] = 0;
		new[2] = 0;
	}
	else {

		new[0] = area[0];
		new[1] = area[1];
		new[2] = area[2];
	}
}

/***************************************************************************
* Goes through all the map to change major for lesser number	 TODO	   *
***************************************************************************/ 
void changeNumber(int **map, int w, int h, int major, int lesser) {

	int i, j;

	for(j = 0; j < h; j++) {
		for(i = 0; i < w; i++) {

			if(map[i][j] == major) 
				map[i][j] = lesser; 
		}
	}
}

/***************************************************************************
* Sees if area has objects and marks them, changing counter if needed TODO *
***************************************************************************/ 
int markAsRead(int **map, int w, int h, int *neighbors, int cont, int x, int y) {

	int i, xn = x, yn = y;
	int objs = cont, lesserNum = 0, foundFirst = -1;

	for(i = 0; i < 3; i++) {

		// he is frontground
		if(neighbors[i] == 0) {

			if(i == 1) {

				xn = x - 1;
				yn = y;
			}
			else if(i == 2) {

				xn = x;
				yn = y - 1;
			}

			if(map[xn][yn] == 0) { // if is not marked yet
				
				if(foundFirst == -1) { // didnt found object in area yet

					foundFirst = 0;
					objs++;
					map[xn][yn] = objs;
					lesserNum = objs;
				}
				else
					map[xn][yn] = lesserNum;
			}
			else if(map[xn][yn] > 0) {

				if(lesserNum == 0) { // first object found in area

					foundFirst = 0;
					lesserNum = map[xn][yn];
				}
				else if(map[xn][yn] < lesserNum) { // new lesser number

					changeNumber(map, w, h, lesserNum, map[xn][yn]); // newer object its older one
					objs--;
					lesserNum = map[xn][yn];
				}
				else if(map[xn][yn] > lesserNum) {

					changeNumber(map, w, h, map[xn][yn], lesserNum);
					objs--;
				}
			}
		}
	}

	return objs;
}

/************************************************************************/
/* Definicao das Funcoes Exportadas                                     */
/************************************************************************/

Image* imgCreate(int w, int h, int dcs)
{
   Image* image = (Image*) malloc (sizeof(Image));
   assert(image);
   image->width  = w;
   image->height = h;
   image->dcs = dcs;
   image->buf = calloc (w * h * dcs , sizeof(float));
   assert(image->buf);
   return image;
}

void imgDestroy(Image* image)
{
   if (image)
   {
      if (image->buf) free (image->buf);
      free(image);
   }
}

Image* imgCopy(Image* image)
{
   int w = imgGetWidth(image);
   int h = imgGetHeight(image);
   int dcs = imgGetDimColorSpace(image);
   Image* img1=imgCreate(w,h,dcs);
   int x,y;
   float rgb[3];

   for (y=0;y<h;y++){
      for (x=0;x<w;x++) {
         imgGetPixel3fv(image,x,y,rgb);
         imgSetPixel3fv(img1,x,y,rgb);
      }
   }

   return img1;
}



Image* imgGrey(Image* image)
{
   int w = imgGetWidth(image);
   int h = imgGetHeight(image);
   Image* img1=imgCreate(w,h,1);
   int x,y;
   float cor[3];

   for (y=0;y<h;y++){
      for (x=0;x<w;x++) {
         imgGetPixel3fv(image,x,y,cor);
		 imgSetPixel3fv(img1, x, y, cor);  /* por img1 so ter um canal a funcao coloca a luminancia*/
      }
   }
   return img1;
}

Image* imgResize(Image* img0, int w1, int h1) 
{
   Image* img1 = imgCreate(w1,h1,img0->dcs);
   float w0 = (float) img0->width;  /* passa para float para fazer contas */
   float h0 = (float) img0->height;

   int x0,y0,x1,y1;
   float color[3];

   for (y1=0;y1<h1;y1++)
      for (x1=0;x1<w1;x1++)
      {
         x0=ROUND(w0*x1/w1);   /* pega a cor do pixel mais proxima */
         y0=ROUND(h0*y1/h1);
         imgGetPixel3fv(img0,x0,y0,color);
         imgGetPixel3fv(img1,x1,y1,color);
      }
      return img1;
}


int imgGetWidth(Image* image)
{
   return image->width;
}

int imgGetHeight(Image* image)
{
   return image->height;
}

int imgGetDimColorSpace(Image* image)
{
   return image->dcs;
}

float* imgGetData(Image* image)
{
   return image->buf;
}

void imgSetPixel3fv(Image* image, int x, int y, float*  color)
{
   int pos = (y*image->width*image->dcs) + (x*image->dcs);
   switch (image->dcs) {
      case 3:
         image->buf[pos  ] = color[0];
         image->buf[pos+1] = color[1];
         image->buf[pos+2] = color[2];
         break;
      case 1:
         image->buf[pos  ] = luminance(color[0],color[1],color[2]);
         break;
      default:
         break;
   }
}

void imgSetPixel3f(Image* image, int x, int y, float R, float G, float B)
{
   int pos = (y*image->width*image->dcs) + (x*image->dcs);
   switch (image->dcs) {
      case 3:
         image->buf[pos  ] = R;
         image->buf[pos+1] = G;
         image->buf[pos+2] = B;
         break;
      case 1:
         image->buf[pos  ] = luminance(R,G,B);
         break;
      default:
         break;
   }
}

void imgGetPixel3fv(Image* image, int x, int y, float* color)
{
   int pos = (y*image->width*image->dcs) + (x*image->dcs);
   switch (image->dcs) {
      case 3:
         color[0] = image->buf[pos  ];
         color[1] = image->buf[pos+1];
         color[2] = image->buf[pos+2];
         break;
      case 1:
         color[0] = image->buf[pos  ];
         color[1] = color[0];
         color[2] = color[0];
         break;
      default:
         break;
   }
}


void imgGetPixel3f(Image* image, int x, int y, float* R, float* G, float* B)
{
   int pos = (y*image->width*image->dcs) + (x*image->dcs);
   switch (image->dcs) {
      case 3:
         *R = image->buf[pos  ];
         *G = image->buf[pos+1];
         *B = image->buf[pos+2];
         break;
      case 1:
         *R = image->buf[pos  ];
         *G = *R;
         *B = *R;
         break;
      default:
         break;
   }
}

void imgSetPixel3ubv(Image* image, int x, int y, unsigned char * color)
{
   int pos = (y*image->width*image->dcs) + (x*image->dcs);
   switch (image->dcs) {
      case 3:
         image->buf[pos  ] = (float)(color[0]/255.);
         image->buf[pos+1] = (float)(color[1]/255.);
         image->buf[pos+2] = (float)(color[2]/255.);
         break;
      case 1:
         image->buf[pos  ] = luminance((float)(color[0]/255.),(float)(color[1]/255.),(float)(color[2]/255.));
         break;
      default:
         break;
   }
}

void imgGetPixel3ubv(Image* image, int x, int y, unsigned char *color)
{
   int pos = (y*image->width*image->dcs) + (x*image->dcs);
   int r,g,b;
   switch (image->dcs) {
      case 3:
         r= ROUND(255*image->buf[pos]);
         g= ROUND (255*image->buf[pos+1]);
         b= ROUND (255*image->buf[pos+2]);
         color[0] = (unsigned char)(r<256) ? r : 255 ;
         color[1] = (unsigned char)(g<256) ? g : 255 ;
         color[2] = (unsigned char)(b<256) ? b : 255 ;
         break;
      case 1:
         r=g=b= ROUND(255*image->buf[pos]);
         color[0] = (unsigned char)(r<256) ? r : 255 ;
         color[1] = (unsigned char)(g<256) ? g : 255 ;
         color[2] = (unsigned char)(b<256) ? b : 255 ;
         break;
      default:
         break;
   }
}


/* Compiler dependent definitions */
typedef unsigned char BYTE;       
typedef unsigned short int USHORT;  
typedef unsigned short int WORD;  
typedef long int LONG;            
typedef unsigned long int DWORD;


Image* imgReadBMP(char *filename)
{
   FILE  *filePtr;            /* ponteiro do arquivo */
   Image*image;            /* imagem a ser criada */
   BYTE *linedata;

   USHORT  bfType;             /* "BM" = 19788           */
   LONG    biWidth;            /* image width in pixels  */
   LONG    biHeight;           /* image height in pixels */
   WORD    biBitCount;         /* bitmap color depth     */
   DWORD   bfSize;

   USHORT  ushortSkip;         /* dado lixo USHORT */
   DWORD   dwordSkip;         /* dado lixo DWORD  */
   LONG    longSkip;         /* dado lixo LONG   */
   WORD    wordSkip;         /* dado lixo WORD   */

   LONG i, j, k, l, linesize, got;

   /* abre o arquivo com a imagem BMP */
   filePtr = fopen(filename, "rb");
   assert(filePtr);

   /* verifica se eh uma imagem bmp */
   getuint(&bfType, filePtr);
   assert(bfType == 19778);

   /* pula os 12 bytes correspondentes a bfSize, Reserved1 e Reserved2 */
   getdword(filePtr, &bfSize);
   getuint(&ushortSkip, filePtr);  /* Reserved1, deve ter valor 0 */
   assert(ushortSkip == 0);
   getuint(&ushortSkip, filePtr);  /* Reserved2, deve ter valor 0 */
   assert(ushortSkip == 0);

   /* pula os 4 bytes correspondentes a bfOffBits, que deve ter valor 54 */
   getdword(filePtr, &dwordSkip);
   assert(dwordSkip == 54);

   /* pula os 4 bytes correspondentes a biSize, que deve ter valor 40 */
   getdword(filePtr, &dwordSkip);
   assert(dwordSkip == 40);

   /* pega largura e altura da imagem */
   getlong(filePtr, &biWidth);
   getlong(filePtr, &biHeight);

   /* verifica que o numero de quadros eh igual a 1 */
   getword(filePtr, &wordSkip);
   assert(wordSkip == 1);

   /* Verifica se a imagem eh de 24 bits */
   getword(filePtr, &biBitCount);
   if(biBitCount != 24)
   {
      fprintf(stderr, "imgReadBMP: Not a bitmap 24 bits file.\n");      
      fclose(filePtr);
      return (NULL);
   }

   /* pula os demais bytes do infoheader */
   getdword(filePtr, &dwordSkip);
   assert(dwordSkip == 0);
   getdword(filePtr, &dwordSkip);
   getlong(filePtr, &longSkip);
   getlong(filePtr, &longSkip);
   getdword(filePtr, &dwordSkip);
   getdword(filePtr, &dwordSkip);

   image = imgCreate(biWidth, biHeight,3);

   /* a linha deve terminar em uma fronteira de dword */
   linesize = 3*image->width;
   if (linesize & 3) {
      linesize |= 3;
      linesize++;
   }

   /* aloca espaco para a area de trabalho */ 
   linedata = (BYTE *) malloc(linesize);
   if (linedata == NULL) {
      fprintf(stderr, "get24bits: Not enough memory.\n");
      return 0;
   }

   /* pega as componentes de cada pixel */
   for (k=0, i=0; i<image->height; i++) {
      got = (unsigned long int)fread(linedata, linesize, 1, filePtr);
      if (got != 1) {
         free(linedata);
         fprintf(stderr, "get24bits: Unexpected end of file.\n");
      }
      for (l=1, j=0; j<image->width; j++, l=l+3) {
         image->buf[k++] = (float)(linedata[l+1]/255.);
         image->buf[k++] = (float)(linedata[l  ]/255.); 
         image->buf[k++] = (float)(linedata[l-1]/255.);
      }
   }

   free(linedata);
   fclose(filePtr);
   return image;
}

int imgWriteBMP(char *filename, Image* bmp)
{
   FILE          *filePtr;         /* ponteiro do arquivo */
   unsigned char *filedata;
   DWORD bfSize;
   int i, k, l;

   int linesize, put;

   if (!bmp) return 0;

   /* cria um novo arquivo binario */
   filePtr = fopen(filename, "wb");
   assert(filePtr);

   /* a linha deve terminar em uma double word boundary */
   linesize = bmp->width * 3;
   if (linesize & 3) {
      linesize |= 3;
      linesize ++;
   }

   /* calcula o tamanho do arquivo em bytes */
   bfSize = 14 +                     /* file header size */
      40 +                     /* info header size */
      bmp->height * linesize;       /* image data  size */

   /* Preenche o cabe�alho -> FileHeader e InfoHeader */
   putuint(19778, filePtr);              /* type = "BM" = 19788                             */
   putdword(filePtr, bfSize);            /* bfSize -> file size in bytes                    */
   putuint(0, filePtr);                  /* bfReserved1, must be zero                       */
   putuint(0, filePtr);                  /* bfReserved2, must be zero                       */
   putdword(filePtr, 54);                /* bfOffBits -> offset in bits to data             */

   putdword(filePtr, 40);                /* biSize -> structure size in bytes                 */
   putlong(filePtr, bmp->width);         /* biWidth -> image width in pixels                  */         
   putlong(filePtr, bmp->height);        /* biHeight -> image height in pixels                */
   putword(filePtr, 1);                  /* biPlanes, must be 1                               */
   putword(filePtr, 24);                 /* biBitCount, 24 para 24 bits -> bitmap color depth */
   putdword(filePtr, 0);                 /* biCompression, compression type -> no compression */
   putdword(filePtr, 0);                 /* biSizeImage, nao eh usado sem compressao          */
   putlong(filePtr, 0);                  /* biXPelsPerMeter                                   */
   putlong(filePtr, 0);                  /* biYPelsPerMeter                                   */
   putdword(filePtr, 0);                 /* biClrUsed, numero de cores na palheta             */
   putdword(filePtr, 0);                 /* biClrImportant, 0 pq todas sao importantes        */

   /* aloca espacco para a area de trabalho */
   filedata = (unsigned char *) malloc(linesize);
   assert(filedata);

   /* a linha deve ser zero padded */
   for (i=0; i<(linesize-(3*bmp->width)); i++) 
      filedata[linesize-1-i] = 0;

   for (k=0; k<bmp->height;k++)
   {
      l = 1;
      /* coloca as componentes BGR no buffer */    
      for (i=0; i<bmp->width; i++) {
         unsigned char color[3];
         int r,g,b;
         imgGetPixel3ubv(bmp,i,k,color);
         r= color[0];
         g= color[1];
         b= color[2];
         filedata[l-1] = (unsigned char)(b<256) ? b : 255 ;
         filedata[l  ] = (unsigned char)(g<256) ? g : 255 ;
         filedata[l+1] = (unsigned char)(r<256) ? r : 255 ;
         l+=3;
      }

      /* joga para o arquivo */
      put = (int)fwrite(filedata, linesize, 1, filePtr);
      if (put != 1) {
         fprintf(stderr, "put24bits: Disk full.");
         free(filedata);
         return 0;
      }
   }

   /* operacao executada com sucesso */
   fprintf(stdout,"imgWriteBMP: %s successfuly generated\n",filename);
   free(filedata);
   fclose(filePtr);
   return 1;
}


/*- PFM Interface Functions  ---------------------------------------*/

Image* imgReadPFM(char *filename) 
{
  FILE *fp;
  Image* img;
  float scale;
  int w,h;

  char line[256];
 
  fp = fopen(filename, "rb");
  if (fp == NULL) {  printf("%s nao pode ser aberto\n",filename); return NULL;}

  fgets(line,256,fp);

  if(strcmp(line,"PF\n")) 
  {
    return 0;
  }

  while (fscanf( fp, " %d ", &w ) != 1)
     fgets(line, 256, fp);
  while (fscanf( fp, " %d ", &h ) != 1)
     fgets(line, 256, fp);
  while (fscanf( fp, " %f", &scale ) != 1)
     fgets(line, 256, fp);

  fgetc(fp);

  img = imgCreate(w,h,3);
  fread( img->buf, 3*w*h, sizeof(float), fp );

   fprintf(stdout,"imgReadPFM: %s successfuly loaded\n",filename);
  fclose(fp);
  return img;
}



int imgWritePFM(char * filename, Image* img)
{
  FILE * fp;
  float  scale=1.f;
 
  if ((fp = fopen(filename, "wb")) == NULL) {
    printf("\nN�o foi possivel abrir o arquivo %s\n",filename);
    return 0;
  }

  /* the ppm file header */
  fprintf(fp,"PF\n%d %d\n%f\n", img->width, img->height, scale);

  fwrite( img->buf, 3*img->width*img->height, sizeof(float), fp );

  fprintf(stdout,"imgWritePFM: %s successfuly created\n",filename);
  fclose(fp);
  return 1;
}

static int comparaCor3(const void * p1, const void * p2)
{
   int *c1 = (int *) p1;  /* aponta para o vermelho quantizado da cor 1 */
   int *c2 = (int *) p2;  /* aponta para o vermelho quantizado da cor 2 */

   /* compara o canal vermelho */
   if (*c1 < *c2) return -1;
   if (*c1 > *c2) return  1;

   /* compara  o canal verde, uma vez que o vermelho e' igual */
   c1++; c2++;
   if (*c1 < *c2) return -1;
   if (*c1 > *c2) return  1;

   /* compara o canal azul, uma vez que o vermelho e o azul sao iguais */
   c1++; c2++;
   if (*c1 < *c2) return -1;
   if (*c1 > *c2) return  1;

   /* sao iguais */
   return 0;
}

static int comparaCor1(const void * p1, const void * p2)
{
   int *c1 = (int *) p1;  /* aponta para a luminosidade quantizada da cor 1 */
   int *c2 = (int *) p2;  /* aponta para a luminosidade quantizada da cor 2 */

   /* compara o canal de luminosidade */
   if (*c1 < *c2) return -1;
   if (*c1 > *c2) return  1;

   /* sao iguais */
   return 0;
}

int imgCountColor(Image * img, float tol)
{
   int numCor = 1;
   int w = imgGetWidth(img);
   int h = imgGetHeight(img);
   int dcs = imgGetDimColorSpace(img);
   float* buf=imgGetData(img);
   int *vet=(int*) malloc(3*w*h*sizeof(int));
   int i;


   /* copia o buffer da imagem no vetor de floats fazendo 
    uma quantizacao para (1/tol) tons de cada componente de cor */
   for (i=0;i<dcs*w*h;i++) 
      vet[i]  = (int)(buf[i]/tol+0.5);

   /* ordena o vetor */
   if (dcs==3) 
      qsort(vet,w*h,3*sizeof(int),comparaCor3);
   else 
      qsort(vet,w*h,sizeof(int),comparaCor1);



   /* conta o numero de cores diferentes */
   if (dcs==3) {
	   int freq=1;
	   printf("%d %d %d ",vet[0],vet[1],vet[2]);
	   for (i=3; i<3*w*h; i+=3){
		  freq++;
		  if (comparaCor3(&vet[i-3],&vet[i])!=0) {
			  printf("%d\n %d %d %d ",freq,vet[i],vet[i+1],vet[i+2]);
			  freq=1;
			  numCor++;
		  }
	   }
		  printf("%d\n",freq);
   } else {
      for (i=1; i<w*h; i+=1)
         if (comparaCor1(&vet[i-1],&vet[i])!=0) numCor++;
   }


   free(vet);
   return numCor;
}




static float apply(float c[9], float v[9])
{
    return c[0]*v[0]+c[1]*v[1]+c[2]*v[2]+c[3]*v[3]+c[4]*v[4]+c[5]*v[5]+c[6]*v[6]+c[7]*v[7]+c[8]*v[8];
}
static float applyV(float c[9], float v[3*9], float out[3])
{
    out[0]= c[0]*v[0*3  ]+c[1]*v[1*3  ]+c[2]*v[2*3  ]+c[3]*v[3*3  ]+c[4]*v[4*3  ]+c[5]*v[5*3  ]+c[6]*v[6*3  ]+c[7]*v[7*3  ]+c[8]*v[8*3  ];
    out[0]= c[0]*v[0*3+1]+c[1]*v[1*3+1]+c[2]*v[2*3+1]+c[3]*v[3*3+1]+c[4]*v[4*3+1]+c[5]*v[5*3+1]+c[6]*v[6*3+1]+c[7]*v[7*3+1]+c[8]*v[8*3+1];
    out[0]= c[0]*v[0*3+2]+c[1]*v[1*3+2]+c[2]*v[2*3+2]+c[3]*v[3*3+2]+c[4]*v[4*3+2]+c[5]*v[5*3+2]+c[6]*v[6*3+2]+c[7]*v[7*3+2]+c[8]*v[8*3+2];
}

static float gauss[9]={1.f/16, 2.f/16, 1.f/16,  2.f/16, 4.f/16, 2.f/16,  1.f/16, 2.f/16, 1.f/16 };
static float sobel_x[9]={-1, 0, 1,  -2, 0, 2,  -1, 0, 1 };
static float sobel_y[9]={1, 2, 1,  0, 0, 0,  -1, -2, -1 };


void imgGauss(Image* img_dst, Image* img_src) 
{
    int w   = imgGetWidth(img_dst);
    int h   = imgGetHeight(img_dst);
    int dcs = imgGetDimColorSpace(img_dst);

    float* src_buffer = imgGetData(img_src);
    float* dst_buffer = imgGetData(img_dst);
    int x,y;


    if (dcs==1) {
        for (y=1;y<h-1;y++){
            for (x=1;x<w-1;x++) {
                int k = y*w+x;
                float v[9] = {src_buffer[k+w-1], src_buffer[k+w],src_buffer[k+w+1],
                    src_buffer[k-1],src_buffer[k],src_buffer[k+1],
                    src_buffer[k-w-1],src_buffer[k-w],src_buffer[k-w+1]};
                    dst_buffer[k] = apply(gauss,v);
            }
        }
    } else {
        for (y=1;y<h-1;y+=1){
            for (x=1;x<w-1;x+=1) {
                int k0 = y*w*3+x*3;
                int k1 = k0+1;
                int k2 = k0+2;
                float r[9] = {src_buffer[k0+3*w-3],src_buffer[k0+3*w],src_buffer[k0+3*w+3],
                    src_buffer[k0-3]    ,src_buffer[k0]    ,src_buffer[k0+3],
                    src_buffer[k0-3*w-3],src_buffer[k0-3*w],src_buffer[k0-3*w+3]};
                float g[9] = {src_buffer[k1+3*w-3],src_buffer[k1+3*w],src_buffer[k1+3*w+3],
                    src_buffer[k1-3]    ,src_buffer[k1]    ,src_buffer[k1+3],
                    src_buffer[k1-3*w-3],src_buffer[k1-3*w],src_buffer[k1-3*w+3]};
                float b[9] = {src_buffer[k2+3*w-3],src_buffer[k2+3*w],src_buffer[k2+3*w+3],
                    src_buffer[k2-3]    ,src_buffer[k2]    ,src_buffer[k2+3],
                    src_buffer[k2-3*w-3],src_buffer[k2-3*w],src_buffer[k2-3*w+3]};

                dst_buffer[k0] = apply(gauss,r);
                dst_buffer[k1] = apply(gauss,g);
                dst_buffer[k2] = apply(gauss,b);
            }
        }
    }
}



/*
* The following routines have been built from knowledge gathered
* around the Web. I am not aware of any copyright problem with
* them, so use it as you want.
* N. Devillard - 1998
*/
typedef float pixelvalue ;
#define PIX_SORT(a,b) { if ((a)>(b)) PIX_SWAP((a),(b)); }
#define PIX_SWAP(a,b) { pixelvalue temp=(a);(a)=(b);(b)=temp; }

/*----------------------------------------------------------------------------
Function : opt_med9()
In : pointer to an array of 9 pixelvalues
Out : a pixelvalue
Job : optimized search of the median of 9 pixelvalues
Notice : in theory, cannot go faster without assumptions on the
signal.
Formula from:
XILINX XCELL magazine, vol. 23 by John L. Smith
The input array is modified in the process
The result array is guaranteed to contain the median
value
in middle position, but other elements are NOT sorted.
---------------------------------------------------------------------------*/
static pixelvalue opt_med9(pixelvalue * p)
{
    PIX_SORT(p[1], p[2]) ; PIX_SORT(p[4], p[5]) ; PIX_SORT(p[7], p[8]) ;
    PIX_SORT(p[0], p[1]) ; PIX_SORT(p[3], p[4]) ; PIX_SORT(p[6], p[7]) ;
    PIX_SORT(p[1], p[2]) ; PIX_SORT(p[4], p[5]) ; PIX_SORT(p[7], p[8]) ;
    PIX_SORT(p[0], p[3]) ; PIX_SORT(p[5], p[8]) ; PIX_SORT(p[4], p[7]) ;
    PIX_SORT(p[3], p[6]) ; PIX_SORT(p[1], p[4]) ; PIX_SORT(p[2], p[5]) ;
    PIX_SORT(p[4], p[7]) ; PIX_SORT(p[4], p[2]) ; PIX_SORT(p[6], p[4]) ;
    PIX_SORT(p[4], p[2]) ; return(p[4]) ;
}


#undef PIX_SORT
#undef PIX_SWAP


void imgMedian(Image* image) 
{
    int w = imgGetWidth(image);
    int h = imgGetHeight(image);
    int dcs = imgGetDimColorSpace(image);
    Image* img = imgCopy(image);  
    float* image_buf = imgGetData(image);
    float* img_buf = imgGetData(img);
    int x,y;

    if (dcs==1) {
        for (y=1;y<h-1;y++){
            for (x=1;x<w-1;x++) {
                int k = y*w+x;
                float v[9] = {img_buf[k+w-1], img_buf[k+w],img_buf[k+w+1],
                    img_buf[k-1],img_buf[k],img_buf[k+1],
                    img_buf[k-w-1],img_buf[k-w],img_buf[k-w+1]};
                image_buf[k]= opt_med9(v);
            }
        }
    } else {
        for (y=1;y<h-1;y+=1){
            for (x=1;x<w-1;x+=1) {
                int k0 = y*w*3+x*3;
                int k1 = k0+1;
                int k2 = k0+2;
                float r[9] = {img_buf[k0+3*w-3],img_buf[k0+3*w],img_buf[k0+3*w+3],
                    img_buf[k0-3]    ,img_buf[k0]    ,img_buf[k0+3],
                    img_buf[k0-3*w-3],img_buf[k0-3*w],img_buf[k0-3*w+3]};
                float g[9] = {img_buf[k1+3*w-3],img_buf[k1+3*w],img_buf[k1+3*w+3],
                    img_buf[k1-3]    ,img_buf[k1]    ,img_buf[k1+3],
                    img_buf[k1-3*w-3],img_buf[k1-3*w],img_buf[k1-3*w+3]};
                float b[9] = {img_buf[k2+3*w-3],img_buf[k2+3*w],img_buf[k2+3*w+3],
                    img_buf[k2-3]    ,img_buf[k2]    ,img_buf[k2+3],
                    img_buf[k2-3*w-3],img_buf[k2-3*w],img_buf[k2-3*w+3]};
                image_buf[k0] = opt_med9(r);
                image_buf[k1] = opt_med9(g);
                image_buf[k2] = opt_med9(b);
            }
        }
    }
    imgDestroy(img);
}

Image* imgEdges(Image* imgIn)
{
    int w = imgGetWidth(imgIn);
    int h = imgGetHeight(imgIn);
    int dcs = imgGetDimColorSpace(imgIn);
    Image* imgOut = imgCreate(w,h,1);
    int x,y;
    Image* img;
    float max=0,inv;

    float* imgOut_buf = imgGetData(imgOut);

    float* img_buf;
    if (dcs==1) 
        img = imgIn;
    else 
        img = imgGrey(imgIn);
    img_buf=imgGetData(img);

    for (y=1;y<h-1;y++){
        for (x=1;x<w-1;x++) {
            int k = y*w+x;
            float v[9] = {img_buf[k+w-1], img_buf[k+w],img_buf[k+w+1],
                img_buf[k-1],img_buf[k],img_buf[k+1],
                img_buf[k-w-1],img_buf[k-w],img_buf[k-w+1]};
            float dx = apply(sobel_x,v);
            float dy = apply(sobel_y,v);
            float val = (float)sqrt(dx*dx+dy*dy);
            max = (max>val)?max:val;
            imgOut_buf[k] = val;
        }
    }

    inv = (max==0)? 1.f : 1.f/max;
    /* arruma a imagem */
    for (y=0;y<h;y++) {
        for (x=0;x<w;x++) {
            int k=y*w+x;
            if ((y==0)||(y==(h-1))||(x==0)||(x==(w-1))) /* borda */
                imgOut_buf[x]=0.f;
            else
                imgOut_buf[x]/=inv;
        }
    }
    return imgOut;
}

Image* imgBinary( Image* greyImage )
{
	int w = imgGetWidth(greyImage);
	int h = imgGetHeight(greyImage);
	Image* imgFinal = imgCreate(w,h,1);
	float  white[3] = { 1, 1, 1 };
	float  black[3] = { 0, 0, 0 };
	float  cor[3];
	int    x, y;

	for (y = 0; y < h; y++)	{
		for (x = 0; x < w; x++) {
			imgGetPixel3fv(greyImage, x, y, cor);
			if (cor[0] >= 0.5f)
				imgSetPixel3fv(imgFinal, x, y, white);
			else
				imgSetPixel3fv(imgFinal, x, y, black);
		}
	}
	return imgFinal;
}

Image * imgChangeBinary(Image *imgBefore) {

	int w = imgGetWidth(imgBefore);
	int h = imgGetHeight(imgBefore);

	Image *imgAfter = imgCreate(w, h, 1);

	float  white[3] = { 1, 1, 1 };
	float  black[3] = { 0, 0, 0 };
	float  cor[3];
	int    x, y;

	for (y = 0; y < h; y++)	{
		for (x = 0; x < w; x++) {

			imgGetPixel3fv(imgBefore, x, y, cor);

			if(cor[0] == 1.0)
				imgSetPixel3fv(imgAfter, x, y, black);
			else
				imgSetPixel3fv(imgAfter, x, y, white);
		}
	}

	return imgAfter;
}

Image * imgErotion(Image *imgBefore) {

	int w = imgGetWidth(imgBefore);
	int h = imgGetHeight(imgBefore);

	Image *imgAfter = imgCreate(w, h, 1);
	Image *tmp;

	float  white[3] = { 1, 1, 1 };
	float  black[3] = { 0, 0, 0 };
	float  pix1[3], pix2[3], pix3[3]; // they represent the area of the mask
	int    x, y;
	int	   new[3], area[3], lastH[3], lastV[3]; // These are 3 pixels each

	for (y = 0; y < (h - 2); y++)	{
		for (x = 0; x < (w - 2); x++) {

			// UP TO DOWN
			imgGetPixel3fv(imgBefore, x, y, pix1);
			imgGetPixel3fv(imgBefore, x, y + 1, pix2);
			imgGetPixel3fv(imgBefore, x, y + 2, pix3);

			area[0] = (int) pix1[0];
			area[1] = (int) pix2[0];
			area[2] = (int) pix3[0];

			hitErotion(area, new);

			if(y == 0 && x == 0) {

				lastV[0] = new[0];
				lastV[1] = new[1];
				lastV[2] = new[2];
			}

			// verifying for not loosing last repaints made
			if(new[0] == 1 && new[0] == lastV[1])
				imgSetPixel3fv(imgAfter, x, y, white);
			else if(new[0] == 0 && new[0] == lastV[1])
				imgSetPixel3fv(imgAfter, x, y, black);

			if(new[1] == 1 && new[1] == lastV[2])
				imgSetPixel3fv(imgAfter, x, y + 1, white);
			else if (new[1] == 0 && new[1] == lastV[2])
				imgSetPixel3fv(imgAfter, x, y + 1, black);
			
			// always the new pixel to paint
			if(new[2] == 1)
				imgSetPixel3fv(imgAfter, x, y + 2, white);
			else
				imgSetPixel3fv(imgAfter, x, y + 2, black);

			lastV[0] = new[0];
			lastV[1] = new[1];
			lastV[2] = new[2];

			// LEFT TO RIGHT
			imgGetPixel3fv(imgBefore, x + 1, y, pix1);
			imgGetPixel3fv(imgBefore, x + 2, y, pix2);
			imgGetPixel3fv(imgBefore, x + 3, y, pix3);

			area[0] = (int) pix1[0];
			area[1] = (int) pix2[0];
			area[2] = (int) pix3[0];

			hitErotion(area, new);

			if(y == 0 && x == 0) {

				lastH[0] = new[0];
				lastH[1] = new[1];
				lastH[2] = new[2];
			}

			// verifying for not loosing last repaints made
			if(new[0] == 1 && new[0] == lastH[1])
				imgSetPixel3fv(imgAfter, x + 1, y, white);
			else if(new[0] == 0 && new[0] == lastH[1])
				imgSetPixel3fv(imgAfter, x + 1, y, black);

			if(new[1] == 1 && new[1] == lastH[2])
				imgSetPixel3fv(imgAfter, x + 2, y, white);
			else if (new[1] == 0 && new[1] == lastH[2])
				imgSetPixel3fv(imgAfter, x + 2, y, black);
			
			if(new[2] == 1)
				imgSetPixel3fv(imgAfter, x + 3, y, white);
			else
				imgSetPixel3fv(imgAfter, x + 3, y, black);
		
			lastH[0] = new[0];
			lastH[1] = new[1];
			lastH[2] = new[2];
		}
	}

	// using first part of erotion as reference
	tmp = imgCopy(imgAfter);

	for (y = h - 1; y > 1; y--)	{
		for (x = w - 1; x > 1; x--) {

			// DOWN TO UP
			imgGetPixel3fv(tmp, x, y - 2, pix1);
			imgGetPixel3fv(tmp, x, y - 1, pix2);
			imgGetPixel3fv(tmp, x, y, pix3);

			area[0] = (int) pix1[0];
			area[1] = (int) pix2[0];
			area[2] = (int) pix3[0];

			hitErotion(area, new);

			if(y == h - 1 && x == w - 1) {

				lastV[0] = new[0];
				lastV[1] = new[1];
				lastV[2] = new[2];
			}

			// verifying for not loosing last repaints made
			if(new[0] == 1)
				imgSetPixel3fv(imgAfter, x, y - 2, white);
			else
				imgSetPixel3fv(imgAfter, x, y - 2, black);

			if(new[1] == 1 && new[1] == lastV[2])
				imgSetPixel3fv(imgAfter, x, y - 1, white);
			else if (new[1] == 0 && new[1] == lastV[2])
				imgSetPixel3fv(imgAfter, x, y - 1, black);
			
			if(new[2] == 1 && new[2] == lastV[1])
				imgSetPixel3fv(imgAfter, x, y, white);
			else if(new[2] == 0 && new[2] == lastV[1])
				imgSetPixel3fv(imgAfter, x, y, black);

			lastV[0] = new[0];
			lastV[1] = new[1];
			lastV[2] = new[2];

			// RIGHT TO LEFT
			imgGetPixel3fv(tmp, x - 3, y, pix1);
			imgGetPixel3fv(tmp, x - 2, y, pix2);
			imgGetPixel3fv(tmp, x - 1, y, pix3);

			area[0] = (int) pix1[0];
			area[1] = (int) pix2[0];
			area[2] = (int) pix3[0];

			hitErotion(area, new);

			if(y == h - 1 && x == w - 1) {

				lastH[0] = new[0];
				lastH[1] = new[1];
				lastH[2] = new[2];
			}

			// verifying for not loosing last repaints made
			if(new[0] == 1)
				imgSetPixel3fv(imgAfter, x - 3, y, white);
			else
				imgSetPixel3fv(imgAfter, x - 3, y, black);

			if(new[1] == 1 && new[1] == lastH[2])
				imgSetPixel3fv(imgAfter, x - 3, y, white);
			else if (new[1] == 0 && new[1] == lastH[2])
				imgSetPixel3fv(imgAfter, x - 2, y, black);
			
			if(new[2] == 1 && new[2] == lastH[1])
				imgSetPixel3fv(imgAfter, x - 1, y, white);
			else if(new[2] == 0 && new[2] == lastH[1])
				imgSetPixel3fv(imgAfter, x - 1, y, black);
		
			lastH[0] = new[0];
			lastH[1] = new[1];
			lastH[2] = new[2];
		}
	}

	return imgAfter;	
}

Image * imgDilatation(Image *imgBefore) {

	int w = imgGetWidth(imgBefore);
	int h = imgGetHeight(imgBefore);

	Image *imgAfter = imgCreate(w, h, 1);
	Image *tmp;

	float  white[3] = { 1, 1, 1 };
	float  black[3] = { 0, 0, 0 };
	float  pix1[3], pix2[3], pix3[3];
	int    x, y;
	int	   new[3], area[3], lastH[3], lastV[3]; // These are 3 pixels each

	for (y = 0; y < (h - 2); y++)	{
		for (x = 0; x < (w - 2); x++) {

			// UP TO DOWN
			imgGetPixel3fv(imgBefore, x, y, pix1);
			imgGetPixel3fv(imgBefore, x, y + 1, pix2);
			imgGetPixel3fv(imgBefore, x, y + 2, pix3);

			area[0] = (int) pix1[0];
			area[1] = (int) pix2[0];
			area[2] = (int) pix3[0];

			hitDilatation(area, new);

			if(y == 0 && x == 0) {

				lastV[0] = new[0];
				lastV[1] = new[1];
				lastV[2] = new[2];
			}

			// verifying for not loosing last repaints made
			if(new[0] == 1 && new[0] == lastV[1])
				imgSetPixel3fv(imgAfter, x, y, white);
			else if(new[0] == 0 && new[0] == lastV[1])
				imgSetPixel3fv(imgAfter, x, y, black);

			if(new[1] == 1 && new[1] == lastV[2])
				imgSetPixel3fv(imgAfter, x, y + 1, white);
			else if (new[1] == 0 && new[1] == lastV[2])
				imgSetPixel3fv(imgAfter, x, y + 1, black);
			
			if(new[2] == 1)
				imgSetPixel3fv(imgAfter, x, y + 2, white);
			else
				imgSetPixel3fv(imgAfter, x, y + 2, black);

			lastV[0] = new[0];
			lastV[1] = new[1];
			lastV[2] = new[2];

			// LEFT TO RIGHT
			imgGetPixel3fv(imgBefore, x + 1, y, pix1);
			imgGetPixel3fv(imgBefore, x + 2, y, pix2);
			imgGetPixel3fv(imgBefore, x + 3, y, pix3);

			area[0] = (int) pix1[0];
			area[1] = (int) pix2[0];
			area[2] = (int) pix3[0];

			hitDilatation(area, new);

			if(y == 0 && x == 0) {

				lastH[0] = new[0];
				lastH[1] = new[1];
				lastH[2] = new[2];
			}

			// verifying for not loosing last repaints made
			if(new[0] == 1 && new[0] == lastH[1])
				imgSetPixel3fv(imgAfter, x + 1, y, white);
			else if(new[0] == 0 && new[0] == lastH[1])
				imgSetPixel3fv(imgAfter, x + 1, y, black);

			if(new[1] == 1 && new[1] == lastH[2])
				imgSetPixel3fv(imgAfter, x + 2, y, white);
			else if (new[1] == 0 && new[1] == lastH[2])
				imgSetPixel3fv(imgAfter, x + 2, y, black);
			
			if(new[2] == 1)
				imgSetPixel3fv(imgAfter, x + 3, y, white);
			else
				imgSetPixel3fv(imgAfter, x + 3, y, black);
		
			lastH[0] = new[0];
			lastH[1] = new[1];
			lastH[2] = new[2];
		}
	}

	// using first part of dilatation as reference
	tmp = imgCopy(imgAfter);

	for (y = h - 1; y > 1; y--)	{
		for (x = w - 1; x > 1; x--) {

			// DOWN TO UP
			imgGetPixel3fv(tmp, x, y - 2, pix1);
			imgGetPixel3fv(tmp, x, y - 1, pix2);
			imgGetPixel3fv(tmp, x, y, pix3);

			area[0] = (int) pix1[0];
			area[1] = (int) pix2[0];
			area[2] = (int) pix3[0];

			hitDilatation(area, new);

			if(y == h - 1 && x == w - 1) {

				lastV[0] = new[0];
				lastV[1] = new[1];
				lastV[2] = new[2];
			}

			// verifying for not loosing last repaints made
			if(new[0] == 1)
				imgSetPixel3fv(imgAfter, x, y - 2, white);
			else
				imgSetPixel3fv(imgAfter, x, y - 2, black);

			if(new[1] == 1 && new[1] == lastV[2])
				imgSetPixel3fv(imgAfter, x, y - 1, white);
			else if (new[1] == 0 && new[1] == lastV[2])
				imgSetPixel3fv(imgAfter, x, y - 1, black);
			
			if(new[2] == 1 && new[2] == lastV[1])
				imgSetPixel3fv(imgAfter, x, y, white);
			else if(new[2] == 0 && new[2] == lastV[1])
				imgSetPixel3fv(imgAfter, x, y, black);

			lastV[0] = new[0];
			lastV[1] = new[1];
			lastV[2] = new[2];

			// RIGHT TO LEFT
			imgGetPixel3fv(tmp, x - 3, y, pix1);
			imgGetPixel3fv(tmp, x - 2, y, pix2);
			imgGetPixel3fv(tmp, x - 1, y, pix3);

			area[0] = (int) pix1[0];
			area[1] = (int) pix2[0];
			area[2] = (int) pix3[0];

			hitDilatation(area, new);

			if(y == h - 1 && x == w - 1) {

				lastH[0] = new[0];
				lastH[1] = new[1];
				lastH[2] = new[2];
			}

			// verifying for not loosing last repaints made
			if(new[0] == 1)
				imgSetPixel3fv(imgAfter, x - 3, y, white);
			else
				imgSetPixel3fv(imgAfter, x - 3, y, black);

			if(new[1] == 1 && new[1] == lastH[2])
				imgSetPixel3fv(imgAfter, x - 3, y, white);
			else if (new[1] == 0 && new[1] == lastH[2])
				imgSetPixel3fv(imgAfter, x - 2, y, black);
			
			if(new[2] == 1 && new[2] == lastH[1])
				imgSetPixel3fv(imgAfter, x - 1, y, white);
			else if(new[2] == 0 && new[2] == lastH[1])
				imgSetPixel3fv(imgAfter, x - 1, y, black);
		
			lastH[0] = new[0];
			lastH[1] = new[1];
			lastH[2] = new[2];
		}
	}

	return imgAfter;	
}

int imgCount(Image *img) {

		int w = imgGetWidth(img);
		int h = imgGetHeight(img);

		// To classify frontground pixels as objects by a number (cont)
		// 0 == frontground pixel not marked; -1 == background pixel; other numbers > 0 number of objects
		int **map;
		int x, y;

		float  pix1[3], pix2[3], pix3[3];

		int cont = 0;

      	map = (int **) malloc(w * sizeof(int *));

      	for (x = 0; x < w; x++)
	        map[x] = (int *) malloc(h * sizeof(int));

		for(y = 0; y < h; y++)
			for(x = 0; x < w; x++)
				map[x][y] = 0; // mark everyone as frontground

		for (y = 0; y < h; y++)	{
			for (x = 0; x < w; x++) {

				// in this case, -1 means its background or out of the image
				int neighbors[3] = {0, 0, 0};

				imgGetPixel3fv(img, x, y, pix1);

				// current pixel must be frontground
				if(pix1[0] == 0) {

					neighbors[0] = 0;

					// Marking neighbor A
					if((x - 1) > 0) {

						imgGetPixel3fv(img, x - 1, y, pix2);
						if(pix2[0] == 1) {

							neighbors[1] = -1;

							if(map[x - 1][y] == 0)
								map[x - 1][y] = -1;
						}
					}
					else
						neighbors[1] = -1;
					// Marking neighbor C
					if((y - 1) > 0) {

						imgGetPixel3fv(img, x, y - 1, pix3);
						if(pix3[0] == 1) {

							neighbors[2] = -1;

							if(map[x][y - 1] == 0)
								map[x][y - 1] = -1;
						}
					}
					else
						neighbors[2] = -1;

					cont = markAsRead(map, w, h, neighbors, cont, x, y);
				}
			}
		}

		for(x = 0; x < w; x++)
        	free(map[x]);
      
		free(map);

		return cont;
}
