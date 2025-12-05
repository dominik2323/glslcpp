#version 330 core

uniform vec2 u_resolution;
uniform float u_time;
out vec4 out_color;

float pi = 3.1415;

float tri(in vec2 uv, in float b) {
    float x = uv.x / 2.0;
    float d1 = x + uv.y;
    float d2 = x + (1.0 - uv.y);
    float d = min(d1, d2) / 1.5; 
    float s = 1.0 / 3.0;
    return smoothstep(s, s + b, d);
}

vec2 rm(in vec2 uv, in vec4 b) {
    float ix = step(b.x, uv.x) - step(b.y, uv.x);
    ix *= (uv.x - b.x) / abs(b.x - b.y);
    float iy = step(b.z, uv.y) - step(b.w, uv.y);
    iy *= (uv.y - b.z) / abs(b.z - b.w);
    return vec2(ix, iy);
}

float b(in float n) {
    float t = sin(u_time) * 0.5 + 0.5;
    return (sin(n * 4.0) * 0.5 + 0.5);
}

float n(in float res, in float t) {
    vec2 r = u_resolution;
    vec4 FC = gl_FragCoord;
    vec2 p = (FC.xy * 2.0 - r) / r.y;
    float l = 2.0 - length(res - 1.0);
    return max(l,-l*1e1) / exp(mod(dot(FC,sin(FC.yxyx))+t,2.)+sin(t+sin(t/.6+p.y)));
}

void main() {
    float t = sin(u_time) * 0.5 + 0.5;
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    float px = 1 / u_resolution.x;

    float blur = b(1.0 - abs((uv.y - 0.5) * 2.0));
    blur *= 0.2;
    float samples = 6.0;

    vec2 ruv1 = rm(uv, vec4(0.0, 1.0, 0.0, 1.0));
    ruv1.x = floor(ruv1.x * samples) / samples;
    float t1 = tri(ruv1, blur);

    float bg = 1.0 - (floor(abs((uv.y - 0.5) * 2.0) * 30.0) / 30.0);

    float res = t1 + bg;
    float o = n(res, t);
    // o *= 0.1;

    out_color = vec4(vec3(res), 1.0);
}
