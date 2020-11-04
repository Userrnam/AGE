## Installation

#### install following libraries

* glslc
  * load binaries from https://github.com/google/shaderc/blob/main/downloads.md
  * copy glslc from install/bin to /usr/local/bin

* glm
sudo apt-get install libglm-dev

* glfw
sudo apt-get install libglfw2-dev

* freetype
sudo apt-get install libfreetype6 libfreetype6-dev

* OpenAL
sudo apt-get install libopenal-dev

* vulkan
sudo apt-get install vulkan-tools libvulkan-dev vulkan-validationlayers-dev

#### when all libraries are installed:

* python install.py

### some error fixes

#### requested unavailable validation layers
sudo apt-get install vulkan-validationlayers-dev

#### glslc fails
sudo apt-get install software-properties-common
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install gcc-4.9
sudo apt-get upgrade libstdc++6
