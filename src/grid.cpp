#include "grid.hpp"


/**
 * @brief Construct a new Grid:: Grid object
 * 
 * @param x Initial screen space x
 * @param y Initial screen space y
 * @param width Grid width in cells
 * @param height Grid height in cells
 * @param autogen Whether to autocomplete all steps or not
 */
Grid::Grid(int x, int y, int width, int height, bool autogen) {
    //0. Create Data

    //TODO Add superset constructurs with default values for these
    this->seed = 5; //rand();
    this->p = 0.45;
    this->smoothness_level = 5;

    createGLData();
    
    //1. Set constants
    this->x = x;
    this->y = y;

    this->width = width;
    this->height = height;

    //Init Vectors
    this->vertices = std::vector<float>();
    this->regions = std::vector<Region>();
    this->indices = std::vector<unsigned int>();

    //Create Grid
    this->grid = new float*[width+1];
    for(int i = 0; i < width+1; i++)
        this->grid[i] = new float[height+1];

    //2. Generate vertex
    generateVertices();

    //4. Randomize Grid
    randomize();

    //5. Smooth Grid
    this->lockBorders();
    for(int i = 0; i < this->smoothness_level; i++)
        this->smooth();

    //6. Filter out small regions

    //7. Connect regions

    //8. Triangulate
    triangulate();

    //9. Save data on the GPU
    
}

Grid::~Grid() {
    //Delete Grid
    for(int i = 0; i < this->width+1; i++)
        delete[] this->grid[i];
    delete[] this->grid;

    //Free GPU space
    deleteGLData();
}

void Grid::lockBorders() {
    //Set all borders to be walls
    for(int i = 0; i < this->height+1; i++) {
        this->grid[0][i] = 1;
        this->grid[this->width][i] = 1;
    }
    for(int i = 0; i < this->width+1; i++) {
        this->grid[i][0] = 1;
        this->grid[i][this->width] = 1;
    }
}

void Grid::randomize() {
    srand(this->seed);

    for(int i=0; i < this->width+1; i++) {
        for(int j=0; j < this->height+1; j++) {
            float rval = float(rand()) / float(RAND_MAX);
            if(rval < p)
                this->grid[i][j] = 1.0;
            else
                this->grid[i][j] = 0.0;
        }
    }
}

void Grid::smooth() {
    //Create a copy of the grid to store everything in
    //Not changing the borders, so we dont need the width+1
    float** newGrid = new float*[this->width];
    for(int i = 1; i < this->width; i++)
        newGrid[i] = new float[this->height];
    
    for(int i = 1; i < this->width; i++) {
        for(int j = 1; j < this->height; j++) {
            
            //Get neighbor values
            int tot = 0;
            for(int dx = -1; dx <= 1; dx++) {
                for(int dy = -1; dy <= 1; dy++) {
                    //Get count of different other cells around
                    if(this->grid[i+dx][j+dy] != this->grid[i][j])
                        tot++;
                }
            }

            //Set self to be the avg of neighbors
            newGrid[i][j] = this->grid[i][j];
            if(tot > 5) 
                newGrid[i][j] = 1.0 - this->grid[i][j];
        }
    }
    
    //Copy new grid back over to grid and delete new grid
    for(int i = 1; i < this->width; i++) {
        for(int j = 1; j < this->height; j++)
            this->grid[i][j] = newGrid[i][j];
        delete[] newGrid[i];
    }
    delete[] newGrid;
    

    this->triangulate(); //TODO Remove this call if not visualizing step by step
}

void Grid::triangulate() {
    //TODO finish this with an actual triangulation table
    this->indices.clear();

    //Currently only 2 cases: wall or not
    for(int i =0; i < (this->width); i++) {
        for(int j =0; j < (this->height); j++) {
            if(grid[i][j]==1.0) {
                unsigned int tl = i + j*(this->width+1);
                unsigned int tr = tl+1;
                unsigned int bl = tl + (this->width+1);
                unsigned int br = bl+1;

                this->indices.push_back(tl);
                this->indices.push_back(tr);
                this->indices.push_back(bl);

                this->indices.push_back(tr);
                this->indices.push_back(bl);
                this->indices.push_back(br);
            }
        }
    }
    updateGLData();
}

void Grid::draw() {
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Grid::createGLData() {
    //Create VAO, VBO, and EBO
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);
	//Tell the GPU how to interperet the data
    

    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Grid::updateGLData() {
    //Bind to the already created VAO
    glBindVertexArray(this->VAO);

    //Send data to the GPU
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);
	
    //Tell the GPU how to interperet the data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

    glBindVertexArray(0);
}

void Grid::deleteGLData() {
    glBindVertexArray(this->VAO);
    
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);

    glDeleteVertexArrays(1, &this->VAO);
    glBindVertexArray(0);
}

void Grid::generateVertices() {
    for(int i = 0; i < (this->width+1); i++) {
        for(int j = 0; j < (this->height+1); j++) {
            //Transform coords to range (-1.0, 1.0)
            float xpos = (((float)i)/((float)this->width) - 0.5)*2; 
            float ypos = (((float)j)/((float)this->height) - 0.5)*-2; //Want pos y to be at the top
            float zpos = 0.0;

            //Fill in info in vertices array
            this->vertices.push_back(xpos);
            this->vertices.push_back(ypos);
            this->vertices.push_back(zpos);
        }
    }
}

void Grid::printGrid() {
    for(int i = 0; i < this->width+1; i++) {
        for(int j = 0; j < this->height +1; j++) {
            printf("%.2f ", this->grid[i][j]);
        }
        std::cout << std::endl;
    }
}