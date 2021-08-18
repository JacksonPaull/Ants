#include "grid.h"


Grid::Node::Node()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->EBO);
	this->isWall = false;
}

Grid::Node::Node(int x, int y, Grid* parent)
{
	this->x = x;
	this->y = y;
	this->parent = parent;
	this->randomize();
	
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->EBO);

	
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, parent->vertexBufferData);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	
	int col = (parent->width * 2 + 1);
	this->indices.push_back(2 * y * col + 2 * x);
	this->indices.push_back(2 * y * col + 2 * x + 2);
	this->indices.push_back(2 * (y + 1) * col + 2 * x);
	this->indices.push_back(2 * y * col + 2 * x + 2);
	this->indices.push_back(2 * (y + 1) * col + 2 * x);
	this->indices.push_back(2 * (y + 1) * col + 2 * x + 2);

	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Grid::Node::~Node()
{
	glDeleteBuffers(1, &this->VAO);
	glDeleteBuffers(1, &this->EBO);
}

void Grid::Node::triangulate()
{

	glBindVertexArray(this->VAO);
	glDeleteBuffers(1, &this->EBO);
	glGenBuffers(1, &this->EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

	this->indices.clear();

	//Check moving down right from this
	int tlw, trw, blw, brw;
	tlw = this->getIsWall();
	trw = this->parent->nodes[this->x + 1][this->y].getIsWall();
	blw = this->parent->nodes[this->x][this->y + 1].getIsWall();
	brw = this->parent->nodes[this->x + 1][this->y + 1].getIsWall();



	int col = (parent->width * 2 + 1);
	unsigned int tl, tm, tr, ml, mm, mr, bl, bm, br;
	tl = 2 * this->y * col + 2 * this->x;
	tr = 2 * this->y * col + 2 * this->x + 2;
	bl = 2 * (this->y + 1) * col + 2 * this->x;
	br = 2 * (this->y + 1) * col + 2 * this->x + 2;

	tm = 2 * y * col + 2 * x + 1;
	ml = (2 * y + 1) * col + 2 * x;
	mm = (2 * y + 1) * col + 2 * x + 1;
	mr = (2 * y + 1) * col + 2 * x + 2;
	bm = 2 * (y + 1) * col + 2 * x + 1;

	int t = tlw << 3 | trw << 2 | blw << 1 | brw;
	switch (t)
	{
		case 0b0000: //Full Square white
			break;
		case 0b0001: //Single Triangle bottom right
			this->indices.push_back(br);
			this->indices.push_back(mr);
			this->indices.push_back(bm);
			break;
		case 0b0010: //Single Triangle bottom left
			this->indices.push_back(bl);
			this->indices.push_back(bm);
			this->indices.push_back(ml);
			break;
		case 0b0011: //half rectangle bottom side
			this->indices.push_back(bl);
			this->indices.push_back(ml);
			this->indices.push_back(mr);

			this->indices.push_back(br);
			this->indices.push_back(mr);
			this->indices.push_back(bl);
			break;
		case 0b0100: //Single Triangle top right
			this->indices.push_back(tr);
			this->indices.push_back(tm);
			this->indices.push_back(mr);
			break;
		case 0b0101: //Half rectangle, right side
			this->indices.push_back(tr);
			this->indices.push_back(br);
			this->indices.push_back(bm);

			this->indices.push_back(tm);
			this->indices.push_back(bm);
			this->indices.push_back(tr);
			break;
		case 0b0110: //Diagonal right side
			this->indices.push_back(tr);
			this->indices.push_back(mr);
			this->indices.push_back(tm);

			this->indices.push_back(ml);
			this->indices.push_back(bl);
			this->indices.push_back(bm);

			this->indices.push_back(ml);
			this->indices.push_back(mr);
			this->indices.push_back(tm);

			this->indices.push_back(ml);
			this->indices.push_back(mr);
			this->indices.push_back(bm);
			break;
		case 0b0111: //Gap top left
			this->indices.push_back(tm);
			this->indices.push_back(ml);
			this->indices.push_back(bl);

			this->indices.push_back(bl);
			this->indices.push_back(br);
			this->indices.push_back(tm);

			this->indices.push_back(tm);
			this->indices.push_back(tr);
			this->indices.push_back(br);
			break;
		case 0b1000: //Single Triangle Top Left
			this->indices.push_back(tl);
			this->indices.push_back(ml);
			this->indices.push_back(tm);
			break;
		case 0b1001: //Diagonal left side
			this->indices.push_back(tl);
			this->indices.push_back(tm);
			this->indices.push_back(ml);

			this->indices.push_back(bm);
			this->indices.push_back(br);
			this->indices.push_back(mr);

			this->indices.push_back(ml);
			this->indices.push_back(mr);
			this->indices.push_back(tm);

			this->indices.push_back(ml);
			this->indices.push_back(mr);
			this->indices.push_back(bm);
			break;
		case 0b1010: //half rectangle left side
			this->indices.push_back(bl);
			this->indices.push_back(tl);
			this->indices.push_back(tm);

			this->indices.push_back(tm);
			this->indices.push_back(bm);
			this->indices.push_back(bl);
			break;
		case 0b1011: //Gap top right
			this->indices.push_back(bl);
			this->indices.push_back(br);
			this->indices.push_back(tm);

			this->indices.push_back(tl);
			this->indices.push_back(bl);
			this->indices.push_back(tm);

			this->indices.push_back(tm);
			this->indices.push_back(mr);
			this->indices.push_back(br);
			break;
		case 0b1100: //Half rectangle, top side
			this->indices.push_back(tl);
			this->indices.push_back(tr);
			this->indices.push_back(mr);

			this->indices.push_back(mr);
			this->indices.push_back(ml);
			this->indices.push_back(tl);
			break;
		case 0b1101: //Gap bottom left
			this->indices.push_back(tr);
			this->indices.push_back(br);
			this->indices.push_back(bm);

			this->indices.push_back(tl);
			this->indices.push_back(tr);
			this->indices.push_back(bm);

			this->indices.push_back(bm);
			this->indices.push_back(ml);
			this->indices.push_back(tl);
			break;
		case 0b1110: //Gap bottom right
			this->indices.push_back(bl);
			this->indices.push_back(bm);
			this->indices.push_back(tl);

			this->indices.push_back(tl);
			this->indices.push_back(tr);
			this->indices.push_back(bm);

			this->indices.push_back(mr);
			this->indices.push_back(tr);
			this->indices.push_back(bm);
			break;
		case 0b1111: //Full square black
			this->indices.push_back(tl);
			this->indices.push_back(tr);
			this->indices.push_back(bl);

			this->indices.push_back(tr);
			this->indices.push_back(bl);
			this->indices.push_back(br);
			break;
	}
	
	

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Grid::Node::draw() 
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Grid::Node::randomize()
{
	this->isWall = rand() % 2 == 0;
}

bool Grid::Node::getIsWall()
{
	return isWall;
}

void Grid::Node::setIsWall(bool isWall)
{
	this->isWall = isWall;
}

Grid::Grid(int x, int y, int screenWidth, int screenHeight, int size, Shader& shd, glm::mat4* ortho)
{
	this->x = x;
	this->y = y;
	this->size = size;
	this->shd = shd;
	this->ortho = ortho;
	this->model = glm::mat4(1.0f);
	this->model = glm::translate(model, glm::vec3(x, y, 0.0f));


	this->width = screenWidth / size;
	this->height = screenHeight / size;

	//Init vertex info
	int vertSize = (this->width*2 + 1) * (this->height*2 + 1);
	for (int i = 0; i < vertSize; i++)
	{	
		
		//Set all x values for vertices
		int column = i % (this->width*2 + 1);
		float xpos = size * width * (float)column / ((float)this->width*2);
		vertices.push_back(xpos);	//Change to macro for screen width

		//Set all xz values for vertices
		int row = i / (this->width*2 + 1);
		float ypos = size * height * (float)row / ((float)this->height*2);
		vertices.push_back(ypos);


		//Set all z values
		vertices.push_back(0.0f);

	}
	

	glGenBuffers(1, &this->vertexBufferData);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferData);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Create nodes
	this->nodes = new Node*[this->width + 1];
	for (int i = 0; i < this->width + 1; i++)
	{
		nodes[i] = new Node[this->height + 1];
		for (int j = 0; j < this->height + 1; j++)
		{
			nodes[i][j] =  Node(i, j, this);
		}
	}

	
	for (int i = 0; i < 10; i++)
		this->smooth();
	
	this->filterRegions();

	this->connectAllClosestRegions();

	this->triangulate();
	
	
}

Grid::~Grid()
{
	for (int i = 0; i < this->width; i++)
	{
		for (int j = 0; j < this->height; j++)
		{
			nodes[i][j].~Node();
		}
		delete[] nodes[i];
	}
	delete[] nodes;
}

void Grid::draw()
{
	shd.SetMatrix4("ortho", *this->ortho, true);
	shd.SetMatrix4("model", this->model, true);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			//Set the color for the node
			nodes[i][j].draw();
		}
	}
}

