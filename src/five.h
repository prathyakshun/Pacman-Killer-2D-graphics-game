#ifndef FIVE_H
#define FIVE_H

#include "main.h"

class Five
{
public:
    Five() {}
    Five(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box();

private:
    VAO * object;
};

#endif // FIVE_H







