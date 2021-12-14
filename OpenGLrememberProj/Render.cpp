#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"


bool textureMode = true;
bool lightMode = true;

void normal(double A[], double B[], double C[], double* tmp) {
	double a[3];
	double b[3];
	double len;
	for (int i = 0; i < 3; i++)
	{
		a[i] = A[i] - B[i];
		b[i] = C[i] - B[i];
	}
	tmp[0] = a[1] * b[2] - b[1] * a[2];
	tmp[1] = -a[0] * b[2] + b[0] * a[2];
	tmp[2] = a[0] * b[1] - b[0] * a[1];
	len = sqrt(pow(tmp[0], 2) + pow(tmp[1], 2) + pow(tmp[2], 2));
	tmp[0] /= len;
	tmp[1] /= len;
	tmp[2] /= len;
}

void CentrCircle(double h, double* Centr) {
	double A[] = { 0,0,h };
	double B[] = { 0,9,h };
	double C[] = { 1,3,h };
	double D[] = { 6,3,h };
	double E[] = { 6,-5,h };
	double F[] = { 1,-2,h };
	double G[] = { -3,-7,h };
	double H[] = { -6,-1,h };
	double I[] = { -1,1,h };
	double M[] = { 5,0,h };

	double z1 = pow(E[0], 2) + pow(E[1], 2);
	double z2 = pow(D[0], 2) + pow(D[1], 2);
	double z3 = pow(M[0], 2) + pow(M[1], 2);
	double x12 = E[0] - D[0];
	double x23 = D[0] - M[0];
	double x31 = M[0] - E[0];
	double y12 = E[1] - D[1];
	double y23 = D[1] - M[1];
	double y31 = M[1] - E[1];
	double zx = y12 * z3 + y23 * z1 + y31 * z2;
	double zy = x12 * z3 + x23 * z1 + x31 * z2;
	double z = x12 * y31 - y12 * x31;
	Centr[0] = -1 * zx / (2 * z);
	Centr[1] = zy / (2 * z);
}

void Cap(double z) {

	double A[] = { 0,0,z };
	double B[] = { 0,9,z };
	double C[] = { 1,3,z };
	double D[] = { 6,3,z };
	double E[] = { 6,-5,z };
	double F[] = { 1,-2,z };
	double G[] = { -3,-7,z };
	double H[] = { -6,-1,z };
	double I[] = { -1,1,z };
	double M[] = { 5,0,z };
	double Centr[2];
	CentrCircle(z, Centr);
	double r = sqrt(pow(G[0] - H[0], 2) + pow(G[1] - H[1], 2) + pow(G[2] - H[2], 2)) / 2;
	double r1 = sqrt(pow(D[0], 2) - 2 * D[0] * Centr[0] + pow(Centr[0], 2) + pow(D[1], 2) - 2 * D[1] * Centr[1] + pow(Centr[1], 2));
	double angleA = acos((H[0] - (H[0] + G[0]) / 2) / r);
	double angleB = acos((G[0] - (H[0] + G[0]) / 2) / r);
	double angleA1 = acos((E[0] - Centr[0]) / r1);
	double angleB1 = acos((D[0] - Centr[0]) / r1);
	double qq[] = { (G[0] + H[0]) / 2, (G[1] + H[1]) / 2, 0 };

	glBegin(GL_TRIANGLE_FAN);
	glColor3d(0, 1, 0);

	if (z == 0) {
		glNormal3d(0, 0, -1);
	}
	else {
		glNormal3d(0, 0, 1);
		glColor4d(0, 1, 0,0.5);
	}

	glVertex3dv(F);
	glVertex3dv(H);
	
	for (double i = -angleB; i <= angleA; i += 0.01)
	{
		glVertex3d(qq[0], qq[1], z);
		glVertex3f(qq[0] - cos(i) * r, qq[1] - sin(i) * r, z);
		glVertex3f(qq[0] - cos(i + 0.01) * r, qq[1] - sin(i + 0.01) * r, z);

	}

	//glColor3d(0, 1, 0);
	glVertex3dv(F);
	for (double i = -angleB1; i >= -1.4 * angleA1; i -= 0.01)
	{
		glVertex3d(Centr[0] + r1 * cos(i), Centr[1] + r1 * sin(i), z);
	}
	glEnd();

	glBegin(GL_TRIANGLES);

	if (z == 0) {
		glNormal3d(0, 0, -1);
	}
	else {
		glNormal3d(0, 0, 1);
		glColor4d(0, 1, 0, 0.5);
	}

	glVertex3dv(F);
	glVertex3dv(I);
	glVertex3dv(H);

	glVertex3dv(F);
	glVertex3dv(I);
	glVertex3dv(C);

	glVertex3dv(B);
	glVertex3dv(I);
	glVertex3dv(C);

	glVertex3dv(D);
	glVertex3dv(F);
	glVertex3dv(C);
	glEnd();

}

