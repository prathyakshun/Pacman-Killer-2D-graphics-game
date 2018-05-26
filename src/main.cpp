#include "main.h"
#include <string>
#include "timer.h"
#include "ball.h"
#include "floor.h"
#include "midbase.h"
#include "pond.h"
#include "trampoline.h"
#include "thorn.h"
#include "plank.h"
#include "magnet.h"
#include "magnetblack.h"
#include <GL/glut.h>
#include "one.h"
#include "two.h"
#include "three.h"
#include "four.h"
#include "five.h"
#include "six.h"
#include "seven.h"
#include "eight.h"
#include "nine.h"
#include "zero.h"
#include "negative.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

extern bool cannon_keyboard_input, drag_pan;
extern double drag_oldx, drag_oldy;
Ball ball1, ball2;
Floor floor1;
Midbase midbase1;
Pond pond1, pond2;
Trampoline trampoline1;
vector <Thorn> thorn;
Thorn thorn1, thorn2, thorn3, thorn4, thorn5;
Magnet magnet1, magnet2;
MagnetBlack magnet1black, magnet2black;
vector <Ball> ballarr;
vector <Plank> plankarr;
One one1[10]; Two two1[10]; Three three1[10]; Four four1[10]; Five five1[10]; Six six1[10]; Seven seven1[10]; Eight eight1[10]; Nine nine1[10]; Zero zero1[10]; Negative negative1;
int counterball = 0, counter2=0, rn = 0, leftright=0, counterlevel=0;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0, right_screen = 4, left_screen = -4;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    // Clear Screen And Depth Buffer
    glClearColor(0.0, 0.0, 0.0, 0.0); //
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //
     // Reset The Current Modelview Matrix //
      glLoadIdentity(); //
//      glClearColor(0.0, 0.0, 0.0, 0.0); //
//    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    magnet1.draw(VP);
    magnet2.draw(VP);
    magnet1black.draw(VP);
    magnet2black.draw(VP);
    floor1.draw(VP);
    midbase1.draw(VP);
    pond1.draw(VP);
    pond2.draw(VP);
    ball1.draw(VP);
    trampoline1.draw(VP);
    int i = 0;
    for (i=0;i<10;i++)
    {
        one1[i].draw(VP);
        two1[i].draw(VP);
        three1[i].draw(VP);
        four1[i].draw(VP);
        five1[i].draw(VP);
        six1[i].draw(VP);
        seven1[i].draw(VP);
        eight1[i].draw(VP);
        nine1[i].draw(VP);
        zero1[i].draw(VP);
    }
    negative1.draw(VP);
    for (i=0;i<thorn.size();i++)   thorn[i].draw(VP);
    for (i=0;i<ballarr.size();i++)
    {
        ballarr[i].draw(VP);
        if (plankarr[i].plank_there)
            plankarr[i].draw(VP);
    }
}


/* For zooming on mouse scroll */
void zooming(double yoffset)
{
    if (yoffset > 0)    screen_zoom+=0.1;
    else if (yoffset < 0)   screen_zoom-=0.1;
    if (screen_zoom <= 0)   screen_zoom = 0.1;
    reset_screen();
}


bool detect_ball_collision(Ball a, Ball b)
{
    if (a.upspeed < 0 && a.position.x < b.position.x + b.radius && a.position.x > b.position.x - b.radius && a.position.y < b.position.y + b.radius && a.position.y > b.position.y - b.radius)
        return 1;
    if (a.upspeed < 0 && b.position.x < a.position.x + a.radius && b.position.x > a.position.x - a.radius && b.position.y < a.position.y + a.radius && b.position.y > a.position.y - a.radius)
        return 1;
    return 0;
}

void mouse(int button, int state, int x, int y)
{
   // Wheel reports as button 3(scroll up) and button 4(scroll down)
   if ((button == 3) || (button == 4)) // It's a wheel event
   {
       // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
       if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events
       printf("Scroll %s At %d %d\n", (button == 3) ? "Up" : "Down", x, y);
   }else{  // normal button event
       printf("Button %s At %d %d\n", (state == GLUT_DOWN) ? "Down" : "Up", x, y);
   }
}