void Grid::setShader(Shader& s)
{
	this->shd = s;
}

void Grid::smooth()
{
	//Create a 2D array to hold all of the information of what the nodes will flip to
	bool** nextStates = new bool* [width];
	for (int i = 0; i < width; i++)
		nextStates[i] = new bool[height];

	//Loop through all the nodes and store their next state into the 2D array
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			//Loop through neighbors and get a count of differences
			int differences = 0;
			bool state = this->nodes[i][j].getIsWall();
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					if (x == 0 && y == 0)
						continue;
					if (i + x > -1 && i + x < width && j + y > -1 && j + y < height)
						if (state != this->nodes[i + x][j + y].getIsWall())
							differences++;
				}
			}

			nextStates[i][j] = differences < 5 ? state : !state;
		}
	}

	//Set each node's isWall to their flipped value
	for (int i = 0; i < width + 1; i++)
		for (int j = 0; j < height + 1; j++)
		{
			//Set the edges to always be walls
			if (i <= 0 || i >= width - 1 || j <= 0 || j >= height - 1)
			{
				this->nodes[i][j].setIsWall(true);
			}
			else
			{
				this->nodes[i][j].setIsWall(nextStates[i][j]);
			}
		}


	//Delete the array we created
	for (int i = 0; i < width; i++)
		delete[] nextStates[i];
	delete[] nextStates;
}

