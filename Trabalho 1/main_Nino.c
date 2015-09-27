/**
 *  @file IUPGL_image.c Simple example of use of OpenGL and IUP to for image processing (Paint) programs.
 * 
 *  Creates a dialog with a tool bar, a canvas and a msg label bar.
 *  It is a simple WIMP program that paints a image into a IUP canvas using OpenGL.
 *  It shows how to handle mouse events and messages and a simple image process function.
 *
 *  Compile this program with:
 *  iups.c and iups.h     <- layer to simply (reduce the number of calls) of the Iup interface
 *  image.c  and image.h  <- simple ADT (Abtract Data Type) for a digital image 
 *
 *  Button images must be available in:
 *  "img/file_open.bmp"
 *  "img/blue_flag.bmp"
 *  "img/file_save.bmp"
 *  "img/green_flag.bmp" 
 *
 *  Link this program with: 
 *  iup.lib;iupgl.lib;comctl32.lib;opengl32.lib;glu32.lib;
 *
 *  You can find iup libs (iup.lib; iupgl.lib;) at:
 *  http://www.tecgraf.puc-rio.br/iup/
 *  the others lib (comctl32.lib;opengl32.lib;glu32.lib;) are inluded in the Visual Studio 
 *
 *  Do not forget to put the path for the include and lib in the project file.
 *  Here we use: c:\tec\inc and c:\tec\lib
 * 
 *  Ignore: libcmt.lib;
 *
 *  Last modification:  Nino Tiriticco, 3set2015.
 *
 **/
/*- Include lib interfaces: ANSI C, IUP and OpenGL ------*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "iups.h"

#include <GL/gl.h>     /* OpenGL functions*/
#include <GL/glu.h>    /* OpenGL utilitary functions*/
#include "image_Nino.h"     /* interface of the image processing functions */

/*- Program context variables(declared as globals to facilitate the comunication in the callback model)-----------*/
struct global_context {
	Ihandle* dialog;                    /* dialog (main window) */
	Ihandle* canvas;                    /* canvas handle */
	Ihandle* msgbar;                    /* message bar  handle */
	Image*   image;                     /* image to be painted in the canvas*/
	int      width,height;              /* width and height of the canvas  */
} gc = { NULL, NULL, NULL, NULL, 640, 480 };

/*- Copy of the gc's image to be loaded----TODO---*/
Image* image_copy = NULL;

/*------------------------------------------*/
/* IUP Callbacks                            */
/*------------------------------------------*/

/* function called when the canvas is exposed in the screen.  self is the iuphandle of the canvas */
int repaint_cb(Ihandle *self) 
{
  int x,y;
  IupGLMakeCurrent(self);                /* all OpenGL primitives will be directed to this canvas */
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);  /* dark grey color in the RGBA mode A is the alpha channel (ignore) */
  glClear(GL_COLOR_BUFFER_BIT);          /* clear the color buffer */

  if (gc.image!=NULL) 
  {
	  int h = imgGetHeight(gc.image);
	  int w = imgGetWidth(gc.image);
	  /* assing to each pixel of the canvas the color of the corresponding pixel in the image */
	  glBegin(GL_POINTS);   
	  for (y=0;y<h;y++) {
		  for (x=0;x<w;x++) {
			  float r,g,b;
			  imgGetPixel3f(gc.image,x,y,&r,&g,&b); /* gets the RGB value the pixel (x,y) */ 
			  glColor3f(r,g,b);        /* define a current color in OpenGL */
			  glVertex2i(x,y);         /* paint the pixel */
		  }

	  }
	  glEnd();
  }
  IupGLSwapBuffers(self);  /* change the back buffer with the front buffer */
  return IUP_DEFAULT; /* returns the control to the main loop */
}


/* function called in the event of changes in the width or in the height of the canvas */
int resize_cb(Ihandle *self, int new_width, int new_height)
{
 IupGLMakeCurrent(self);  /* Make the canvas current in OpenGL */

 /* define the entire canvas as the viewport  */
 glViewport(0,0,new_width,new_height);

 /* transformation applied to each vertex */
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();           /* identity, i. e. no transformation */

 /* projection transformation (orthographic in the xy plane) */
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluOrtho2D (0.0, (GLsizei)(new_width), 0.0, (GLsizei)(new_height));  /* window of interest [0,w]x[0,h] */

 /* update canvas size and repaint */
 gc.width=new_width;
 gc.height=new_height;
 repaint_cb(gc.canvas);

 return IUP_DEFAULT; /* return to the IUP main loop */
}

