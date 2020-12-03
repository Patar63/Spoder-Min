#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include <string>

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	void GUI() override;

	void GUIWindowUI();
	void GUIWindowOne();
	void GUIWindowTwo();
	void SwingMechanic();
	std::vector<float>xPos;
	std::vector<float>yPos;
	

	void MouseClick(SDL_MouseButtonEvent evnt)override;
	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
	bool swing = false;
	COORD MousePosition{ 1,0 };
	COORD PlayerPosition{ 0,0 };
protected:
	bool m_firstWindow = false;
	bool m_secondWindow = false;
	
	std::string m_fileInput;

	PhysicsPlaygroundListener listener;

	int puzzleWall1;
	int puzzleWall2;
};
