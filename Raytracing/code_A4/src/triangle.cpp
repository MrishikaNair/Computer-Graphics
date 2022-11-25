//triangle.cpp

#include "triangle.h"

bool checkposi(double value){
	if(value>0){
		return true;
	}
	else{
		return false;
	}
}

bool Triangle::intersect(Ray& ray) const
{
    const float epsi = 0.0000001;

    //Triangle vertices
    Vector3D v0(-10,0,-11);
    Vector3D v1(6,0,-11);
    Vector3D v2(-2,8,-11);

    Vector3D rayvec=ray.getDirection();
    

    Vector3D v1v0 = v1 - v0;
    Vector3D v2v0 = v2 - v0;
    Vector3D h = crossProduct(rayvec,v2v0);
    float prl = dotProduct(v1v0,h);    //to check if ray is parallel to the shape
    
    //using Möller–Trumbore triangle intersection algorithm
    if (prl > -epsi && prl < epsi)
        return false;    //Indicates ray is parallel

    float f = 1.0/prl;

    Vector3D  dir = ray.getOrigin() - v0;
    float u = f * dotProduct(dir,h);
    if (u < 0.0 || u > 1.0)
        return false;
    Vector3D q = crossProduct(dir,v1v0);
    float v = f * dotProduct(rayvec,q);
    if (v < 0.0)
        return false;
    else if(u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * dotProduct(v2v0,q);
    if (t > epsi) // ray intersection
    {
        return ray.setParameter(t,this);
    }
    else //line intersection but no ray intersection.
        return false;

	return false;

}
Color Triangle::shade(const Ray& ray) const{

    std::vector<LightSource*> lightSourceList = world->getLightSourceList();
    Color color = material->shade(ray, isSolid);
    
    //Triangle vertices
    Vector3D v0(-10,0,-11);
    Vector3D v1(6,0,-11);
    Vector3D v2(-2,8,-11);  /*Vector3D camera_position(0, 0, 10), 
                            aligning with it for spec comp visibility*/
    

	
	
	Vector3D v1v0 = v1 - v0;
	Vector3D v2v0 = v2 - v0;
	
	//Getting normal value
	Vector3D normal = crossProduct(v2v0, v1v0);
	normal.normalize();
	
    //ambient shading
	Color Ia = world->getAmbient() * material->ka;

	Color Id = Color(0.0);
	Color Is = Color(0.0);


	for(LightSource* l: lightSourceList){   //iterating through the elements of the light source list

		//Lambertian shading
		Vector3D lightdir = ray.getPosition() - l->getPosition();
		lightdir.normalize();

        Vector3D H= ray.getDirection() + lightdir; //calculating halfway vector
		H = H/H.length();

        //diffused shading
		double nl = dotProduct(normal, lightdir);
		if(checkposi(nl)){
			Id = Id + (l->getIntensity()*material->kd * nl);	//kd fraction of diffused light that is reflected
		}

        //specular shading
		double nh = pow(dotProduct(normal, H), material->n);
		if(checkposi(nh)){
			Is = Is + (world->getSpecular() *material->ks *nh);
		}

	}
	

	return Ia + Id + Is;
}
