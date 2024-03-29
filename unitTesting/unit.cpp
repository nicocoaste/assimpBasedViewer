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

/*
 * This unit test shows how the library must be used. A global pointer towards the class must be created (here: global_pointer).
 * Then, in order to use members of CassimpBasedViewer as functions for GLUT, we create five global functions that respectively call the 
 * following members of global_pointer: 
 *             display, reshape, MouseCB, MotionCB and loop
 * Then we must create two thread functions.
 * The first function will correspond to the display: it calls the function loop()
 * The second thread is the rest of the program: the assimp elements can be modified, and in real-time, the modifications will appear in the viewer.
 * Finally, in main(), we create an object 'aBViewer' of the class CassimpBasedViewer, and we set:
 *             global_pointer = &aBViewer;
 * Before launching the threads, it is better to add a first assimp object to the aBViewer, so that initial dimensions of the window can be computed.
 * But this might become unnecessary in future versions. 
 * 
 */

#include "assimpBasedViewer/assimpBasedViewer.h"
#include <sys/time.h>
#include <pthread.h>

#define OBJECT_TO_DISPLAY "/home/perrin/Desktop/ROBOT_DATA/DLR_HUMANOID/3DS_LWO_DAE/box.dae"
// #define OBJECT_TO_DISPLAY "/home/perrin/Desktop/ROBOT_DATA/CoMan/SteeveLinks/Wavefronts/pelvis_and_waist.obj"
// #define OBJECT_TO_DISPLAY "/home/perrin/Desktop/ROBOT_DATA/CoMan/SteeveLinks/Wavefronts/hip_and_thigh_right.obj"
// #define OBJECT_TO_DISPLAY "/home/perrin/Desktop/ROBOT_DATA/CoMan/SteeveLinks/Wavefronts/shin_right.obj"

CassimpBasedViewer *global_pointer;

int64_t ts_now (void) {
  struct timeval tv;
  gettimeofday (&tv,NULL);
  return (int64_t) tv.tv_sec * 1000000 + tv.tv_usec;
}

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

void loop(void) {
    if (global_pointer) {
	global_pointer->loop();
    }    
}

void *thread_1(void *ptr) {
    
    glutCreateWindow("assimpBasedViewer");    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(MouseCB);
    glutMotionFunc(MotionCB);
    
    if (global_pointer) {
	global_pointer->loop();
    }    
    
    return NULL;
}

void *thread_2(void *ptr) {
    
//     const struct aiScene* ai_scene2 = aiImportFile("../../unitTesting/cube.3ds",aiProcessPreset_TargetRealtime_Quality);
    const struct aiScene* ai_scene2 = aiImportFile(OBJECT_TO_DISPLAY,aiProcessPreset_TargetRealtime_Quality);
    
    if (global_pointer) {
// 	global_pointer->addObject(ai_scene2, "cube2");
    }
    
    int64_t time_start = ts_now();
    
    while(1) {
	aiVector3D vshift( 10.0* sin( ((double) (ts_now() - time_start)) / 1000000.0 ), 0.0, 0.0 );
	aiMatrix4x4 matr;
	aiMatrix4x4::Translation(vshift, matr);     
	ai_scene2->mRootNode->mTransformation = matr;
    }
    
    return NULL;
}

int main (int argc, char *argv[]) {
    
    CassimpBasedViewer aBViewer;
    global_pointer = &aBViewer;

    const struct aiScene* ai_scene = aiImportFile(OBJECT_TO_DISPLAY,aiProcessPreset_TargetRealtime_Quality);
    
    aBViewer.addObject(ai_scene, "cube");
    
    pthread_t thread1, thread2;
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    
    pthread_create( &thread1, NULL, thread_1, (void*) message1);
    pthread_create( &thread2, NULL, thread_2, (void*) message2);
    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL); 
    
    return 0;

}
