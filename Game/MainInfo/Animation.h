#pragma once
#include <vector>
#include <string>
#include "AnimFrames.h"

class Animation
{
public:
	Animation(void);
	~Animation(void);

	// Accessors & Mutators
	std::string		GetAnimationName	(void);
	int						GetAnimImageID			(void);
	bool					GetIsLooping					(void);

	void					SetAnimationName	(std::string name);
	void					SetAnimImageID			(int ID);
	void					SetIsLooping					(bool looping);

	void					AddFrame						(AnimFrames frame);
	int						GetNumFrames			(void);
	AnimFrames	GetCurrFrame				(int currFrame);
	std::vector<AnimFrames>		GetAnimationFrames	(void);

	void					ClearAllFrames();

private:
	std::string animationName;
	std::vector<AnimFrames> animationFrames;
	bool isLooping;
	int animImageID;
};

