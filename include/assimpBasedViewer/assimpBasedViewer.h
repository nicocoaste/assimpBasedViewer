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

#ifndef ASSIMPBASEDVIEWER_H
#define ASSIMPBASEDVIEWER_H

#include <iostream>
#include <set>
// #include <iomanip>
// #include <sstream>
// #include <fstream>
// #include <vector>
// #include <string>

// #include <stdio.h>

// assimp include files. These three are usually needed.
#include <assimp.h>
#include <aiPostProcess.h>
#include <aiScene.h>

#include "GL/glut.h"

using namespace std;

struct aBVobject {
    const struct aiScene * object;
    const char * id;    
};

bool operator < (const aBVobject& o1,const aBVobject& o2) {
    return (o1.id < o2.id);
}


class CassimpBasedViewer
{

	public:

		/*!
		 * Constructor.
		 */
		CassimpBasedViewer();

		/*!
		 * Destructor.
		 */
		~CassimpBasedViewer();
		
		/*!
		*  The display loop.
		*/
		void loop();
	
		/*!
		 * Adds an object (aiScene) to the set of objects (the private member assimpObjects), and gives it the unique identifier (a char array) defined by the user (obj_id). 
		 * If the identifier already exists for some object of assimpObjects, nothing is done.
		 */
		int addObject( const struct aiScene * obj_ptr, const char * obj_id); //returns 1 if an object was actually added (new id), 0 otherwise

		/*!
		 * Removes the object of assimpObjects whose identifier is obj_id.
		 * If this identifier is not found, nothing is done.
		 */
		int eraseObject(const char * obj_id); //returns 1 if an object was actually removed (id found), 0 otherwise
		
		void MouseCB(int _b, int _s, int _x, int _y)
		{
		    if (_s == GLUT_UP)
		    {
			dis += ddis;
			azim += dazim;
			elev += delev;
			azim2 += dazim2;
			elev2 += delev2;
			ddis = 0.0;
			dazim = 0.0;
			delev = 0.0;
			dazim2 = 0.0;
			delev2 = 0.0;
			return;
		    }

		    if (_b ==  GLUT_MIDDLE_BUTTON)
		    {
			mode = 0;
			beginx = _x;
			beginy = _y;
			return;
		    }
		    else if (_b == GLUT_RIGHT_BUTTON)
		    {
			mode = 1;
			beginx = _x;
			beginy = _y;
		    }
		    else 
		    {
			mode = 2;
			beginx = _x;
			beginy = _y;
		    }
		}
		    
		void MotionCB(int _x, int _y)
		{
		    if (mode == 0)
		    {
			ddis = dis * ((_y - beginy)/100.0 - (_x - beginx)/100.0); //these are parameters for glTranslate
		    }
		    else if(mode == 1)
		    {
			dazim = (_x - beginx)/2.5; //these are parameters for glRotate
			delev = (_y - beginy)/2.5; //these are parameters for glRotate     
		    }
		    else if(mode == 2)
		    {
			dazim2 = (_x - beginx)/105.5; //these are parameters for glTranslate
			delev2 = (_y - beginy)/105.5; //these are parameters for glTranslate     
		    }
		    glutPostRedisplay();
		}

		void reshape(int width, int height)
		{
		    const float aspectRatio = (float) width / height, fieldOfView = 45.0;

		    glMatrixMode(GL_PROJECTION);
		    glLoadIdentity();
		    gluPerspective(fieldOfView, aspectRatio, 0.05, 100.0);  /* Znear and Zfar */
		    glViewport(0, 0, width, height);
		}

		void display(void);

	private:
	    
		set<struct aBVobject> assimpObjects;
		
		
		int mode;
		float beginx, beginy;
		float dis, azim, azim2, elev, elev2;
		float ddis, dazim, dazim2, delev, delev2;
		struct aiVector3D ai_environment_min, ai_environment_max, ai_environment_center;

		void get_bounding_box_for_node (const struct aiScene* ai_scene, const struct aiNode* nd, struct aiVector3D* min, struct aiVector3D* max, struct aiMatrix4x4* trafo);

		void get_bounding_box (const struct aiScene* ai_scene, struct aiVector3D* min, struct aiVector3D* max);

		void color4_to_float4(const struct aiColor4D *c, float f[4]);

		void set_float4(float f[4], float a, float b, float c, float d);

		void apply_material(const struct aiMaterial *mtl);

		void Color4f(const struct aiColor4D *color);

		void recursive_render (const struct aiScene *sc, const struct aiNode* nd);
};

#endif
