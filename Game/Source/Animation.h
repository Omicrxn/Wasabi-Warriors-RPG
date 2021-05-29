#pragma once
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 100
class Animation
{
public:
	bool loop = true;
	bool flow = false;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES]; //array of sdl_rect to store each sprite of the animation

private:
	float currentFrame = 0.0f;
	int lastFrame = 0;
	int loops = 0;
	enum class FLOW 
	{
		FORWARDS,
		BACKWARDS
	} direction = FLOW::FORWARDS;

public:
	Animation() {}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), lastFrame(anim.lastFrame) 
	{ // the colon is used to inherit or in this case to initialize variables before the constructor is called
		SDL_memcpy(&frames, anim.frames, sizeof(frames));//copies the info in anim.frames to local variable frames
	}

	void PushBack(const SDL_Rect& rect) //stores the rect into frames array
	{
		frames[lastFrame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		switch (direction)
		{
		case FLOW::FORWARDS:
		{
			currentFrame += speed;
			if (currentFrame >= lastFrame)
			{
				currentFrame = (loop || flow) ? 0.0f : lastFrame - 1;
				direction = flow ? FLOW::BACKWARDS : FLOW::FORWARDS;
				loops++;
			}
		}
		break;
		case FLOW::BACKWARDS:
		{
			currentFrame -= speed;
			if (currentFrame <= 0.0f)
			{
				currentFrame = 0.0f;
				direction = FLOW::FORWARDS;
				loops++;
			}
		}
		break;
		}

		return frames[(int)currentFrame];
	}

	SDL_Rect& GetFrame(int frameNumber) 
	{
		return frames[frameNumber];
	}
	bool Finished() const
	{
		return loops > 0;
	}
	void Update()
	{
		currentFrame += speed;
		if (currentFrame >= lastFrame)
		{
			currentFrame = (loop) ? 0.0f : lastFrame - 1;
			++loops;
		}
	}
	void Reset()
	{
		loops = 0;
		currentFrame = 0.0f;
	}

};

#endif