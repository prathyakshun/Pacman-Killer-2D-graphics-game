#include "ball.h"
#include "pond.h"
#include "main.h"

Ball::    Ball(float x, float y, color_t color1,color_t color2,color_t color3,color_t color4,color_t color5,color_t color6,color_t color7,color_t color8, color_t color9, float r, int plank_flag) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->speed = 0.0;
    this->inair = 0;
    this->inwater = 0;
    this->radius = r;
    this->intrampoline = 0;
    this->counter = 0;
    this->in_magnet1 = 0;
    this->in_magnet2 = 0;
    this->magnet_acc = 0.0001;
    this->score = 0;
    this->deceleration = -0.01;
    this->xdeceleration = -0.01;
    this->magnet_speed = 0.005;
    this->water_speed = 0.007;
    this->water_upspeed = 0.01;
    this->upspeed = 0;
    this->e = 0.25;
    this->waterflag = 0;
    this->plank_degree = 30;
    this->level = 1;

    // Ball
    double pi = 22.0/7;                                     // pi in radians
    double in_angle = (pi/180.0f);                         // Angle of rotation in radians
    printf("in_angle is %lf\n",in_angle);
    GLfloat g_vertex_buffer_data[3*3*360];                // Array containing the vertices of each triangle
    GLfloat g_vertex_buffer1[] = {-1*this->radius,0.0f};               // Initital x,y point to be rotated later on
    int k=0,i=0;
    for (i=0;i<360;i++)
    {
        g_vertex_buffer_data[k] = 0.0f; k++;
        g_vertex_buffer_data[k] = 0.0f; k++;
        g_vertex_buffer_data[k] = 0.0f; k++;
        g_vertex_buffer_data[k] = g_vertex_buffer1[0];  k++;
        g_vertex_buffer_data[k] = g_vertex_buffer1[1];  k++;
        g_vertex_buffer_data[k] = 0.0f; k++;
        g_vertex_buffer_data[k] = g_vertex_buffer1[0]*cos(in_angle) - g_vertex_buffer1[1]*sin(in_angle);    k++;
        g_vertex_buffer_data[k] = g_vertex_buffer1[0]*sin(in_angle) + g_vertex_buffer1[1]*cos(in_angle);    k++;
        g_vertex_buffer_data[k] = 0.0f; k++;
        double tmpx = g_vertex_buffer1[0], tmpy = g_vertex_buffer1[1];
        g_vertex_buffer1[0] = tmpx*cos(in_angle) - tmpy*sin(in_angle);
        g_vertex_buffer1[1] = tmpx*sin(in_angle) + tmpy*cos(in_angle);
    }
    this->sector[0] = create3DObject(GL_TRIANGLES, 3*360/8, g_vertex_buffer_data, color1, GL_FILL);
    this->sector[1] = create3DObject(GL_TRIANGLES, 3*360/8, g_vertex_buffer_data+45*3*3, color2, GL_FILL);
    this->sector[2] = create3DObject(GL_TRIANGLES, 3*360/8, g_vertex_buffer_data+45*3*3*2, color3, GL_FILL);
    this->sector[3] = create3DObject(GL_TRIANGLES, 3*360/8, g_vertex_buffer_data+45*3*3*3, color4, GL_FILL);
    this->sector[4] = create3DObject(GL_TRIANGLES, 3*360/8, g_vertex_buffer_data+45*3*3*4, color5, GL_FILL);
    this->sector[5] = create3DObject(GL_TRIANGLES, 3*360/8, g_vertex_buffer_data+45*3*3*5, color6, GL_FILL);
    this->sector[6] = create3DObject(GL_TRIANGLES, 3*360/8, g_vertex_buffer_data+45*3*3*6, color7, GL_FILL);
    this->sector[7] = create3DObject(GL_TRIANGLES, 3*360/8, g_vertex_buffer_data+45*3*3*7, color8, GL_FILL);
}

