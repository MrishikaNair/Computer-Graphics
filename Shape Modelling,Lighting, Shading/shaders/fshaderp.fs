#version 330 core

in vec3 Vertex;
in vec3 Normal;

in vec3 eyepos;

in vec3 lpos;
in vec3 fragColor;

vec3 Ls = vec3(1.0, 1.0, 1.0);
vec3 Ld = vec3(0.7, 0.7, 0.7);
vec3 La = vec3(0.6, 0.2, 0.2);

vec3 ks = vec3(1.0, 1.0, 1.0);
vec3 kd = vec3(0.5, 0.6, 0.4);
vec3 ka = vec3(1.0, 1.0, 1.0);

float spec_exp = 32.0;


vec3 v = normalize(eyepos-Vertex);
vec3 r = normalize(reflect(Vertex-lpos, Normal));




out vec4 outColor;
void main(void) {

        
        vec3 h = (lpos + v)/length(lpos + v);
	vec3 l = lpos - Vertex;

        vec3 Ia = ka * La;
        vec3 Id = kd * max(dot(l, Normal) * Ld, 0);
        vec3 Is = ks * Ls * max(pow(dot(h, Normal), spec_exp), 0);

        outColor = vec4(fragColor*(Is+Ia+Id), 1.0);
}