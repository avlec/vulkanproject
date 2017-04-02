## Vulkan Project

This documents learning the ropes to the Vulkan API from the ground up, with **_zero_** understanding of how the 3D API works, following along with the book _Learning Vulkan_ by _Parminder Singh_

Currently working on getting the basic understanding of the Vulkan API, and of graphics API concepts.

### Getting the code working on your system

Currently only supporting instructions for Windows 64-bit in VS 14 2015

#### Windows (Visual Studio v14 2015 or v15 2017)
Currently the only OS that this is being developed on

1. Clone into an empty folder
2. Download the [LunarG Vulkan SDK][1]
3. Edit the CMakeLists.txt on line 12 to turn auto locate on, if you want to specify where you installed the Vulkan SDK manually you can leave it as off and edit lines 43 and 44.
4. Make two folders, _build_ and _binaries_. Build is where the visual studio and cmake files will be stored, and the binaries folder is where the executable will be put.
5. Open a console window and cd into the build directory and run this command `cmake -G "Visual Studio 14 2015 Win64" ..` or `cmake -G "Visual Studio 15 2017 Win64" ..`
6. Open the __vulkanproject__ visual studio solution which will open 3 different visual studio projects ALL_BUILD, vulkanproject, and ZERO_CHECK
7. Right click on the __vulkanproject__ project and click __Set as StartUp Project__
8. Build or run and you should see the program run

### Plans for this Project
Spend time developing a functional Vulkan enviornment which will eventually turn into a platform to develop a program/game of sorts.

[1]: https://www.lunarg.com/vulkan-sdk/
