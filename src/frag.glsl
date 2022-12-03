#version 330 core
precision mediump float;

in vec2 TexCoord;
out vec4 FragColor;


uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;


void main() {
  FragColor = mix(texture(ourTexture, TexCoord), texture(ourTexture2, TexCoord), 0.2);
}