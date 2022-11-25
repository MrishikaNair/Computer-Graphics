#ifndef _POINTLIGHTSOURCE_H_
#define _POINTLIGHTSOURCE_H_

#include "lightsource.h"

class PointLightSource : public LightSource
{
private:
	Vector3D position;
public:
	PointLightSource(World *_wld, const Vector3D& _pos, const Color& _intensity):
		LightSource(_wld, _intensity), position(_pos) {}
	Vector3D getPosition() const 
	{
		return position;
	}
};
#endif
