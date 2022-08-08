#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <vector>
#include <iostream>
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

        float** grid; //Datastructure to hold grid values
        std::vector<float> vertices;
        std::vector<Region> regions;
        std::vector<unsigned int> indices;  //Holds the indexes of the vertices to draw
        unsigned int VAO, EBO, VBO;

        void generateVertices();

        void smooth();                      //Use 4-5 Cellular Automata
        void triangulate();                 //Triangulate all nodes

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

        void test();
};