#include "objects.h"
#include "main.h"

Trampoline::Trampoline(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    
    GLfloat vertex_buffer_data[45] ={ 
        0.9f, 0.0f, 0.0f,
        0.9f, 1.1f, 0.0f,
        0.7f, 1.0f, 0.0f,

        0.9f, 0.0f, 0.0f,
        0.7f, 1.1f, 0.0f,
        0.7f, 0.0f, 0.0f,

        0.7f, 1.0f, 0.0f,
       -0.7f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,

       -0.7f, 0.0f, 0.0f,
       -0.9f, 1.1f, 0.0f,
       -0.7f, 1.0f, 0.0f,

       -0.7f, 0.0f, 0.0f,
       -0.9f, 1.1f, 0.0f,
       -0.9f, 0.0f, 0.0f

    }; 

    this->object = create3DObject(GL_TRIANGLES, 15, vertex_buffer_data, color, GL_FILL);
}

void Trampoline::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = glm::translate(glm::vec3(0, 0, 0)) * rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Trampoline::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool Trampoline::detect_collision(Ball ball) {
    return std::fabs(this->position.x - ball.position.x) < (ball.radius + 0.7f) && std::fabs(this->position.y - ball.position.y) < (ball.radius);
}

bounding_box_t Trampoline::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 1.7f, 1.1f };
    return bbox;
}


Magnet::Magnet(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 90;
    
    GLfloat vertex_buffer_data[45] ={ 
        0.9f, 0.0f, 0.0f,
        0.9f, 1.1f, 0.0f,
        0.7f, 1.0f, 0.0f,

        0.9f, 0.0f, 0.0f,
        0.7f, 1.1f, 0.0f,
        0.7f, 0.0f, 0.0f,

        0.7f, 1.0f, 0.0f,
       -0.7f, 1.0f, 0.0f,
        0.0f, 2.0f, 0.0f,

       -0.7f, 0.0f, 0.0f,
       -0.9f, 1.1f, 0.0f,
       -0.7f, 1.0f, 0.0f,

       -0.7f, 0.0f, 0.0f,
       -0.9f, 1.1f, 0.0f,
       -0.9f, 0.0f, 0.0f

    }; 

    this->object = create3DObject(GL_TRIANGLES, 15, vertex_buffer_data, color, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = glm::translate(glm::vec3(0, 0, 0)) * rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Magnet::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Magnet::set_rotation(float rotation) {
    this->rotation = rotation;
}

bool Magnet::detect_collision(Ball ball) {
    return std::fabs(this->position.y - ball.position.y) < (ball.radius + 0.7f);
}

bounding_box_t Magnet::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 1.7f, 2.0f };
    return bbox;
}

Pool::Pool(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
   
    radius = 1.99;
    speed = 0.01;
    yspeed = 0;

    GLfloat vertex_buffer_data[450]; 
    int sides = 100;
    float angle = (2*3.14159)/sides;

    for(int i =0; i < sides/2;i++)
    {
        vertex_buffer_data[9*i] = 0.0f;
        vertex_buffer_data[9*i + 1] = 0.0f;  
        vertex_buffer_data[9*i + 2] = 0.0f;
        vertex_buffer_data[9*i + 3] = radius*cos(i*angle + 3.14159); 
        vertex_buffer_data[9*i + 4] = radius*sin(i*angle + 3.14159) ;
        vertex_buffer_data[9*i + 5] = 0.0f; 
        vertex_buffer_data[9*i + 6] = radius*cos((i+1)*angle + 3.14159); 
        vertex_buffer_data[9*i + 7] = radius*sin((i+1)*angle + 3.14159); 
        vertex_buffer_data[9*i + 8] = 0.0f;
    }


    this->object = create3DObject(GL_TRIANGLES, 3*sides/2, vertex_buffer_data, color, GL_FILL);

}

void Pool::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = glm::translate(glm::vec3(0, 0, 0)) * rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Pool::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

float Pool::detect_collision(Ball ball) {
    return (this->position.x - ball.position.x)*(this->position.x - ball.position.x) 
          + (this->position.y - ball.position.y)*(this->position.y - ball.position.y) 
          - (ball.radius - this->radius)*(ball.radius - this->radius);
}

bounding_box_t Pool::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, this->radius, this->radius };
    return bbox;
}

Spike::Spike(float x, float y,float width,float speed,float range, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->center_x = x;
    this->width = width;
    this->speed = speed;
    this->height = 0.2f;
    float no_of_spikes = 5.0;
    float spike_size = width/no_of_spikes;
    this->range = range;

    GLfloat vertex_buffer_data[45];

    for(int i=0;i<no_of_spikes;i++){

        vertex_buffer_data[9*i] = i*spike_size;
        vertex_buffer_data[9*i + 1] = 0.0f;  
        vertex_buffer_data[9*i + 2] = 0.0f;
        vertex_buffer_data[9*i + 3] = (i+1/2.0)*spike_size; 
        vertex_buffer_data[9*i + 4] =  height;
        vertex_buffer_data[9*i + 5] = 0.0f; 
        vertex_buffer_data[9*i + 6] = (i+1)*spike_size; 
        vertex_buffer_data[9*i + 7] = 0.0f; 
        vertex_buffer_data[9*i + 8] = 0.0f;
    }


    this->object = create3DObject(GL_TRIANGLES, 15, vertex_buffer_data, color, GL_FILL);
}

void Spike::tick(){
    this->position.x -= speed;
    if(std::fabs(this->position.x - this->center_x - 0.5f) > this->range/4)
        this->speed = -this->speed;

}

void Spike::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = glm::translate(glm::vec3(0, 0, 0)) * rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Spike::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool Spike::detect_collision(Ball ball) {

    return std::fabs(this->position.y +this->height/2 - ball.position.y) < (ball.radius + this->height/2) && std::fabs(this->position.x  + this->width/2 - ball.position.x) < (ball.radius + this->width/2);

}

bounding_box_t Spike::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 1.7f, 1.1f };
    return bbox;
}