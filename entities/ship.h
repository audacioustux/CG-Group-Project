#include "../lib.h"

class AnimatedShip;
class Ship {
protected:
  double x, y, width, height;
  unsigned int color;
  HDirection facing_to = HDirection::left;

public:
  void draw() {
    auto ship_color = Hex2glRGB(color);

    glPushMatrix();
    glTranslatef(x, y, 0);
    if (facing_to == HDirection::left) {
      glScalef(width, height, 0);
    } else {
      glScalef(-width, height, 0);
    }
    glColor3ub(217, 132, 126);
    drawFilledElipsis(.8, 1.35, .8, .1);

    glColor3fv((GLfloat *)&ship_color);

    glBegin(GL_TRIANGLES);
    glVertex2f(0, 1);
    glVertex2f(.40, 0);
    glVertex2f(.40, 1);
    glEnd();
    glBegin(GL_QUADS);
    float pal[] = {.45, 0, .45, 1, .50, 1, .50, .2};
    glVertex2fv(pal);
    glVertex2fv(pal + 2);
    glVertex2fv(pal + 4);
    glVertex2fv(pal + 6);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(.55, .2);
    glVertex2f(.55, 1);
    glVertex2f(1, 1);
    glEnd();
    glBegin(GL_QUADS);
    float base[] = {-.2, 1.1, 0, 1.4, 1.1, 1.4, 1.2, 1.1};
    glVertex2fv(base);
    glVertex2fv(base + 2);
    glVertex2fv(base + 4);
    glVertex2fv(base + 6);
    glEnd();
    glPopMatrix();
  }

  Ship(int x, int y, int width, int height, unsigned int color,
       HDirection facing_to)
      : x(x), y(y), width(width), height(height), facing_to(facing_to),
        color(color) {}
  AnimatedShip animate(int min_x, int max_x, int speed);
};

class AnimatedShip : Ship {
public:
  AnimatedShip(Ship ship, int min_x, int max_x, int speed)
      : Ship(ship), min_x(min_x), max_x(max_x), speed(speed), start_x(x),
        last_render(GetCurrentTime()) {}
  void draw() {
    double current_time = GetCurrentTime();
    double epoch = current_time - last_render;
    double x_delta = epoch * speed;

    if (facing_to == HDirection::left) {
      x -= x_delta;

      if (x < min_x)
        facing_to = HDirection::right;
    } else {
      x += x_delta;

      if (x > max_x)
        facing_to = HDirection::left;
    }
    Ship::draw();

    last_render = current_time;
  }

private:
  double start_x, min_x, max_x;
  int speed; // pixel per second;
  double last_render;
};

inline AnimatedShip Ship::animate(int min_x, int max_x, int speed) {
  return AnimatedShip(*this, min_x, max_x, speed);
}
