#pragma once
#include "vector.h"

class Hud
{
public:
	Hud() { offset = vec2(0.0f, 0.0f); }
	~Hud() {}

	vec2 offset;
};