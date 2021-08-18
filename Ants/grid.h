#pragma once
#include "shader.h"
#include "init.h"
#include <vector>
#include <iostream>
#include <queue>

struct coord {
	int x, y;
	bool isWall;
};

struct region {
	std::vector<coord> cells;
	std::vector<coord> borderCells;
	bool isWall;
	int ID;
	std::vector<int> connectedRegions;
};

struct cellsAndBorder
{
	std::vector<coord> cells;
	std::vector<coord> borderCells;
};

class Grid {
private:
	class Node {
	private:
		unsigned int EBO, VAO;
		int x, y;
		bool isWall;
		std::vector<unsigned int> indices;
		Grid* parent;
	public:
		bool getIsWall();
		void setIsWall(bool isWall);
		void randomize();
		Node();
		Node(int x, int y, Grid* parent);
		~Node();
		void draw();
		void triangulate();
	};

	int x, y;
	int width;
	int height;
	int size;
	unsigned int vertexBufferData, VAO;
	Shader shd;
	Node** nodes;
	std::vector<float> vertices;
	std::vector<region> regions;
	glm::mat4* ortho;
	glm::mat4 model;

public:

	Grid(int x, int y, int screenWidth, int screenHeight, int size, Shader& shd, glm::mat4* ortho);
	~Grid();
	void setShader(Shader& s);
	void processInput();
	void draw();				//Calls each node's draw function
			
	void smooth();				//Uses 4-5 rule to generate shapes
	void randomize();			//Randomizes cave
	void triangulate();			//Uses triangulation table to create smooth shapes
	cellsAndBorder detectRegion(int startX, int startY);
	std::vector<region> detectAllRegions();
	void filterRegions();
	void connectAllClosestRegions();
	void ensureConnectivity();
};