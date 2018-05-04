#include "main.h"
#include "ball.h"

#ifndef OBJECT_H
#define OBJECT_H

class Trampoline {
public:
    Trampoline() {}
    Trampoline(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool detect_collision(Ball ball);
    bounding_box_t bounding_box();
private:
    VAO *object;    // How an object looks 
};

class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void set_rotation(float rotation);
    bool detect_collision(Ball ball);
    bounding_box_t bounding_box();
private:
    VAO *object;    // How an object looks 
};


class Pool {
public:
    Pool() {}
    Pool(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float radius;
    double speed;
    double yspeed;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    float detect_collision(Ball ball);
    bounding_box_t bounding_box();
private:
    VAO *object;    // How an object looks 
};

class Spike {
public:
    Spike() {}
    Spike(float x, float y, float width,float speed, float range,color_t color);
    glm::vec3 position;
    float rotation;
    float speed;
    float width;
    float height;
    float range;
    float center_x;
    void tick();
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool detect_collision(Ball ball);
    bounding_box_t bounding_box();
private:
    VAO *object;    // How an object looks 
};


#endif
