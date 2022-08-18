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
#define epsilon 1e-5

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

    friend ostream& operator<<(ostream &output, Point &p) {
        output << '[' << p.x << ", " << p.y << ", " << p.z << ']';
        return output;
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
        p.x=this->x/mag;
        p.y=this->y/mag;
        p.z=this->z/mag;

        return p;

    }

    double getDistance(Point target){
        return sqrt(pow(x-target.x, 2.0)+pow(y-target.y, 2.0)+pow(z-target.z, 2.0));

    }
    double val(){
        return sqrt(x*x+y*y+z*z);
    }
    void print(){
        cout << "[ " << x << ", " << y << ", " << z << " ]\n";
    }
};

Point eye;
Point u;
Point r;
Point l;

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

    void draw(){

        glPointSize(3);
        glBegin(GL_POINTS);
        glColor3f(color.r, color.g, color.b);
        glVertex3f(light_pos.x, light_pos.y, light_pos.z);
        glEnd();
        glPointSize(1);

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

      void draw(){

        glPointSize(3);
        glBegin(GL_POINTS);
        glColor3f(point_light.color.r, point_light.color.g, point_light.color.b);
        glVertex3f(point_light.light_pos.x, point_light.light_pos.y, point_light.light_pos.z);
        glEnd();
        glPointSize(1);

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
        this->dir = dir.normalize();
    }

    void draw(){

        glPushMatrix();
        glColor3f(1, 1, 1);
        glBegin(GL_LINES);{

            glVertex3f(this->start.x, this->start.y, this->start.z);
            glVertex3f(this->dir.x, this->dir.y, this->dir.z);


        }
        glEnd();
        glPopMatrix();

    }
};


