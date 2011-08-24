/*
* Copyright 2010, 2011
*
* Nicolas Perrin
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
*
*/

#include "assimpBasedViewer/assimpBasedViewer.h"

CassimpBasedViewer *global_pointer;

void display(void) {
    if (global_pointer) {
	global_pointer->display();
    }    
}

void reshape(int width, int height) {
    if (global_pointer) {
	global_pointer->reshape(width, height);
    }   
}

void MouseCB(int _b, int _s, int _x, int _y) {
   if (global_pointer) {
	global_pointer->MouseCB(_b,_s,_x,_y);
    }       
}

void MotionCB(int _x, int _y) {
    if (global_pointer) {
	global_pointer->MotionCB(_x,_y);
    }        
}

int main (int argc, char *argv[]) {
    
    CassimpBasedViewer aBViewer;
    global_pointer = &aBViewer;
    
    glutCreateWindow("assimpBasedViewer");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(MouseCB);
    glutMotionFunc(MotionCB);

    const struct aiScene* ai_scene = aiImportFile("../../unitTesting/cube.3ds",aiProcessPreset_TargetRealtime_Quality);
    
    aBViewer.addObject(ai_scene, "cube");
    
    aBViewer.loop();
    
    return 0;

}