void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_A);
    int right = glfwGetKey(window, GLFW_KEY_D);
    int up = glfwGetKey(window, GLFW_KEY_SPACE);


    /* Arrow keys to pan the screen */
    if (glfwGetKey(window, GLFW_KEY_LEFT))
    {
        screen_center_x -= 0.03;
        reset_screen();
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT))
    {
        screen_center_x += 0.03;
        reset_screen();
    }
    if (glfwGetKey(window, GLFW_KEY_UP))
    {
        screen_center_y += 0.03;
        reset_screen();
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN))
    {
        screen_center_y -= 0.03;
        reset_screen();
    }
    /* Mouse drag detection */
    int drag_pan = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (drag_pan)
    {
        if (drag_oldx == -1 || drag_oldy == -1)
        {
            glfwGetCursorPos(window, &drag_oldx, &drag_oldy);
        }
        else
        {
            double newx, newy;
            glfwGetCursorPos(window, &newx, &newy);
            if (newx < drag_oldx)   left = 1;
            else if (newx > drag_oldx)  right = 1;
            if (newy < drag_oldy)   up = 1;
            drag_oldx = newx;
            drag_oldy = newy;
        }
    }


    /* Move the player ball left */
    if (left) {
        leftright = 1;
        ball1.speed = -abs(0.012);
        ball1.tick(2);
        //        if (detect_collision(ball1.bounding_box(), pond1.bounding_box()) && ball1.position.y <= -2.388)  {ball1.speed = -abs(0.03);  ball1.upspeed = 0;}
        /* To prevent the ball from going through the trampoline */
        if (detect_collision(ball1.bounding_box(), trampoline1.bounding_box()) && ball1.position.y <= -1.6)  {ball1.speed = abs(0.012);  ball1.upspeed = 0;}
        ball1.tick(2);
        //ball1.rotation = ball1.rotation + 1;
    }

    /* Move the player ball right */
    if (right){
        leftright = 2;
        ball1.speed = abs(0.012);
        ball1.tick(1);
    //    printf("ball1.position.y is %lf\n",ball1.position.y);
        //        if (detect_collision(ball1.bounding_box(), pond1.bounding_box()) && ball1.position.y <= -2.388)  {ball1.speed = abs(0.03);  ball1.upspeed = 0;}
        /* To prevent the ball from going through the trampoline */
        if (detect_collision(ball1.bounding_box(), trampoline1.bounding_box()) && ball1.position.y <= -1.6) {ball1.speed = -abs(0.012);   ball1.upspeed = 0;}
        ball1.tick(1);
    }


    /* When ball is near the trampoline and player presses up */
    if (up /*&& ball1.inair == 0*/ && ball1.intrampoline == 1 && ball1.position.y<=-1.6)
    {
        ball1.upspeed = 0.2;
        ball1.position.y += ball1.upspeed;
        ball1.counter = 1;
        ball1.inair = 1;
        ball1.intrampoline = 0;
        ball1.tick(0);
    }


    /* Jump when on the ground */
    if (up && ball1.inair==0 && (ball1.position.x<=-1.75 || ball1.position.x>=-0.24))  // Can't jump from the surface of the pond
    {
        ball1.upspeed = 0.2;
        ball1.counter = 1;
        ball1.inair = 1;
        ball1.tick(0);
    }


    /* Jump when on the water */
    if (up && ball1.inwater == 1)
    {
    //    ball1.upspeed = 0.2;
    //    ball1.inair = 1;
        ball1.tick(3);
    }

    /* Infinite movement */
    if (ball1.position.x>=7.8) ball1.position.x = -4.24;            // right to left
    else if (ball1.position.x<=-4.25)   ball1.position.x = 7.8;     // left to right
    /* Moving the screen with player movement */
    if (ball1.position.x <= (screen_center_x) && (left || right) || ball1.speed!=0)    {screen_center_x = ball1.position.x;   left_screen -= 0.027;  right_screen -= 0.027; reset_screen();}
    if (ball1.position.x >= (screen_center_x) && (right || left) || ball1.speed!=0)    {screen_center_x = ball1.position.x;   right_screen += 0.027;  left_screen += 0.027; reset_screen();}
}

