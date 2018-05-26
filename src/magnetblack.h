#include "main.h"


#ifndef MAGNETBLACK_H
#define MAGNETBLACK_H


class MagnetBlack
{
public:
    MagnetBlack() {}
    MagnetBlack(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box();

private:
    VAO * object;
};

#endif // MAGNETBLACK_H