static char* strip_path_from_filename(char* file_name) {
	char* p=file_name;
	while (*p!='\0') p++; /* go to the end of string */     
	while ( (p>file_name) && (*p!='\\')) p--;  /* return up to '\' */
	return (*p=='\\')?p+1:p;
}

int load_cb(void)
{
	char* file_name = IupSelectFile("File Selection","*.bmp","Load a BMP file");
    if (!file_name) {
        IupSetfAttribute(gc.msgbar, "TITLE", "Selection failed"); 
		return IUP_DEFAULT;
	}

	if (gc.image!=NULL) imgDestroy(gc.image);
   
	gc.image=imgReadBMP(file_name);  /* loads the image */
	if (gc.image!=NULL) {
	   gc.width = imgGetWidth(gc.image);
	   gc.height = imgGetHeight(gc.image);
	   IupCanvasResize(gc.canvas,gc.dialog,gc.width,gc.height);
	   repaint_cb(gc.canvas);
       IupSetfAttribute(gc.msgbar, "TITLE", "%s", strip_path_from_filename(file_name));

	} else
	   IupSetfAttribute(gc.msgbar, "TITLE", "Can't open %s", strip_path_from_filename(file_name)); 

	return IUP_DEFAULT;
}

int binary_cb(void)
{
	Image* tmp = gc.image;
	if (tmp != NULL){
		IupSetfAttribute(gc.msgbar, "TITLE", "Grey scale image ...");
		gc.image = imgGrey(tmp);
		repaint_cb(gc.canvas);   /* repaint canvas */
		imgDestroy(tmp);
		tmp = gc.image;
		gc.image = imgBinary(tmp);
		repaint_cb(gc.canvas);   /* repaint canvas */
		imgDestroy(tmp);
		IupSetfAttribute(gc.msgbar, "TITLE", "Binary image ...");
	}
     return IUP_DEFAULT;
}

// TODO

int change_cb(void) {

	Image *tmp = gc.image;
	
	if(tmp == NULL || imgGetDimColorSpace(tmp) != 1) {

		IupSetfAttribute(gc.msgbar, "TITLE", "Image doesn't exist or not binary");
		return IUP_DEFAULT;
	}

	IupSetfAttribute(gc.msgbar, "TITLE", "Changing binary colors ...");
	gc.image = imgChangeBinary(tmp);
	repaint_cb(gc.canvas);   /* repaint canvas */
	imgDestroy(tmp);
	IupSetfAttribute(gc.msgbar, "TITLE", "Binary colors changed ...");

	return IUP_DEFAULT;
}

// TODO

int erode_cb(void)
{
	Image *tmp = gc.image;
	
	if(tmp == NULL || imgGetDimColorSpace(tmp) != 1) {

		IupSetfAttribute(gc.msgbar, "TITLE", "Image doesn't exist or not binary");
		return IUP_DEFAULT;
	}

	IupSetfAttribute(gc.msgbar, "TITLE", "Image eroding ...");
	gc.image = imgErotion(tmp);
	repaint_cb(gc.canvas);   /* repaint canvas */
	imgDestroy(tmp);
	IupSetfAttribute(gc.msgbar, "TITLE", "Image eroded ...");

	return IUP_DEFAULT;
}


// TODO

int dilate_cb(void)
{
	Image *tmp = gc.image;
	
	if(tmp == NULL || imgGetDimColorSpace(tmp) != 1) {

		IupSetfAttribute(gc.msgbar, "TITLE", "Image doesn't exist or not binary");
		return IUP_DEFAULT;
	}

	IupSetfAttribute(gc.msgbar, "TITLE", "Image dilatating ...");
	gc.image = imgDilatation(tmp);
	repaint_cb(gc.canvas);   /* repaint canvas */
	imgDestroy(tmp);
	IupSetfAttribute(gc.msgbar, "TITLE", "Image dilatated ...");

	return IUP_DEFAULT;
}

