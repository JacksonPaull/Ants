/*
TEST FILE PLAYGROUND FOR QUICK VALIDATION OF SMALL SEGMENTS WHICH DONT REQUIRE COMPILATION WITH THE REST OF THE PROJECT

*/


//#include "src/core.hpp"
#include <iostream>
#include <typeinfo>
#include <vector>

using std::string;

class A {
    public:
    virtual void foo() {
        std::cout << "A\n";
    };
};

class B : public A {
    public:
    void foo() {
        std::cout << "B\n";
    }
};

class C : public A {
    public:
    void foo() {
        std::cout << "C\n";
    }
};


int main() {
    //engine->resource_manager.loadShader("base", "./shaders/base");
    /*
    std::cout << "Beginning mainloop\n";
    while(engine->running) {
        //Main Loop
        engine->processInput();
        engine->renderWindow();

        glfwPollEvents();
    }*/

    std::vector<A* > t;
    t.push_back(new A());
    t.push_back(new B());
    t.push_back(new C());

    std::cout << t.size() << std::endl;
    for(int i = 0; i < t.size(); i++) {
        t[i]->foo();
    }
}

