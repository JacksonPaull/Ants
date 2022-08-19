#pragma once
#include "core.hpp"
#include <queue>

#define CELL_HEIGHT 10

struct coord {
    int x, y;
};

struct Region {
    int id;
    std::vector<coord> cells;
    std::vector<int> connectedRegions;
    bool isWall;
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
        void triangulate();                 //Triangulate all nodes

        void detectRegions();               //Uses BFS of nodes with same value as the node to create regions
        void filterRegions();
        void connectRegions();

        void createGLData();
        void updateGLData();
        void deleteGLData();

    public:
        Grid(int x, int y, int width, int height, bool autogen);
        Grid(int width, int height, bool autogen);
        ~Grid();

        void setSeed();
        void draw();
        void randomize();
        void nextStep();

        void smooth();                      //Use 4-5 Cellular Automata

        void printGrid();
};