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
* ~~Better SSAO shader.~~
  * ~~Make deferred lighting and SSAO optional toggles separate.~~
  * ~~Allow for phong shading and deferred lighting together.~~
* ~~Shadow shader.~~
* ~~Decouple shadow rendering from SSAO shader.~~
* ~~Decouple dynamic lighting from SSAO shader.~~
* ~~Better readme file.~~
* ~~Split up VoxGame file into VoxRender, VoxUpdate, VoxSetup(?), etc.~~
* ~~App player class and put voxel character functionality in player class.~~
* ~~Add ability to switch voxel character model.~~
* ~~Rebuild vs2013 libs using non-dll version of linking~~
* ~~Fix up cmake for the different visual studio versions and also for the different lib linking.~~
* ~~Weapon trails controlled by the animation file.~~
* ~~Create vs2012 libs and VS2012 project files~~
* ~~Add blur shader.~~
* ~~Implement MSAA (or other AA) in glsl shader.~~
* ~~Convert shadow rendering calls to use renderer - remove GL from gamecode.~~
* ~~Add LUA bindings.~~
* ~~Make weapons trails time based, rather than "numpoints" based, better weapon trail alpha rendering.~~
* ~~Switch over to using glm maths lib.~~
* ~~Solution to allow for running the exe from Visual studio, because atm the working directory needs to be ../~~
* ~~Textures on GUI.~~
* ~~Frontend manager for textures and common GUI objects.~~
* ~~Allow camera rotations around world coordinates, so 'upside down' doesnt happen.~~
* ~~Add full GUI skin textures and control to change 'theme' on GUI.~~
* ~~Add limits to rotation so we dont go over and under the max/min up-axis.~~
* ~~Add different camera modes, option toggle on GUI.~~
* ~~Add player camera controls.~~
* ~~Enable cross platform compiling and support, linux, mac~~
* ~~Convert all data, media, folders to lowercase to fix Linux issues.~~
* ~~When in auto-camera mode, slowly orient to behind the player, after a time.~~
* ~~Add gamepad support.~~
* ~~Fix up auto-camera mode for gamepad and remove hacks for storing the up, right, forward target camera vectors.~~
  * ~~Store a camera-to-player vector in the player class and use this for camera offsets in auto-mode.~~
  * ~~Only use the x, z components when working out the offset camera position, plus the players up from the facing.~~
* ~~Proper first person mode camera.~~
* ~~Implement mouse rotation camera mode for when in game mode.~~
* ~~Turn cursor on/off and fix cursor in position when doing mouse rotations in game mode.~~
* ~~Make camera auto-mode rotate to behind the player after a short delay.~~
* ~~Settings file loading at startup, change options and rendering based on settings.~~
* ~~Player alpha when camera zooms in.~~
* ~~Add proper ini reader functionality - add other startup params to ini, resolution, fullscreen etc.~~
* ~~Camera clipping mode.~~
* ~~Add ability to create and render the Voxel objects/characters without merging, 2 vertex arrays, or rebuild?~~
* ~~Terrain generation via chunks.~~
  * ~~Chunk loading and unloading based on player position~~
* ~~Create Vox EXE launcher. (C# app)~~
* ~~Debug settings option to toggle the Debug GUI from the settings ini file.~~
* ~~Settings in ini to run straight into game, with player and camera controls - or run into front-end etc.~~
* ~~Ordering of the chunk loading based on the camera/player position, so that loading filters outwards from player.~~
* ~~Add back in skyboxes and support tweeningblending from one skybox to the other.~~
* ~~Wait before spawning player in world, until world is generated.~~
* ~~Better physics response and detection when jumping and from above~~
* ~~Add version string to config ini file and display on GUI.~~
* ~~Add DEBUG/RELEASE switch depending on what build was made, display on GUI.~~
* ~~Player movement on voxel world and terrain.~~
  * ~~Walking up single block steps automatically.~~
  * ~~Smooth out camera when doing single 1-size block step up animation. (Was because of different interpolator values)~~ 
* ~~Add exit/quit popup when the game is closed or quit using various exit methods.~~
* ~~Add pause menu when ESC is pressed, and add popup in center of screen.~~
* ~~Add console and logger to the in-game gui for output and logging information.~~
  * ~~Log when loading/saving model files.~~
  * Log to output file and log files as well as console GUI.
  * Remove old log messages, only allow a certain nunber of log messages to be shown.
  * Add debug log information for model loading and rendering - num verts, tris, faces, etc.
  * Log on errors.
  * Allow input commands.
  * Clear log text functionality
  * Rendering of many log messages is slow, output to file?
* Convert shaders over to be more unified.
* Add VLD support to debug version, allow for memory leak detection.
* Add audio and music playback.
* Add biome manager
  * Add colour variation management to biome manager
* Add scenery manager.
  * Add scenery to work, generation based on point distribution, poisson disc, etc.
  * Flower generation.
  * Poisson-disc sparation and generation for random placement of scenery.
  * Convert scenery manager to use instance rendering and the instance shader.
* Add item manager.
* Create basic front-end.
* Create front end scene and character line up.
* Character creation and customization.
* Saving and loading chunks.
  * Group together chunks for saving, loading. Not individual
* Add character creator and customization from front-end.
* Add options menu window to pause menu.
* Add voxel editor.
* Add texture support for voxel blocks.
* Add inventory GUI.
* Add character GUI.
* Add Quest GUI.
* Water, creating and manipulation, rendering, updating.
* Investigate continuous automatic builds on github when files are submitted to depot, using cmake and make.
* Add mode to play in reduced and low FPS.
  * Low FPS causes bad physics and also bad step-up block functionality, investigate and fix.
* Custom cursors - fingers.
* Dont allow jumping multiple times by holding down the space (or joy button).
* Fix camera glitching when going in and out of pause menu (also quit menu).
* Allow for font changing on the fly, allow different font styles to go with the themes.
* Add shadow receiving on instanced rendered objects. (Add texture matrix to shader??)
* Tidy up inventory and item managers!
  * Add back in TODO code (TODO : Add me back in).
  * Test ALL functionality with player equipping items and weapons.
  * Add hooks to GUI.
* Player particle trail on walking.
* Cinematic letterbox mode.
