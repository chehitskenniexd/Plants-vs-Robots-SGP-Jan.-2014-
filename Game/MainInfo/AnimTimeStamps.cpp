#include "AnimTimeStamps.h"
#include "../Systems/AnimationSystem.h"
#include "../SGD Wrappers/CSGD_EventSystem.h"
#include "../SGD Wrappers/CSGD_MessageSystem.h"
#include "../States/GameplayState.h"
#include "../MainInfo/UnitConvertTile.h"
#include "../MainInfo/DestroyEntityMessage.h"
#include "../MainInfo/UnitMoveMessage.h"
#include "../MainInfo/MovingEntity.h"


AnimTimeStamps::AnimTimeStamps(void)
{
	this->currentFrame=0;
	this->timeOnFrame = 0.0f;
	this->hasPlayed = false;
	//CSGD_EventSystem::GetInstance()->RegisterClient("CREATE_RECTANGLE", this);
}


AnimTimeStamps::~AnimTimeStamps(void)
{
	this->ownerEntity = nullptr;
}

void	AnimTimeStamps::Update(float fElapsedTime)
{
	this->timeOnFrame += fElapsedTime;

	AnimFrames currFrame;
	Animation currAnim;

	if (this->currentAnimation.length() != 0)
	{
		currFrame = AnimationSystem::GetInstance()->GetCurrentAnimation(this->currentAnimation).GetCurrFrame(this->currentFrame);
		currAnim = AnimationSystem::GetInstance()->GetCurrentAnimation(this->currentAnimation);
	}

	if ((this->timeOnFrame > currFrame.GetFrameDuration() && this->hasPlayed == false)
		||(this->timeOnFrame > currFrame.GetFrameDuration() && this->hasPlayed == true && currAnim.GetIsLooping() == true))
	{
		this->currentFrame += 1;
		this->timeOnFrame = 0;
	}

	if (this->currentFrame == AnimationSystem::GetInstance()->GetCurrentAnimation(this->currentAnimation).GetNumFrames() )
	{
		this->currentFrame = 0;
		this->hasPlayed = true;
	}

	if (currFrame.GetCurrentEvent() == "UNIT_CONVERT")
	{
		CSGD_MessageSystem::GetInstance()->QueueMessage(new CUnitConvertTileMessage(
			this->ownerEntity, dynamic_cast<CMovingEntity*>(this->ownerEntity)->GetTargetTile(),
			CGameplayState::GetInstance()->GetGameMap()));

		CGameplayState::GetInstance()->Deselect();
		this->currentFrame = 0;
		this->ownerEntity->SetCurrentAnimStamp(ownerEntity->GetIdleAnimationStamps());
	}

	if (currFrame.GetCurrentEvent() == "ENTITY_DESTROY")
	{
		CSGD_MessageSystem::GetInstance()->QueueMessage(new CDestroyEntityMessage(
			this->ownerEntity));

		for (unsigned int i = 0; i < AnimationSystem::GetInstance()->GetStampsToRender().size(); ++i)
		{
			if (AnimationSystem::GetInstance()->GetStampsToRender()[i] == this)
			{
				AnimationSystem::GetInstance()->RemoveAnimTimeStamp(i);
			}
		}

		CGameplayState::GetInstance()->Deselect();
		this->currentFrame = 0;
		this->ownerEntity->SetCurrentAnimStamp(ownerEntity->GetIdleAnimationStamps());
	}

	if (currFrame.GetCurrentEvent() == "UNIT_MOVE")
	{
		CSGD_MessageSystem::GetInstance()->QueueMessage(new CUnitMoveMessage(
			this->ownerEntity, dynamic_cast<CMovingEntity*>(this->ownerEntity)->GetTargetTile(),
			dynamic_cast<CMovingEntity*>(this->ownerEntity)->GetPreviousTile()));

		CGameplayState::GetInstance()->Deselect();
		this->currentFrame = 0;
		this->ownerEntity->SetCurrentAnimStamp(ownerEntity->GetIdleAnimationStamps());
	}

	if (currFrame.GetCurrentEvent() == "END_APDEC")
	{
		this->currentFrame = 0;
		CGameplayState::GetInstance()->SetAPDef(false);
	}

	if (this->hasPlayed == true && currAnim.GetIsLooping() == false 
		&& (currFrame.GetCurrentEvent() == "None" || currFrame.GetCurrentEvent() == "Destroy"))
	{
		for (unsigned int i = 0; i < AnimationSystem::GetInstance()->GetStampsToRender().size(); ++i)
		{
			if (AnimationSystem::GetInstance()->GetStampsToRender()[i] == this)
			{
				AnimationSystem::GetInstance()->RemoveAnimTimeStamp(i);
			}
		}
	}
}

