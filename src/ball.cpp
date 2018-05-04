#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, float radius,float speed, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius = radius;
    this->speed = speed;
    yspeed = 0;

    GLfloat vertex_buffer_data[900]; 
    int sides = 100;
    float angle = (2*3.14159)/sides;

    for(int i =0; i < sides;i++)
    {
        vertex_buffer_data[9*i] = 0.0f;
        vertex_buffer_data[9*i + 1] = 0.0f;  
        vertex_buffer_data[9*i + 2] = 0.0f;
        vertex_buffer_data[9*i + 3] = radius*cos(i*angle); 
        vertex_buffer_data[9*i + 4] = radius*sin(i*angle) ;
        vertex_buffer_data[9*i + 5] = 0.0f; 
        vertex_buffer_data[9*i + 6] = radius*cos((i+1)*angle); 
        vertex_buffer_data[9*i + 7] = radius*sin((i+1)*angle); 
        vertex_buffer_data[9*i + 8] = 0.0f;
    }


    this->object = create3DObject(GL_TRIANGLES, 3*sides, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = glm::translate(glm::vec3(0, 0, 0)) * rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {

    this->position.x -= speed;
    this->position.y += yspeed;
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, this->radius, this->radius };
    return bbox;
}

void Enemy_Ball::tick(float viscous) {


    if(speed > 0.2f)
        this->speed = 0.2f;

    else if(speed < -0.2f)
        this->speed = -0.2f;

    this->position.x -= viscous*speed;

    // if(std::abs(yspeed) > 0.5f)
    //     this->yspeed = 0.5f;


    if(yspeed > 0.5f)
        this->yspeed = 0.5f;

    else if(yspeed < -0.5f)
        this->yspeed = -0.5f;

    yspeed = yspeed - 0.01; // gravity

    this->position.y += viscous*yspeed ;

    if(this->position.y > 6.5)
        this->position.y = 6.5;

        
    if(this->position.y < -2.75)
        this->position.y = -2.75;


}