#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec2 iResolution;
uniform vec2 iCenter;
uniform float iTime;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

#define t iTime

vec2 N22 ( vec2 p) {
    vec3 a = fract(p.xyx*vec3(124.34, 241.59, 341.54));
    a += dot(a, a+31.55);
    return fract(vec2(a.x*a.y, a.y*a.z));
}

float sdCircle(vec2 p, float  r) {
    return length(p) - r;
}


void main() {
    vec2 uv = (2.0 * gl_FragCoord.xy - iResolution.xy) / iResolution.y;
    vec3 col = vec3(0.5);
    float minDist=100.0;

    if (false) {
    for (int i=0; i<50; i++) {
        vec2 p = sin(t *N22(vec2(i)));
        float d = length(uv - p);

        if (d< minDist) minDist = d;

    }
    }

    uv *= 2.;
    vec2 gv = fract(uv.rg) - 0.5;
    vec2 id = floor(uv);
    for (float y = -1.; y <= 1.; y ++) {
        for (float x = -1.; x <= 1.; x ++) {
            vec2 offset = vec2(x, y);
            vec2 n = N22(id+offset);
            vec2 p = offset + sin(n*t)*0.5;
            float d = length(gv - p);
            if (d< minDist) minDist = d;
        }
    }


    col = vec3(smoothstep(1.1, -0.5, minDist));

    vec4 texColor = texture(texture0, fragTexCoord);
    col = mix(texColor.rgb, col, sin(t)*0.5-0.5);
	finalColor=vec4(col, 1.0);
}