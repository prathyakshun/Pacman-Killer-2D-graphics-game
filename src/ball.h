#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color1,color_t color2,color_t color3,color_t color4,color_t color5,color_t color6,color_t color7,color_t color8, color_t color9, float radius, int plank_flag);
    ~Ball();
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int flag);
 //   bool detect_ball_collision(Ball a, Ball b);
    int inair;
    int inwater;
    int intrampoline;
    int counter;
    int level;
    double radius;
    double speed;
    double plank_degree;
    double upspeed;
    int waterflag;
    int in_magnet1;
    double e;
    double score;
    int in_magnet2;
    double magnet_acc;
    double magnet_speed;
    double water_speed;
    double water_upspeed;
    double deceleration;
    double xdeceleration;
    bounding_box_t bounding_box();
private:
    VAO *object;
    VAO *sector[8];
};


#endif // BALL_H
