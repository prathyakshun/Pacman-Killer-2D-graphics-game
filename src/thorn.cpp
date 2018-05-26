#include "thorn.h"
#include "main.h"

Thorn::Thorn(float x, float y, color_t color, float xmin, float xmax)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this -> speed = 0.01;
    this -> xmin = xmin;
    this -> xmax = xmax;
    // Thorns
    GLfloat g_vertex_buffer_data[] = {
    -0.075, 0, 0,
    0, 0.2, 0,
    0.075, 0, 0
    };
    this->object = create3DObject(GL_TRIANGLES, 3, g_vertex_buffer_data, color, GL_FILL);
}

void Thorn::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0)); // changed mid from -0.6 to 0
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Thorn::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Thorn::tick() {
//    if (this->position.y + upspeed <= -1.79)
//    {
//        upspeed = 0;
//        inair = 0;
//    }
//    else
//    {
//        inair = 1;
//    }
//    this->position.x -= speed;
//    if (flag == 0)  this->position.y += upspeed;
}

bounding_box_t Thorn::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
