#include "ant.h"
#include "grid.h"

Ant::Ant()
{
	//Initialize everything

	glGenBuffers(1, &this->VBO);
	glGenVertexArrays(1, &this->VAO);

							//Position of a quad	//Texcoords
    float vertices[] = {
        // pos      // tex
        0.0f, 100.0f, 0.0f, 1.0f,
        100.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 100.0f, 0.0f, 1.0f,
        100.0f, 100.0f, 1.0f, 1.0f,
        100.0f, 0.0f, 1.0f, 0.0f
    };
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(this->VAO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ResourceManager::GetTexture("ant").Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);


}

void Ant::move()
{

}

bool Ant::checkCollisionAtPoint(int x, int y)
{
	
    return true;
}

void Ant::draw()
{
    glm::mat4 model = glm::mat4(1.0f);
    ResourceManager::GetShader("ant").SetMatrix4("model", model, true);
    glm::mat4 ortho = glm::ortho(0.0f, 1500 + 0.0f, 900.0f,  0.0f, -100.0f, 100.0f);
    ResourceManager::GetShader("ant").SetMatrix4("projection",ortho, true);
    
    glActiveTexture(GL_TEXTURE0);
    ResourceManager::GetTexture("ant").Bind();
    

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}