void	AnimTimeStamps::Update (float fElapsedTime, bool &m_bUpdate, CEntity* pTarget)
{
	this->timeOnFrame += fElapsedTime;

	AnimFrames currFrame = AnimationSystem::GetInstance()->GetCurrentAnimation(this->currentAnimation).GetCurrFrame(this->currentFrame);
	if (this->timeOnFrame > currFrame.GetFrameDuration())
	{
		this->currentFrame += 1;
		this->timeOnFrame = 0.0f;
	}

	if (this->currentFrame == AnimationSystem::GetInstance()->GetCurrentAnimation(this->currentAnimation).GetNumFrames())
	{
		this->currentFrame = 0;
		this->timeOnFrame = 0.0f;
		m_bUpdate = false;
		pTarget = nullptr;
	}

	CSGD_EventSystem::GetInstance()->QueueEvent(AnimationSystem::GetInstance()->GetCurrentAnimation(this->currentAnimation).GetCurrFrame(this->currentFrame).GetCurrentEvent().c_str());
}

void AnimTimeStamps::Update (float fElapsedTime, bool &m_bUpdate)
{
	this->timeOnFrame += fElapsedTime;

	AnimFrames currFrame = AnimationSystem::GetInstance()->GetCurrentAnimation(this->currentAnimation).GetCurrFrame(this->currentFrame);
	if (this->timeOnFrame > currFrame.GetFrameDuration())
	{
		this->currentFrame += 1;
		this->timeOnFrame = 0.0f;
	}

	if (this->currentFrame == AnimationSystem::GetInstance()->GetCurrentAnimation(this->currentAnimation).GetNumFrames())
	{
		this->currentFrame = 0;
		this->timeOnFrame = 0.0f;
		m_bUpdate = false;
	}

	CSGD_EventSystem::GetInstance()->QueueEvent(AnimationSystem::GetInstance()->GetCurrentAnimation(this->currentAnimation).GetCurrFrame(this->currentFrame).GetCurrentEvent().c_str());
}

void	AnimTimeStamps::Update (float fElapsedTime, bool &m_bUpdate,  bool &m_bOther)
{
	this->timeOnFrame += fElapsedTime;

	AnimFrames currFrame = AnimationSystem::GetInstance()->GetCurrentAnimation(this->currentAnimation).GetCurrFrame(this->currentFrame);
	if (this->timeOnFrame > currFrame.GetFrameDuration())
	{
		this->currentFrame += 1;
		this->timeOnFrame = 0.0f;
	}

	if (this->currentFrame == AnimationSystem::GetInstance()->GetCurrentAnimation(this->currentAnimation).GetNumFrames())
	{
		this->currentFrame = 0;
		this->timeOnFrame = 0.0f;
		m_bUpdate = false;
		m_bOther = false;
	}

	CSGD_EventSystem::GetInstance()->QueueEvent(AnimationSystem::GetInstance()->GetCurrentAnimation(this->currentAnimation).GetCurrFrame(this->currentFrame).GetCurrentEvent().c_str());
}

#pragma region Accessors and Mutators
std::string		AnimTimeStamps::GetCurrentAnimation (void)
{
	return this->currentAnimation;
}

int						AnimTimeStamps::GetCurrentFrame(void)
{
	return this->currentFrame;
}

float					AnimTimeStamps::GetTimeOnFrame(void)
{
	return this->timeOnFrame;
}

void					AnimTimeStamps::SetCurrentAnimation	(std::string animation)
{
	this->currentAnimation.clear();
	this->currentAnimation = animation;
}

void					AnimTimeStamps::SetCurrentFrame			(int frame)
{
	this->currentFrame = frame;
}
void					AnimTimeStamps::SetTimeOnFrame		(float time)
{
	this->timeOnFrame = time;
}
#pragma endregion

void		AnimTimeStamps::HandleEvent (const CEvent* pEvent) 
{
	if (pEvent->GetEventID() == "CREATE_RECTANGLE")
	{
		CGameplayState::GetInstance()->drawTestRect = true;
	}
}
