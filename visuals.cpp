#include <stdio.h>     // - Just for some ASCII messages
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "visuals.h"   // Header file for our OpenGL functions
#include <stdlib.h>
#include <time.h>
#include "GL/glut.h"
  // - An interface and windows management library

model md;

#define MAX_STARS 2000
#define MAX 100


static float tx = 0.0;
static float ty = 0.0;
static float tz = 0.0;
static float planet_rotate = 0.0;
static float rotx[2];
static bool animate = true;
static bool up = true;
static double sun_radius = 5;
double stars_positions[MAX_STARS][3];


void DrawAxes()
{
	glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_LINES);
	glVertex2f(0.0, 0.0);
	glVertex2f(100.0, 0.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 100.0);
	glEnd();
}

void DisplayStars() {//create the stars

	for (int i = 0; i < MAX_STARS; i++) {
		glPushMatrix();
		glTranslatef(stars_positions[i][0], stars_positions[i][1], stars_positions[i][2]);   //move them to the ramdom x,y,z position of array

		glColor3f(1.0, 1.0, 1.0);// Set drawing colour white
		glutSolidSphere(0.05, 30, 24);

		glPopMatrix();

	}

}

using namespace std;

void Render()
{
  //CLEARS FRAME BUFFER ie COLOR BUFFER& DEPTH BUFFER (1.0)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  glTranslatef(0, 0, -20);
  glTranslatef(tx, 0.0, 0.0);	//rotate the camera from keyboard interrupt
  glTranslatef(0.0, ty, 0.0);
  glTranslatef(0, 0, -20);


  DrawAxes();

  glClear(GL_COLOR_BUFFER_BIT);
  DisplayStars();//create stars





  glPushMatrix();
	glTranslatef(0, 0, -15);
	glRotatef(rotx[0], 0, 1, 0);
	glTranslatef(0, 0, 9);
	glColor3f(0.1, 0.1, 0.8);                            // Set drawing colour
	displayPlanet(md, 0, 0, 0,0.004);	//create earth


	glRotatef(rotx[1], 1, 0, 0);			//so the moon rotates around earth
	glTranslatef(-1, -1, -17);
	glTranslatef(0, -1, 20);


	glColor3f(1, 1, 1);                            // Set drawing colour
	displayPlanet(md, 0, 0, 0, 0.001);		//create moon


 glPopMatrix();
 DisplaySun();//create sun

  glutSwapBuffers();             // All drawing commands applied to the
                                 // hidden buffer, so now, bring forward
                                 // the hidden buffer and hide the visible one
}

//-----------------------------------------------------------

void Resize(int w, int h)
{
  // define the visible area of the window ( in pixels )
  if (h==0) h=1;
  glViewport(0,0,w,h);

  // Setup viewing volume

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(60.0, (float)w/(float)h, 1.0, 500.0);
}

void Idle()
{
	if(animate){			//if there's not an I/O interrupt

		//modify the halo around the sun
		if ( (sun_radius < 5.9) && up){
			sun_radius += 0.03;
		}
		else if( (sun_radius > 5 ) && !up ){
			sun_radius -= 0.03;

		}else if (sun_radius >= 5.9 && up){
			up = false;
			sun_radius -= 0.03;

		}
		else if (sun_radius <= 5 && !up) {
			up = true;
			sun_radius += 0.03;
		}

		planet_rotate += 4.0; //speed of planet's rotation

		rotx[0] += 1.2;//increase the speed of planets (for earth it's 1.2)
		rotx[1] += 1.2 + 9.0;
	}

	glutPostRedisplay();
}



void Keyboard(unsigned char key,int xx,int xy)
{
	switch (key)
	{
	case 'q': exit(0);
		break;
	case 'a': tx += 0.5f;
		break;
	case 'd': tx -= 0.5f;
		break;
	case 'w': ty -= 0.5f;
		break;
	case 's': ty += 0.5f;
		break;
	default: break;
	}
	glutPostRedisplay();

}

