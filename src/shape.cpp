#include "..\include\shape.h"



Shape::Shape() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

}

void Shape::Cube()
{
	//vertices of cube
    verticiePtr = new float [108]{
        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  

        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f  
    };

    verticieCount = 36;

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, verticiePtr, GL_STATIC_DRAW);

	glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

void Shape::Sphere() {

    //Create verticies of the sphere

    const int dotCount = 20;

    //xAxisRadian
    for (float zAxisR = 0; zAxisR <= M_PI; (zAxisR += M_PI / 8)) {
        for (float xAxisR = 0; xAxisR <= M_PI * 2; (xAxisR += M_PI / 16)) {
            glm::vec3 newPoint = glm::vec3(cos(xAxisR) * sin(zAxisR), sin(xAxisR) * sin(zAxisR), cos(zAxisR));
            points.push_back(newPoint);
        }
    }

    verticieCount = points.size();

    for (unsigned int elemCount = 0; elemCount < points.size(); (elemCount++)) {
        elements.push_back(0);
        for (int i = 1; i < 3; i++) {
            elements.push_back(elemCount + i);
        }
    }
    elements.push_back( (static_cast<unsigned int>(elements.size()) - 1) / 3);
    elements.push_back(0);
    elements.push_back(1);

    //verticieCount = elements.size();

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size() * 3, points.data(), GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements.size(), elements.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);


}

void Shape::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, verticieCount);
   // glDrawElements(GL_TRIANGLES, verticieCount, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Shape::DeleteExisting() {


}