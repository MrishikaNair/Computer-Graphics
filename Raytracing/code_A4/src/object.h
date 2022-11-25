//object.h
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "ray.h"
#include "vector3D.h"
#include "color.h"
#include "material.h"
#include "world.h"

class Object
{
protected:
	Material *material;
	World *world;
public:
	bool isSolid;	//changed to public, use in refected part
	Object(Material *mat, World *world): material(mat), world(world) {}	
	Material* getMaterial() const 
	{
		return material;	//material is called, so declaring getMaterial here
	}	
	virtual bool intersect(Ray& rayvec) const = 0;
	virtual Color shade(const Ray& rayvec) const
	{
		return material->shade(rayvec, isSolid);
	}
	
};

#endif
