#pragma once
#include <vector>
#include "init.h"



class Ant
{
private:

	//REMOVE ONCE ANTHILL IS FORMED AND BEGIN TO DRAW INSTANCED
	unsigned int VBO, VAO;

	bool carryingFood;
	int x;
	int y;
	float movespeed;
	float randomTurnMagnitude;
	glm::vec3 direction;

public:
	Ant();
	bool checkCollisionAtPoint(int x, int y);
	void move();
	void draw();
};


class AntHill
{
private:
	std::vector<Ant> ants;
	int x;
	int y;
	int numFood;

	Shader shd;

public:
};

class FoodPile
{

private:

public:

};