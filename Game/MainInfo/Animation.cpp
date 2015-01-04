#include "Animation.h"


Animation::Animation(void)
{

}


Animation::~Animation(void)
{
}

#pragma region Accessors & Mutators
// Accessors & Mutators
std::string		Animation::GetAnimationName	(void)
{
	return this->animationName;
}

void					Animation::SetAnimImageID			(int ID)
{
	this->animImageID = ID;
}

bool					Animation::GetIsLooping					(void)
{
	return this->isLooping;
}


int						Animation::GetAnimImageID			(void)
{
	return this->animImageID;
}

void					Animation::SetAnimationName	(std::string name)
{
	this->animationName = name;
}

void					Animation::SetIsLooping					(bool looping)
{
	this->isLooping = looping;
}
#pragma endregion

void					Animation::AddFrame						(AnimFrames frame)
{
	this->animationFrames.push_back(frame);
}

int	Animation::GetNumFrames			(void)
{
	return this->animationFrames.size();
}


AnimFrames	Animation::GetCurrFrame				(int currFrame)
{
	return this->animationFrames[currFrame];
}

	std::vector<AnimFrames>		Animation::GetAnimationFrames	(void)
	{
		return this->animationFrames;
	}

	void		Animation::ClearAllFrames()
	{
		if (animationFrames.size() > 0)
		{
			while (animationFrames.size() != 0)
			{
				animationFrames.erase(animationFrames.begin());
			}
		}
	}


