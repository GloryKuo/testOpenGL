#include "Model_assimp.h"
#include <GL/glut.h>

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

Model_assimp::Model_assimp() : Model()
{
	float pos[3] = {0,0,0};
	m_scene_center = new Vertex(pos, NULL);
	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	m_stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&m_stream);

	// ... same procedure, but this stream now writes the
	// log messages to assimp_log.txt
	m_stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	aiAttachLogStream(&m_stream);
}

// ----------------------------------------------------------------------------

Model_assimp::~Model_assimp()
{
	// cleanup - calling 'aiReleaseImport' is important, as the library 
	// keeps internal resources until the scene is freed again. Not 
	// doing so can cause severe resource leaking.
	aiReleaseImport(m_scene);
	delete m_scene_center;

	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
	aiDetachAllLogStreams();
}

// ----------------------------------------------------------------------------

bool Model_assimp::loadAsset(std::string path)
{
	// we are taking one of the postprocessing presets to avoid
	// spelling out 20+ single postprocessing flags here.
	m_scene = aiImportFile(path.c_str(),aiProcessPreset_TargetRealtime_MaxQuality);

	if (m_scene) {
		findBoundBox();
		Vertex *min = boundBox[4];   //(0,0,0)
		for(int i=0;i<3;i++)
			m_scene_center->position[i] = min->position[i] + width[i]/2;
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------

void Model_assimp::findBoundBox()
{
	aiVector3D min, max;
	aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min.x = min.y = min.z =  1e10f;
	max.x = max.y = max.z = -1e10f;
	findBoundBox_for_node(m_scene->mRootNode, &min, &max, &trafo);

	width[0] = max.x - min.x;
	width[1] = max.y - min.y;
	width[2] = max.z - min.z;

	float pos[3];
	pos[0] = max.x; pos[1] = min.y; pos[2] = min.z;      //(x,0,0)
	boundBox[0] = new Vertex(pos, NULL);
	pos[0] = max.x; pos[1] = max.y; pos[2] = min.z;      //(x,y,0)
	boundBox[1] = new Vertex(pos, NULL);
	pos[0] = max.x; pos[1] = max.y; pos[2] = max.z;		//(x,y,z)
	boundBox[2] = new Vertex(pos, NULL);
	pos[0] = max.x; pos[1] = min.y; pos[2] = max.z;		//(x,0,z)
	boundBox[3] = new Vertex(pos, NULL);
	pos[0] = min.x; pos[1] = min.y; pos[2] = min.z;		//(0,0,0)
	boundBox[4] = new Vertex(pos, NULL);
	pos[0] = min.x; pos[1] = max.y; pos[2] = min.z;		//(0,y,0)
	boundBox[5] = new Vertex(pos, NULL);
	pos[0] = min.x; pos[1] = max.y; pos[2] = max.z;		//(0,y,z)
	boundBox[6] = new Vertex(pos, NULL);
	pos[0] = min.x; pos[1] = min.y; pos[2] = max.z;		//(0,0,z)
	boundBox[7] = new Vertex(pos, NULL);

}

// ----------------------------------------------------------------------------

void Model_assimp::findBoundBox_for_node(const aiNode* nd, 
	aiVector3D* min, 
	aiVector3D* max, 
	aiMatrix4x4* trafo
){
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = m_scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);

			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);

			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		findBoundBox_for_node(nd->mChildren[n],min,max,trafo);
	}
	*trafo = prev;
}

// ----------------------------------------------------------------------------

void Model_assimp::drawModel()
{
	static GLuint scene_list = 0;
	float tmp;
	tmp = width[0];
	tmp = aisgl_max(width[1], tmp);
	tmp = aisgl_max(width[2], tmp);
	tmp = 1.0f / tmp;
	glScalef(tmp, tmp, tmp);

	glTranslatef(-m_scene_center->position[0], -m_scene_center->position[1], -m_scene_center->position[2]);

	if(scene_list == 0){
		scene_list = glGenLists(1);
		glNewList(scene_list, GL_COMPILE);
			recursive_render(m_scene, m_scene->mRootNode);
		glEndList();
	}
	glCallList(scene_list);
}

// ----------------------------------------------------------------------------

void Model_assimp::recursive_render(const aiScene *sc, const aiNode* nd)
{
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = m_scene->mMeshes[nd->mMeshes[n]];

		apply_material(sc->mMaterials[mesh->mMaterialIndex]);

		if(mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if(mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if(mesh->mNormals != NULL) 
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(sc, nd->mChildren[n]);
	}

	glPopMatrix();
}

// ----------------------------------------------------------------------------
void Model_assimp::color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

// ----------------------------------------------------------------------------
void Model_assimp::set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

// ----------------------------------------------------------------------------
void Model_assimp::apply_material(const aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if(ret1 == AI_SUCCESS) {
    	max = 1;
    	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if(ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
        else
        	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else 
		glEnable(GL_CULL_FACE);
}

// ----------------------------------------------------------------------------