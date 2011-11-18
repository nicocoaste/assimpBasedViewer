/*
* Copyright 2010, 2011
*
* Nicolas Perrin
*
* LAAS-CNRS
*
* This file is part of assimpBasedViewer.
* assimpBasedViewer is a free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* assimpBasedViewer is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Lesser Public License for more details.
* You should have received a copy of the GNU Lesser General Public License
* along with assimpBasedViewer. If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "assimpBasedViewer.h"

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

CassimpBasedViewer::CassimpBasedViewer() {  
	dis = 10.0; azim = 0.0; azim2 = 0.0; elev = 0.0; elev2 = 0.0;
	ddis = 0.0; dazim = 0.0; dazim2 = 0.0; delev = 0.0; delev2 = 0.0;
	
	glutInitWindowSize(600,600);
	glutInitWindowPosition(100,100);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	int u = 0;
	glutInit(&u, NULL);
}

CassimpBasedViewer::~CassimpBasedViewer() {  
}

int CassimpBasedViewer::addObject( const struct aiScene * obj_ptr, aBVid obj_id) {
    aBVobject ObjTemp;
    ObjTemp.object = obj_ptr;
    ObjTemp.id = obj_id;
    unsigned int tmp = assimpObjects.size();
    assimpObjects.insert(ObjTemp);
    if(assimpObjects.size() > tmp) return 1;
    else return 0;    
}

int CassimpBasedViewer::eraseObject( aBVid obj_id) {
    aBVobject ObjTemp;
    ObjTemp.object = NULL;
    ObjTemp.id = obj_id;
    unsigned int tmp = assimpObjects.size();
    assimpObjects.erase(ObjTemp);
    if(assimpObjects.size() < tmp) return 1;
    else return 0;    
}

void CassimpBasedViewer::loop() {
    
    glClearColor(0.1f,0.1f,0.1f,1.f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);    // Uses default lighting parameters

    glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_DEPTH_TEST);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_NORMALIZE);

    if(getenv("MODEL_IS_BROKEN"))  
	    glFrontFace(GL_CW);

    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    
    if(assimpObjects.size() >= 1) {
	
	ai_environment_min.x = ai_environment_min.y = ai_environment_min.z =  1e10f;
	ai_environment_max.x = ai_environment_max.y = ai_environment_max.z = -1e10f;

	set<aBVobject>::iterator it;
	for(it = assimpObjects.begin(); it != assimpObjects.end(); it++) get_bounding_box((*it).object, &ai_environment_min,&ai_environment_max);
		
	ai_environment_center.x = (ai_environment_min.x + ai_environment_max.x) / 2.0f;
	ai_environment_center.y = (ai_environment_min.y + ai_environment_max.y) / 2.0f;
	ai_environment_center.z = (ai_environment_min.z + ai_environment_max.z) / 2.0f;
	
	glutMainLoop();
    }
}

void CassimpBasedViewer::get_bounding_box_for_node (
	const struct aiScene* ai_scene,
	const struct aiNode* nd, 
	struct aiVector3D* min, 
	struct aiVector3D* max, 
	struct aiMatrix4x4* trafo)
{
	struct aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = ai_scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			struct aiVector3D tmp = mesh->mVertices[t];
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
		get_bounding_box_for_node(ai_scene, nd->mChildren[n],min,max,trafo);
	}
	*trafo = prev;
}

void CassimpBasedViewer::get_bounding_box (const struct aiScene* ai_scene, struct aiVector3D* min, struct aiVector3D* max)
{
	struct aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);
	get_bounding_box_for_node(ai_scene, ai_scene->mRootNode, min, max, &trafo);
}

void CassimpBasedViewer::color4_to_float4(const struct aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void CassimpBasedViewer::set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void CassimpBasedViewer::apply_material(const struct aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
// 	int ret1, ret2;
	struct aiColor4D diffuse;
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
// 	float shininess, strength;
// 	int two_sided;
// 	int wireframe;
	int max;

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

// 	max = 1;
// 	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
// 	max = 1;
// 	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
// 	if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
// 		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
// 	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
// 	}

	max = 1;
// 	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
// 		fill_mode = wireframe ? GL_LINE : GL_FILL;
// 	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
// 	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
// 		glEnable(GL_CULL_FACE);
// 	else 
		glDisable(GL_CULL_FACE);
}


void CassimpBasedViewer::Color4f(const struct aiColor4D *color) // Can't send color down as a pointer to aiColor4D because AI colors are ABGR.
{
	glColor4f(color->r, color->g, color->b, color->a);
}

void CassimpBasedViewer::recursive_render (const struct aiScene *sc, const struct aiNode* nd)
{
	unsigned int n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];

		apply_material(sc->mMaterials[mesh->mMaterialIndex]);

		if(mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}

		if(mesh->mColors[0] != NULL) {
			glEnable(GL_COLOR_MATERIAL);
		} else {
			glDisable(GL_COLOR_MATERIAL);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(unsigned i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if(mesh->mColors[0] != NULL)
					Color4f(&mesh->mColors[0][index]);
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

void CassimpBasedViewer::display(void)
{
	float tmp;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

// 	gluLookAt(0.f,0.f,3.f,0.f,0.f,-5.f,0.f,-1.f,0.f);
// 	gluLookAt( ai_environment_center.x, ai_environment_center.y + 3.0f, ai_environment_center.z, ai_environment_center.x, ai_environment_center.y, ai_environment_center.z, 0.f,0.f,1.f);

	gluLookAt( 0.f, 14.f, 0.f, 0.f, 0.f, 0.f, 0.f,0.f,1.f);
	
	glTranslatef(azim2+dazim2, -elev2-delev2, 0.0);
	glTranslatef(0.0, 0.0, (dis+ddis));
	glRotated(azim+dazim, 0.0, 1.0, 0.0);
	glRotated(elev+delev, 1.0, 0.0, 0.0);
	glRotated(dis2+ddis2, 0.0, 0.0, 1.0);
	
// 	glTranslatef(azim2+dazim2, -elev2-delev2, 0.0);
// 	glTranslatef(0.0, 0.0, -(dis+ddis));
// 	glRotated(azim+dazim, 0.0, 1.0, 0.0);
// 	glRotated(elev+delev, 1.0, 0.0, 0.0);
// 	glRotated(dis2+ddis2, 0.0, 0.0, 1.0);
	
		// scale the whole asset to fit into our view frustum 
		//(BEFORE THIS PART OF THE CODE WAS AFTER THE TRANSLATIONS AND ROTATIONS
// 	tmp = ai_environment_max.x-ai_environment_min.x;
// 	tmp = aisgl_max(ai_environment_max.y - ai_environment_min.y,tmp);
// 	tmp = aisgl_max(ai_environment_max.z - ai_environment_min.z,tmp);
// 	tmp = 1.f / tmp;
// 	glScalef(tmp, tmp, tmp);

        // center the model
// 	glTranslatef( -ai_environment_center.x, -ai_environment_center.y, -ai_environment_center.z );
	
	
	GLuint scene_list = 0;
	if(scene_list == 0) {
	    scene_list = glGenLists(1);
	    glNewList(scene_list, GL_COMPILE);
	    glEndList();
	}

	set<aBVobject>::iterator it;
	for(it = assimpObjects.begin(); it != assimpObjects.end(); it++) recursive_render((*it).object, (*it).object->mRootNode);
	
	glCallList(scene_list);

	glutSwapBuffers();
	
	glutPostRedisplay();
}