void Walls(double z) {
	double A[] = { 0,0,z };
	double B[] = { 0,9,z };
	double C[] = { 1,3,z };
	double D[] = { 6,3,z };
	double E[] = { 6,-5,z };
	double F[] = { 1,-2,z };
	double G[] = { -3,-7,z };
	double H[] = { -6,-1,z };
	double I[] = { -1,1,z };
	double N[3];
	double Centr[2];
	CentrCircle(z, Centr);
	double r = sqrt(pow(G[0] - H[0], 2) + pow(G[1] - H[1], 2) + pow(G[2] - H[2], 2)) / 2;
	double r1 = sqrt(pow(D[0], 2) - 2 * D[0] * Centr[0] + pow(Centr[0], 2) + pow(D[1], 2) - 2 * D[1] * Centr[1] + pow(Centr[1], 2));
	double angleA = acos((H[0] - (H[0] + G[0]) / 2) / r);
	double angleB = acos((G[0] - (H[0] + G[0]) / 2) / r);
	double angleA1 = acos((E[0] - Centr[0]) / r1);
	double angleB1 = acos((D[0] - Centr[0]) / r1);
	double qq[] = { (G[0] + H[0]) / 2, (G[1] + H[1]) / 2, 0 };
	glBegin(GL_QUADS);
	//glColor3d(0, 1, 1);

	double B1[] = { B[0],B[1],0 };
	normal(C, B, B1,N);
	glNormal3dv(N);

	glVertex3d(B[0], B[1], 0);
	glVertex3d(B[0], B[1], B[2]);
	glVertex3d(C[0], C[1], C[2]);
	glVertex3d(C[0], C[1], 0);

	B1[0] = C[0];
	B1[1] = C[1];
	B1[2] = 0;
	normal(D, C, B1,N);
	glNormal3dv(N);

	glVertex3d(C[0], C[1], C[2]);
	glVertex3d(D[0], D[1], D[2]);
	glVertex3d(D[0], D[1], 0);
	glVertex3d(C[0], C[1], 0);

	B1[0] = F[0];
	B1[1] = F[1];
	B1[2] = 0;
	normal(E, B1, F,N);
	glNormal3dv(N);

	glVertex3d(E[0], E[1], E[2]);
	glVertex3d(F[0], F[1], F[2]);
	glVertex3d(F[0], F[1], 0);
	glVertex3d(E[0], E[1], 0);

	B1[0] = G[0];
	B1[1] = G[1];
	B1[2] = 0;
	normal(F, B1, G,N);
	glNormal3dv(N);

	glVertex3d(F[0], F[1], F[2]);
	glVertex3d(G[0], G[1], G[2]);
	glVertex3d(G[0], G[1], 0);
	glVertex3d(F[0], F[1], 0);

	B1[0] = H[0];
	B1[1] = H[1];
	B1[2] = 0;
	normal(I, H, B1,N);
	glNormal3dv(N);

	glVertex3d(H[0], H[1], H[2]);
	glVertex3d(I[0], I[1], I[2]);
	glVertex3d(I[0], I[1], 0);
	glVertex3d(H[0], H[1], 0);

	B1[0] = I[0];
	B1[1] = I[1];
	B1[2] = 0;
	normal(B, I, B1,N);
	glNormal3dv(N);

	glVertex3d(I[0], I[1], I[2]);
	glVertex3d(B[0], B[1], B[2]);
	glVertex3d(B[0], B[1], 0);
	glVertex3d(I[0], I[1], 0);
	glEnd();

	glBegin(GL_QUADS);
	//glColor3d(0, 1, 1);
	for (double i = -angleB; i <= angleA; i += 0.01) {
		double x1 = qq[0] - cos(i) * r;
		double y1 = qq[1] - sin(i) * r;
		double x2 = qq[0] - cos(i + 0.01) * r;
		double y2 = qq[1] - sin(i + 0.01) * r;
		double N1[] = { x1,y1,0 };
		double N2[] = { x1,y1,z };
		double N3[] = { x2,y2,0 };
		double N4[] = { x2,y2,z };
		normal(N3, N1, N2, N);
		glNormal3dv(N);
		glVertex3d(x1, y1, 0);
		glVertex3d(x1, y1, z);
		glVertex3d(x2, y2, z);
		glVertex3d(x2, y2, 0);
	}
	glEnd();

	glBegin(GL_QUADS);

	for (double i = -angleB1 - 0.01; i >= -1.4 * angleA1; i -= 0.01)
	{
		//glColor3d(0, 1, 1);
		double N1[] = {Centr[0] + r1 * cos(i), Centr[1] + r1 * sin(i), 0 };
		double N2[] = {Centr[0] + r1 * cos(i), Centr[1] + r1 * sin(i), z };
		double N3[] = {Centr[0] + r1 * cos(i + 0.01),Centr[1] + r1 * sin(i + 0.01), z };
		double N4[] = {Centr[0] + r1 * cos(i + 0.01),Centr[1] + r1 * sin(i + 0.01), 0 };
		normal(N2, N1, N3, N);
		glNormal3dv(N);
		glVertex3dv(N1);
		glVertex3dv(N2);
		glVertex3dv(N3);
		glVertex3dv(N4);
	}
	glEnd();
}

