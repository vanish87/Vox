# TODO List

* ~~Create VoxGame VoxWindow and VoxApplication classes.~~
* ~~Refactor windows functionality into discrete files and using the VoxWindow class.~~
* ~~Add camera controls and movement to mouse and keyboard controls.~~
* ~~Window and renderer resize functionality.~~
* ~~Add proper license.~~
* ~~Add Shader support to renderer.~~
* ~~Add frame buffer support to renderer.~~
* ~~Add selection of shaders to toggle different render modes for testing.~~
* ~~Add ability to camera class to rotate around point (rotate position around view).~~
* ~~Deferred lighting support.~~
* ~~Lighting effects.~~
* ~~Particle effects.~~
* ~~Add weapon trails.~~
* ~~Remove any GL calls from VoxGame - Add InitZeroTexture() functionality to renderer.~~
* ~~Use Cmake for build management.~~
* ~~Add fullscreen support and toggle.~~
* ~~Enable x64 build configurations.~~
* ~~Phong lighting shader.~~
* ~~Add basic lighting to scene - OpenGL lighting.~~
* ~~Add pre-built release exe to root of git, so people can run exe without compiling - VoxGame.exe~~
* App player class and put voxel character functionalit in player class.
* Switch over to using glm maths lib.
* Enable cross platform compiling and support, linux.
* Better readme file.
* Weapon trails controlled by the animation file.
* Convert shaders over to be more unified.
* Better SSAO shader.
  * Make deferred lighting and SSAO optional toggles separate.
  * Allow for phong shading and deferred lighting together.
* Shadow shader.


# BUGS
* ~~End of animation frame state - level up animation goes to mummy walk?~~
* Weapon trails are not frame rate independent.
* Fix block particle lighting params, rework shader for better lighting.
* Fix transparency and transparent textures when using the SSAO shader.
* Animated weapon sections get messed up when simulation is paused, via window focus, breakpoint, etc...
* CRASH when switching between particle weapon effects fast, only in release mode? uninitialized variable. :[
* GL Errors when closing application, shaders deletion.
