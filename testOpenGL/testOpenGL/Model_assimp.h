#ifndef MODEL_ASSIMP_H
#define MODEL_ASSIMP_H

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
private:
	virtual void findBoundBox();
	void findBoundBox_for_node(const aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo);
	void recursive_render (const aiScene *sc, const aiNode* nd);
	void apply_material(const aiMaterial *mtl);

	void color4_to_float4(const aiColor4D *c, float f[4]);
	void set_float4(float f[4], float a, float b, float c, float d);

	const aiScene *m_scene;
	Vertex *m_scene_center;

	aiLogStream m_stream;
};
#endif