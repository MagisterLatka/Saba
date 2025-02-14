# Saba Engine
Saba Engine is a simple app and rendering engine. Currently it only supports Linux and Windows.

***

## Getting started
* For Linux, cmake and clang/g++ are required

<ins>**1. Downloading the repository**</ins>

Clone the repository: `git clone --recursive https://github.com/MagisterLatka/Saba`

If the repository was cloned non-recursively, use `git submodule update --init` to clone the submodules.

<ins>**2. Configuring the dependencies:**</ins>

* Linux
1. Open the terminal in the workspace
2. Run the [Linux-GenProjects.sh](https://github.com/MagisterLatka/Saba/blob/master/Scripts/Linux-GenProjects.sh) file found in `scripts` folder
(`./Scripts/Linux-GenProjects.sh`) to generate CMake project files. You will be prompted to choose compiler and build type, providing nothing will automaticly
choose debug build using clang. If changes are made or you want to regenerate project files or switch build type, rerun the script.
3. To compile, either run [Linux-Compile.sh](https://github.com/MagisterLatka/Saba/blob/master/Scripts/Linux-Compile.sh) or go to `bin-int` folder and run ninja. The compiled
project should be in the `./bin` folder. 

* Windows
1. 


### Main features:<br />
To be done

### Goals:
