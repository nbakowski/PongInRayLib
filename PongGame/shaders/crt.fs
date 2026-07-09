#version 330 core

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float time;

// Output fragment color
out vec4 finalColor;

void main()
{
    // Calculate CRT curve (slight barrel distortion) first
    vec2 cc = fragTexCoord - 0.5;
    float dist = dot(cc, cc) * 0.1;
    vec2 distortedUV = fragTexCoord + cc * (1.0 + dist) * dist;

    // Bezel Guard: If distortion pushes UV coordinates out of bounds, render black edge
    if (distortedUV.x < 0.0 || distortedUV.x > 1.0 || distortedUV.y < 0.0 || distortedUV.y > 1.0)
    {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // RGB shift for chromatic aberration using the distorted grid
    float shift = 0.002;
    float r = texture(texture0, distortedUV + vec2(shift, 0.0)).r;
    float g = texture(texture0, distortedUV).g;
    float b = texture(texture0, distortedUV - vec2(shift, 0.0)).b;
    vec4 color = vec4(r, g, b, 1.0);

    // CRT scanlines 
    float scanline = sin(distortedUV.y * 800.0) * 0.04;
    color.rgb -= scanline;

    // Vignette shadow effect
    float vignette = smoothstep(0.8, 0.5, length(cc));
    color.rgb *= vignette;

    // Subtle screen flicker
    color.rgb *= 0.97 + 0.03 * sin(time * 10.0);

    // Micro static noise
    float noise = fract(sin(dot(distortedUV, vec2(12.9898, 78.233)) + time) * 43758.5453);
    color.rgb += noise * 0.02;

    finalColor = color * colDiffuse * fragColor;
}
