#include "Mesh.h"
#include "Texture.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

Mesh::Mesh(std::vector<sVertex> vertices, std::vector<sTriangle> faces, std::vector<Texture*> textures)
	: offset(0.0f, 0.0f, 0.0f), orientation(0.0f, 0.0f, 0.0f), textures(textures), colorOverride(1.0f, 1.0f, 1.0f, 1.0f)
{
	this->vertices = vertices;
	this->faces = faces;

	this->scale = 1.0f;
	this->isWireframe = false;
	this->ignoreLighting = false;
	this->isOverrideColor = false;

	this->SetupMesh();
}

void Mesh::Draw(const CompiledShader& shader, const glm::vec3& position, const glm::vec3& xRot, const glm::vec3& yRot, const glm::vec3& zRot, const glm::vec3& scale)
{
	glm::mat4 matModel = glm::mat4(1.0f);

	glm::vec3 meshPosition = position + this->offset;
	glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f), meshPosition); // Translation matrix
	matModel = matModel * matTranslate; // Aply translation to our matrix

	// Apply all the rotations to our matrix
	matModel[0] = glm::vec4(xRot.x, xRot.y, xRot.z, 0.0f) * scale.x; // X axis rotation
	matModel[1] = glm::vec4(yRot.x, yRot.y, yRot.z, 0.0f) * scale.y; // Y axis rotation
	matModel[2] = glm::vec4(zRot.x, zRot.y, zRot.z, 0.0f) * scale.z; // Z axis rotation

	glm::mat4 matInvTransposeModel = glm::inverse(glm::transpose(matModel));

	glUseProgram(shader.ID);

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "matModel"), 1, GL_FALSE, glm::value_ptr(matModel)); // Tell shader the model matrix (AKA: Position orientation and scale)
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "matModelInverseTranspose"), 1, GL_FALSE, glm::value_ptr(matInvTransposeModel));

	GLuint diffuse = 1;
	GLuint specular = 1;

	// WARNING: Even though there are technically 32 texture spots, the amount of texture slots you can use is dependent on the platform you are on.
	for (unsigned int i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture slot
		glBindTexture(GL_TEXTURE_2D, this->textures[i]->GetID()); // Bind texture

		std::stringstream ss;
		std::string name = this->textures[i]->GetType();

		if (name == "texture_diffuse")
		{
			ss << diffuse++;
		}
		else if (name == "texture_specular")
		{
			ss << specular++;
		}

		std::string number = ss.str();
		glUniform1i(glGetUniformLocation(shader.ID, (name, number).c_str()), i); // Tell shader what texture we are using by passing is the current texture slot that is bound
	}

	if (this->isOverrideColor)
	{
		glUniform1f(glGetUniformLocation(shader.ID, "isOverrideColor"), (float) GL_TRUE);
		glUniform4f(glGetUniformLocation(shader.ID, "colorOverride"), this->colorOverride.r, this->colorOverride.b, this->colorOverride.g, this->colorOverride.a);
	}
	else
	{
		glUniform1f(glGetUniformLocation(shader.ID, "isOverrideColor"), (float) GL_FALSE);
	}

	glUniform1f(glGetUniformLocation(shader.ID, "isIgnoreLighting"), this->ignoreLighting ? (float)GL_TRUE : (float)GL_FALSE);

	if (this->isWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Draw the mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Unbind textures
	for (unsigned int i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


void Mesh::AddTexture(Texture* texture)
{
	this->textures.push_back(texture);
}

void Mesh::SetupMesh()
{
	// Generate IDs for our VAO, VBO and EBO
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	// Tell open GL where to look for for vertex data
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(sVertex), (GLvoid*)&this->vertices[0], GL_STATIC_DRAW);

	// Tell open GL where our index buffer begins (AKA: where to look for faces)
	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->faces.size() * sizeof(sTriangle), (GLvoid*)&this->faces[0], GL_STATIC_DRAW);

	// Now ANY state that is related to vertex or index buffer
	//	and vertex attribute layout, is stored in the 'state' 
	//	of the VAO... 

	// Set the vertex attributes for this shader
	// Layout information can be found in the vertex shader, currently:
	// 0 = position
	// 1 = normals
	// 2 = texture coordinates
	glEnableVertexAttribArray(0);	    // position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sVertex), (GLvoid*) offsetof(sVertex, x));

	glEnableVertexAttribArray(1);	    // normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(sVertex), (GLvoid*) offsetof(sVertex, nx));

	glEnableVertexAttribArray(2);	    // textureCoordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sVertex), (GLvoid*)offsetof(sVertex, u0));

	// Now that all the parts are set up, unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}