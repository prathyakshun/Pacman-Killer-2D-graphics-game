#ifndef PLANK_H
#define PLANK_H

#include "main.h"

class Plank
{
public:
    Plank() {}
    Plank(float x, float y, color_t color, float radius, int plank_there);
    glm::vec3 position;
    float rotation;
    float radius;
    float plank_there;
    double speed;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box();
private:
    VAO * object;
};

#endif // PLANK_H