Ball::~Ball(void)
{
    double y = -1 + (rand()%50)*0.1;
    set_position(-4,y);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0)); // changed mid from -0.6 to 0
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->sector[0]);
    draw3DObject(this->sector[1]);
    draw3DObject(this->sector[2]);
    draw3DObject(this->sector[3]);
    draw3DObject(this->sector[4]);
    draw3DObject(this->sector[5]);
    draw3DObject(this->sector[6]);
    draw3DObject(this->sector[7]);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick(int flag) {

//    printf("ball1.upspeed is %lf\n",this->upspeed);

    /* Check if the ball is inside the water */
    if (this -> position.y <= -2.1 - this->radius)   this->waterflag = 1;           // 2.1 is the y position of the pond
    if (this -> position.y >= -1.8)   this->waterflag = 0;                          // -1.8 is the ground level
    printf("this -> position.x is %lf this -> position.y is %lf\n",this -> position.x, this -> position.y);


    printf("magnet1 %d magnet2 %d\n",this->in_magnet1, this->in_magnet2);
    /* This part handles when the ball is under the influence of the magnet */
    if (this->in_magnet1)
    {
        this->magnet_speed += this->magnet_acc;
        this->position.x -= this->magnet_speed;
    }
    else if (this->in_magnet2)
    {
        this->magnet_speed += this->magnet_acc;
        this->position.x += this->magnet_speed;
    }

    /* This part handles the movement of the ball inside the water */
    if (this -> inwater == 1)
    {
        if (0.75*0.75 - (this->position.x + this->water_speed + 1)*(this->position.x + this->water_speed + 1) < 0)    {this->position.y = -1.8;}
        if (0.75*0.75 - (this->position.x - this->water_speed + 1)*(this->position.x - this->water_speed + 1) < 0)    {this->position.y = -1.8;}
        if (flag == 2 && this->position.y <= (-sqrt(0.75*0.75 - (this->position.x + 1)*(this->position.x+1)) - 2.1 + 0.3))  {this->position.x -= this->water_speed; this->rotation += 3; this->position.y = -sqrt(0.75*0.75 - (this->position.x + 1)*(this->position.x+1)) - 2.1 + 0.3;}
        else if (flag == 2) {this->position.x -= this->water_speed;}
        else if (flag == 1 && this->position.y <= (-sqrt(0.75*0.75 - (this->position.x + 1)*(this->position.x+1)) - 2.1 + 0.3)) {this->position.x += this->water_speed; this->rotation -= 3; this->position.y = -sqrt(0.75*0.75 - (this->position.x + 1)*(this->position.x+1)) - 2.1 + 0.3;}
        else if (flag == 1) {this -> position.x += this -> water_speed;}
        else if (flag == 0 && this->position.y > (-sqrt(0.75*0.75 - (this->position.x + 1)*(this->position.x+1)) - 2.1 + 0.3)) {this->position.y -= this->water_upspeed;    if (this->upspeed > 0)  this->position.y += this->upspeed;}       // when ball not hit the surface
        else if (flag == 0 && this->position.y <= (-sqrt(0.75*0.75 - (this->position.x + 1)*(this->position.x+1)) - 2.1 + 0.3))                                                 // when ball hits the surface
        {
            if (this -> position.x < -0.95)    {this -> position.x += this->water_speed;    this->rotation -= 3;}
            if (this -> position.x > -0.95)    {this -> position.x -= this->water_speed;    this->rotation += 3;}
        }
        else if (flag == 3 && this->position.y <= (-sqrt(0.75*0.75 - (this->position.x + 1)*(this->position.x+1)) - 2.1 + 0.3)) {this -> upspeed = 0.2; this->position.y += this->upspeed;}

//        printf("InWater flag %d\n",flag);
//        this -> upspeed = -0.01;
//        if (this -> position.x < -0.95)    {this -> speed = 0.008; this -> position.x += this->speed;}
//        if (this -> position.x > -0.95)    {this -> speed = 0.008; this -> position.x -= this->speed;}
//        if (this -> position.y + this -> upspeed >= -2.65) {this -> position.y += this -> upspeed;}
//        if (flag == 2 && this->waterflag)  {this ->speed = 0.025;  this -> upspeed = 0.025;    if (this->position.x >= -1.7)   this -> position.x -= this -> speed;  this -> position.y += this -> upspeed;}
//        if (flag == 1 && this->waterflag)  {this ->speed = 0.025;  this -> upspeed = 0.025;    if (this->position.x <= -0.27)  this -> position.x += this -> speed;  this -> position.y += this -> upspeed;}
//        if (this -> position.y >= -1.8) this -> position.y = -1.8;
//    //    if (flag == 0)  {printf("HAPPENING\n");    this->position.y += this->upspeed;}           // Bring the ball down from the air
//        if (flag == 3 && this -> position.y <= -2)  {this -> position.y += 1;   this -> inair = 1;}
        return;
    }


    /* condition when the ball goes below the ground */
    if (this->position.y + this->upspeed <= -1.79 && this->inwater == 0)            // -1.8 is the level of the gorund
    {
        this->upspeed = 0;
        this->inair = 0;
    }
    else
    {
        if (this -> inwater == 0)
            this->inair = 1;                    // Checks if ball is in the air
    }
//    if (this->position.y > -1.79)   this->inair = 1;
//    else    this->inair = 0;

    /* Prevent ball from going below the pond */
    if (this->position.y + this ->upspeed <= (-sqrt(0.75*0.75 - (this->position.x + 1)*(this->position.x+1)) - 2.1 + 0.3) && flag!=3)
    {
        this -> upspeed = 0;
        this->position.y = -sqrt(0.75*0.75 - (this->position.x + 1)*(this->position.x+1)) - 2.1 + 0.3;
    }

    this->position.x += this->speed;                             // Move the ball left and right
    if (this->speed > 0)    this->rotation -= 3;
    else if (this->speed < 0)   this->rotation += 3;
    if (flag == 0)  this->position.y += this->upspeed;           // Bring the ball down from the air


    /* If the ball is on top of the trampoline */
    if (/*flag!=0 && */this->intrampoline==1 && this->position.y > -1.6)        // -1.6 is the top of the trampoline
    {
        this->upspeed=0.26;
        this->position.y += this->upspeed;
    }
//    if (this->position.x<=-4)  this->position.x = -4;
//    if (this->position.x>=4)    this->position.x = 4;
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, (this->radius)/2, (this->radius)/2 };
    return bbox;
}

//bool detect_ball_collision(Ball a, Ball b)
//{
//    if (a.upspeed < 0 && a.position.x < b.position.x + b.radius && a.position.x > b.position.x - b.radius && a.position.y < b.position.y + b.radius && a.position.y > b.position.y - b.radius)
//        return 1;
//    if (a.upspeed < 0 && b.position.x < a.position.x + a.radius && b.position.x > a.position.x - a.radius && b.position.y < a.position.y + a.radius && b.position.y > a.position.y - a.radius)
//        return 1;
//    return 0;
//}
