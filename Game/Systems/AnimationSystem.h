#pragma once
#include <vector>
#include <map>
#include "../MainInfo/Animation.h"
#include "../MainInfo/AnimTimeStamps.h"
#include "../MainInfo/Entity.h"

class AnimationSystem
{
public:
	//Singleton
	static AnimationSystem* GetInstance(void);

	bool LoadAnimation(std::string animation);
	void Render(int posX, int posY, AnimTimeStamps stamp);
	void Render(CEntity* pTarget, AnimTimeStamps stamp);
	void Render(int imageID);
	void Update(float fElapsedTime);
	void Render();
	void AddAnimTimeStamps(AnimTimeStamps &m_aTimeStamp);
	void RemoveAnimTimeStamp(int pNum);
	Animation	GetCurrentAnimation (std::string animName);
	std::vector<int>	GetLoadedTextures() {return this->loadedTextures;}
	std::vector<AnimTimeStamps*> GetStampsToRender() {return this->stampsToRender;}
	int	GetLoadedTexture(int texture) {return this->loadedTextures[texture];}

private:
	// Singleton
	AnimationSystem(void);
	~AnimationSystem(void);
	AnimationSystem& operator=(const AnimationSystem&);

	std::map<std::string, Animation> loadedAnimations;
	std::vector<AnimTimeStamps*> stampsToRender;
	std::vector <int> loadedTextures;
};

