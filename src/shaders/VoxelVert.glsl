#version 430 core

layout (location = 0) in vec4 aPos;

uniform mat4 uVP;

out vec4 vColor;

vec4 g_Colors[] = {
    // gray
    vec4(0.53, 0.51, 0.53, 1.0),
    vec4(0.39, 0.39, 0.40, 1.0),
    vec4(0.27, 0.27, 0.31, 1.0),
    // blue
    vec4(0.64, 0.86, 0.78, 1.0),
    vec4(0.41, 0.76, 0.83, 1.0),
    vec4(0.29, 0.50, 0.79, 1.0),
    // red
    vec4(0.93, 0.88, 0.62, 1.0),
    vec4(0.83, 0.63, 0.41, 1.0),
    vec4(0.71, 0.32, 0.32, 1.0),
    // brown
    vec4(0.90, 0.81, 0.71, 1.0),
    vec4(0.65, 0.48, 0.36, 1.0),
    vec4(0.50, 0.29, 0.23, 1.0),
    // green
    vec4(0.76, 0.83, 0.41, 1.0),
    vec4(0.54, 0.69, 0.38, 1.0),
    vec4(0.34, 0.48, 0.47, 1.0),
    // dark green
    vec4(0.56, 0.63, 0.21, 1.0),
    vec4(0.34, 0.49, 0.18, 1.0),
    vec4(0.14, 0.28, 0.27, 1.0),
};

void main()
{
    gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    vColor = g_Colors[uint(aPos.w) % 18];
}