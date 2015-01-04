#pragma once
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include <string>

class AnimFrames
{
public:
	AnimFrames(void);
	~AnimFrames(void);

	// Accessors and Mutators
	int						GetAnchorPtX			(void);
	int						GetAnchorPtY			(void);
	float					GetFrameDuration	(void);
	RECT					GetRenderRect			(void);
	RECT					GetActiveRect			(void);
	std::string		GetCurrentEvent		(void);

	void					SetAnchorPtX			(int anchorPtX);
	void					SetAnchorPtY			(int anchorPtY);
	void					SetFrameDuration	(float fDuration);
	void					SetRenderRect			(int left, int top, int right, int bottom);
	void					SetRenderRect			(RECT& rect);
	void					SetCurrentEvent		(std::string currEvent);


private:
	int anchorPtX, anchorPtY;
	float frameDuration;
	RECT renderRect;
	std::string currentEvent;
};

