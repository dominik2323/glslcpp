#version 330 core

uniform vec2 u_resolution;
uniform float u_time;

out vec4 out_color;

float pi = 3.1415;

void main() {
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    vec2 st = (2. * uv - 1.) * vec2(u_resolution.x / u_resolution.y, 1.);

    float b = (sin(u_time * 1.3) + 1.0) / 2.0; // 0 - 1
    float bf = floor(b);
    // b *= 1.0;

    float t = atan(st.y, st.x) + pi; // 0 - 2pi
    // st *= fract(t / (2.0 * pi));
    // st.x *= st.x * st.x;

    st = st.yx;
    st /= 0.5;
    st *= vec2(cos(1.0 * st.x) * (1.2 + (b * 1.2)), sin(st.y));

    float x1 = sin( 10.0 * t) * 0.3;
    float x2 = cos( 2.0 * t) * 0.2;
    float x3 = sin( 300.0 * t) * 0.1;
    float d = 0.0;
    d = x1 + x2 + x3;
    // d *= b;
    // d *= .2;

    float c = sqrt((st.x * st.x) + st.y * st.y) + d;
    vec3 bg = mix(vec3(1.0), vec3(1.0, 0.7, 0.2), c);
    // bg = bg.zyx;

    out_color = vec4(smoothstep(0.88, 0.91, c) * bg, 1.0);
}