// TODO

int count_cb(void) {
	
	int num;

	if(gc.image == NULL || imgGetDimColorSpace(gc.image) != 1) {

		IupSetfAttribute(gc.msgbar, "TITLE", "Image doesn't exist or not binary");
		return IUP_DEFAULT;
	}

	num = imgCount(gc.image);

	IupSetfAttribute(gc.msgbar, "TITLE", "Objects in this picture: %d", num);

	return IUP_DEFAULT;
}

int save_image_cb(void)
{
	if(gc.image == NULL) {

		IupSetfAttribute(gc.msgbar, "TITLE", "There's no image to save");
		return IUP_DEFAULT;
	}

	image_copy = imgCopy(gc.image);
	IupSetfAttribute(gc.msgbar, "TITLE", "State of the image saved"); 

    return IUP_DEFAULT;
}

// TODO

int load_image_cb(void) {

	if(image_copy == NULL) {

		IupSetfAttribute(gc.msgbar, "TITLE", "There's no copy of image saved");
		return IUP_DEFAULT;
	}

	gc.image = imgCopy(image_copy);
	repaint_cb(gc.canvas);
	IupSetfAttribute(gc.msgbar, "TITLE", "Last state saved is loaded");

	return IUP_DEFAULT;
}

int exit_cb(void)
{
     printf("Function to free memory and do finalizations...\n");
     return IUP_CLOSE;
}


/*-------------------------------------------------------------------------*/
/* Incializa o programa.                                                   */
/*-------------------------------------------------------------------------*/

Ihandle* InitToolbar(void)
{
     Ihandle* toolbar;

    /* Create four buttons */
     Ihandle* load = IupSButton("img/file_open.bmp","load a BMP image file",(Icallback) load_cb);
     Ihandle* binary = IupSButton("img/binary.bmp","convert RGB to binary image callback function",(Icallback)binary_cb);
	 Ihandle* change = IupSButton("img/blue_flag.bmp","change binary colors",(Icallback)change_cb);
	 Ihandle* erode = IupSButton("img/erode.bmp", "do erotion in the binary image", (Icallback)erode_cb);
	 Ihandle* dilate = IupSButton("img/dilate.bmp", "do dilatation in the binary image", (Icallback)dilate_cb);
	 Ihandle* count = IupSButton("img/count.bmp", "give number of objects in this image", (Icallback)count_cb);
	 Ihandle* save_image = IupSButton("img/file_save.bmp", "save current state of the image", (Icallback)save_image_cb);
	 Ihandle* load_image = IupSButton("img/pencil.bmp", "load last state of image saved", (Icallback)load_image_cb);

     toolbar=IupHbox(load,binary,change,erode,dilate,count,save_image,load_image, NULL);

     return toolbar;
}

Ihandle* InitDialog(void)
{
  Ihandle* dialog;   /* dialog containing the canvas */
  Ihandle* content;  /* dialog content */

  Ihandle* toolbar=InitToolbar();                                                 /* buttons tool bar */
  gc.canvas = IupSGLCanvas(640,480,(Icallback) repaint_cb, (Icallback) resize_cb);   /* canvas to paint with OpenGL */ 
  gc.msgbar = IupSMessageBar("Message...",20);     /* a msg bar */

  content = IupVbox(toolbar,gc.canvas,gc.msgbar,NULL);

  /* create the dialog and set its attributes */
  dialog = IupSDialog("Trabalho 1 - Nino",content, (Icallback) exit_cb, "NO");
 
  return dialog;
}

/*-----------------------*/
/* Main function.        */
/*-----------------------*/
int main(int argc, char* argv[]) 
{
     IupSOpen(&argc, &argv);                      /* opens the IUP lib */

     gc.dialog = InitDialog();                      /* local function to create a dialog with buttons and canvas */
     IupShowXY(gc.dialog, IUP_CENTER, IUP_CENTER);  /* shows dialog in the center of screen */

     IupMainLoop();                        /* handle the program control to the IUP lib until a return IUP_CLOSE */

     IupClose();                           /* closes the IUP lib */ 
}
