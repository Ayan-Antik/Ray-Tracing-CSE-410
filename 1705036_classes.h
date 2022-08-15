#include <bits/stdc++.h>
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
};

class Object{

public:
    Point reference_point; // should have x, y, z
    double height, width, length;
    double color[3];
    double coEfficients[4]; // ambient, diffuse, specular, reflection coefficients
    int shine; // exponent term of specular component
    Object(){

    }
    virtual void draw(){}
    void setColor(){}
    void setShine(){}
    void setCoEfficients(){}
};

class Sphere:public Object{

    Sphere(Point center, double radius){
        reference_point = center;
        length = radius;
    }

    void draw(){

    }

};

class PointLight{

public:
    Point light_pos;
    double color[3];

};

class SpotLight{
    PointLight point_light;
    Point light_direction;
    double cutoff_angle;

};
