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

int main (int argc, char *argv[]) {

    const struct aiScene* ai_scene = aiImportFile("/home/perrin/Desktop/blenderStuff/cube.3ds",aiProcessPreset_TargetRealtime_Quality);
    
    CassimpBasedViewer aBViewer;
    
    aBViewer.addObject(ai_scene, "cube");
    
    return 0;

}
