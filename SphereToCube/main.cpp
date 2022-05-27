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
double a,r;
double A;
struct point
{
	double x,y,z;
};

struct vect
{
    double x,y,z;
};

struct point pos;
struct point sop;
struct vect left;
struct vect right;
struct vect up;

struct vect crossProduct(struct vect v1,struct vect v2)
{
    struct vect v3;
    v3.x = v1.y * v2.z - v1.z * v2.y;
    v3.y = v1.z * v2.x - v1.x * v2.z;
    v3.z = v1.x * v2.y - v1.y * v2.x;
    return v3;
}
struct vect addVector(struct vect v1,struct vect v2)
{
    struct vect v3;
    v3.x = v1.x + v2.x;
    v3.y = v1.y + v2.y;
    v3.z = v1.z + v2.z;
    return v3;
}
struct vect subVector(struct vect v1,struct vect v2)
{
    struct vect v3;
    v3.x = v1.x - v2.x;
    v3.y = v1.y - v2.y;
    v3.z = v1.z - v2.z;
    return v3;
}
struct vect cosVector(struct vect v,double a)
{
    struct vect w;
    w.x = v.x * cos(a);
    w.y = v.y * cos(a);
    w.z = v.z * cos(a);
    return w;
}
struct vect sinVector(struct vect v,double a)
{
    struct vect w;
    w.x = v.x * sin(a);
    w.y = v.y * sin(a);
    w.z = v.z * sin(a);
    return w;
}
double dotProduct(struct vect v1,struct vect v2)
{
    double d;
    d = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    return d;
}

void drawAxes()
{
	if(true)
	{

		glBegin(GL_LINES);{
		    glColor3f(1, 0, 1);
			glVertex3f(0,0, 150);//z axis
			glVertex3f(0,0,-150);
            glColor3f(1, 1, 0);
			glVertex3f(0,150,0);//y axis red
			glVertex3f(0,-150,0);
			glColor3f(0, 1, 1);
			glVertex3f(150,0,0);//x axis blue
			glVertex3f(-150,0,0);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 1, 0.6);	//grey
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
	if(drawgrid==2)
	{
		glColor3f(1, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to -axis
				glVertex3f(0, -90, i*10);
				glVertex3f(0,  90, i*10);

				//lines parallel to -axis
				glVertex3f(0, i*10, -90);
				glVertex3f(0, i*10, 90);
			}
		}glEnd();
	}
	if(drawgrid==3)
	{
		glColor3f(0.6, 0.6, 1);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to -axis
				glVertex3f(-90, 0, i*10);
				glVertex3f(90,  0, i*10);

				//lines parallel to -axis
				glVertex3f(i*10, 0, -90);
				glVertex3f(i*10, 0, 90);
			}
		}glEnd();
	}
}

