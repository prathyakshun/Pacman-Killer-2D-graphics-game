#include "trampoline.h"
#include "main.h"

Trampoline::Trampoline(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    double speed = 0.01;

    // Trampoline
    GLfloat g_vertex_buffer_data[3*3*180+12*3];                // Array containing the vertices of each triangle
    int k=0;
    g_vertex_buffer_data[k++] = -0.5;   g_vertex_buffer_data[k++] = 0;   g_vertex_buffer_data[k++] = 0;
    g_vertex_buffer_data[k++] = -0.5;   g_vertex_buffer_data[k++] = -0.5;   g_vertex_buffer_data[k++] = 0;
    g_vertex_buffer_data[k++] = -0.45;   g_vertex_buffer_data[k++] = 0;   g_vertex_buffer_data[k++] = 0;
    g_vertex_buffer_data[k++] = -0.45;   g_vertex_buffer_data[k++] = 0;   g_vertex_buffer_data[k++] = 0;
    g_vertex_buffer_data[k++] = -0.45;   g_vertex_buffer_data[k++] = -0.5;   g_vertex_buffer_data[k++] = 0;
    g_vertex_buffer_data[k++] = -0.5;   g_vertex_buffer_data[k++] = -0.5;   g_vertex_buffer_data[k++] = 0;
    g_vertex_buffer_data[k++] = 0.5;   g_vertex_buffer_data[k++] = 0;   g_vertex_buffer_data[k++] = 0;
    g_vertex_buffer_data[k++] = 0.5;   g_vertex_buffer_data[k++] = -0.5;   g_vertex_buffer_data[k++] = 0;
    g_vertex_buffer_data[k++] = 0.45;   g_vertex_buffer_data[k++] = 0;   g_vertex_buffer_data[k++] = 0;
    g_vertex_buffer_data[k++] = 0.45;   g_vertex_buffer_data[k++] = 0;   g_vertex_buffer_data[k++] = 0;
    g_vertex_buffer_data[k++] = 0.45;   g_vertex_buffer_data[k++] = -0.5;   g_vertex_buffer_data[k++] = 0;
    g_vertex_buffer_data[k++] = 0.5;   g_vertex_buffer_data[k++] = -0.5;   g_vertex_buffer_data[k++] = 0;

    double pi = 22.0/7;                                     // pi in radians
    double in_angle = (pi/180.0f);                         // Angle of rotation in radians
    printf("in_angle is %lf\n",in_angle);
    GLfloat g_vertex_buffer1[] = {-0.5f,0.0f};               // Initital x,y point to be rotated later on
    int i=0;
    for (i=0;i<180;i++)
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
    this->object = create3DObject(GL_TRIANGLES, 3*180+12, g_vertex_buffer_data, color, GL_FILL);
}

void Trampoline::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0)); // changed mid from -0.6 to 0
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Trampoline::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Trampoline::tick() {
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

bounding_box_t Trampoline::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 1.4, 0.4 };
    return bbox;
}
