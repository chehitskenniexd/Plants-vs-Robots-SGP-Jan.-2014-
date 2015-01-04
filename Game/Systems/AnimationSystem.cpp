#include "AnimationSystem.h"
#include "../MainInfo/Animation.h"
#include "../MainInfo/AnimFrames.h"
#include "../MainInfo/AnimTimeStamps.h"
#include "../MainInfo/MovingEntity.h"
#include "../States/GameplayState.h"
#include <fstream>
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/tinyxml.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"

/*static*/ AnimationSystem* AnimationSystem::GetInstance(void)
{
	static AnimationSystem s_pInstance;

	return &s_pInstance;
}

AnimationSystem::AnimationSystem(void)
{
	LoadAnimation("Attacks");
	LoadAnimation("CoinFlip");
	LoadAnimation("IdleAnimations");
	LoadAnimation("ConvertAnimations");
	LoadAnimation("Explosion");
	LoadAnimation("Fireworks");
	LoadAnimation("Moving");
	LoadAnimation("Sparkles");
	LoadAnimation("N Idle Ani");
}

AnimationSystem::~AnimationSystem(void)
{
	this->loadedAnimations.clear();
}

Animation	AnimationSystem::GetCurrentAnimation (std::string animName)
{
	return this->loadedAnimations[animName];
}

bool AnimationSystem::LoadAnimation(std::string animation)
{
	// initialize the Animation
	Animation currAnimation;
	AnimFrames currFrame;

	// Create the input and open it if it exists
	TiXmlDocument input;
	std::string fileName = "../Animations/" + animation + ".xml";

	if (input.LoadFile(fileName.c_str()) == false)
		return false;

	// Access the root and check to see that it exists
	TiXmlElement* pRoot = input.RootElement();

	if (pRoot == nullptr)
		return false;

	// Load data from the xml

	// Set the Animation Name
	TiXmlElement* pAnimations = pRoot->FirstChildElement("Animation");
	while (pAnimations!=nullptr)
	{
		std::string animName;
		animName = pAnimations->Attribute("name");
		currAnimation.SetAnimationName(animName);

		// Set the looping flag
		int isLooping;
		pAnimations->Attribute("isLooping", &isLooping);
		if (isLooping == 0)
			currAnimation.SetIsLooping(false);
		else if (isLooping == 1)
			currAnimation.SetIsLooping(true);

		// Set the animation image ID
		TOSTRINGSTREAM animImageID;
		animImageID << "Resources/Images/" << pAnimations->Attribute("animImageID");
		currAnimation.SetAnimImageID
			(
			CSGD_TextureManager::GetInstance()->LoadTexture(animImageID.str().c_str())
			);
		loadedTextures.push_back(currAnimation.GetAnimImageID());

#pragma region Frames

		TiXmlElement* pFrames = pAnimations->FirstChildElement("AnimFrames");
		currAnimation.ClearAllFrames();
		while (pFrames != nullptr)
		{
			// Add Frames to the Animation
			AnimFrames addFrame;

			int anchorX, anchorY;
			double duration;
			pFrames->Attribute("anchorPtX", &anchorX);
			pFrames->Attribute("anchorPtY", &anchorY);
			pFrames->Attribute("frameDuration", &duration);

			addFrame.SetAnchorPtX(anchorX);
			addFrame.SetAnchorPtY(anchorY);
			addFrame.SetFrameDuration((float)duration);

			int rleft, rtop, rright, rbottom;
			pFrames->Attribute("rleft", &rleft);
			pFrames->Attribute("rtop", &rtop);
			pFrames->Attribute("rright", &rright);
			pFrames->Attribute("rbottom", &rbottom);

			addFrame.SetRenderRect(rleft, rtop, rright, rbottom);

			std::string frameEvent;
			frameEvent = pFrames->Attribute("event");
			addFrame.SetCurrentEvent(frameEvent);

			currAnimation.AddFrame(addFrame);

			pFrames = pFrames->NextSiblingElement("AnimFrames");
		}

#pragma endregion

		this->loadedAnimations[currAnimation.GetAnimationName()] = currAnimation;
		pAnimations = pAnimations->NextSiblingElement("Animation");
	}
	return true;
}

void AnimationSystem::Render(int imageID)
{
	RECT toRender;
	toRender.left = (LONG)0.0f;
	toRender.top =  (LONG)0.0f;
	toRender.right =  (LONG)48.0f;
	toRender.bottom =  (LONG)48.0f;

	CSGD_TextureManager::GetInstance()->Draw(
		imageID, 0, 0, 1.0f, 1.0f, &toRender);
}

void AnimationSystem::Render(int posX, int posY, AnimTimeStamps stamp)
{
	CSGD_TextureManager::GetInstance()->Draw(
		this->loadedAnimations[stamp.GetCurrentAnimation()].GetAnimImageID(), 
		posX + this->loadedAnimations[stamp.GetCurrentAnimation()].GetCurrFrame(stamp.GetCurrentFrame()).GetAnchorPtX(),
		posY + this->loadedAnimations[stamp.GetCurrentAnimation()].GetCurrFrame(stamp.GetCurrentFrame()).GetAnchorPtY(),
		1.0f, 1.0f, &loadedAnimations[stamp.GetCurrentAnimation()].GetCurrFrame(stamp.GetCurrentFrame()).GetRenderRect()
		);
}

void AnimationSystem::Render(CEntity* pTarget, AnimTimeStamps stamp)
{
	CSGD_TextureManager::GetInstance()->Draw(
		this->loadedAnimations[stamp.GetCurrentAnimation()].GetAnimImageID(),
		pTarget->GetPosX() + CGameplayState::GetInstance()->OffsetX(), 
		pTarget->GetPosY() + CGameplayState::GetInstance()->OffsetY(), 1.0f, 1.0f,
		&loadedAnimations[stamp.GetCurrentAnimation()].GetCurrFrame(stamp.GetCurrentFrame()).GetRenderRect()
		);
}

void AnimationSystem::Update(float fElapsedTime)
{
	if (stampsToRender.empty() == false)
	{
		for (unsigned int i = 0; i < stampsToRender.size(); ++i)
		{
			stampsToRender[i]->Update(fElapsedTime);
		}
	}
}

void AnimationSystem::Render()
{
	if (stampsToRender.empty() == false && stampsToRender.size() > 0)
	{
		for (unsigned int i = 0; i < stampsToRender.size(); ++i)
		{
			if (stampsToRender[i]->GetCurrentAnimation() == "Plant Attack" 
				|| stampsToRender[i]->GetCurrentAnimation() == "Robot Attack")
			{
				if (stampsToRender[i]->GetOwningEntity() != nullptr)
					Render(dynamic_cast<CMovingEntity*>(stampsToRender[i]->GetOwningEntity())->GetTargetUnit(), *stampsToRender[i]);
			}

			else if (stampsToRender[i]->GetCurrentAnimation() == "Explosion")
			{
				if (stampsToRender[i]->GetOwningEntity() != nullptr)
					Render(dynamic_cast<CEntity*>(stampsToRender[i]->GetOwningEntity()), *stampsToRender[i]);
			}
		}
	}
}

void AnimationSystem::AddAnimTimeStamps(AnimTimeStamps &m_aTimeStamp)
{
	this->stampsToRender.push_back(&m_aTimeStamp);
}

void AnimationSystem::RemoveAnimTimeStamp(int pNum)
{
	stampsToRender.erase(stampsToRender.begin() + pNum);
}