//����� ��� ��������� ������
class CustomCamera : public Camera
{
public:
	//��������� ������
	double camDist;
	//���� �������� ������
	double fi1, fi2;

	
	//������� ������ �� ���������
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	void SetUpCamera()
	{
		//�������� �� ������� ������ ������
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//������� ��������� ������
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������


//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(1, 1, 3);
	}

	
	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//����� �� ��������� ����� �� ����������
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//������ ���������
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// ��������� ��������� �����
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// �������������� ����������� �����
		// ������� ��������� (���������� ����)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// ��������� ������������ �����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// ��������� ���������� ������������ �����
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //������� �������� �����




//������ ���������� ����
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//������� ���� �� ���������, � ����� ��� ����
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}



GLuint texId;

//����������� ����� ������ ��������
void initRender(OpenGL *ogl)
{
	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);
	

	//������ ����������� ���������  (R G B)
	RGBTRIPLE *texarray;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	
	
	//���������� �� ��� ��������
	glGenTextures(1, &texId);
	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, texId);

	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//�������� ������
	free(texCharArray);
	free(texarray);

	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// ������������ �������� : �� ����� ����� ����� 1
	glEnable(GL_NORMALIZE);

	// ���������� ������������� ��� �����
	glEnable(GL_LINE_SMOOTH); 


	//   ������ ��������� ���������
	//  �������� GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  ������� � ���������� �������� ���������(�� ���������), 
	//                1 - ������� � ���������� �������������� ������� ��������       
	//                �������������� ������� � ���������� ��������� ����������.    
	//  �������� GL_LIGHT_MODEL_AMBIENT - ������ ������� ���������, 
	//                �� ��������� �� ���������
	// �� ��������� (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}





void Render(OpenGL *ogl)
{



	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//��������������
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//��������� ���������
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//�������
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//��������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//����������
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//������ �����
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//���� ���� �������, ��� ����������� (����������� ���������)
	glShadeModel(GL_SMOOTH);
	//===================================
	//������� ���  

	Cap(0);
	Walls(3);
	Cap(3);

	//����� ��������� ���������� ��������


   //��������� ������ ������

	
	glMatrixMode(GL_PROJECTION);	//������ �������� ������� ��������. 
	                                //(���� ��������� ��������, ����� �� ������������.)
	glPushMatrix();   //��������� ������� ������� ������������� (������� ��������� ������������� ��������) � ���� 				    
	glLoadIdentity();	  //��������� ��������� �������
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //������� ����� ������������� ��������

	glMatrixMode(GL_MODELVIEW);		//������������� �� �����-��� �������
	glPushMatrix();			  //��������� ������� ������� � ���� (��������� ������, ����������)
	glLoadIdentity();		  //���������� �� � ������

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //������� ����� ��������� ��� ������� ������ � �������� ������.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - ���/���� �������" << std::endl;
	ss << "L - ���/���� ���������" << std::endl;
	ss << "F - ���� �� ������" << std::endl;
	ss << "G - ������� ���� �� �����������" << std::endl;
	ss << "G+��� ������� ���� �� ���������" << std::endl;
	ss << "�����. �����: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "�����. ������: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "��������� ������: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}