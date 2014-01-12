#pragma once
#ifndef RAINBOW_MATH_RANDOM_HPP
#define RAINBOW_MATH_RANDOM_HPP

namespace rb {

void random_seed();
float random_scale();
float random_range(float min, float max);
int random_range(int min, int max);

}

#endif // RAINBOW_MATH_RANDOM_HPP
