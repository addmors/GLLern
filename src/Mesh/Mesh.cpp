#include "Mesh.h"


Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weight));
	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(4, 4, GL_UNSIGNED_INT, sizeof(Vertex), (void*)offsetof(Vertex, id));
	glBindVertexArray(0);
}

void Mesh::Draw(Shader shader)
{	
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.ID, ("material."+name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		shader.SetFloat("material.shininess", 0.4f*128.0f);
	}
	//std::cout << diffuseNr << " " << specularNr << " " << std::endl;
	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}
void Mesh::DrawRigged(Shader shader, glm::mat4 model, glm::mat4 wiew, glm::mat4 projection)
{
	shader.Use();
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.ID, ("material." + name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	shader.SetFloat("material.shininess", 0.4f*128.0f);
	//string gBone = "gBones[]";
	//for (int i = 0; i < skelet.boneMats.size(); i++) {
	//	gBone.insert(7, std::to_string(i));
	//	shader.SetMat4(&gBone, skelet.boneMats[i]);
	//	//std::cout << gBone << endl;
	//	gBone.erase(7, std::to_string(i).length());

	//}
	glBindBuffer(GL_ARRAY_BUFFER, VBO_IDs);
	unsigned int vbids = glGetAttribLocation(shader.ID, "s_vIDs");
	glVertexAttribIPointer(vbids, 4, GL_INT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Weights);
	unsigned int vweights = glGetAttribLocation(shader.ID, "s_vWeights");
	glVertexAttribPointer(vweights, 4, GL_FLOAT, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(vbids);
	glEnableVertexAttribArray(vweights);

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "gBones"),  sceneLoaderSkeleton.boneMats.size(), GL_FALSE, glm::value_ptr(sceneLoaderSkeleton.boneMats[0]));
	//std::cout << diffuseNr << " " << specularNr << " " << std::endl;
	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}