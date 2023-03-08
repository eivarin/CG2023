
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>


#include "wavefront.hpp"

std::vector<float> drawCylinder(float radius, float height, int slices, float px, float py, float pz)
{
	float aCil = 0;
	float rotation = (2 * M_PI);
	float delta = rotation / slices;
	height = height / 2;
	std::vector<float> vs;
	// glPolygonMode(GL_FRONT, GL_LINE);
	// glBegin(GL_TRIANGLES);
	while (aCil <= rotation)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		float px1 = px + (radius * sin(aCil)), py1 = pz + (radius * cos(aCil));
		float px2 = px + (radius * sin(aCil + delta)), py2 = pz + (radius * cos(aCil + delta));

		vs.push_back(px);
		vs.push_back(height);
		vs.push_back(pz);

		vs.push_back(px1);
		vs.push_back(height);
		vs.push_back(py1);

		vs.push_back(px2);
		vs.push_back(height);
		vs.push_back(py2);


		vs.push_back(px1);
		vs.push_back(height);
		vs.push_back(py1);

		vs.push_back(px1);
		vs.push_back(-height);
		vs.push_back(py1);

		vs.push_back(px2);
		vs.push_back(-height);
		vs.push_back(py2);


		vs.push_back(px1);
		vs.push_back(height);
		vs.push_back(py1);

		vs.push_back(px2);
		vs.push_back(-height);
		vs.push_back(py2);

		vs.push_back(px2);
		vs.push_back(height);
		vs.push_back(py2);


		vs.push_back(px);
		vs.push_back(-height);
		vs.push_back(pz);

		vs.push_back(px2);
		vs.push_back(-height);
		vs.push_back(py2);

		vs.push_back(px1);
		vs.push_back(-height);
		vs.push_back(py1);

		aCil += delta;
	}
	// glColor3f(1.0f, 0.0f, 0.0f);
	// glEnd();
	// printModel();

	return vs;
}


void drawCone(int height, int radius, int slices, int stacks)
{
    float delta = (2*M_PI) / (float)slices;
    float stack_h = height / (float)stacks;
    float curr_h = height;
    float stack_r = radius / (float)stacks;
    float curr_r = 0;
    int i = 0;
    glPolygonMode(GL_FRONT, GL_LINE);
    glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 0.0f);
    curr_r += stack_r;
    while (i < slices)
    {
        float aCil = delta*i;
        float px1 = (sin(aCil)), py1 = (cos(aCil));
        float px2 = (sin(aCil + delta)), py2 = (cos(aCil + delta));
        glVertex3f(0.0f, curr_h, 0.0f);
        glVertex3f(px1 * curr_r, curr_h - stack_h, py1  * curr_r);
        glVertex3f(px2 * curr_r, curr_h - stack_h, py2  * curr_r);
        i++;
    }
    curr_h-=stack_h;
    while (curr_h > 0)
    {
        i = 0;
        while (i < slices)
        {
            float aCil = delta*i;
            float px1 = (sin(aCil)), py1 = (cos(aCil));
            float px2 = (sin(aCil + delta)), py2 = (cos(aCil + delta));
            
            glVertex3f(px1 * curr_r, curr_h, py1 * curr_r);
            glVertex3f(px1 * (curr_r+stack_r), curr_h - stack_h, py1 * (curr_r+stack_r));
            glVertex3f(px2 * (curr_r+stack_r), curr_h - stack_h, py2 * (curr_r+stack_r));

            glVertex3f(px2 * curr_r, curr_h, py2 * curr_r);
            glVertex3f(px1 * curr_r, curr_h, py1 * curr_r);
            glVertex3f(px2 * (curr_r+stack_r), curr_h - stack_h, py2 * (curr_r+stack_r));
            i++;
        }
        curr_h-=stack_h;
        curr_r+=stack_r;
    }
    i=0;
    while (i < slices)
    {
        float aCil = delta*i;
        float px1 = (sin(aCil)), py1 = (cos(aCil));
        float px2 = (sin(aCil + delta)), py2 = (cos(aCil + delta));
        glVertex3f(px2 * curr_r, curr_h, py2  * curr_r);
        glVertex3f(px1 * curr_r, curr_h, py1  * curr_r);
        glVertex3f(0.0f, curr_h, 0.0f);
        i++;
    }
    
    glEnd();
}

void drawSphere(int radius, int slices, int stacks)
{
    float delta = (2*M_PI) / (float)slices;
	float stack = M_PI / (float)stacks;
    int curr_delta = 0;
	int curr_stack = 1;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLES);
	while (curr_delta < slices)
	{
		float alpha = delta*curr_delta;
		float beta = (stack*curr_stack) - (M_PI/2);
		float px1 = radius * cos(beta) * sin(alpha); float py1 = (radius * sin(beta)) + radius; float pz1 = radius * cos(beta) * cos(alpha);
		float px2 = radius * cos(beta) * sin(alpha+delta); float py2 = (radius * sin(beta)) + radius; float pz2 = radius * cos(beta) * cos(alpha+delta);
		float px3,px4,py3,py4,pz3,pz4;
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(px2,py2,pz2);
		glVertex3f(px1,py1,pz1);
		glVertex3f(0.0f,0.0f,0.0f);

		while (curr_stack < stacks)
		{
			beta = (stack*curr_stack) - (M_PI/2);
			px1 = radius * cos(beta - stack) * sin(alpha); 
			py1 = (radius * sin(beta - stack)) + radius; 
			pz1 = radius * cos(beta - stack) * cos(alpha);
			
			px2 = radius * cos(beta) * sin(alpha); 
			py2 = (radius * sin(beta)) + radius; 
			pz2 = radius * cos(beta) * cos(alpha);

			px3 = radius *  cos(beta - stack) * sin(alpha+delta); 
			py3 = (radius * sin(beta - stack)) + radius; 
			pz3 = radius *  cos(beta - stack) * cos(alpha+delta);

			px4 = radius * cos(beta) * sin(alpha+delta); 
			py4 = (radius * sin(beta)) + radius; 
			pz4 = radius * cos(beta) * cos(alpha+delta);

			glVertex3f(px1,py1,pz1);
			glVertex3f(px4,py4,pz4);
			glVertex3f(px2,py2,pz2);

			glVertex3f(px4,py4,pz4);
			glVertex3f(px1,py1,pz1);
			glVertex3f(px3,py3,pz3);

			curr_stack++;
		}
		curr_stack--;
		beta = (stack*curr_stack) - (M_PI/2);
		px1 = radius * cos(beta) * sin(alpha); py1 = (radius * sin(beta)) + radius; pz1 = radius * cos(beta) * cos(alpha);
		px2 = radius * cos(beta) * sin(alpha+delta); py2 = (radius * sin(beta)) + radius; pz2 = radius * cos(beta) * cos(alpha+delta);
		glVertex3f(0.0f,2*radius,0.0f);
		glVertex3f(px1,py1,pz1);
		glVertex3f(px2,py2,pz2);

		curr_delta++;
		curr_stack = 1;
	}
	
    glEnd();
}