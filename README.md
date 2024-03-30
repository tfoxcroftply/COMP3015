# COMP3015 CW1 By Tobey Foxcroft

## Development environment
This program was developed using Visual Studio 2022 on Windows 11. It was based off the program template provided in the Labs. GitHub was used extensively for version control, and for allowing seamless workflow across my desktop and my laptop.

## How does it work?
### Code structure and interaction
For navigating the code, it is split up in sections. The areas of interest are “scenebasic_uniform.h”, where lighting variables are defined, and “scenebasic_uniform.cpp” where the base of the project runs. There are additional files I have included, such as “Camera.h/.cpp”, and “ObjectGen.h”. These file's functions are respective to their name, where the camera file controls keyboard and mouse interactions and applies/returns the necessary transformations, and the ObjectGen file contains functions for generating the necessary objects/textures for the scene.

During initialisation, the program begins to load the models and textures using the included lab libraries, as well as my “ObjectGen.h” file. The data is stored as variables which are used in the render loop. Additionally, shader uniforms responsible for lighting settings are also set based on the variables provided in the header file of “scenebasic_uniform”.
During the render loop, each object is sent individually to the shaders with their corresponding transformations and settings. The shaders then sample the data from these meshes to create lighting effects, which produces the desired outcome.

The fragment shader is organised in two sections, one being responsible for handling the skybox and the other for everything else. The skybox has little to no visual effects applied, and therefore does not need extensive processing. The other section is organised in stages, where visual effects are applied in order of priority, aka "stages". The order goes as follows: Phong > Reflection > Haze > Fog (overlay) > Output. 

### Interaction
Ensuring that the program has started, interaction with the scene relies on the user's keyboard and mouse. To move the camera forward/back/left/right, the user must use the WASD keys on the keyboard. To aim the camera, the user must move the mouse around. 

## Additional information
If a static scene is preferred for inspecting visual effects/shading methods, there is a variable in “scenebasic_uniform.h” called “MovementEnabed” which can be set to false. 

## YouTube video
Not set

## Credits
Skybox: https://polyhaven.com/a/qwantani_puresky
Model: https://www.thingiverse.com/thing:1752110 (Texture has been made by me)
