# Mach1 Spatial Decode Plugin [Unreal Engine]

### Contents:
 - Binary Plugin (Blueprint)
 - Source Plugin (Cpp & Blueprint)

## Instructions:

### General
Copy required plugin (from within `Source` or `Binary`) "Mach1DecodePlugin" directory to your project's `Plugin` directory at the root of your project.
Then add "Mach1 Decode Plugin" through browser class panel.

### Source / Cpp 
1. Create cpp project
2. Copy Plugin folder to project folder
3. Check that plugin activated . go to menu "edit" -> "plugins" -> "audio" -> "Mach1 Decode Plugin"
4. In class/objects browser find "Mach1Spatial Actor" and add to scene


For bluprint project, 
1. Yon can convert it to cpp : http://allarsblog.com/2015/11/05/Converting-BP-Project-To-CPP/
2. Or you fisrt create base cpp project, make 4 steps
and then copy plugin (with compiled dll) folder to your blueprint project

## QA:

### Source
QA to final Packaging of project completed on:
4.17, 4.18, 4.19, 4.22

### Binary
QA to final Packaging of project completed on: 