void tick_elements() {

    char stitle[1000];   stitle[0] = '\0';
    char title1st[] = "Pacman Killer - Level "; char time1st[] = " Points \t\t\t Time remaining "; char req_score[] = " GET ";
    char levelstr[10], timestr[10], req_scorestr[10];
    sprintf(levelstr,"%d",ball1.level);
    sprintf(timestr, "%d",60-counterlevel/60);
    sprintf(req_scorestr, "%d", ball1.level*300);
    strcat(stitle,title1st);
    strcat(stitle, levelstr);
    strcat(stitle, req_score);
    strcat(stitle, req_scorestr);
    strcat(stitle,time1st);
    strcat(stitle,timestr);
    glfwSetWindowTitle(window, stitle);



    /* Not using */
//    if (ball1.position.y <= (screen_center_y))    {screen_center_y = ball1.position.y;   left_screen -= 0.027;  right_screen -= 0.027; reset_screen();}
//    if (ball1.position.y >= (screen_center_y))    {screen_center_y = ball1.position.y;   right_screen += 0.027;  left_screen += 0.027; reset_screen();}

    int i=0;
    /* When no thorns are present, genereate new thorns */
    if (thorn.size() == 0 && ball1.level>1)
    {
        thorn.push_back(Thorn(-3, -2.1, COLOR_GRAY, -4, -2.4));
        thorn.push_back(Thorn(-1.8,-2.1, COLOR_GRAY, -2.4, -1.72));
        thorn.push_back(Thorn(0, -2.1, COLOR_GRAY, -0.28, 0.8));
        thorn.push_back(Thorn(1, -2.1, COLOR_GRAY,0.8, 2.4));
        thorn.push_back(Thorn(3, -2.1, COLOR_GRAY, 2.4, 4));
    }

    /* Magnet moving in the screen */
    if (magnet1.position.x >= -5 && magnet1.position.x <=-3.5)  {magnet1.position.x += magnet1.speed; magnet1black.position.x += magnet1.speed;}
    if (magnet2.position.x <= 5 && magnet2.position.x >=3.5) {magnet2.position.x -= magnet2.speed; magnet2black.position.x -= magnet2.speed;}

    /* Check if the player ball in magnet's range */
    if (ball1.position.x <= magnet1.position.x + 1 && ball1.position.y >= 0.5 && ball1.level>2)
        ball1.in_magnet1 = 1;
    else if (ball1.position.x >= magnet2.position.x - 1 && ball1.position.y >= 0.5 && ball1.level>1)
        ball1.in_magnet2 = 1;
    else
    {
        ball1.in_magnet1 = 0;
        ball1.in_magnet2 = 0;
        ball1.magnet_speed = 0.001;
    }


    /* Detect collision between thorn and player ball */
    for (i=0;i<thorn.size();i++)
    {
        if (ball1.position.x >= thorn[i].position.x && ball1.position.x <= thorn[i].position.x+0.15 && ball1.position.y <= -1.78)
        {
            ball1.score -= 50*ball1.level;
            ball1.upspeed = 0.16;
            thorn.erase(thorn.begin()+i);
        }
    }

    /* Send 8 balls every 200 counts in 60 fps*/
    for (i=0;i<8 && counterball%200==0;i++)
    {
        double y = -1 + (rand()%50)*0.1;
        printf("y is %lf\n",y);
        double radii = (rand()%3+1)*0.1;
        int plank_there = rand()%2;
        int color = rand()%3;
        Ball b1;
        if (color == 0) b1 = Ball(-7.5,y,COLOR_YELLOW,COLOR_YELLOW,COLOR_YELLOW,COLOR_YELLOW,COLOR_YELLOW,COLOR_YELLOW,COLOR_YELLOW,COLOR_BLACK, COLOR_PLANK_BROWN, radii,plank_there);
        else if (color == 1) b1 = Ball(-7.5,y,COLOR_DARK_BLUE,COLOR_DARK_BLUE,COLOR_DARK_BLUE,COLOR_DARK_BLUE,COLOR_DARK_BLUE,COLOR_DARK_BLUE,COLOR_DARK_BLUE,COLOR_BLACK, COLOR_PLANK_BROWN, radii,plank_there);
        else if (color == 2) b1 = Ball(-7.5,y,COLOR_FLY_VIOLET, COLOR_FLY_VIOLET,COLOR_FLY_VIOLET,COLOR_FLY_VIOLET,COLOR_FLY_VIOLET,COLOR_FLY_VIOLET,COLOR_FLY_VIOLET,COLOR_BLACK, COLOR_PLANK_BROWN, radii,plank_there);
        Plank p1 = Plank(-7.5, y, COLOR_PLANK_BROWN, radii,plank_there);
        b1.speed = (rand()%3+1)*0.01;
//        b1.rotation = 60;
        b1.rotation = (rand()%180+1)-90;
        p1.rotation = b1.rotation;
        p1.speed = b1.speed;
        ballarr.push_back(b1);
        plankarr.push_back(p1);
    }
    counterball++;


    /* Move the flying balls at their own speeds */
    for (i=0;i<ballarr.size();i++)
    {
        ballarr[i].position.x += ballarr[i].speed;
        plankarr[i].position.x += plankarr[i].speed;
        if (ballarr[i].position.x >= 12)
        {
            ballarr.erase(ballarr.begin()+i);
            plankarr.erase(plankarr.begin()+i);
        }
    }

    /* Thorns move in one direction every 200 counts */
    counter2++;
    for (i=0;i<thorn.size();i++)
    {
        if (counter2%200 == 0)
            rn = rand()%2;
        if (rn == 0 && thorn[i].position.x + thorn[i].speed <= thorn[i].xmax)
            thorn[i].position.x += thorn[i].speed;
        else if (rn == 1 && thorn[i].position.x - thorn[i].speed >= thorn[i].xmin)
            thorn[i].position.x -= thorn[i].speed;
    }


    /* Detect if the ball is in the water */
    if (detect_collision(ball1.bounding_box(), pond1.bounding_box()) || (ball1.position.y >= -2.65 && ball1.position.y <= -1.8))
        ball1.inwater = 1;
    else
        ball1.inwater = 0;


    /* Decrease the player ball's upspeed as it moves in the air */
    if (ball1.inair == 1 && !ball1.intrampoline)
        ball1.upspeed += ball1.deceleration;
    /* Decrease the player ball's upspeed as it moves in the water */
    if (ball1.inwater == 1)
        ball1.upspeed += ball1.deceleration;


    /* Detect if ball is touching the trampoline */
    if (detect_collision(ball1.bounding_box(), trampoline1.bounding_box())) {
          ball1.upspeed = 0;
          ball1.inair = 0;
          ball1.speed = 0;
          ball1.intrampoline = 1;
    }
    else
        ball1.intrampoline = 0;


    /* Ball doesn't move when left/right is not pressed */
    if (leftright == 1)
    {
        if (ball1.speed > 0)
            ball1.speed += ball1.xdeceleration;
        else
            ball1.speed = 0;
    }
    else if (leftright == 2)
    {
        if (ball1.speed < 0)
            ball1.speed -= ball1.xdeceleration;
        else
            ball1.speed = 0;
    }


    /* Detects collision with player ball with other balls and follows laws of reflection */
    for (i=0;i<ballarr.size();i++)
    {
        if (detect_ball_collision(ball1, ballarr[i]))
        {
            if (plankarr[i].plank_there)
            {
                ball1.score += (100 * ballarr[i].radius * 2);
                double vx = ball1.speed;
                double vy =  ball1.upspeed;
                double theta = atan(vy/vx)* (180 / 3.1415);
                //printf("Angle is %lf\n",theta);
                double vr = sqrt(vx*vx + vy*vy);
                double alpha = 180-plankarr[i].rotation;
                //            double finalVx = -1 * vr * cos (glm::radians(90-plankarr[i].rotation));
                //            double finalVy = 1* vr * sin (glm::radians(90-plankarr[i].rotation));
                //            printf("plank angle is %lf\n",plankarr[i].rotation);
                //            printf("cosx is %lf sinx is %lf\n",cos(glm::radians(90-plankarr[i].rotation)), sin(glm::radians(90-plankarr[i].rotation)));
                double finalVx = 1 * 0.2 * cos(glm::radians(2*alpha + theta));
                double finalVy = -1 * 0.2 * sin(glm::radians(2*alpha + theta));
                //            printf("finalVx %lf finalVy %lf\n",finalVx, finalVy);
                ball1.position.x += finalVx;
                ball1.position.y += finalVy;
                ball1.upspeed = finalVy;
                ball1.speed = finalVx;
                if (ball1.speed > 0)    leftright = 1;
                else    leftright = 2;
            }
            else    {ball1.score += (100 * ballarr[i].radius);  printf("ballarr[]i.radius*100 is %lf\n",ballarr[i].radius);  ball1.upspeed = 0.17;   ball1.position.y += ball1.upspeed;  }
            ballarr.erase(ballarr.begin()+i);   plankarr.erase(plankarr.begin()+i);
        }
    }
    ball1.tick(0);

    /* Removing the digits from the screen */
    for (i=0;i<10;i++)
    {
        one1[i].set_position(1000,1000);
        two1[i].set_position(1000,1000);
        three1[i].set_position(1000,1000);
        four1[i].set_position(1000,1000);
        five1[i].set_position(1000,1000);
        six1[i].set_position(1000,1000);
        seven1[i].set_position(1000,1000);
        eight1[i].set_position(1000,1000);
        nine1[i].set_position(1000,1000);
        zero1[i].set_position(1000,1000);
    }

    /* Increase the level when target obtained */
    counterlevel++;
    if (counterlevel == 60*60)
    {
        if (ball1.score < ball1.level*300)
        {
            printf("GAME OVER\n");
            printf("Successfully completed %d levels\n",ball1.level-1);
            exit(0);
        }
        else
        {
            int j = 0;
            ball1.score = 0;
            counterlevel = 0;
            ball1.level += 1;
            /* Regenerate thorns */
            for (i=0;i<thorn.size();i++)    thorn.erase(thorn.begin()+i);
            thorn.push_back(Thorn(-3, -2.1, COLOR_GRAY, -4, -2.4));
            thorn.push_back(Thorn(-1.8,-2.1, COLOR_GRAY, -2.4, -1.72));
            thorn.push_back(Thorn(0, -2.1, COLOR_GRAY, -0.28, 0.8));
            thorn.push_back(Thorn(1, -2.1, COLOR_GRAY,0.8, 2.4));
            thorn.push_back(Thorn(3, -2.1, COLOR_GRAY, 2.4, 4));
            /* Magnets coming in */
            if (ball1.level == 2)
            {
                magnet2 = Magnet(5, 2.35, COLOR_MAGNET_RED);        // become 3.5
                magnet2black = MagnetBlack(5, 2.95, COLOR_BLACK);   // become 3.5
                magnet2.rotation = 180;
                magnet2black.rotation = 180;
            }
            else if (ball1.level == 3)
            {
                magnet1 = Magnet(-5, 3.6,COLOR_MAGNET_RED);         // become -3.5
                magnet1black = MagnetBlack(-5, 3, COLOR_BLACK);     // become -3.5
            }
        }
    }

    printf("Level: %d\n",ball1.level);



    printf("Score is %d\n",(int)round(ball1.score));

    /* Scoring in the screen */
    int tmp_score = (int)round(ball1.score), j=0;
    if (tmp_score < 0)  {negative1.set_position(-4+ball1.position.x,-2.75);  tmp_score = abs(tmp_score);}
    else    {negative1.set_position(1000,1000);}
    char s1[100];
    i=0;
    printf("score is %d\n",tmp_score);
    while (tmp_score)
    {
        s1[i++] = (tmp_score % 10) + '0';
        tmp_score /= 10;
    }
    i=0;
    for (j=strlen(s1)-1;j>=0;j--)
    {
        one1[i].set_position(1000,1000);
        two1[i].set_position(1000,1000);
        three1[i].set_position(1000,1000);
        four1[i].set_position(1000,1000);
        five1[i].set_position(1000,1000);
        six1[i].set_position(1000,1000);
        seven1[i].set_position(1000,1000);
        eight1[i].set_position(1000,1000);
        nine1[i].set_position(1000,1000);
        zero1[i].set_position(1000,1000);
        if ((s1[j]-'0') % 10 == 1)
            one1[i].set_position(-3.4+i*0.6+ball1.position.x,-2.75);
        else if ((s1[j]-'0') % 10 == 2)
            two1[i].set_position(-3.4+i*0.6+ball1.position.x,-2.75);
        else if ((s1[j]-'0') % 10 == 3)
            three1[i].set_position(-3.4+i*0.6+ball1.position.x,-2.75);
        else if ((s1[j]-'0') % 10 == 4)
            four1[i].set_position(-3.4+i*0.6+ball1.position.x,-2.75);
        else if ((s1[j]-'0') % 10 == 5)
            five1[i].set_position(-3.4+i*0.6+ball1.position.x,-2.75);
        else if ((s1[j]-'0') % 10 == 6)
            six1[i].set_position(-3.4+i*0.6+ball1.position.x,-2.75);
        else if ((s1[j]-'0') % 10 == 7)
            seven1[i].set_position(-3.4+i*0.6+ball1.position.x,-2.75);
        else if ((s1[j]-'0') % 10 == 8)
            eight1[i].set_position(-3.4+i*0.6+ball1.position.x,-2.75);
        else if ((s1[j]-'0') % 10 == 9)
            nine1[i].set_position(-3.4+i*0.6+ball1.position.x,-2.75);
        else if ((s1[j]-'0') % 10 == 0)
            zero1[i].set_position(-3.4+i*0.6+ball1.position.x,-2.75);
        i++;
    }


}



