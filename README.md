if requested unavailable validation layers
sudo apt-get install vulkan-validationlayers-dev

# installing glslc
1. load binaries from https://github.com/google/shaderc/blob/main/downloads.md
2. copy glslc from install/bin to /usr/local/bin

# if glslc fails
sudo apt-get install software-properties-common
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install gcc-4.9
sudo apt-get upgrade libstdc++6

# install glm
sudo apt-get install libglm-dev

# install glfw
sudo apt-get install libglfw2-dev

# install freetype
sudo apt-get install libfreetype6 libfreetype6-dev

# install OpenAL
sudo apt-get install libopenal-dev

# install vulkan
sudo apt-get install vulkan-tools libvulkan-dev vulkan-validationlayers-dev
