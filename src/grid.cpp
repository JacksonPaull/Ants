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
    this->seed = 5; //rand();
    
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

    //6. Filter out small regions

    //7. Connect regions

    //8. Triangulate
    triangulate();

    //9. Save data on the GPU
    createGLData();
}

Grid::~Grid() {
    //Delete Grid
    for(int i = 0; i < this->width+1; i++)
        delete[] this->grid[i];
    delete[] this->grid;

    //Free GPU space
    deleteGLData();
}

void Grid::randomize() {
    srand(this->seed);

    for(int i=0; i < this->width+1; i++) {
        for(int j=0; j < this->height+1; j++) {
            int val = rand()%2;

            this->grid[i][j] = (float)val;
        }
    }
}

void Grid::triangulate() {
    //TODO

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
    glBindVertexArray(this->VAO);
    //Create Vertex Buffer Object to hold vertex positions
    glDeleteBuffers(1, &this->VBO);
    glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    glDeleteBuffers(1, &this->EBO);
    glGenBuffers(GL_ELEMENT_ARRAY_BUFFER, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

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

void Grid::test() {
    std::cout << "Vertices: \n";
    for(int i = 0; i < this->vertices.size(); i++) {
        printf("%.2f ", this->vertices.data()[i]);
    }
    std::cout << std::endl;

    std::cout << "Indices: \n";
    for(int i = 0; i < this->indices.size(); i++) {
        printf("%d ", this->indices.data()[i]);
    }
    std::cout << std::endl;
    
    for(int i = 0; i < this->width+1; i++) {
        for(int j = 0; j < this->height +1; j++) {
            printf("%.2f ", this->grid[i][j]);
        }
        std::cout << std::endl;
    }
}