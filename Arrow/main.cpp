#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include<GL/gl.h>
#include<GL/glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point3D
{
	double x,y,z;
};
struct vect
{
    double x,y;
};
struct point2D
{
    double x,y;
};
struct point2D pos;
struct vect v,m;
struct vect perpVector(struct vect v)
{
    struct vect w;
    w.x = (-1)*v.y;
    w.y = v.x;
    return w;
}
struct vect rotateVector(struct vect v,double a)
{
    struct vect w,p;
    p = perpVector(v);
    w.x = v.x * cos(a) + p.x * sin(a);
    w.y = p.y * sin(a) + v.y * cos(a);
    printf("Rotated: %lf %lf\n",w.x,w.y);
    return w;
}
struct vect addVector(struct vect v1,struct vect v2)
{
    struct vect v3;
    v3.x = v1.x + v2.x;
    v3.y = v1.y + v2.y;
    return v3;
}
struct vect subVector(struct vect v1,struct vect v2)
{
    struct vect v3;
    v3.x = v1.x - v2.x;
    v3.y = v1.y - v2.y;
    return v3;
}
struct vect cosVector(struct vect v,double a)
{
    struct vect w;
    w.x = v.x * cos(a);
    w.y = v.y * cos(a);
    return w;
}
struct vect sinVector(struct vect v,double a)
{
    struct vect w;
    w.x = v.x * sin(a);
    w.y = v.y * sin(a);
    return w;
}
double dotProduct(struct vect v1,struct vect v2)
{
    double d;
    d = v1.x * v2.x + v1.y * v2.y;
    return d;
}

void drawSquare(double a)
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);{
        glVertex2f(a,a);
        glVertex2f(a,-a);

        glVertex2f(a,-a);
        glVertex2f(-a,-a);

        glVertex2f(-a,-a);
        glVertex2f(-a,a);

        glVertex2f(-a,a);
        glVertex2f(a,a);
    }
    glEnd();
}
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
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

void drawSquareFill(double a)//2D Fill
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}


void drawCircle(double radius,int segments,double fraction)//2D Non Fill
{
    int i;
    struct point3D points[100];
    glColor3f(0,0,1);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi*fraction);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi*fraction);
        points[i].z = 0;
        //printf("%lf %lf %lf",points[i].x,points[i].y,points[i].z);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,points[i].z);
			glVertex3f(points[i+1].x,points[i+1].y,points[i+1].z);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments,double fraction)//3D Fill
{
    int i;
    double shade;
    struct point3D points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi*fraction);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi*fraction);
        points[i].z=0;
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        //change color
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,points[i].z);
			glVertex3f(points[i+1].x,points[i+1].y,points[i+1].z);
        }
        glEnd();
    }
}

