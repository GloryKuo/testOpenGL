#ifndef MODEL_ASSIMP_H
#define MODEL_ASSIMP_H

#include <map>
#include <GL/glut.h>
#include "model.h"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


class Model_assimp : public Model
{
public:
	Model_assimp();
	~Model_assimp();
	bool loadAsset(std::string path);
	virtual void drawModel();
protected:
	virtual void findBoundBox();
	void findBoundBox_for_node(const aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo);
	void recursive_render (const aiScene *sc, const aiNode* nd);
	void apply_material(const aiMaterial *mtl);
	bool loadGLTextures(const aiScene* scene, std::string path);

private:
	void color4_to_float4(const aiColor4D *c, float f[4]);
	void set_float4(float f[4], float a, float b, float c, float d);
	std::string getBasePath(const std::string& path);

	// assimp scene object
	const aiScene *m_scene;
	Vertex *m_scene_center;

	// images / texture
	std::map<std::string, GLuint*> textureIdMap;	// map image filenames to textureIds
	GLuint*		textureIds;							// pointer to texture Array
	bool Only_Mesh;

	// assimp log
	aiLogStream m_stream;
};
#endif