void Mouse(int button,int state,int x,int y)
{
	 if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	 {
		 animate = !animate;
         glutPostRedisplay();
	 }
}


void Setup()
{
	rotx[0] = 0.0; //initialize the value of rotation of earth
	rotx[1] = 0.0; //initialize the value of rotation of moon
	srand(time(NULL));

	for (int i = 0; i < MAX_STARS; i++) {

		stars_positions[i][0] = (rand() % 200) - MAX;			//generate  random number x from -100 to 100
		stars_positions[i][1] = (rand() % 200) - MAX;			//generate  random number y from -100 to 100
		stars_positions[i][2] = (rand() % 200) - MAX;			//generate  random number z from -100 to 100

	}

	ReadFile(&md);
	glShadeModel(GL_FLAT);


	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_NORMALIZE);


	//Parameter handling
	glShadeModel (GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1);

	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//set up transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//Set up light source
	GLfloat light_position[] = { 0.0, 0.0, 15.0, 0.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position);

	GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };


	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glFrontFace(GL_CW);


}

void DisplaySun() {
	glPushMatrix();
		glTranslatef(0, 0, -15);

		glColor3f(1.0, 1.0, 0.0);							   // Sun
		glutSolidSphere(5, 200, 20);

		double opacity = (5.9 - sun_radius) / 0.9 + 0.15;
		glColor4f(1, 0.90, 0.0, opacity);							   // Radiation
		glutSolidSphere(sun_radius, 200, 20);
	glPopMatrix();

}

void ReadFile(model *md)
{


	FILE * file = fopen("planet.obj", "r");
	if (file == NULL) {
		printf("Error opening file\n");
		return;
	}

	while (1) {

		char lineHeader[1024];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0) {
			point vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			md->obj_points.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			point normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			md->normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			face vertexIndex, normalIndex;
			int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex.vtx[0], &normalIndex.vtx[0], &vertexIndex.vtx[1], &normalIndex.vtx[1], &vertexIndex.vtx[2], &normalIndex.vtx[2]);
			if (matches != 6) {

				return;
			}

			md->vertexIndices.push_back(vertexIndex);

			md->normalIndices.push_back(normalIndex);
		}
	}

	 fclose(file);
}




void displayPlanet(model md, double x, double y, double z, float size){
	glPushMatrix();
	glTranslatef(x,y,z);

	glScalef(size, size, size);
	glRotatef(planet_rotate, 1, 0, 0);  //planet's rotation

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < md.vertexIndices.size(); i++)
	{
		glVertex3f(md.obj_points[md.vertexIndices[i].vtx[0]-1].x,md.obj_points[md.vertexIndices[i].vtx[0]-1].y,md.obj_points[md.vertexIndices[i].vtx[0]-1].z);
		glVertex3f(md.obj_points[md.vertexIndices[i].vtx[1]-1].x,md.obj_points[md.vertexIndices[i].vtx[1]-1].y,md.obj_points[md.vertexIndices[i].vtx[1]-1].z);
		glVertex3f(md.obj_points[md.vertexIndices[i].vtx[2]-1].x,md.obj_points[md.vertexIndices[i].vtx[2]-1].y,md.obj_points[md.vertexIndices[i].vtx[2]-1].z);

		glNormal3f(md.obj_points[md.normalIndices[i].vtx[0]-1].x,md.obj_points[md.normalIndices[i].vtx[0]-1].y,md.obj_points[md.normalIndices[i].vtx[0]-1].z);
		glNormal3f(md.obj_points[md.normalIndices[i].vtx[1]-1].x,md.obj_points[md.normalIndices[i].vtx[1]-1].y,md.obj_points[md.normalIndices[i].vtx[1]-1].z);
		glNormal3f(md.obj_points[md.normalIndices[i].vtx[2]-1].x,md.obj_points[md.normalIndices[i].vtx[2]-1].y,md.obj_points[md.normalIndices[i].vtx[2]-1].z);
	}

	glEnd();
	glPopMatrix();
}
