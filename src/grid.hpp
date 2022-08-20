#pragma once
#include "core.hpp"
#include <queue>

#define CELL_HEIGHT 10

struct coord {
    int x, y;
    coord() {
        this->x = 0;
        this->y = 0;
    }

    coord(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

struct Region {
    int id;
    std::vector<coord> cells;
    std::vector<coord> borderCells;
    std::vector<int> connectedRegions;
    bool isWall;

    Region(int id, bool isWall) {
        this->id = id;
        this->isWall = isWall;

        this->cells = std::vector<coord>();
        this->connectedRegions = std::vector<int>();
        this->borderCells = std::vector<coord>();
    }
};


class Grid {
    private:
        int x, y;                           //Screen space coords
        int seed;
        int width;
        int height;
        int current_step;                   //Current step for autogen purposes
        int smoothness_level;               //Number of smoothing iterations
        float p;                            //Odds a cell is constructed as a wall

        int** grid; //Datastructure to hold grid values
        std::vector<float> vertices;
        std::vector<Region> regions;
        std::vector<unsigned int> indices;  //Holds the indexes of the vertices to draw
        unsigned int VAO, EBO, VBO;

        void generateVertices();

        void lockBorders();                 //Lock the borders to be walls

        

        void createGLData();
        void updateGLData();
        void deleteGLData();

        bool inBounds(int x, int y);

    public:
        Grid(int x, int y, int width, int height, bool autogen);
        Grid(int width, int height, bool autogen);
        ~Grid();

        void triangulate();                 //Triangulate all nodes
        std::vector<Region> detectAllRegions();               //Uses BFS of nodes with same value as the node to create regions
        Region detectRegion(int startX, int startY, bool** checked, int id);
        void filterRegions(int threshold);
        void connectRegions();

        void setSeed();
        void draw();
        void randomize();
        void nextStep();

        void smooth();                      //Use 4-5 Cellular Automata

        void printGrid();
};