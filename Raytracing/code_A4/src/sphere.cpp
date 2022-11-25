//sphere code

#include "sphere.h"

bool checkpos(double value){
	if(value>0){
		return true;
	}
	else{
		return false;
	}
}

bool Sphere::intersect(Ray& r) const
{
	Vector3D centerVector = r.getOrigin() - position;
	double a = 1.0;
	double b = 2*dotProduct(r.getDirection(), centerVector);
	double c = dotProduct(centerVector, centerVector) - radius*radius;
	double discriminant = b*b - 4.0*a*c;

	//now check if discr. is posivtive or zero, then only we have an intersection!
	if(discriminant >=0.0)
	{

		if(discriminant == 0)
		{
			double t;
			t = -b/(2.0*a);
			r.setParameter(t, this);
			return true;
		}
		else
		{
			//Calculate both roots
			double D = sqrt(discriminant);
			double t1 = (-b +D)/(2.0*a);
			double t2 = (-b -D)/(2.0*a);

			bool b1 = r.setParameter(t1, this);
			bool b2 = r.setParameter(t2, this);

			return b1||b2;    
		}
	}
	return false;


}
Color Sphere::shade(const Ray& rayvec)const
{
	std::vector<LightSource*> lightSourceList =world->getLightSourceList();
	Color color = material->shade(rayvec, isSolid);
	
	Vector3D raydir = rayvec.getDirection();
	float dist = raydir.length();
	raydir.normalize();

	Vector3D normal=position-rayvec.getPosition();
	normal.normalize();

	Color Ia = world->getAmbient() * material->ka;
	
	Color Id=Color(0.0);	//initialising to ensure there are no error(double type)
	Color Is=Color(0.0);

	for(LightSource* l: lightSourceList)	//iterating through the elements of the light source list
	{
		Vector3D lightdir = rayvec.getPosition() - l->getPosition();
		lightdir.normalize();

		Vector3D H= rayvec.getDirection() + lightdir; //calculating halfway vector
		H = H/H.length();

		//diffused shading
		double nl = dotProduct(normal, lightdir);
		if(checkpos(nl)){
			
			Id = Id + (l->getIntensity()*material->kd * nl);	//kd fraction of diffused light that is reflected
		}

		
		//specular shading
		double nh = pow(dotProduct(normal, H), material->n);
		if(checkpos(nh)){
			Is = Is + (world->getSpecular() *material->ks *nh);
		}

	}
	
	return Ia + Id + Is;	//ambient+diffused+specular
}