void drawCylinder(double radius,double height,int segments,double fraction)//3D Fill
{
    int i;
    double shade;
    struct point3D points1[100],points2[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points1[i].x=radius*cos(((double)i/(double)segments)*2*pi*fraction);
        points2[i].x = points1[i].x;
        points1[i].y=radius*sin(((double)i/(double)segments)*2*pi*fraction);
        points2[i].y = points1[i].y;
        points1[i].z=0;
        points2[i].z = height;
    }
    for(i=0;i<segments;i++)
    {
        //create shading effect
        //change color
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_QUADS);
        {
			glVertex3f(points1[i].x,points1[i].y,points1[i].z);
			glVertex3f(points1[i+1].x,points1[i+1].y,points1[i+1].z);
			glVertex3f(points2[i+1].x,points2[i+1].y,points2[i+1].z);
			glVertex3f(points2[i].x,points2[i].y,points2[i].z);
        }
        glEnd();
    }
}
void drawSphere(double radius,int slices,int stacks,double fraction)//3D Fill
{
	struct point3D points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi*fraction);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi*fraction);
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
    drawSquareFill(20);//Sun
    //drawCircle(20,24);

    glRotatef(angle,0,0,1);//Barshik Goti
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);//Arnik Goti
    glColor3f(0,1,0);
    drawSquareFill(15);//Planet

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);//Barshik Goti
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);//Arnik Goti
        glColor3f(0,0,1);
        drawSquareFill(10);//Moon 1
        /**
        glRotatef(3*angle,0,0,1);//Barshik Goti
        glTranslatef(40,0,0);
        glRotatef(4*angle,0,0,1);//Arnik Goti
        glColor3f(1,1,0);
        drawSquareFill(5);//Moon of Moon 1
        **/
    }
    glPopMatrix();
    glPushMatrix();
    {
        glRotatef(3*angle,0,0,1);//Barshik Goti
        glTranslatef(40,0,0);
        glRotatef(4*angle,0,0,1);//Arnik Goti
        glColor3f(1,1,0);
        drawSquareFill(5);//Moon 2
        /**
        glRotatef(3*angle,0,0,1);//Barshik Goti
        glTranslatef(40,0,0);
        glRotatef(4*angle,0,0,1);//Arnik Goti
        glColor3f(1,0,1);
        drawSquareFill(5);//Moon of Moon 2
        **/
    }
    glPopMatrix();
}
void drawArrow()
{
    struct vect w1,w2;
    struct point2D pos1,pos2;
    w1 = perpVector(v);
    w2.x = (-1)*w1.x;
    w2.y = (-1)*w1.y;
    pos1.x = pos.x+2*v.x;
    pos1.y = pos.y+2*v.y;
    pos2.x = pos.x+4*v.x;
    pos2.y = pos.y+4*v.y;
    glBegin(GL_LINES);{
        glColor3f(1.0, 1.0, 1.0);
        glVertex2f(pos.x,pos.y);
        glVertex2f(pos1.x,pos1.y);
        glVertex2f(pos1.x,pos1.y);
        glVertex2f(pos2.x,pos2.y);
        glColor3f(1.0, 0, 0);
        glVertex2f(pos2.x,pos2.y);
        glVertex2f(pos1.x+w1.x,pos1.y+w1.y);
        glVertex2f(pos1.x+w2.x,pos1.y+w2.y);
        glVertex2f(pos2.x,pos2.y);
        glVertex2f(pos1.x+w1.x,pos1.y+w1.y);
        glVertex2f(pos1.x+w2.x,pos1.y+w2.y);
    }
    glEnd();
}
void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
    struct vect w;
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			//cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			//cameraAngle += 0.03;
			w = rotateVector(v,-2);;
			v.x = w.x;
			v.y = w.y;
			break;
		case GLUT_KEY_LEFT:
			//cameraAngle -= 0.03;
			w = rotateVector(v,2);
			v.x = w.x;
			v.y = w.y;
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
				drawaxes=1-drawaxes;
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
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,100,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	//drawAxes();
	//drawGrid();
    drawSquare(60);
    drawArrow();
    //glColor3f(1,0,0);
    //drawSquareFill(10);

    //drawSS();

    //drawCircle(30,24,0.25);

    //drawCone(20,50,24,0.25);
    //drawCylinder(20,50,24,0.5);

	//drawSphere(30,24,20,0.25);

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
    if((pos.x <60 && pos.x>-60)&&(pos.y<60 && pos.y>-60)){
        pos.x += v.x;
        pos.y += v.y;
    }
    else if((pos.x >=60 || pos.x <=-60)&&(pos.y >=60 || pos.y <=-60)){
        v.x = (-1)*v.x;
        v.y = (-1)*v.y;
        pos.x += v.x;
        pos.y += v.y;
    }
    else if((pos.x >=60 || pos.x <=-60)&&(pos.y<60 && pos.y>-60)){
        m.x = v.x;
        m.y = 0;
        v.x -= 2*m.x;
        v.y -= 2*m.y;
        pos.x += v.x;
        pos.y += v.y;
    }
    else if((pos.x <60 && pos.x>-60)&&(pos.y >=60 || pos.y <=-60)){
        m.x = 0;
        m.y = v.y;
        v.x -= 2*m.x;
        v.y -= 2*m.y;
        pos.x += v.x;
        pos.y += v.y;
    }
    printf("Vector: %lf %lf\n",v.x,v.y);
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
    pos.x = 30;
    pos.y = 50;
    v.x = 1;
    v.y = 1;
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
