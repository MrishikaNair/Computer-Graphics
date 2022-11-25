#version 330
layout (location = 0)in vec3 vVertex;
layout (location = 1)in vec3 vertex_norm;

uniform vec3 vNormal;
uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
uniform vec3 vColor;
uniform vec3 l_pos;

vec3 light_color = vec3(0.5,0.5,0.5);

vec3 eyepos=vec3(1.0,1.0,0.5);
vec3 eye_normal;

vec3 v = normalize(eyepos-vVertex);
vec3 r = normalize(reflect(vVertex-l_pos, vertex_norm));

vec3 Ls = vec3(1.0, 1.0, 1.0);
vec3 Ld = vec3(0.7, 0.7, 0.7);
vec3 La = vec3(0.6, 0.2, 0.2);

vec3 ks = vec3(1.0, 1.0, 1.0);
vec3 kd = vec3(0.5, 0.6, 0.4);
vec3 ka = vec3(1.0, 1.0, 1.0);

float spec_exp = 32.0;

out vec3 fragColor;

void main(){

    gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);
    
    vec3 h = (l_pos + v)/length(l_pos + v);
	vec3 l = l_pos - vVertex;

    vec3 Ia = ka * La;
    vec3 Id = kd * max(dot(l, vertex_norm) * Ld, 0);
    vec3 Is = ks * Ls * max(pow(dot(h, vertex_norm), spec_exp), 0);
    vec3 Isp= ks * Ls * max(pow(dot(r, v), spec_exp), 0);
	//vColor = vColor * (Id);
    //vColor = vColor * (Id+Ia+Isp);//*light_color;
    fragColor = vec3(0.0,0.5,0.1) * (Id);
    
}
