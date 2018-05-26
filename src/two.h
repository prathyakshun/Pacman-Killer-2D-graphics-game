#ifndef TWO_H
#define TWO_H

#include "main.h"

class Two
{
public:
        Two() {}
        Two(float x, float y, color_t color);
        glm::vec3 position;
        float rotation;
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick();
        bounding_box_t bounding_box();

    private:
        VAO * object;
    };

#endif // TWO_H