class Object{

public:
    Point center; // should have x, y, z
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



vector <Object*> objects = vector<Object*>();

class Sphere:public Object{

public:
    Sphere(Point center, double radius){
        this->center = center;
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
        glTranslatef(center.x, center.y, center.z);
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
        ray.start = ray.start - this->center;
//        cout << "In intersect of sphere" << endl;
        double t, t1, t2;
//        ray->start.print();
        double ld = (ray.start*(-1.0)) * ray.dir.normalize();
//        cout << "Here\n" << "ld: " << ld << endl;
        if(ld < 0)return -1.0;
        double d_sq = (ray.start*ray.start) - ld*ld;

        if(d_sq > length*length)return -1.0;
        double l_sq = length*length - d_sq;
//        t1 = ld + sqrt(l_sq);
        t2 = ld - sqrt(l_sq);
        t = t2;
        if(level==0)return t;

        if(t<0)return -1.0;

        ///RTX ON
        Point intersectionPoint = (ray.start+ this->center) + (ray.dir)*t;
        Color intersectionPointColor = getColorAt(intersectionPoint);
//        intersectionPointColor.print();
        color->r = intersectionPointColor.r*coEfficients[0]; ///ambient
        color->g = intersectionPointColor.g*coEfficients[0]; ///ambient
        color->b = intersectionPointColor.b*coEfficients[0]; ///ambient

        Ray N = Ray(intersectionPoint, intersectionPoint - this->center); ///Normal Ray

        for(int i = 0; i<pointLights.size(); i++){
            Ray L = Ray(pointLights[i].light_pos, intersectionPoint - (pointLights[i].light_pos)); ///Incident Ray
            Ray R = Ray(intersectionPoint, L.dir - N.dir*(L.dir*N.dir)*2);

            ///shadow check
             double t, tmin =INF;

            for(int j=0; j<objects.size(); j++) {

                t = objects[j]->intersect(L, new Color(), 0);

                if(t>0.0 && t<tmin) {
                    tmin = t;
                }
            }

            Point shadowIntersection = L.start + (L.dir*tmin);

            double actual_distance = intersectionPoint.getDistance(L.start); ///distance between intersection and light source
            double shadow_distance = shadowIntersection.getDistance(L.start);

            if((actual_distance - epsilon) > shadow_distance){
                ///actual intersection is in shadow so ignore diffuse and specular
//                logfile << "actual intersection is in shadow so ignore diffuse and specular\n";
                continue;
            }

//            logfile << "L dir: " << L.dir << endl;
//            Point Ln = L.dir.negate_();
//            logfile << "L dir neg: " << Ln << endl;
//            logfile << "N dir: " << N.dir << endl;
            double lambert = (L.dir*(-1.0))*N.dir;
//            logfile << "Lambert lambert what a prick: " << lambert << endl;
//            if(lambert > 0.0)logfile << "Lambert lambert what a prick: " << lambert << endl;
            color->r += pointLights[i].color.r*this->coEfficients[1]*max(0.0, lambert)*intersectionPointColor.r;
            color->g += pointLights[i].color.g*this->coEfficients[1]*max(0.0, lambert)*intersectionPointColor.g;
            color->b += pointLights[i].color.b*this->coEfficients[1]*max(0.0, lambert)*intersectionPointColor.b;

            double phongValue = max(0.0, (ray.dir.negate_())*(R.dir));
//            if(phongValue > 0.0)logfile << "Phonging: " << phongValue<< endl;
            color->r += pointLights[i].color.r*this->coEfficients[2]*pow(phongValue, this->shine)*intersectionPointColor.r;
            color->g += pointLights[i].color.g*this->coEfficients[2]*pow(phongValue, this->shine)*intersectionPointColor.g;
            color->b += pointLights[i].color.b*this->coEfficients[2]*pow(phongValue, this->shine)*intersectionPointColor.b;

        }



        return t;
    {


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
    }

    void print(){
        cout << "printing sphere...\n";
        cout << "Center: ";
        center.print();
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

    Ray getNormal(Point point, Ray L){

        Point dir = (this->p2 - this->p1).cross_mult(this->p3 - this->p1);
        dir = dir.normalize();
        if(L.dir.negate_()*dir >0){
            return Ray(point, dir);
        }
        return Ray(point, dir.negate_());
    }
    double intersect(Ray ray, Color *color, int level){

        double BaseMatrix[3][3];

        BaseMatrix[0][0] = p1.x-p2.x; BaseMatrix[0][1] = p1.x-p3.x; BaseMatrix[0][2] = ray.dir.x;
        BaseMatrix[1][0] = p1.y-p2.y; BaseMatrix[1][1] = p1.y-p3.y; BaseMatrix[1][2] = ray.dir.y;
        BaseMatrix[2][0] = p1.z-p2.z; BaseMatrix[2][1] = p1.z-p3.z; BaseMatrix[2][2] = ray.dir.z;

        double BetaMatrix[3][3];

        BetaMatrix[0][0] = p1.x-ray.start.x; BetaMatrix[0][1] = p1.x-p3.x; BetaMatrix[0][2] = ray.dir.x;
        BetaMatrix[1][0] = p1.y-ray.start.y; BetaMatrix[1][1] = p1.y-p3.y; BetaMatrix[1][2] = ray.dir.y;
        BetaMatrix[2][0] = p1.z-ray.start.z; BetaMatrix[2][1] = p1.z-p3.z; BetaMatrix[2][2] = ray.dir.z;

        double GammaMatrix[3][3];

        GammaMatrix[0][0] = p1.x-p2.x; GammaMatrix[0][1] = p1.x-ray.start.x; GammaMatrix[0][2] = ray.dir.x;
        GammaMatrix[1][0] = p1.y-p2.y; GammaMatrix[1][1] = p1.y-ray.start.y; GammaMatrix[1][2] = ray.dir.y;
        GammaMatrix[2][0] = p1.z-p2.z; GammaMatrix[2][1] = p1.z-ray.start.z; GammaMatrix[2][2] = ray.dir.z;

        double tMatrix[3][3];

        tMatrix[0][0] = p1.x-p2.x; tMatrix[0][1] = p1.x-p3.x; tMatrix[0][2] = p1.x-ray.start.x;
        tMatrix[1][0] = p1.y-p2.y; tMatrix[1][1] = p1.y-p3.y; tMatrix[1][2] = p1.y-ray.start.y;
        tMatrix[2][0] = p1.z-p2.z; tMatrix[2][1] = p1.z-p3.z; tMatrix[2][2] = p1.z-ray.start.z;

        double x = BaseMatrix[0][0] * (BaseMatrix[1][1] * BaseMatrix[2][2] - BaseMatrix[1][2] * BaseMatrix[2][1]);
        double y = BaseMatrix[0][1] * (BaseMatrix[1][2] * BaseMatrix[2][0] - BaseMatrix[1][0] * BaseMatrix[2][2]);
        double z = BaseMatrix[0][2] * (BaseMatrix[1][0] * BaseMatrix[2][1] - BaseMatrix[1][1] * BaseMatrix[2][0]);

        double determinant = x + y + z;
        if(fabs(determinant)<epsilon){
            return -1;
        }


        x = BetaMatrix[0][0] * (BetaMatrix[1][1] * BetaMatrix[2][2] - BetaMatrix[1][2] * BetaMatrix[2][1]);
        y = BetaMatrix[0][1] * (BetaMatrix[1][2] * BetaMatrix[2][0] - BetaMatrix[1][0] * BetaMatrix[2][2]);
        z = BetaMatrix[0][2] * (BetaMatrix[1][0] * BetaMatrix[2][1] - BetaMatrix[1][1] * BetaMatrix[2][0]);
        double BetaMatDet = x+y+z;


        x = GammaMatrix[0][0] * (GammaMatrix[1][1] * GammaMatrix[2][2] - GammaMatrix[1][2] * GammaMatrix[2][1]);
        y = GammaMatrix[0][1] * (GammaMatrix[1][2] * GammaMatrix[2][0] - GammaMatrix[1][0] * GammaMatrix[2][2]);
        z = GammaMatrix[0][2] * (GammaMatrix[1][0] * GammaMatrix[2][1] - GammaMatrix[1][1] * GammaMatrix[2][0]);
        double GammaMatDet= x+y+z;


        x = tMatrix[0][0] * (tMatrix[1][1] * tMatrix[2][2] - tMatrix[1][2] * tMatrix[2][1]);
        y = tMatrix[0][1] * (tMatrix[1][2] * tMatrix[2][0] - tMatrix[1][0] * tMatrix[2][2]);
        z = tMatrix[0][2] * (tMatrix[1][0] * tMatrix[2][1] - tMatrix[1][1] * tMatrix[2][0]);
        double tMatDet= x+y+z;

        double beta = BetaMatDet/determinant;
        double gamma = GammaMatDet/determinant;
        double t = tMatDet/determinant;

        if(beta < 0 || gamma < 0 || t<0 || beta+gamma >1)return -1;

        if(level == 0)return t;

        Point intersectionPoint = ray.start + (ray.dir)*t;
        Color intersectionPointColor = getColorAt(intersectionPoint);
        color->r = intersectionPointColor.r*coEfficients[0]; ///ambient
        color->g = intersectionPointColor.g*coEfficients[0]; ///ambient
        color->b = intersectionPointColor.b*coEfficients[0]; ///ambient


        ///RTX ON
        for(int i = 0; i<pointLights.size(); i++){
            Ray L = Ray(pointLights[i].light_pos, intersectionPoint - (pointLights[i].light_pos)); ///Incident Ray
            Ray N = getNormal(intersectionPoint, L);
            Ray R = Ray(intersectionPoint, L.dir - N.dir*(L.dir*N.dir)*2);///Reflected
//            L.draw();
            ///shadow check
             double t, tmin =INF;

            for(int j=0; j<objects.size(); j++) {

                t = objects[j]->intersect(L, new Color(), 0);

                if(t>0.0 && t<tmin) {
                    tmin = t;
                }
            }

            Point shadowIntersection = L.start + (L.dir*tmin);

            double actual_distance = intersectionPoint.getDistance(L.start); ///distance between intersection and light source
            double shadow_distance = shadowIntersection.getDistance(L.start);

            if((actual_distance - epsilon) > shadow_distance){
                ///actual intersection is in shadow so ignore diffuse and specular
//                logfile << "actual intersection is in shadow so ignore diffuse and specular\n";
                continue;
            }

//            logfile << "L dir: " << L.dir << endl;
//            Point Ln = L.dir.negate_();
//            logfile << "L dir neg: " << Ln << endl;
//            logfile << "N dir: " << N.dir << endl;
            double lambert = (L.dir*(-1.0))*N.dir;
//            logfile << "Lambert lambert what a prick: " << lambert << endl;
            if(lambert > 0.0)logfile << "Lambert lambert what a prick triangle: " << lambert << endl;
            color->r += pointLights[i].color.r*this->coEfficients[1]*max(0.0, lambert)*intersectionPointColor.r;
            color->g += pointLights[i].color.g*this->coEfficients[1]*max(0.0, lambert)*intersectionPointColor.g;
            color->b += pointLights[i].color.b*this->coEfficients[1]*max(0.0, lambert)*intersectionPointColor.b;

            double phongValue = max(0.0, (ray.dir.negate_())*(R.dir));
            if(phongValue > 0.0)logfile << "Phonging triangle: " << phongValue<< endl;
            color->r += pointLights[i].color.r*this->coEfficients[2]*pow(phongValue, this->shine)*intersectionPointColor.r;
            color->g += pointLights[i].color.g*this->coEfficients[2]*pow(phongValue, this->shine)*intersectionPointColor.g;
            color->b += pointLights[i].color.b*this->coEfficients[2]*pow(phongValue, this->shine)*intersectionPointColor.b;

        }
        return t;

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

    bool ClipCheck(Point p){

        if(p.x < this->center.x)return false;
        if(p.y < this->center.y)return false;
        if(p.y < this->center.y)return false;

        if(fabs(this->length) > epsilon && p.x > (this->center.x+this->length))return false;
        if(fabs(this->width) > epsilon && p.y > (this->center.y+this->width))return false;
        if(fabs(this->height) > epsilon && p.z > (this->center.z+this->height))return false;

        return true;
    }
    double intersect(Ray ray, Color *color, int level){

        ///source: http://skuld.bmsc.washington.edu/people/merritt/graphics/quadrics.html
        double x0 = ray.start.x, y0 = ray.start.y, z0 = ray.start.z;
        double xd = ray.dir.x, yd = ray.dir.y, zd = ray.dir.z;

        double Aq = this->A*xd*xd + this->B*yd*yd + this->C*zd*zd + this->D*xd*yd + this->E*xd*zd + this->F*yd*zd;
        double Bq = 2*this->A*x0*xd + 2*this->B*y0*yd + 2*this->C*z0*zd + this->D*(x0*yd + y0*xd) + this->E*(x0*zd + z0*xd) + this->F*(y0*zd + yd*z0) + this->G*xd + this->H*yd + this->I*zd;
        double Cq = this->A*x0*x0 + this->B*y0*y0 + this->C*z0*z0 + this->D*x0*y0 + this->E*x0*z0 + this->F*y0*z0 + this->G*x0 + this->H*y0 + this->I*z0 + this->J;

        double t, t0, t1;

        if(Aq == 0){
            t = -Cq/Bq;
            if(t < 0 ) return -1;
        }

        else{
            double discriminant = Bq*Bq - 4*Aq*Cq;
            if(discriminant < 0.0){return -1;}
            t0 = (-Bq-sqrt(discriminant))/ (2*Aq);
            t1 = (-Bq+sqrt(discriminant))/ (2*Aq);
            if(t0 > 0.0){t = t0;}
            else if(t1 > 0.0){
                t=t1;
            }
            else{
                    return -1;
                }
        }

        ///clip

        Point p = ray.start + ray.dir*t;
        if(!ClipCheck(p)){
            return -1;
        }

        if(level == 0) return t;

        /// Light handle
        Point intersectionPoint = ray.start + (ray.dir)*t;

        Color intersectionPointColor = getColorAt(intersectionPoint);
//        intersectionPointColor.print();
        color->r = intersectionPointColor.r*coEfficients[0]; ///ambient
        color->g = intersectionPointColor.g*coEfficients[0]; ///ambient
        color->b = intersectionPointColor.b*coEfficients[0]; ///ambient

        return t;



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
        this->center.x = -floorWidth/2;
        this->center.y = -floorWidth/2;
        this->center.z = 0;
        this->length = tileWidth;
        this->floorWidth = floorWidth;
        this->tileWidth = tileWidth;
        this->type = "floor";
        this->coEfficients[0] = 0.4;
        this->coEfficients[1] = 0.5;
        this->coEfficients[2] = 0.1;
        this->coEfficients[3] = 0.1;
        this->shine = 10;
    }

    void draw(){

        int tileCount = floorWidth/tileWidth;
        for(int i = -tileCount/2; i<tileCount/2; i++){
            for(int j = -tileCount/2; j<tileCount/2; j++){

                double a = tileWidth/2;
                glPushMatrix();
                glTranslatef(i*tileWidth, j*tileWidth, 0);

                if((i+j)%2)glColor3f(0, 0, 0);
                else glColor3f(1, 1, 1);
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

        }

    }
    Color getColorAt(Point p){

        int i = ceil(p.x/tileWidth);
        int j = ceil(p.y/tileWidth);
        Color black(0, 0, 0);
        Color white(1, 1, 1);
        if((i+j) % 2)return white;
        else return black;


    }
    double intersect(Ray ray, Color *color, int level){

        Point n(0, 0, 1);
        if((eye*n) < 0)n.z=-1;

        if(n*ray.dir == 0)return -1;
        double t = -(ray.start*n)/(ray.dir*n);
        Point intersectionPoint;
        if(t>0){
            intersectionPoint = ray.start + ray.dir*t;
            if(intersectionPoint.x < (-floorWidth/2) || intersectionPoint.x > (floorWidth/2 - tileWidth) || intersectionPoint.y < (-floorWidth/2)|| intersectionPoint.y > (floorWidth/2 - tileWidth)){
                return -1;
            }


        }
        else{
            return -1;
        }

        if(level == 0)return t;

        Color intersectionPointColor = getColorAt(intersectionPoint);
//        intersectionPointColor.print();
        color->r = intersectionPointColor.r*coEfficients[0]; ///ambient
        color->g = intersectionPointColor.g*coEfficients[0]; ///ambient
        color->b = intersectionPointColor.b*coEfficients[0]; ///ambient

        ///RTX ON

        for(int i = 0; i<pointLights.size(); i++){
            Ray L = Ray(pointLights[i].light_pos, intersectionPoint - (pointLights[i].light_pos)); ///Incident Ray
            Ray N(intersectionPoint, Point(0, 0, 1));
            if(ray.start.z<=0){
                N.dir.z = -1;
            }
//            Ray N = getNormal(intersectionPoint, L);
            Ray R = Ray(intersectionPoint, L.dir - N.dir*(L.dir*N.dir)*2);///Reflected
//            L.draw();
            ///shadow check
             double t, tmin =INF;

            for(int j=0; j<objects.size(); j++) {

                t = objects[j]->intersect(L, new Color(), 0);

                if(t>0.0 && t<tmin) {
                    tmin = t;
                }
            }

            Point shadowIntersection = L.start + (L.dir*tmin);

            double actual_distance = intersectionPoint.getDistance(L.start); ///distance between intersection and light source
            double shadow_distance = shadowIntersection.getDistance(L.start);

            if((actual_distance - epsilon) > shadow_distance){
                ///actual intersection is in shadow so ignore diffuse and specular
//                logfile << "actual intersection is in shadow so ignore diffuse and specular\n";
                continue;
            }

//            logfile << "L dir: " << L.dir << endl;
//            Point Ln = L.dir.negate_();
//            logfile << "L dir neg: " << Ln << endl;
//            logfile << "N dir: " << N.dir << endl;
            double lambert = (L.dir*(-1.0))*N.dir;
//            logfile << "Lambert lambert what a prick: " << lambert << endl;
//            if(lambert > 0.0)logfile << "Lambert lambert what a prick triangle: " << lambert << endl;
            color->r += pointLights[i].color.r*this->coEfficients[1]*max(0.0, lambert)*intersectionPointColor.r;
            color->g += pointLights[i].color.g*this->coEfficients[1]*max(0.0, lambert)*intersectionPointColor.g;
            color->b += pointLights[i].color.b*this->coEfficients[1]*max(0.0, lambert)*intersectionPointColor.b;

            double phongValue = max(0.0, (ray.dir.negate_())*(R.dir));
//            if(phongValue > 0.0)logfile << "Phonging triangle: " << phongValue<< endl;
            color->r += pointLights[i].color.r*this->coEfficients[2]*pow(phongValue, this->shine)*intersectionPointColor.r;
            color->g += pointLights[i].color.g*this->coEfficients[2]*pow(phongValue, this->shine)*intersectionPointColor.g;
            color->b += pointLights[i].color.b*this->coEfficients[2]*pow(phongValue, this->shine)*intersectionPointColor.b;

        }

        return t;


    }

};

