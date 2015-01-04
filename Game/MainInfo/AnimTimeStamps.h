#pragma once
#include <string>
#include "../SGD Wrappers/IListener.h"

class CEntity;
class CMovingEntity;

class AnimTimeStamps: 
	public IListener
{
public:
	AnimTimeStamps(void);
	~AnimTimeStamps(void);

	// Accessors and Mutators
	std::string		GetCurrentAnimation (void);
	int						GetCurrentFrame(void);
	float					GetTimeOnFrame(void);
	CEntity*			GetOwningEntity (void) {return ownerEntity;}

	void					SetCurrentAnimation	(std::string animation);
	void					SetCurrentFrame			(int frame);
	void					SetTimeOnFrame		(float time);
	void					SetHasPlayed				(bool bPlayed) {hasPlayed = bPlayed;}
	void					SetOwnerEntity			(CEntity* m_eOwner) {ownerEntity = m_eOwner;}

	void					Update (float fElapsedTime);
	void					Update (float fElapsedTime, bool &m_bUpdate, CEntity* pTarget);
	void					Update (float fElapsedTime, bool &m_bUpdate);
	void					Update (float fElapsedTime, bool &m_bUpdate, bool &m_bOther);

	virtual void		HandleEvent (const CEvent* pEvent) override;

private:
	std::string currentAnimation;
	int currentFrame;
	float timeOnFrame;
	bool hasPlayed;
	CEntity* ownerEntity;
};

