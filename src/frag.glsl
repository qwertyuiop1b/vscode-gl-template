#version 330 core
precision mediump float;

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;


uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;
uniform float uTime;

// uniform float time;

void main() {
  FragColor = mix(texture(ourTexture, TexCoord), texture(ourTexture2, TexCoord), 0.2);
}