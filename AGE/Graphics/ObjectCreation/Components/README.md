custom globals in shader:

VertexShader:
1. transform - current transform
2. instanceIndex

FragmentShader:
1. color - current color
2. instanceIndex

Global:
1. time - current time
2. resolution - screen resolution

if using texture
vec2 texCoords
