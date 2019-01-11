uniform ImageParameters disparityParameters;
uniform sampler2D disparityColor;
uniform sampler2D disparityDepth;

uniform ImageParameters outportParameters;

// distance is included in this
uniform float disparityScale_x;
uniform float disparityScale_y;

void main() {
    vec2 texCoords = gl_FragCoord.xy * disparityParameters.reciprocalDimensions;
    float disparity = texture(entryDepth, texCoords).x;
    float disparity_x = disparity * disparityScale_x;
    float disparity_y = disparity * disparityScale_x;

    vec2 dimensions = disparityParameters.dimensions;
    float coord_x = clamp(texCoords.x + disparity_x, 0, dimensions.x);
    float coord_y = clamp(texCoords.y + disparity_y, 0, dimensions.y);

    FragData0 = texture(disparityColor, vec2(coord_x, coord_y)); 
}