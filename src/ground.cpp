#include "ground.h"
#include "main.h"

Ground::Ground(float x, float y,float width, float height,float rotation,float xpeed, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = rotation;
    this->height = height;  
    this->width = width;
    this->speed = xpeed;
    GLfloat vertex_buffer_data[] = { 
        width/2, -height/2, 0.0f,
        width/2, height/2, 0.0f,
       -width/2, height/2, 0.0f,

        width/2, -height/2, 0.0f,
       -width/2, height/2, 0.0f,
       -width/2, -height/2, 0.0f 
        };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Ground::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = glm::translate(glm::vec3(0, 0, 0)) * rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ground::tick(){
    this->position.x -= speed;
}

void Ground::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool Ground::detect_collision(Ball ball) {

    return std::fabs(this->position.y - ball.position.y + 0.09f) < (ball.radius + this->height/2) && std::fabs(this->position.x - ball.position.x - 0.1f) < (ball.radius + this->width/2);
                                            // 0.09f is an error term to stop the ball from continously bouncing

}


bounding_box_t Ground::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, this->width, this->height };
    return bbox;
}

bool Plank::detect_collision(Ball ball){

     // checking whether the distnance between objects > sqrt(height*height + radius*radius) 
     // if this is negative then the ball is inside the triangle created

    return  (ball.position.y - this->position.y)*(ball.position.y - this->position.y)
     + (ball.position.x - this->position.x)*(ball.position.x - this->position.x) 
     -(ball.radius + this->width/2)*(ball.radius + this->width/2) - (this->height/2*this->height/2) < 0;


}