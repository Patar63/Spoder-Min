#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include <string>
#include "BackEnd.h"

#define TOPDOWN

enum AnimEnums
{
	IDLELEFT,
	IDLERIGHT,

	//Only in Top down
#ifdef TOPDOWN
	IDLEUP,
	IDLEDOWN,
#endif

	WALKLEFT,
	WALKRIGHT,

	//Only in Top down
#ifdef TOPDOWN
	WALKUP,
	WALKDOWN,
#endif

	ATTACKLEFT,
	ATTACKRIGHT,

	//Only in Top down
#ifdef TOPDOWN
	ATTACKUP,
	ATTACKDOWN
#endif
};

enum AnimTypes
{
#ifdef TOPDOWN
	IDLE = 0,
	WALK = 4,
	ATTACK = 8
#endif
#ifndef TOPDOWN
	IDLE = 0,
	WALK = 2,
	ATTACK = 4
#endif
};

enum AnimDir
{
	LEFT,
	RIGHT,
	//Only in Top Down
#ifdef TOPDOWN
	UP,
	DOWN
#endif
};

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height,
		Sprite* sprite, AnimationController* controller, Transform* transform, bool hasPhys = false, PhysicsBody* body = nullptr);

	void AnimationUpdate();

	void Update() override;

	void GUI() override;

	void GUIWindowUI();
	void GUIWindowOne();
	void GUIWindowTwo();
	void SwingMechanic();
	std::vector<float>xPos;
	std::vector<float>yPos;
	

	void MouseClick(SDL_MouseButtonEvent evnt) override;
	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
	bool swing = false;
	COORD MousePosition{ 1,0 };
	COORD PlayerPosition{ 0,0 };

private:
	void SetActiveAnimation(int anim);

	//Basically, any animation OTHER than moving will not have a cancel, and we'll be checking whether or not that animation is done
	bool m_moving = false;
	//Are you currently attacking?????
	bool m_attacking = false;
	//Have we locked the player from moving during this animation?
	bool m_locked = false;

	//A reference to our sprite
	Sprite* m_sprite = nullptr;
	//A reference to our animation controller
	AnimationController* m_animController = nullptr;
	//A reference to our player transform
	Transform* m_transform = nullptr;

	//Physics importance
	//A reference to our physics body
	PhysicsBody* m_physBody = nullptr;
	//Does this player have physics?
	bool m_hasPhysics = false;

	//Default animation direction (feel free to change this to suit your game. If you're making a side-scroller, left or right would be better
	AnimDir m_facing = LEFT;

protected:
	bool m_firstWindow = false;
	bool m_secondWindow = false;
	
	std::string m_fileInput;

	PhysicsPlaygroundListener listener;

	int puzzleWall1;
	int puzzleWall2;
};