void Grid::randomize()
{
	clock_t start = clock();
	for (int i = 0; i < width + 1; i++)
	{
		for (int j = 0; j < height + 1; j++)
		{
			this->nodes[i][j].randomize();
		}
	}

	for (int i = 0; i < 10; i++)
		this->smooth();
	this->filterRegions();
	this->connectAllClosestRegions();
	this->triangulate();

	double delta = clock() - start;
	std::cout << "Finished generating new room in " << delta / CLOCKS_PER_SEC * 1000 << "ms\n";
}

void Grid::triangulate()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			nodes[i][j].triangulate();
		}
	}
}

void Grid::processInput()
{
	static bool processed_enter = false;

	if (Keys[GLFW_KEY_SPACE] && !processed_enter)
	{
		processed_enter = true;
		this->randomize();
	}
	else if(!Keys[GLFW_KEY_SPACE])
	{
		processed_enter = false;
	}
	
}

void Grid::filterRegions()
{
	//detect all regions
	this->regions = this->detectAllRegions();

	//For each region in regions
		//If region.size() < THRESHOLD:
			//For each coord in region:
				//flip nodes @ coord 
	std::vector<int> remove;

	for (int i = 0; i < regions.size(); i++)
	{
		region reg = regions[i];
		std::vector<coord> r = reg.cells;
		if (r.size() < 50)
		{
			for (int j = 0; j < r.size(); j++)
			{
				Node n = nodes[r[j].x][r[j].y];
				n.setIsWall(!n.getIsWall());
				nodes[r[j].x][r[j].y] = n;
			}
			remove.push_back(i);
		}
	}

	this->regions = this->detectAllRegions();
}

cellsAndBorder Grid::detectRegion(int startX, int startY)
{
	std::queue<coord> cells;
	std::vector<coord> result;
	std::vector<coord> borderCells;
	bool** checked = new bool* [this->width+1];
	for (int i = 0; i < this->width+1; i++)
	{
		checked[i] = new bool[this->height+1];
		for (int j = 0; j < height + 1; j++)
		{
			checked[i][j] = false;
		}
	}

	cells.push(coord{startX, startY, nodes[startX][startY].getIsWall()});
	while (!cells.empty())
	{
		//Access first element from queue
		coord cell = cells.front();
		cells.pop();

		//Update checked array
		checked[cell.x][cell.y] = true;

		//Add these coords to the result
		result.push_back(cell);

		//Add all neighbors of the same type to the queue
		bool isBorder = false;
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (i == 0 && j == 0)
					continue;
				if (i + cell.x >= 0 && j + cell.y >= 0 && i + cell.x < width + 1 && cell.y + j < height + 1)
				{	//Cell is in bounds
					if (!checked[cell.x + i][cell.y + j] && nodes[cell.x + i][cell.y + j].getIsWall() == cell.isWall)
					{	//Cell has not been checked, add it to queue
						cells.push(coord{ cell.x + i, cell.y + j, nodes[cell.x + i][cell.y + j].getIsWall() });
					}
					if (nodes[cell.x + i][cell.y + j].getIsWall() != cell.isWall && !isBorder)
					{
						isBorder = true;
						borderCells.push_back((coord{ cell.x, cell.y, nodes[cell.x][cell.y].getIsWall() }));
					}
					checked[cell.x + i][cell.y + j] = true;
				}
			}
		}

		
	}


	//Clean up heap memory
	for (int i = 0; i < this->width; i++)
	{
		delete[] checked[i];
	}
	delete[] checked;

	return cellsAndBorder{result, borderCells};
}

