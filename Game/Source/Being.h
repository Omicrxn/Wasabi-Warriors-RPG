#pragma once
#ifndef __BEING_H__
#define __BEING_H__

#include "Entity.h"

class Being : public Entity
{
public:
	Being();
	~Being();
private:
	fPoint velocity = fPoint(0.0f, 0.0f);
};
#endif //__BEING_H__