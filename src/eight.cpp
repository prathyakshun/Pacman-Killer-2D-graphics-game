#include "eight.h"
#include "main.h"

Eight::Eight(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    double speed = 0.01;

    // Rectangle base
       GLfloat g_vertex_buffer_data[] = {
//           -4, 0, 0,
//           -4, -1.5,0,
//            4, 0, 0,
//           4, 0, 0,
//           4,-1.5,0,
//           -4, -1.5, 0

           0, -0.5, 0,
           0, 0, 0,
           0.05, 0, 0,
           0.05, 0, 0,
           0.05, -0.5, 0,
           0,-0.5 ,0,

           0, 0, 0,
           0.5,  0, 0,
           0, -0.05, 0,
           0.5, 0, 0,
           0, -0.05, 0,
           0.5, -0.05, 0,

           0.5, 0, 0,
           0.5, -0.25, 0,
           0.52, 0, 0,
           0.52, 0, 0,
           0.5, -0.25, 0,
           0.52, -0.25, 0,

           0, -0.25, 0,
           0.52, -0.25, 0,
           0, -0.27, 0,
           0, -0.27, 0,
           0.52, -0.25, 0,
           0.52, -0.27, 0,

           0.5, -0.25, 0,
           0.52, -0.25, 0,
           0.5, -0.5, 0,
           0.5, -0.5, 0,
           0.52, -0.25, 0,
           0.52, -0.5, 0,

           0, -0.5, 0,
           0.5, -0.5, 0,
           0,-0.52, 0,
           0,-0.52, 0,
           0.5, -0.5, 0,
           0.5, -0.52, 0

       };
    this->object = create3DObject(GL_TRIANGLES, 36, g_vertex_buffer_data, color, GL_FILL);
}

void Eight::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Eight::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Eight::tick() {
//    this->position.x -= speed;
    // this->position.y -= speed;
}

bounding_box_t Eight::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
