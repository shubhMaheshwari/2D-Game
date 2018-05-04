#include "main.h"
#include "timer.h"
#include "ball.h"
#include "ground.h"
#include "objects.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/
int main_score;
const int ENEMY_COUNT = 40;
int up_flag = 0;
float viscous = 1;
int dead_balls[ENEMY_COUNT];
int dead_enemy_count = 0;

//  Objects

Enemy_Ball myball;
Ball balls[ENEMY_COUNT];
Ground ground1, ground2, ground3;
Plank enemy_plank[ENEMY_COUNT/2];
Trampoline trampoline;
Magnet magnet;
Pool pool;
Spike spike;

bounding_box_t screen_box;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 1.0;
double mouse_x = 0.0, mouse_y=0.0;
int mouse_action = 0;

int level = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
	// clear the color and depth in the frame buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use the loaded shader program
	// Don't change unless you know what you are doing
	glUseProgram (programID);

	// Eye - Location of camera. Don't change unless you are sure!!
	// glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
	// Target - Where is the camera looking at.  Don't change unless you are sure!!
	// glm::vec3 target (0, 0, 0);
	// Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
	// glm::vec3 up (0, 1, 0);

	// Compute Camera matrix (view)
	// Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
	// Don't change unless you are sure!!

	Matrices.view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

	// Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
	// Don't change unless you are sure!!
	glm::mat4 VP = Matrices.projection * Matrices.view;

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	// Don't change unless you are sure!!
	glm::mat4 MVP;  // MVP = Projection * View * Model

	// Scene render the order is important for overwriting the objects

	for(int i=0;i<ENEMY_COUNT;i++)
		if(dead_balls[i]==0)	
			{
				balls[i].draw(VP);
				if(i%2 == 0)
					enemy_plank[i/2].draw(VP);
			}

	ground1.draw(VP);
	ground2.draw(VP);
	ground3.draw(VP);

	trampoline.draw(VP);
	
	if(level > 0)
	magnet.draw(VP);

	pool.draw(VP);

	myball.draw(VP);

	if(level > 1)
		spike.draw(VP);


}

void tick_input(GLFWwindow *window) {
	int left  = glfwGetKey(window, GLFW_KEY_LEFT);
	int right = glfwGetKey(window, GLFW_KEY_RIGHT);
	int up    = glfwGetKey(window, GLFW_KEY_UP);
	int down    = glfwGetKey(window, GLFW_KEY_DOWN);
	int a 	= glfwGetKey(window, GLFW_KEY_A);
	int s 	= glfwGetKey(window, GLFW_KEY_S);
	int d 	= glfwGetKey(window, GLFW_KEY_D);
	int w 	= glfwGetKey(window, GLFW_KEY_W);

	if(a){
		screen_center_x -= 0.01;
		reset_screen(2);
	}
	
	if(s){
		screen_center_y -= 0.01;
		reset_screen(2);
	}

	if(d){
		screen_center_x += 0.01;
		reset_screen(2);
	}

if(w){
		screen_center_y += 0.01;
		reset_screen(2);
	}



	// Accelerate on key press and friction on no key press 
	if (left) {
		myball.speed += 0.002;
	}
	else if (right) {
		myball.speed -= 0.002;
	}
	else {
		if (myball.speed < 0) {
			myball.speed += 0.001;
		}
		else if (myball.speed > 0) {
			myball.speed -= 0.001;
		}
		else {
			myball.speed = 0;
		}
	}

	// For making myball jump
	if (up == GLFW_PRESS and !up_flag) {
		up_flag = 1;
		myball.yspeed = 0.25f;
	}

	// if (up == GLFW_RELEASE) {
	// 	up_flag = 0;
	// }


}

