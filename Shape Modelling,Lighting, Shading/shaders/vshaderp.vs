#version 330
layout (location = 0)in vec3 vVertex;
layout (location = 1)in vec3 vNormal;

out vec3 Vertex;
out vec3 Normal;

uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
uniform vec3 vColor;
uniform vec3 l_pos;

out vec3 lpos;
out vec3 eyepos;
out vec3 fragColor;

void main(){
    lpos = l_pos;
    eyepos=vec3(1.0,1.0,0.5);
    gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);
    
    Vertex=vec3(vModel*vec4(vVertex,1.0));
    Normal= vec3(vec4(vNormal,1.0)*inverse(vModel));
    
    fragColor =vec3(0.0,0.5,0.1);

}
