#ifndef PHYSICS_HPP
#define PHYSICS_HPP

bool doAABBsIntersect(float x1, float y1, float w1, float h1,
                      float x2, float y2, float w2, float h2)
{
  float maxX1 = x1 + w1;
  float maxY1 = y1 + h1;
  float maxX2 = x2 + w2;
  float maxY2 = y2 + h2;

  return maxX1 >= x2 && x1 <= maxX2 && maxY1 >= y2 && y1 <= maxY2;
}

#endif // !PHYSICS_HPP
