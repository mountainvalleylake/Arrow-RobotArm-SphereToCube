#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/gl.h>
#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double h1,h2,h3,h4,h5;
double a1,a2,a3,a4,a5,a6;
struct point
{
	double x,y,z;
};


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		double z = 100;
		glBegin(GL_LINES);{
			glVertex3f( z,0,0);
			glVertex3f(-z,0,0);

			glVertex3f(0,-z,0);
			glVertex3f(0, z,0);

			glVertex3f(0,0, z);
			glVertex3f(0,0,-z-300);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}
void part1()
{
    glRotatef(a1,0,1,0);
    glRotatef(a2,1,0,0);
    glPushMatrix();
    {
        h1 = -75;
        glTranslatef(0,0,h1);
        glScalef(1,1,5);
        glutWireSphere(15,10,10);
    }
    glPopMatrix();
}
void part2()
{
    h2 = h1-70;
    glTranslatef(0,0,h2);
    glRotatef(a3,0,1,0);
    glPushMatrix();
    {
        glTranslatef(0,0,-60);
        glScalef(1,1,5);
        glutWireSphere(12,10,10);
    }
    glPopMatrix();
}
void part3()
{
    h3 = h2 - 10;
    h4 = 20;
    h5 = -20;
    glTranslatef(0,0,h3);
    glRotatef(a4,0,0,1);
    glPushMatrix();
    {
        glBegin(GL_TRIANGLES);{
		glVertex3f(0,0,40);
		glVertex3f(0,h4,0);
		glVertex3f(0,h5,0);
        }glEnd();
    }
    glPopMatrix();
}
void part4()
{
    glPushMatrix();
    {
        glTranslatef(0,h4,0);
        glRotatef(a5,0,0,1);
        glTranslatef(0,0,-20);
        glScalef(1,1,2);
        glutWireSphere(10,5,5);
    }
    glPopMatrix();
}
void part5()
{
    glPushMatrix();
    {
        glTranslatef(0,h5,0);
        glRotatef(a6,0,1,0);
        glTranslatef(0,0,-20);
        glScalef(1,1,2);
        glutWireSphere(10,5,5);
    }
    glPopMatrix();
}
void drawRobotArm()
{
    part1();
    part2();
    part3();
    part4();
    part5();
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '1':
            if(a1<=45&&a1>=-45){
                a1 += 3;
            }
            if(a1>45){
                a1 = 45;
            }
			break;
        case '2':
            if(a1<=45&&a1>=-45){
                a1 -= 3;
            }
            if(a1<-45){
                a1 = -45;
            }
			break;
        case '3':
            if(a3<=90&&a3>=0){
                a3 += 3;
            }
            if(a3>90){
                a3 = 90;
            }
			break;
        case '4':
            if(a3<=90&&a3>=0){
                a3 -= 3;
            }
            if(a3<0){
                a3 = 0;
            }
			break;
        case '5':
            if(a4<=30&&a4>=-30){
                a4 += 3;
            }
            if(a4>30){
                a4 = 30;
            }
			break;
        case '6':
            if(a4<=30&&a4>=-30){
                a4 -= 3;
            }
            if(a4<-30){
                a4 = -30;
            }
			break;
        case '7':
            if(a5<=180&&a5>=-180){
                a5 += 3;
            }
            if(a5>180){
                a5 = 180;
            }
			break;
        case '8':
            if(a5<=180&&a5>=-180){
                a5 -= 3;
            }
            if(a5<-180){
                a5 = -180;
            }
			break;
        case '9':
            if(a6<=90&&a6>=0){
                a6 += 3;
            }
            if(a6>90){
                a6 = 90;
            }
			break;
        case '0':
            if(a6<=90&&a6>=0){
                a6 -= 3;
            }
            if(a6<0){
                a6 = 0;
            }
			break;
        case 'q':
            if(a2<=90&&a2>=0){
                a2 += 3;
            }
            if(a2>90){
                a2 = 90;
            }
			break;
        case 'w':
            if(a2<=90&&a2>=0){
                a2 -= 3;
            }
            if(a2<0){
                a2 = 0;
            }
			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				//drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,-200,		0,0,1);
	//gluLookAt(100,100,0,	0,0,-100,	0,0,1);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();
    drawRobotArm();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
