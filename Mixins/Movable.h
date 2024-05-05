#pragma once
#include "Uncopyable.h"

template<typename T>
class Movable : Uncopyable
{
public:
	Movable(Movable&& other) : object(other.object) {
		other.object = nullptr;
	}

protected:
	T object;
};