void drawSquareFill(double a)//2D Fill
{
    glColor3f(0,0,1);
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
    struct point points[100];
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
    struct point points[100];
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

void drawCylinder(double radius,double height,int segments,double fraction,double color)//3D Fill
{
    int i;
    double shade;
    struct point points1[100],points2[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points1[i].x=radius*cos(((double)i/(double)segments)*2*pi*fraction);
        points2[i].x = points1[i].x;
        points1[i].y=radius*sin(((double)i/(double)segments)*2*pi*fraction);
        points2[i].y = points1[i].y;
        points1[i].z= (height) * (-1);
        points2[i].z = height;
    }
    for(i=0;i<segments;i++)
    {
        //create shading effect
        //change color
        //if(i<segments/2)shade=2*(double)i/(double)segments;
        //else shade=2*(1.0-(double)i/(double)segments);
        shade = color;
        glColor3f(0,shade,0);

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
void drawSphereUpper(double radius,int slices,int stacks,double fraction)//3D Fill
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
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi*fraction);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi*fraction);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(1,0,0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                //glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawSphereLower(double radius,int slices,int stacks,double fraction)//3D Fill
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
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi*fraction);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi*fraction);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(1,0,0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				//glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
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

void drawS2C(){
    double r1,h1;
    r1 = r;
    h1 = a;
    //Square
    glPushMatrix();//Up
    {
        glTranslatef(0,0,h1+r1);
        drawSquareFill(a);
    }
    glPopMatrix();
    glPushMatrix();//Up
    {
        glTranslatef(0,0,-h1-r1);
        drawSquareFill(a);
    }
    glPopMatrix();
    glPushMatrix();//Side1
    {
        glTranslatef(0,h1+r1,0);
        glRotatef(90,1,0,0);
        drawSquareFill(a);
    }
    glPopMatrix();
    glPushMatrix();//Side1
    {
        glTranslatef(0,-h1-r1,0);
        glRotatef(90,1,0,0);
        drawSquareFill(a);
    }
    glPopMatrix();
    glPushMatrix();//Side2
    {
        glTranslatef(h1+r1,0,0);
        glRotatef(90,0,1,0);
        drawSquareFill(a);
    }
    glPopMatrix();
    glPushMatrix();//Side2
    {
        glTranslatef(-h1-r1,0,0);
        glRotatef(90,0,1,0);
        drawSquareFill(a);
    }
    glPopMatrix();
    //Sphere
    glPushMatrix();
    {
        glTranslatef(h1,h1,h1);
        drawSphereUpper(r,50,50,0.25);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(-h1,h1,h1);
        glRotatef(90,0,0,1);
        drawSphereUpper(r,50,50,0.25);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(-h1,-h1,h1);
        glRotatef(180,0,0,1);
        drawSphereUpper(r,50,50,0.25);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(h1,-h1,h1);
        glRotatef(270,0,0,1);
        drawSphereUpper(r,50,50,0.25);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(h1,h1,-h1);
        drawSphereLower(r,50,50,0.25);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(-h1,h1,-h1);
        glRotatef(90,0,0,1);
        drawSphereLower(r,50,50,0.25);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(-h1,-h1,-h1);
        glRotatef(180,0,0,1);
        drawSphereLower(r,50,50,0.25);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(h1,-h1,-h1);
        glRotatef(270,0,0,1);
        drawSphereLower(r,50,50,0.25);
    }
    glPopMatrix();
    //Cylinder
    glPushMatrix();//Cylinder 1
    {
        glTranslatef(a,a,0);
        drawCylinder(r1,h1,50,0.25,1);
    }
    glPopMatrix();
    glPushMatrix();//Cylinder 2
    {
        glTranslatef(a,0,a);
        glRotatef(90,1,0,0);//x axis
        drawCylinder(r1,h1,50,0.25,1);
    }
    glPopMatrix();
    glPushMatrix();//Cylinder 3
    {
        glTranslatef(0,a,-a);
        glRotatef(90,0,1,0);//y axis
        drawCylinder(r1,h1,50,0.25,1);
    }
    glPopMatrix();
    glPushMatrix();//Cylinder 1
    {
        glTranslatef(-a,a,0);
        glRotatef(90,0,0,1);
        drawCylinder(r1,h1,50,0.25,1);
    }
    glPopMatrix();
    glPushMatrix();//Cylinder 2
    {
        glTranslatef(a,0,-a);
        glRotatef(90,0,1,0);//y axis
        glRotatef(90,1,0,0);//x axis
        drawCylinder(r1,h1,50,0.25,1);
    }
    glPopMatrix();
    glPushMatrix();//Cylinder 3
    {
        glTranslatef(0,a,a);
        glRotatef(90,0,1,0);//y axis
        glRotatef(90,0,0,1);//z axis
        drawCylinder(r1,h1,50,0.25,1);
    }
    glPopMatrix();
    glPushMatrix();//Cylinder 1
    {
        glTranslatef(-a,-a,0);
        glRotatef(180,0,0,1);
        drawCylinder(r1,h1,50,0.25,1);
    }
    glPopMatrix();
    glPushMatrix();//Cylinder 2
    {
        glTranslatef(-a,0,-a);
        glRotatef(180,0,1,0);//y axis
        glRotatef(90,1,0,0);//x axis
        drawCylinder(r1,h1,50,0.25,1);
    }
    glPopMatrix();
    glPushMatrix();//Cylinder 3
    {
        glTranslatef(0,-a,a);
        glRotatef(90,0,1,0);//y axis
        glRotatef(180,0,0,1);//z axis
        drawCylinder(r1,h1,50,0.25,1);
    }
    glPopMatrix();
    glPushMatrix();//Cylinder 1
    {
        glTranslatef(a,-a,0);
        glRotatef(270,0,0,1);
        drawCylinder(r1,h1,50,0.25,1);
    }
    glPopMatrix();
    glPushMatrix();//Cylinder 2
    {
        glTranslatef(-a,0,a);
        glRotatef(270,0,1,0);//y axis
        glRotatef(90,1,0,0);//x axis
        drawCylinder(r1,h1,50,0.25,1);
    }
    glPopMatrix();
    glPushMatrix();//Cylinder 3
    {
        glTranslatef(0,-a,-a);
        glRotatef(90,0,1,0);//y axis
        glRotatef(270,0,0,1);//z axis
        drawCylinder(r1,h1,50,0.25,1);
    }
    glPopMatrix();
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
	    struct vect l1,l2,r1,r2,u1,u2,m,n;
	    case '1':
	        A = 0.5;
            r1 = right;
            l1 = left;
            m = cosVector(r1,-A);
            n = sinVector(l1,-A);
            r2 = addVector(m,n);
            m = cosVector(l1,-A);
            n = sinVector(r1,-A);
            l2 = subVector(m,n);
            left = l2;
            right = r2;
            break;
        case '2':
            A = 0.5;
            r1 = right;
            l1 = left;
            m = cosVector(r1,A);
            n = sinVector(l1,A);
            r2 = addVector(m,n);
            m = cosVector(l1,A);
            n = sinVector(r1,A);
            l2 = subVector(m,n);
            left = l2;
            right = r2;
            break;
        case '3':
            A = 0.5;
            u1 = up;
            l1 = left;
            m = cosVector(l1,A);
            n = sinVector(u1,A);
            l2 = addVector(m,n);
            m = cosVector(u1,A);
            n = sinVector(l1,A);
            u2 = subVector(m,n);
            left = l2;
            up = u2;
            break;
        case '4':
            A = 0.5;
            u1 = up;
            l1 = left;
            m = cosVector(l1,-A);
            n = sinVector(u1,-A);
            l2 = addVector(m,n);
            m = cosVector(u1,-A);
            n = sinVector(l1,-A);
            u2 = subVector(m,n);
            left = l2;
            up = u2;
            break;
        case '5':
            A = 0.5;
            u1 = up;
            r1 = right;
            m = cosVector(u1,A);
            n = sinVector(r1,A);
            u2 = addVector(m,n);
            m = cosVector(r1,A);
            n = sinVector(u1,A);
            r2 = subVector(m,n);
            up = u2;
            right = r2;
            break;
        case '6':
            A = 0.5;
            u1 = up;
            r1 = right;
            m = cosVector(u1,-A);
            n = sinVector(r1,-A);
            u2 = addVector(m,n);
            m = cosVector(r1,-A);
            n = sinVector(u1,-A);
            r2 = subVector(m,n);
            up = u2;
            right = r2;
            break;

        case 'm':
            drawgrid=1;
            break;
        case 'n':
            drawgrid=2;
            break;
        case 'o':
            drawgrid=3;
        default:
            break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_UP:		// up arrow key
		    pos.x += left.x;
            pos.y += left.y;
            pos.z += left.z;
			break;
        case GLUT_KEY_DOWN:		//down arrow key
            pos.x -= left.x;
            pos.y -= left.y;
            pos.z -= left.z;
			break;
        case GLUT_KEY_LEFT:
            pos.x -= right.x;
            pos.y -= right.y;
            pos.z -= right.z;
			break;
		case GLUT_KEY_RIGHT:
            pos.x += right.x;
            pos.y += right.y;
            pos.z += right.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x -= up.x;
            pos.y -= up.y;
            pos.z -= up.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    pos.x += up.x;
            pos.y += up.y;
            pos.z += up.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    if (a > 0){
                a = a - 0.5;
                r = r + 0.5;
		    }
			break;
		case GLUT_KEY_END:
		    if (r > 0){
                a = a + 0.5;
                r = r - 0.5;
		    }
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
	//gluLookAt(pos.x,pos.y,pos.z,sop.x, sop.y, sop.z,up.x,up.y,up.z);
    gluLookAt(pos.x,pos.y,pos.z,pos.x + left.x, pos.y + left.y, pos.z + left.z,up.x,up.y,up.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //drawSquareFill(10);

    //drawSS();
    drawS2C();

    //drawCircle(30,24,0.25);

    //drawCone(20,50,24,0.25);
    //glTranslatef(0,10,0);
    //drawCylinder(10,20,24,0.25,1);

	//drawSphere(30,24,20,0.25);

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
	drawaxes=0;
	cameraHeight=150.0;
	cameraAngle=1.0;
	a = 25;
	r = 5;
	angle=0;
    pos.x = 100;
    pos.y = 100;
    pos.z = 0;
    left.x = -1;
    left.y = -1;
    left.z = 0;
    right.x = -1;
    right.y = 1;
    right.z = 0;
    up.x = 0;
    up.y = 0;
    up.z = 1;
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