/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    int i = 0;
    for (i=0;i<10;i++)
    {
        one1[i] = One(1000, 1000, COLOR_YELLOW);
        two1[i] = Two(1000,1000,COLOR_YELLOW);
        three1[i] = Three(1000,1000,COLOR_YELLOW);
        four1[i] = Four(1000,1000,COLOR_YELLOW);
        five1[i] = Five(1000,1000,COLOR_YELLOW);
        six1[i] = Six(1000,1000,COLOR_YELLOW);
        seven1[i] = Seven(1000,1000,COLOR_YELLOW);
        eight1[i] = Eight(1000,1000,COLOR_YELLOW);
        nine1[i] = Nine(1000,1000,COLOR_YELLOW);
        zero1[i] = Zero(1000,1000,COLOR_YELLOW);
    }
    negative1 = Negative(1000,1000,COLOR_YELLOW);
    pond1 = Pond(-1, -2.1, COLOR_BLUE);
    pond2 = Pond(11,-2.1, COLOR_BLUE);
    ball1 = Ball(0, 0, COLOR_BALL_VIOLET,COLOR_BALL_INDIGO,COLOR_BALL_BLUE, COLOR_BALL_GREEN, COLOR_BALL_YELLOW,COLOR_BALL_ORANGE, COLOR_BALL_RED, COLOR_BALL_WHITE, COLOR_PLANK_BROWN, 0.3,0);
