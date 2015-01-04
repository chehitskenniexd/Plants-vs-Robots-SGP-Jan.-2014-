#include "AnimFrames.h"


AnimFrames::AnimFrames(void)
{
}


AnimFrames::~AnimFrames(void)
{
}

// Accessors and Mutators
int			AnimFrames::GetAnchorPtX			(void)
{
	return this->anchorPtX;
}

int			AnimFrames::GetAnchorPtY			(void)
{
	return this->anchorPtY;
}

float		AnimFrames::GetFrameDuration	(void)
{
	return this->frameDuration;
}

RECT		AnimFrames::GetRenderRect			(void)
{
	return this->renderRect;
}

std::string		AnimFrames::GetCurrentEvent(void)
{
	return this->currentEvent;
}

void		AnimFrames::SetAnchorPtX			(int anchorPtX)
{
	this->anchorPtX = anchorPtX;
}

void		AnimFrames::SetAnchorPtY			(int anchorPtY)
{
	this->anchorPtY = anchorPtY;
}

void		AnimFrames::SetFrameDuration	(float fDuration)
{
	this->frameDuration = fDuration;
}

void		AnimFrames::SetRenderRect			(int left, int top, int right, int bottom)
{
	this->renderRect.left = (LONG)left;
	this->renderRect.top = (LONG)top;
	this->renderRect.right = (LONG)right;
	this->renderRect.bottom = (LONG)bottom;
}

void		AnimFrames::SetRenderRect			(RECT& rect)
{
	this->renderRect = rect;
}

void		AnimFrames::SetCurrentEvent			(std::string currEvent)
{
	this->currentEvent = currEvent;
}
