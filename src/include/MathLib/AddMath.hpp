#pragma once

#include <vector>
#include <cmath>

float clamp(float a, float b, float v){return std::min(std::max(v, a), b);}
float sign(float a) { return (0 < a) - (a < 0); }
float step(float edge, float x) { return x > edge; }
float lerp(float a, float b, float f){return a + f * (b - a);}

//bool isCollided(pair<vec2, vec2> rect1, pair<vec2, vec2> rect2){
//    return (rect1.first.x + rect1.second.x < rect2.first.x) || (rect1.first.y + rect1.second.y < rect2.first.y);
//}