//    ball1 = Ball(0, 0, COLOR_BALL_RED,COLOR_BALL_RED,COLOR_BALL_RED, COLOR_BALL_RED, COLOR_BALL_RED,COLOR_BALL_RED, COLOR_BALL_RED, COLOR_BALL_RED, COLOR_PLANK_BROWN, 0.3,0);
    floor1 = Floor(0, 0, COLOR_BROWN);
    midbase1 = Midbase(0, 0, COLOR_GREEN);
    trampoline1 = Trampoline(3,-1.6,COLOR_RED);
    magnet1 = Magnet(-1000, 1000,COLOR_MAGNET_RED);
    magnet2 = Magnet(1000, 1000, COLOR_MAGNET_RED);
    magnet1black = MagnetBlack(-1000, 1000, COLOR_BLACK);
    magnet2black = MagnetBlack(1000, 1000, COLOR_BLACK);
    magnet2.rotation = 180;
    magnet2black.rotation = 180;

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
    int width  = 1200;
    int height = 900;
//    glutMouseFunc(mouse);
    window = initGLFW(width, height);
    initGL (window, width, height);
    glfwSetScrollCallback(window, scroll_callback);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

//bool detect_ball_collision(Ball a, Ball b)
//{
//    return (b.position.x < a.position.x + a.radius && b.position.x > a.position.x - a.radius && b.position.y < a.position.y + a.radius && b.position.y > a.position.y - a.radius);
//}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
