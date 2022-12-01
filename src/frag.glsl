#version 330 core
precision mediump float;

in vec3 vColor;
out vec4 FragColor;
uniform float time;

void main() {
  vec3 color = vColor * sin(time);
  FragColor = vec4(color, 1.0);
}