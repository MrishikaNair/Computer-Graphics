#include "world.h"

using namespace std;

double eta = 3.3;		//didn't use in material
double eta_air = 1.5;

Vector3D dielec(Vector3D d, Vector3D n, double mue){	
    double nd = dotProduct(d, n);
	//mue = refractive index
    Vector3D t = unitVector(((d - n*nd)/mue) - (n*sqrt(1 - ((1 - pow(nd, 2))/pow(mue, 2)))));
    return t;
}


float World::firstIntersection(Ray& ray)
{
	//finding out the first instace on ray intersection
	for(int i=0; i<objectList.size(); i++)
		objectList[i]->intersect(ray);
	
	return ray.getParameter();
}


Color World::shade_ray(Ray& ray, int count)
{	
	
	firstIntersection(ray);
	if(ray.didHit())
	{
			Color Id = Color(0);
			for(LightSource* light: lightSourceList){
				Ray shad(ray.getPosition(), light->getPosition() - ray.getPosition());
				firstIntersection(shad);
				if(shad.didHit()){
					Id = Id + (ambient * shad.intersected()->getMaterial()->ka);
					//if ray hits then the material ambient coefficient is considered
				}
				else{
					//ray doesn't hit
					Id = Id + (ray.intersected())->shade(ray);		
				}
			}
			return Id;
		}
		
	return background;
	

}



bool root_posi(Vector3D d, Vector3D n, double mue){	
	//value inside the root should be positive
    double nd = dotProduct(d, n);
    double rootin = 1 - (pow(eta_air, 2)*(1 - pow(nd, 2)))/pow(mue, 2);
    if(rootin < 0){
        return true;
    }
    return false;
}
/*
Color World::shade_ray(Ray& ray, int count)
{
	
	firstIntersection(ray);
	if(ray.didHit()) {
        if (count <= 10) {		//maintaing the number of hits
            if(ray.intersected()->isSolid == true){

                
                double nd = dotProduct(ray.getDirection(), ray.getNormal());
                Vector3D r = unitVector(ray.getDirection() - (2 * nd * ray.getNormal()));

                Ray ray_reflect(ray.getPosition(), r);
                return ((ray.intersected())->shade(ray) + Color(0.7, 0.3, 0.2)*shade_ray(ray_reflect, count + 1));
            }
            else{

                Vector3D n = ray.getNormal();
                Vector3D d = ray.getDirection();
                double nd = dotProduct(d, n);

                Vector3D r = unitVector(d - (2 * nd * n));
                Ray ray_reflect(ray.getPosition(), r);

				Color k= Color(1.0, 1.0, 1.0);
                double c = 0.0;

                if(nd < 0){
                    Vector3D t = dielec(d, n, eta);
                    c = -1 * nd;

                    double at = ray.getParameter();
                    // double ar = exp(-1*0.019*at), ag = exp(-1*0.010*at), ab = exp(-1*0.019*at)
                    // k = Color(ar, ag, ab);

                    Ray ray_refract(ray.getPosition(), t);

                    double r_i = pow((eta - 1.0), 2)/pow((eta + 1.0), 2);
                    double r_f = r_i + (1 - r_i)*pow((1-c), 5);		//r_f refers to reflecting factor
                    
					return k*(1 - r_f)*shade_ray(ray_refract, count+1) + k*r_f*shade_ray(ray_reflect, count+1); 
                }
                else{

                    if(!root_posi(d, -1 * n, 1.0/eta)){

						Vector3D t = dielec(d, -1 * n, 1.0/eta);
                        c = dotProduct(t, n);
                        Ray ray_refract(ray.getPosition(), t);
						
                        double r_i = pow((eta - 1.0), 2)/pow(eta + 1.0, 2);
                        double r_f = r_i + (1 - r_i)*pow((1-c), 5);
                        return k*(1 - r_f)*shade_ray(ray_refract, count + 1) + k*r_f*shade_ray(ray_reflect, count + 1); 
                    }
                    else{
                        return k*shade_ray(ray_reflect, count+1);
                    }
                }
            }
        }
        return (ray.intersected()->shade(ray));
    }
	return background;
}
*/
