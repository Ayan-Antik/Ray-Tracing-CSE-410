#include <bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <windows.h>
#include <GL/glut.h>
#include <fstream>
#include "bitmap_image.hpp"
#define pi (2*acos(0.0))
#define INF 99999


using namespace std;
ofstream logfile("D:\\BUET\\L4T1\\CSE 410 Computer Graphics Sessional\\Assignment 3\\Ray Tracing\\log.txt");

class Point
{
public:
	double x,y,z;
	 Point(){
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
    }
	Point(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
	}

    Point operator*(const double mult){
        return Point(x*mult, y*mult, z*mult);
    }

    double operator*(const Point mult)
    {
        return x*mult.x+ y*mult.y+ z*mult.z;
    }

    Point operator+(const Point add){
        return Point(x+add.x, y+add.y, z+add.z);
    }

    Point operator-(const Point sub){
        return Point(x-sub.x, y-sub.y, z-sub.z);
    }

    Point negate_(){
        return Point(-x, -y, -z);
    }

    Point cross_mult(Point mult){
        double x1, y1, z1;
        x1 = y*mult.z - z*mult.y;
        y1 = z*mult.x - x*mult.z;
        z1 = x*mult.y - y*mult.x;

        return Point(x1, y1, z1);
    }

    Point normalize(){
        Point p;
        double mag = sqrt(pow(x, 2.0)+pow(y,2.0)+ pow(z, 2.0));
        p.x=x/mag;
        p.y=y/mag;
        p.z=z/mag;

        return p;

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

    Color(){this->r = 0; this->g = 0; this->b = 0;}
    Color(double red, double green, double blue){
        this->r = red;
        this->g = green;
        this->b = blue;
    }

    void clip(){

        if(this->r > 1){this->r=1;}
        else if(this->r < 0){this->r=0;}

        if(this->g > 1){this->g=1;}
        else if(this->g < 0){this->g=0;}

        if(this->b > 1){this->b=1;}
        else if(this->b < 0){this->b=0;}


    }

    Color operator*(const double d){
        return Color(this->r*d, this->g*d, this->b*d);
    }
    void print(){
        cout << "[ r: " << r << ", g: " << g << ", b: " << b << " ]\n";
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

vector <PointLight> pointLights = vector <PointLight>();
vector <SpotLight> spotLights = vector <SpotLight>();

class Ray{
public:
    Point start;
    Point dir;

    Ray(Point start, Point dir){
        this->start = start;
        this->dir = dir;
    }
};


class Object{

public:
    Point reference_point; // should have x, y, z
    double height, width, length;
    Color color;
    double coEfficients[4]; // ambient, diffuse, specular, reflection coefficients
    int shine; // exponent term of specular component
    string type;
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
        ///ambient diffues specular recursive_reflection
        this->coEfficients[0] = a;
        this->coEfficients[1] = d;
        this->coEfficients[2] = sp;
        this->coEfficients[3] = rc;
//        cout << coEfficients[0] << " " << coEfficients[1] << " " << coEfficients[2] << " " << coEfficients[3] << endl;
    }

    virtual double intersect(Ray ray, Color *color, int level){
        return -1.0;
    }

    virtual Color getColorAt(Point p){
        return Color(this->color.r, this->color.g, this->color.b);
    }

    virtual void print(){}

    string getType(){return type;}
};

class Sphere:public Object{

public:
    Sphere(Point center, double radius){
        this->reference_point = center;
        this->length = radius;
        this->type = "sphere";
    }

    void draw(){

        Point Points[100][100];
        int slices=24;
        int stacks=20;
        int i,j;
        double h,r;

        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
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

        glPopMatrix();
    }

    double intersect(Ray ray, Color *color, int level){

//        ray->start.print();
//        if(level == 1)cout << "HEMLO\n";
        ray.start = ray.start - reference_point;
//        cout << "In intersect of sphere" << endl;
        double t, t1, t2;
//        ray->start.print();
        double ld = ray.start.negate_() * (ray.dir.normalize());
//        cout << "Here\n" << "ld: " << ld << endl;
        if(ld < 0)return -1.0;
        double d_sq = (ray.start*ray.start) - ld*ld;

        if(d_sq > length*length)return -1.0;
        double l_sq = length*length - d_sq;
        t1 = ld + sqrt(l_sq);
        t2 = ld - sqrt(l_sq);
        t = min(t1, t2);
//        cout << "Here\n" << "t1: " << t1 << " t2: " << t2 << endl;
        if(level==0)return t;


//        if(t<0)return -1.0;

        /// Light handle
        Point intersectionPoint = Point(ray.start + (ray.dir)*t);
        Color intersectionPointColor = getColorAt(intersectionPoint);
//        intersectionPointColor.print();
        color->r = intersectionPointColor.r*coEfficients[0]; ///ambient
        color->g = intersectionPointColor.g*coEfficients[0]; ///ambient
        color->b = intersectionPointColor.b*coEfficients[0]; ///ambient

        return t;

//        double a = 1;
//        double b = (ray.start*ray.dir)*2;
//        double c = (ray.start*ray.start) - (length * length);
//
//        double t1, t2;
//        double d = (b*b - 4*a*c);
//        if(d<0){
//            cout << "EHHE\n";
//            return -1;
//        }
//        d = sqrt(d);
//        t1 = (-b-d)/(2*a);
//        t2 = (-b+d)/(2*a);
//        if(t1>0)return t1;
//        if(t2>0)return t2;
//        cout << "Here\n" << "t1: " << t1 << " t2: " << t2 << endl;
//        return -1;

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
        this->type = "triangle";
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
        this->p1.print();
        cout << "p2: ";
        this->p2.print();
        cout << "p3: ";
        this->p3.print();
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
        this->type = "general";

    }
    void draw(){
//        cout << "can't draw weird shapes yet\n";
        return;
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
        this->reference_point.x = -floorWidth/2;
        this->reference_point.y = -floorWidth/2;
        this->reference_point.z = 0;
        this->length = tileWidth;
        this->floorWidth = floorWidth;
        this->tileWidth = tileWidth;
        this->type = "floor";

    }

    void draw(){

        int tileCount = floorWidth/tileWidth;
        bool black = false;
        for(int i = -tileCount/2; i<tileCount/2; i++){
            for(int j = -tileCount/2; j<tileCount/2; j++){

                double a = tileWidth/2;
                glPushMatrix();
                glTranslatef(i*tileWidth, j*tileWidth, 0);
                if(!black){
                    glColor3f(1, 1, 1);
                    black = true;
                }
                else{
                    glColor3f(0, 0, 0);
                    black = false;
                }
                glBegin(GL_QUADS);{
                    glVertex3f( a, a,0);
                    glVertex3f( a,-a,0);
                    glVertex3f(-a,-a,0);
                    glVertex3f(-a, a,0);
                }glEnd();
                glPopMatrix();

//                break;
//                cout << "i, j: " << i << ", " << j << endl;
            }
            if(!black){
                black = true;
            }
            else{
                black = false;
            }
//            break;
        }



    }

};


vector <Object*> objects = vector<Object*>();
