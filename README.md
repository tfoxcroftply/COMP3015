# COMP3015 CW1 By Tobey Foxcroft

## Development environment
This program was developed using Visual Studio 2022 on Windows 11. It was based off the program template provided in the Labs. GitHub was used extensively for version control, and for allowing seamless workflow across my desktop and my laptop. It has been tested for Intel and NVIDIA GPUs. 

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

## Features
### Phong
Ambient, diffuse and specular lighting have been implemented in this project, which is the concept of Phong. Phong has been implemented from lines 48-53 in the fragment shader. In this case, to reduce complexity, all rendered objects (apart from the skybox) have a set "shininess" and intensity values.
### Texture sampling
I have implemented texture sampling in the form of reflections, where depending on the angle a face is facing, the skybox is sampled and is overlaid. It has been implemented from lines 57-61 in the fragment shader.
### Fog
A fog overlay has been added to fade the colour of the ocean as the distance gets further, somewhat softening the edges slightly. It is one of the last stages in the shader, so it applies as a final overlay. It has been implemented in lines 67-68 in the fragment shader.
### Texture mixing
Texture mixing has been implemeneted, and is controlled by a uniform boolean. If the uniform boolean is set to true, it reads from GL_TEXTURE_1 and GL_TEXTURE_2, mixing them together. This takes place in lines 36-39 in the fragment shader.
### Skybox
The skybox is loaded using the GenerateSkybox() command from ObjectGen.h which returns a VAO. A uniform boolean is set to notify the vertex and fragment shader of the skybox being rendered, which means that the skybox will be rendered in a static position with no lighting effects applied to it.
### Model animation
The model in this scene has timed animations applied. The majority of the transformations take place from line 139-147 in scenebasic_uniform.cpp.
### Keyboard and mouse controls
As explained before, the keyboard and mouse affect the camera position and angle. The header file called "Camera.h" is responsible for taking in inputs every render loop, and transforming the camera's transformation matrix.

## YouTube video
https://www.youtube.com/watch?v=PgslRnZOkpg

## Credits
Skybox: https://polyhaven.com/a/qwantani_puresky
Model: https://www.thingiverse.com/thing:1752110 (Texture has been made by me)
