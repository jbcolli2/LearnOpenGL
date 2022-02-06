#version 330 core

in vec2 UV;

out vec4 FragColor;

uniform bool horizontal;
uniform sampler2D image;

void main()
{
    vec2 texelSize = 1.0/textureSize(image,0);
    const float weights[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
    
    vec3 blurColor = texture(image, UV).rgb*weights[0];
    if(horizontal)
    {
        for(int ii = 1; ii < 5; ++ii)
        {
            blurColor += texture(image, vec2(UV.x + texelSize.x * ii, UV.y)).rgb * weights[ii];
            blurColor += texture(image, vec2(UV.x - texelSize.x * ii, UV.y)).rgb * weights[ii];
        }
    }
    else
    {
        for(int ii = 1; ii < 5; ++ii)
        {
            blurColor += texture(image, vec2(UV.x, UV.y + texelSize.y * ii)).rgb * weights[ii];
            blurColor += texture(image, vec2(UV.x, UV.y - texelSize.y * ii)).rgb * weights[ii];
        }
    }

    FragColor = vec4(blurColor, 1.0);
}
