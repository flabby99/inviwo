uniform ImageParameters entryParameters;
uniform sampler2D entryColor;
uniform sampler2D entryDepth;

uniform ImageParameters outportParameters;
uniform sampler2D exitColor;

uniform CameraParameters camera;

uniform float cameraBaseline;

float disparity_from_coord(vec2 texCoords)