std::vector<region> Grid::detectAllRegions()
{
	std::vector<region> regions;

	bool** checked = new bool* [this->width + 1];
	for (int i = 0; i < this->width + 1; i++)
	{
		checked[i] = new bool[this->height + 1];
		for (int j = 0; j < height + 1; j++)
		{
			checked[i][j] = false;
		}
	}

	//for every cell
		//If not checked
			//Find region
			//For every coord in region
			//Update checked
	int id = 0;
	for (int i = 0; i < width + 1; i++)
	{
		for (int j = 0; j < height + 1; j++)
		{
			if (!checked[i][j])
			{
				cellsAndBorder cnb = detectRegion(i, j);
				std::vector<coord> r = cnb.cells;
				region reg = region{ r, cnb.borderCells, r[0].isWall, id++, std::vector<int>()};
				regions.push_back(reg);

				for (int k = 0; k < r.size(); k++)
				{
					coord c = r[k];
					checked[c.x][c.y] = true;
				}
			}
		}
	}


	for (int i = 0; i < this->width; i++)
	{
		delete[] checked[i];
	}
	delete[] checked;

	return regions;
}

void Grid::connectAllClosestRegions()
{
	//For every region
		//Find closest other region (closest two cells)
		//Create line between them
		//Flip walls to white which lie on the line between them
		//Update connection IDs for both
	if (regions.size() < 3)
		return;

	for (int i = 0; i < this->regions.size(); i++)
	{
		region roomA = regions[i];
		if (roomA.isWall == true)
			continue;

		int minDist = INT_MAX;
		int closestRoomID = -1;
		coord closestA, closestB;
		for (int j = 0; j < this->regions.size(); j++)
		{
			if (j == i)
				continue;
			
			region roomB = regions[j];
			if (roomB.isWall == true)
				continue;

			for (int x = 0; x < roomA.borderCells.size(); x++)
			{
				for (int y = 0; y < roomB.borderCells.size(); y++)
				{
					//Calculate dist
					int dist = pow(roomA.borderCells[x].x - roomB.borderCells[y].x, 2) + pow(roomA.borderCells[x].y - roomB.borderCells[y].y, 2);
					if (dist < minDist)
					{
						minDist = dist;
						closestA = roomA.borderCells[x];
						closestB = roomB.borderCells[y];
						closestRoomID = roomB.ID;
					}
				}
			}
		}
		

		//By here we have found the roomID closest to roomA, and the two cells closest together
		//Implementation of Bresenham's algorithm for drawing lines
		int x0, x1, y0, y1;
		if (closestA.x < closestB.x)
		{
			x0 = closestA.x;
			x1 = closestB.x;
			y0 = closestA.y;
			y1 = closestB.y;
		}
		else
		{
			x0 = closestB.x;
			x1 = closestA.x;
			y0 = closestB.y;
			y1 = closestA.y;
		}

		float dx = x1 - x0;
		float dy = y1 - y0;
		if (abs(dy) > abs(dx)) //function of y
		{
			float m = dx / dy;
			int dir = 1;
			if (dy < 0)
			{
				dir = -1;
			}

			for (int y = y0; y != y1; y+= dir)
			{
				int x = m * (y - y0) + x1;
				nodes[x][y].setIsWall(false);
				nodes[x+1][y].setIsWall(false);
				nodes[x-1][y].setIsWall(false);
			}
		}
		else //Function of x
		{
			float m = dy / dx;
			int dir = 1;
			if (dx < 0)
			{
				dir = -1;
			}
			for (int x = x0; x != x1; x+= dir)
			{
				int y = m * (x - x0) + y0;
				nodes[x][y].setIsWall(false);
				nodes[x][y+1].setIsWall(false);
				nodes[x][y-1].setIsWall(false);
			}
		}
		
		roomA.connectedRegions.push_back(closestRoomID);
		regions[closestRoomID].connectedRegions.push_back(roomA.ID);

	}
}

void Grid::ensureConnectivity()
{
	//Find the biggest region

	//Create a list of regions not in connections

	//While that list is not empty
		//For every room not in biggest regions connected list
			//Find closest room
			//Connect them
}