# Vox
Vox is a voxel based RPG adventure game with a heavy focus on player customization and creation.

## Features
* Qubicle Binary (.qb) voxel model loading.
* Polygon model loading via ms3d and obj files.
* Keyframe and skeleton animation support.

## License
This software is provided under the **GNU General Public License**, more information is provided in the [license file](https://github.com/AlwaysGeeky/Vox/blob/master/LICENSE.md).

## Setup
Setting up and compiling the project is a fairly simple process. Cmake can be used to build the project files for each environment and for different platforms, depending on what you are developing on.

**Windows**

Visual Studio projects are included inside the ```\project``` sub-folder.

**Linux**

// Todo //

**Mac**

// Todo //

## Running
The Vox executable needs to be run from the root directory so that it has access at the top level to the ```\media``` folder that contains all the game content files. If you want to run the game's executable from within the Visual Studio projects then you will need to set the working directy in the Debugging options to ```'..\'``` since the visual studio project files reside in a sub directory above the root.

Alternatively you can run the pre-built executable ```VoxGame.exe``` that is containted within this github repo. Please note however that there is no guarantee that this exe will contain the latest code from the repo.

## How To Contribute
Contributions are always welcome, either reporting issues/bugs or forking the repository and then issuing pull requests when you have completed some additional coding that you feel will be beneficial to the main project. If you are interested in contributing in a more dedicated capacity, then please contact me.

# Screenshots
![Vox](http://i.imgur.com/usPCqJt.png)

# Videos
