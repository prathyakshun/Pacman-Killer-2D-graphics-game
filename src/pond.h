#ifndef POND_H
#define POND_H

#include "main.h"

class Pond
{
public:
    Pond() {}
    Pond(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box();

private:
    VAO * object;
};
#endif // POND_H

