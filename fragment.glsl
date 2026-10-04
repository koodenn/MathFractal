#version 330 core
out vec4 FragColor;

uniform vec2 resolution;
uniform vec2 offset;
uniform float zoom;
uniform float time;
uniform int fractalType;
uniform int paletteType;
uniform int maxIterations;
uniform vec2 mousePos;
uniform int juliaMouseControl;

// Cosine based palette generator (Inigo Quilez)
vec3 palette(float t, vec3 a, vec3 b, vec3 c, vec3 d) {
    return a + b * cos(6.2831853 * (c * t + d));
}

vec3 getColor(float iter, float maxIt, int pal, float t) {
    if (iter >= maxIt) {
        // Deep obsidian core
        return vec3(0.015, 0.015, 0.03);
    }

    // Color cycling over iterations + time
    float norm = iter * 0.04 + t * 0.03;

    vec3 col;
    if (pal == 0) {
        // Cyberpunk Neon (Cyan / Purple / Gold)
        col = palette(norm, vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), vec3(0.0, 0.33, 0.67));
    } else if (pal == 1) {
        // Magma / Fire
        col = palette(norm, vec3(0.5, 0.4, 0.3), vec3(0.5, 0.4, 0.3), vec3(2.0, 1.0, 0.5), vec3(0.0, 0.15, 0.3));
    } else if (pal == 2) {
        // Bioluminescent Ocean / Emerald
        col = palette(norm, vec3(0.1, 0.5, 0.5), vec3(0.2, 0.5, 0.4), vec3(1.0, 1.5, 2.0), vec3(0.2, 0.5, 0.8));
    } else if (pal == 3) {
        // Psychedelic Rainbow
        col = palette(norm, vec3(0.8, 0.5, 0.4), vec3(0.2, 0.4, 0.2), vec3(2.0, 1.0, 1.0), vec3(0.0, 0.25, 0.25));
    } else {
        // Amethyst & Rose
        col = palette(norm, vec3(0.5, 0.2, 0.5), vec3(0.5, 0.3, 0.5), vec3(1.0, 1.0, 1.0), vec3(0.3, 0.6, 0.9));
    }

    return clamp(col, 0.0, 1.0);
}

void main() {
    vec2 st = (gl_FragCoord.xy - 0.5 * resolution) / min(resolution.x, resolution.y);
    vec2 c = st * (3.0 / zoom) + offset;

    vec2 z;
    vec2 juliaC;

    if (juliaMouseControl == 1) {
        juliaC = mousePos;
    } else {
        // Cinematic Lissajous orbit for Julia constant
        juliaC = vec2(0.7885 * cos(time * 0.15), 0.7885 * sin(time * 0.2) * 0.85);
    }

    if (fractalType == 0) {
        // Julia set
        z = c;
    } else if (fractalType == 1) {
        // Mandelbrot set
        z = vec2(0.0);
    } else if (fractalType == 2) {
        // Burning Ship
        z = vec2(0.0);
    } else {
        // Tricorn / Mandelbar
        z = vec2(0.0);
    }

    float maxIt = float(maxIterations);
    float n = maxIt;

    for (int i = 0; i < 1000; i++) {
        if (float(i) >= maxIt) break;

        if (fractalType == 0) {
            z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + juliaC;
        } else if (fractalType == 1) {
            z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        } else if (fractalType == 2) {
            z = vec2(z.x * z.x - z.y * z.y, -2.0 * abs(z.x * z.y)) + c;
        } else {
            z = vec2(z.x * z.x - z.y * z.y, -2.0 * z.x * z.y) + c;
        }

        float dotZ = dot(z, z);
        if (dotZ > 64.0) {
            float log_zn = log(dotZ) * 0.5;
            float nu = log(log_zn / log(2.0)) / log(2.0);
            n = float(i) + 1.0 - nu;
            break;
        }
    }

    vec3 finalColor = getColor(n, maxIt, paletteType, time);
    FragColor = vec4(finalColor, 1.0);
}
