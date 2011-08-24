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

	private:
	    
		set<struct aBVobject> assimpObjects;
};

#endif
