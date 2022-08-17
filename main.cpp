#include "1705036_classes.h"

int drawgrid;
int drawaxes;
double angle;

//vector <Object*> objects = objects;
//vector <PointLight> pointLights = pointLights;
//vector <SpotLight> spotLights = spotLights;

Point eye;
Point u;
Point r;
Point l;

int windowHeight = 500;
int windowWidth = 500;
double viewAngle = 80;

void drawAxes()
{
//    cout << "AYOO\n";
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

int recursion_level, imageWidth, imageHeight, object_count;
string type_of_object;

void loadData(){
    ifstream ifile("D:\\BUET\\L4T1\\CSE 410 Computer Graphics Sessional\\Assignment 3\\Ray Tracing\\scene.txt");
    if(ifile.is_open()){
//        cout << "File is open";
//        string line;
        while (!ifile.eof())
        {
            ifile >> recursion_level;
            cout << recursion_level << endl;

            ifile >> imageWidth;
            cout << imageWidth << endl;
            imageHeight = imageWidth;
            ifile >> object_count;
            cout << object_count << endl;

            for(int i = 0; i< object_count; i++){
                ifile >> type_of_object;
                if(type_of_object == "sphere"){
                    cout << "sphere " << i << endl;
                    Point center;
                    double radius, a, d, sp, rc;
                    Color color;
                    int shine;
                    ifile >> center.x >> center.y >> center.z;
//                    center.print();
                    ifile >> radius;
//                    cout << radius << endl;
                    ifile >> color.r >> color.g >> color.b;
//                    color.print();
                    ifile >> a >> d >> sp >> rc;
                    ifile >> shine;
//                    ifile >>
                    Object *temp;
                    temp = new Sphere(center, radius);
                    temp->setColor(color);
                    temp->setCoEfficients(a, d, sp, rc);

                    temp->setShine(shine);
                    objects.push_back(temp);


                }

                else if(type_of_object == "triangle"){
                    cout << "triangle " << i << endl;

                    Point p1, p2, p3;
                    Color color;
                    double a, d, sp, rc;
                    int shine;

                    ifile >> p1.x >> p1.y >> p1.z;
                    ifile >> p2.x >> p2.y >> p2.z;
                    ifile >> p3.x >> p3.y >> p3.z;
                    ifile >> color.r >> color.g >> color.b;
                    ifile >> a >> d >> sp >> rc;
                    ifile >> shine;

                    Object *temp;
                    temp = new Triangle(p1, p2, p3);
                    temp->setColor(color);
                    temp->setCoEfficients(a, d, sp, rc);
                    temp->setShine(shine);
                    objects.push_back(temp);

//                    temp->print();
//                    break;
                }

                else if(type_of_object == "general"){
                    cout << "general " << i << endl;
                    double A, B, C, D, E, F, G, H, I ,J;
                    Color color;
                    Point point;
                    double length, width, height;
                    double a, d, sp, rc;
                    int shine;

                    ifile >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
                    ifile >> point.x >> point.y >> point.z >> length >> width >> height;
                    ifile >> color.r >> color.g >> color.b;
                    ifile >> a >> d >> sp >> rc;
                    ifile >> shine;
                    Object *temp;
                    temp = new General(A, B, C, D, E, F, G, H, I ,J);
                    temp->setColor(color);
                    temp->setCoEfficients(a, d, sp, rc);
                    temp->setShine(shine);
//                    temp->print();
                    objects.push_back(temp);
//                    break;

                }

                else{
                    cout << "Not one of the defined objects?!?\n";
                }

            }

            int point_light_count;
            ifile >> point_light_count;

            for(int i = 0; i<point_light_count; i++){
                cout << "point light " << i << endl;
                Point light_position;
                Color color;

                ifile >> light_position.x >> light_position.y >> light_position.z;
                ifile >> color.r >> color.g >> color.b;

                PointLight pl(light_position, color);
//                pl.print();
                pointLights.push_back(pl);


            }

            int spotlight_count;
            ifile >> spotlight_count;
            if(ifile.eof()){
//                cout << "EOF reached\n";
                break;
            }

            for(int i = 0; i<spotlight_count; i++){
                cout << "spot light " << i << endl;
                Point light_position;
                Point light_dir;
                Color color;
                double angle;

                ifile >> light_position.x >> light_position.y >> light_position.z;
                ifile >> color.r >> color.g >> color.b;

                PointLight pl(light_position, color);
                ifile >> light_dir.x >> light_dir.y >> light_dir.z;
                ifile >> angle;
                SpotLight sl(pl, light_dir, angle);
//                sl.print();
                spotLights.push_back(sl);
            }

            break;


        }

        Object *temp = new Floor(1000, 20);
//        temp->setColor()
        objects.push_back(temp);
//        for(int i=0;i<pointLights.size(); i++){
//            pointLights[i].print();
//        }
//        for(int i=0;i<objects.size(); i++){
//            objects[i]->print();
//        }


    }

    else{
        cout << "WHAAAT\n";
    }
//    cout << "Loaded Diaper\n";
}

int img_count = 1;
void capture(){
    cout << "capturing..." << endl;
    bitmap_image image(imageWidth, imageHeight);
    for(int i=0;i<imageWidth;i++){
        for(int j=0;j<imageHeight;j++){
            image.set_pixel(i,j,0, 0, 0);
        }
    }

    double planeDistance = (windowHeight/2.0)/tan(viewAngle/2.0*(pi/180));

    Point topLeft = eye + (l*planeDistance) - (r*(windowWidth/2)) + (u*(windowHeight/2));
//    topLeft.print();

    double du = 1.0*windowWidth/imageWidth;
    double dv = 1.0*windowHeight/imageHeight;

    // Choose middle of the grid cell
    topLeft = topLeft + r*(0.5*du) - u*(0.5*dv);
//    topLeft.print();
    int nearest;
    double t;
    double tMin=INF;

    for(int i = 0; i< imageWidth; i++){
        for(int j = 0; j< imageHeight; j++){
            nearest = -INF;
            tMin = INF;
            Point curPixel = topLeft + (r*i*du) - (u*j*dv);
            Ray ray(eye, curPixel - eye);
            Color *color = new Color();

            for(int k = 0; k<objects.size(); k++){
//                logfile << "K: " << k << endl;
                t = objects[k]->intersect(ray, color, 0);
                if(t!=-1.0)logfile << "t: " << t << endl;
                if(t>0 && t<tMin){
                    tMin = t;
                    nearest = k;
                }
            }

            if(nearest != -INF){

                double t2 = objects[nearest]->intersect(ray, color, 1);
//                color->print();
                color->clip();
                image.set_pixel(i+1, j+1, round(color->r * 255), round(color->g * 255), round(color->b * 255));
            }
        }


    }
    string imgname = "output_1" + to_string(img_count) + ".bmp";
    image.save_image("D:\\BUET\\L4T1\\CSE 410 Computer Graphics Sessional\\Assignment 3\\Ray Tracing\\" + imgname);
    img_count++;
    cout << "captured" << endl;
}

void keyboardListener(unsigned char key, int x,int y){

	switch(key){
        case 'q':
        exit(0);

		case 'g':
			drawgrid=1-drawgrid;
			break;

        case '1':

            l = l*(cos(-2*pi/180)) + l.cross_mult(u)*(sin(-2*pi/180));
            r = r*(cos(-2*pi/180)) + l.cross_mult(u)*(sin(-2*pi/180));
			break;

        case '2':
            l = l*(cos(2*pi/180)) + l.cross_mult(u)*(sin(2*pi/180));
            r = r*(cos(2*pi/180)) + l.cross_mult(u)*(sin(2*pi/180));
			break;

        case '3':
            l = l*(cos(2*pi/180)) + r.cross_mult(l)*(sin(2*pi/180));
            u = u*(cos(2*pi/180)) + r.cross_mult(u)*(sin(2*pi/180));
			break;
        case '4':
            l = l*(cos(-2*pi/180)) + r.cross_mult(l)*(sin(-2*pi/180));
            u = u*(cos(-2*pi/180)) + r.cross_mult(u)*(sin(-2*pi/180));
			break;
        case '5':
            r = r*(cos(2*pi/180)) + l.cross_mult(u)*(sin(2*pi/180));
            u = u*(cos(2*pi/180)) + l.cross_mult(u)*(sin(2*pi/180));
			break;

          case '6':
            r = r*(cos(-2*pi/180)) + l.cross_mult(u)*(sin(-2*pi/180));
            u = u*(cos(-2*pi/180)) + l.cross_mult(u)*(sin(-2*pi/180));
			break;

         case '8':

            angle += 2;
            break;

         case '9':
            angle -= 2;
			break;

         case '0':
            capture();
            break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_UP:		// up arrow key
            eye.x -= 2.0;
            eye.y -= 2.0;
			break;
		case GLUT_KEY_DOWN:		//down arrow key
            eye.x += 2.0;
            eye.y += 2.0;
			break;

		case GLUT_KEY_RIGHT:
            eye.x -= 2.0;
            eye.y += 2.0;
			break;
		case GLUT_KEY_LEFT:
			eye.x += 2.0;
            eye.y -= 2.0;

			break;

		case GLUT_KEY_PAGE_UP:
			eye.z += 3.0;
			break;
		case GLUT_KEY_PAGE_DOWN:
			eye.z -= 3.0;
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

double cameraAngle;


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

	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
//	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(eye.x, eye.y, eye.z,
           eye.x+l.x ,eye.y+l.y, eye.z+l.z,
           u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

//	drawSphere(30,24,20);
    for(int i=0;i<objects.size(); i++){
        objects[i]->draw();
    }



	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	angle=0;
	cameraAngle=1.0;

    eye.x = 80, eye.y = 80, eye.z = 20;
    u.x = 0, u.y = 0, u.z = 1;
    r.x = -1/sqrt(2), r.y = 1/sqrt(2), r.z = 0;
    l.x = -1/sqrt(2), l.y = -1/sqrt(2), l.z = 0;
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
	gluPerspective(viewAngle, 1, 1, 1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}
//goto function: ctrl-shift-g



int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(windowHeight, windowWidth);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Ray Tracing");

	init();

    loadData();
	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
