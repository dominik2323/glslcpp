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
    return max(l,-l*1e1) / exp(mod(dot(FC,sin(FC.yxyx))+t,2.));
    //+sin(t+sin(t/.6+p.y)));
}

float st(in float s, in float n) {
    return floor(s * n) / n;
}

float g_map(in float d) {
    float m = step(0.001, d);
    return sin(d * pi);
}

float range(in float x, in vec2 i) {
    return step(i.x, x) * (1.0 - step(i.y, x));
}

float rmap(in float x, in vec2 b) {
    return ((x - b.x) / abs(b.x - b.y));
}

const int l = 8;

float[l] ints = float[](0.0, 0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 1.0);
vec3[l] pal = vec3[](
    vec3(0, 1, 1),
    vec3(1, 1, 1),
    vec3(0, 0, 0),
    vec3(1, 1, 0),
    vec3(1, 1, 0),
    vec3(0.0, 0.8, 0.50),
    vec3(0.5),
    vec3(1, 1, 1)
);

vec3 fill(in float x) {
    vec3 c = vec3(0.0); 
    for (int i = 0; i < l-1; i++) {
        float even = 1.0 - mod(float(i), 2.0);
        vec2 b = vec2(ints[i], ints[i+1]);
        float m = range(x - 0.001, b);
        c += mix(pal[i], pal[i + 1], rmap(x, b)) * m;
    }
    return c;
}

void main() {
    float t = sin(u_time) * 0.5 + 0.5;
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    float px = 1.0 / u_resolution.x;

    vec2 ruv1 = rm(uv, vec4(0.0, 1.0, 0.0, 1.0));
    // ruv1.x = st(ruv1.x, 10.0);
    float t1 = tri(ruv1, 0.2);
    // t1 = ruv1.x;
    float m = step(0.001, t1);

    out_color = vec4(fill(t1) * m, 1.0);
}
