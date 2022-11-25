//triangle.h
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object.h"
#include "ray.h"
#include "vector3D.h"
#include "color.h"

class Triangle : public Object
{
private:
	Vector3D a,b,c;

public:
	Triangle(const Vector3D& a, const Vector3D& b,const Vector3D& c, Material* mat, World *world):
		Object(mat, world), a(a),b(b),c(c)
	{
		isSolid = true;
	}
	
	virtual bool intersect(Ray& r) const;
    //calling the shade method
    virtual Color shade(const Ray& ray) const;
};
#endif