bool tick_elements() {
	myball.tick(viscous);
	// ball2.tick(); //Stop moving the ball 
	for(int i=0;i<ENEMY_COUNT;i++)
		{
			if(dead_balls[i]==0)
			{
				balls[i].tick();

				if(i%2 == 0) 
					{
						enemy_plank[i/2].tick();										
						if (enemy_plank[i/2].detect_collision(myball))
						{	
							main_score+=100;   
							float m  = tan((enemy_plank[i/2].rotation + 90)*M_PI/180.0);
							myball.speed = myball.speed - enemy_plank[i/2].speed;	
							myball.speed = (myball.speed*(1 - m*m) + 2*m*myball.yspeed)/(1 + m*m);
							myball.speed = myball.speed + enemy_plank[i/2].speed;
							myball.yspeed =(myball.yspeed*(m*m - 1) + 2*m*myball.speed)/(1 + m*m);             

						}
					}

				if (detect_collision(myball.bounding_box(), balls[i].bounding_box()))
					{
						dead_balls[i] = 1;
						dead_enemy_count++;
						main_score+=100;
						if(myball.position.y > balls[i].position.y)
							myball.yspeed += 0.15f;                
					}
	
				if(!detect_collision(balls[i].bounding_box(),screen_box) )
					{
						// printf("crossing border:%d\n",i );
						balls[i].position.x = -balls[i].position.x;
						if(i%2 == 0)
							{
								enemy_plank[i/2].position.x = balls[i].position.x + balls[i].radius*cos(enemy_plank[i/2].rotation*M_PI/180.0);						
								enemy_plank[i/2].position.y = balls[i].position.y + balls[i].radius*sin(enemy_plank[i/2].rotation*M_PI/180.0);						
								
							}

					}


			}

		}


	if(trampoline.detect_collision(myball))
		{
			
			myball.yspeed  =0.27f;	
			myball.position.y =1.0f;
		}

	if(magnet.detect_collision(myball) && level > 0)
		{
			// printf("Magnet Collision:%lf\n",myball.yspeed);
			myball.speed += 0.003;
		}	


	if(!detect_collision(myball.bounding_box(),screen_box))	
		myball.position.x = -myball.position.x;


	if(ground1.detect_collision(myball) || ground2.detect_collision(myball))
	{
		// printf("Ground Collision\n");
		myball.yspeed = -0.5*myball.yspeed;
		myball.position.y = 0;
		
		up_flag = 0;

	}


	if( std::fabs(pool.detect_collision(myball) ) <= 0.4f  && pool.position.y > myball.position.y )
	{

		// slope of the replecting plane 
		float m  = (pool.position.x - myball.position.x)/(pool.position.y - myball.position.y);
		myball.speed = (myball.speed*(1 - m*m) + 2*m*myball.yspeed)/(1 + m*m);
		myball.yspeed =(myball.yspeed*(m*m - 1) + 2*m*myball.speed)/(1 + m*m);

		up_flag = 0;


	}

	if (pool.position.y > myball.position.y)
		viscous = 0.6;

	else
		viscous = 1;
		// printf("No Collision detected\n");

	spike.tick();

	if(spike.detect_collision(myball) && level > 1)
	{
		myball.position.x = 0;
		myball.position.y = 0;
		main_score -=50;
	}


	return false;    
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
	/* Objects should be created before any other gl function and shaders */
	// Create the models
	main_score = 0;

	myball       = Enemy_Ball(0, 0,0.2,0.0, COLOR_BLACK);

	for(int i=0;i<ENEMY_COUNT;i++){
		color_t BALL_COLOR = {rand()%265, rand()%265, rand()%265};
		balls[i] = Ball(16*(rand()%1000)/1000.0 -8, 3*(rand()%1000)/1000.0 + 1, 0.2 + (rand()%100)/500.0 ,0.01 + (rand()%1000)/100000.0,BALL_COLOR);
	}

	// Attach a plank to i/2 th ball by taking its cordinates 
	float plank_x,plank_y,angle;

	for(int i=0;i<ENEMY_COUNT/2;i++)
		{
			angle = rand()%90;
			plank_x = balls[i*2].position.x + balls[i*2].radius*cos(angle*M_PI/180.0);
			plank_y = balls[i*2].position.y + balls[i*2].radius*sin(angle*M_PI/180.0);

			enemy_plank[i] = Plank(plank_x,plank_y,0.1,2*balls[i*2].radius,angle,balls[i*2].speed,COLOR_BROWN);
		}


	ground1       = Ground(6,-1.0,8,1.4,0.0,0.0,COLOR_GREEN);
	ground2       = Ground(-6,-1.0,8,1.4,0.0,0.0,COLOR_GREEN);
	ground3       = Ground(0,-3,20,4,0.0,0.0,COLOR_YELLOW);
	trampoline 	  = Trampoline(6,-0.25,COLOR_BLACK);
	magnet 	  	  = Magnet(-5.5,4,COLOR_RED);
	pool 		  = Pool(0,-1,COLOR_BLUE);
	spike 		  = Spike(ground2.position.x,ground2.position.y +ground2.height/2.0 , 1.0f, 0.05f, ground2.width - 1.0f,COLOR_BROWN);	
 

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


	reshapeWindow (window, width, height);

	// Background color of the scene
	glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
	glClearDepth (1.0f);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

	cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
	cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
	cout << "VERSION: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
	srand(time(0));
	int width  = 1600;
	int height = 800;

	window = initGLFW(width, height);

	initGL (window, width, height);

    screen_box = { screen_center_x, screen_center_y, 7.0f,10.0f};



	/* Draw in loop */
	for(int i=0;!glfwWindowShouldClose(window) && dead_enemy_count < ENEMY_COUNT;i++) {
		// Process timers

		if (t60.processTick()) {
			// 60 fps
			// OpenGL Draw commands
			draw();
			// Swap Frame Buffer in double buffering
			glfwSwapBuffers(window);

			tick_elements();

			tick_input(window);

			reset_screen(2);

			level = main_score/1000;
		}

		// Poll for Keyboard and mouse events
		glfwPollEvents();

		
		// if(mouse_action==1){
		// 	printf("x:%lf y:%lf\n",myball.position.x,myball.position.y);
		// 	printf("mx:%lf my:%lf\n",mouse_x,mouse_y);


		// 	// myball.position.x = mouse_x;
		// 	// myball.position.y = mouse_y;
 	// 	}

		if( i%100000 == 0){

			std::string  str_score = "##Flying Ball##";
    		const char *dis_score = str_score.c_str();
			glfwSetWindowTitle(window,dis_score);
					
		}

	}



	quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
	// checking that the center of the circle is less than their sum of radius
	// (width or height) = radius of the circle
	return ( ( ( a.x- b.x )*( a.x- b.x ) + ( a.y- b.y )*( a.y- b.y ) ) < ( a.width + b.width ) * ( a.height + b.height ) );    
}

/* Also takes care of the projection matrix on window resize
https://stackoverflow.com/questions/3267243/in-opengl-how-can-i-adjust-for-the-window-being-resized */

void reset_screen(float ratio) {
	float top    = screen_center_y + 4 / screen_zoom;
	float bottom = screen_center_y - 4 / screen_zoom;
	float left   = screen_center_x - 4 / screen_zoom;
	float right  = screen_center_x + 4 / screen_zoom;
	Matrices.projection = glm::ortho(left*ratio, right*ratio, bottom, top, 0.1f, 500.0f);
}
