#include <bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <windows.h>
#include <GL/glut.h>
#include <fstream>
#define pi (2*acos(0.0))

using namespace std;
//extern vector <Object*> objects;
//extern vector <PointLight> pointLights;
//extern vector <SpotLight> spotLights;
class Point
{
public:
	double x,y,z;
	Point(){
        x = y = z = 0.0;
	}

	Point(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
	}

    Point operator*(const double mult){
        return Point(x*mult, y*mult, z*mult);
    }


    Point operator+(const Point add){
        return Point(x+add.x, y+add.y, z+add.z);
    }

    Point cross_mult(Point mult){
        double x1, y1, z1;
        x1 = y*mult.z - z*mult.y;
        y1 = z*mult.x - x*mult.z;
        z1 = x*mult.y - y*mult.x;

        return Point(x1, y1, z1);
    }

    void print(){
        cout << "[ " << x << ", " << y << ", " << z << " ]\n";
    }
};

class Color{

public:
    double r;
    double g;
    double b;

    Color(){}
    Color(double red, double green, double blue){
        r = red;
        g = green;
        b = blue;
    }

    void print(){
        cout << "[ r: " << r << ", g: " << g << ", b: " << b << " ]\n";
    }

};
class Object{

public:
    Point reference_point; // should have x, y, z
    double height, width, length;
    Color color;
    double coEfficients[4]; // ambient, diffuse, specular, reflection coefficients
    int shine; // exponent term of specular component
    Object(){

    }
    virtual void draw(){}
    void setColor(Color color){
        this->color = color;
    }
    void setShine(int shine){
        this->shine = shine;
//        cout << this->shine << endl;
    }
    void setCoEfficients(double a, double d, double sp, double rc){
        coEfficients[0] = a;
        coEfficients[1] = d;
        coEfficients[2] = sp;
        coEfficients[3] = rc;
//        cout << coEfficients[0] << " " << coEfficients[1] << " " << coEfficients[2] << " " << coEfficients[3] << endl;
    }

    virtual void print(){}
};

class Sphere:public Object{

public:
    Sphere(Point center, double radius){
        reference_point = center;
        length = radius;
    }

    void draw(){

        Point Points[100][100];
        int slices=24;
        int stacks=20;
        int i,j;
        double h,r;


    //    glColor3f(1,0,0);
        //generate points
        for(i=0;i<=stacks;i++)
        {
            h=length*sin(((double)i/(double)stacks)*(pi/2));
            r=length*cos(((double)i/(double)stacks)*(pi/2));
            for(j=0;j<=slices;j++)
            {
                Points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                Points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                Points[i][j].z=h;
            }
        }
        //draw quads using generated points
        for(i=0;i<stacks;i++)
        {
            glColor3f(color.r, color.g, color.b);
            for(j=0;j<slices;j++)
            {
                glBegin(GL_QUADS);{
                    //upper hemisphere
                    glVertex3f(Points[i][j].x,Points[i][j].y,Points[i][j].z);
                    glVertex3f(Points[i][j+1].x,Points[i][j+1].y,Points[i][j+1].z);
                    glVertex3f(Points[i+1][j+1].x,Points[i+1][j+1].y,Points[i+1][j+1].z);
                    glVertex3f(Points[i+1][j].x,Points[i+1][j].y,Points[i+1][j].z);
                    //lower hemisphere
                    glVertex3f(Points[i][j].x,Points[i][j].y,-Points[i][j].z);
                    glVertex3f(Points[i][j+1].x,Points[i][j+1].y,-Points[i][j+1].z);
                    glVertex3f(Points[i+1][j+1].x,Points[i+1][j+1].y,-Points[i+1][j+1].z);
                    glVertex3f(Points[i+1][j].x,Points[i+1][j].y,-Points[i+1][j].z);
                }glEnd();
            }
        }
    }

    void print(){
        cout << "printing sphere...\n";
        cout << "Center: ";
        reference_point.print();
        cout << "radius: " << length << endl;
    }

};

class Triangle: public Object{

public:
    Point p1, p2, p3;

    Triangle(Point p1, Point p2, Point p3){
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
    }

    void draw(){

        glColor3f(color.r, color.g, color.b);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p3.x, p3.y, p3.z);
        }
        glEnd();

    }

    void print(){
        cout << "printing triangle...\n";
        cout << "p1: ";
        p1.print();
        cout << "p2: ";
        p2.print();
        cout << "p3: ";
        p3.print();
    }
};

class General: public Object{
public:
    double A, B, C, D, E, F, G, H, I ,J;
    General(){}
    General(double A, double B, double C, double D, double E, double F, double G, double H, double I , double J){
        this->A = A;
        this->B = B;
        this->C = C;
        this->D = D;
        this->E = E;
        this->F = F;
        this->G = G;
        this->H = H;
        this->I = I;
        this->J = J;

    }
    void draw(){
//        cout << "can't draw weird shapes yet\n";
    }
     void print(){
        cout << "printing general object...\n";
        cout << A << " " << B << " " << C << " " << D << " " << E << " " << F << " " << G << " " << H << " " << I << " " << J << endl;

    }

};

class Floor: public Object{
public:
    double floorWidth, tileWidth;
    Floor(){}
    Floor(double floorWidth, double tileWidth){
        reference_point.x = -floorWidth/2;
        reference_point.x = -floorWidth/2;
        reference_point.z = 0;
        length = tileWidth;
        this->floorWidth = floorWidth;
        this->tileWidth = tileWidth;

    }

    void draw(){

        double a = tileWidth/2;

        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);{
            glVertex3f( a, a,0);
            glVertex3f( a,-a,0);
            glVertex3f(-a,-a,0);
            glVertex3f(-a, a,0);
        }glEnd();


    }






};
class PointLight{

public:
    Point light_pos;
    Color color;

    PointLight(){}
    PointLight(Point pos, Color color){
        light_pos = pos;
        this->color = color;
    }

    void print(){
        cout << "printing Point Light...\n";
        light_pos.print();
        color.print();
    }

};

class SpotLight{
public:
    PointLight point_light;
    Point light_direction;
    double cutoff_angle;

    SpotLight(){}
    SpotLight(PointLight pl, Point ld, double angle){
        point_light = pl;
        light_direction = ld;
        cutoff_angle = angle;
    }


    void print(){
        cout << "printing Spot Light...\n";
        point_light.print();
        cout << "Light direction...\n";
        light_direction.print();
        cout << "angle: " << cutoff_angle << endl;
    }

};
