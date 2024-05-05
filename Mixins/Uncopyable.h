#pragma once

class Uncopyable
{
public:
	Uncopyable() {};

	Uncopyable(const Uncopyable& other) = delete;

private:
};