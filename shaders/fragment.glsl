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
    return (sin(n * 4.0) * 0.5 + 0.5) * t;
}

void main() {
    float t = sin(u_time) * 0.5 + 0.5;
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    vec4 FC = gl_FragCoord;
    vec2 r = u_resolution;

    vec2 ruv1 = rm(uv, vec4(0.0, 0.5, 0.0, 1.0));
    float t1 = tri(ruv1, b(uv.x));

    vec2 ruv2 = rm(uv, vec4(0.5, 0.7, 0.75, 0.25));
    float t2 = tri(ruv2, b(uv.x));

    vec2 ruv3 = rm(uv, vec4(0.7, 1.0, 0.9, 0.1));
    float t3 = tri(ruv3, b(uv.x));

    float res = t1 + t2 + t3;
    res *= uv.x + 0.2;

    vec2 p = (FC.xy * 2.0 - r) / r.y;
    float l = 2.0 - length(res - 1.0);
    float o = max(l,-l*1e1) / exp(mod(dot(FC,sin(FC.yxyx))+u_time,2.)+sin(t+sin(t/.6+p.y)));
    o *= 0.1;

    out_color = vec4(res * vec3(1.0, 0.4, 2.2) / o, 1.0);
}
