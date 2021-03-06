#include "PhysicsPlayground.h"
#include "Utilities.h"

#include <random>

PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{

	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -60.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
}

void backgroundSpawn(int x)
{
	/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);

	//Set up the components
	std::string fileName = "City.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 800, 400);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(x, 190.f, 0.f));
}

//void PhysicsPlayground::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite,
//	AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
//{
//	//Store references to the components
//	m_sprite = sprite;
//	m_animController = controller;
//	m_transform = transform;
//	m_hasPhysics = hasPhys;
//	if (hasPhys)
//	{
//		m_physBody = body;
//	}
//
//	//Initialize UVs
//	m_animController->InitUVs(fileName);
//
//	//Loads the texture and sets width and height
//	m_sprite->LoadSprite(fileName, width, height, true, m_animController);
//	m_animController->SetVAO(m_sprite->GetVAO());
//	m_animController->SetTextureSize(m_sprite->GetTextureSize());
//
//	//Loads in the animations json file
//	nlohmann::json animations = File::LoadJSON(animationJSON);
//
//	//IDLE ANIMATIONS\\
//	
//	//Idle Left
//	m_animController->AddAnimation(animations["Standing"].get<Animation>());
//	//Idle Right
//	m_animController->AddAnimation(animations["Standing"].get<Animation>());
//
//	//Walk Animations\\
//
//	//WalkLeft
//	m_animController->AddAnimation(animations["Walk"].get<Animation>());
//	//WalkRight
//	m_animController->AddAnimation(animations["Walk"].get<Animation>());
//
//	//Attack Animations\\
//
//	//AttackLeft
//	m_animController->AddAnimation(animations["Jump"].get<Animation>());
//	//AttackRight
//	m_animController->AddAnimation(animations["Jump"].get<Animation>());
//
//	//Set Default Animation
//	m_animController->SetActiveAnim(IDLELEFT);
//
//
//}

void PhysicsPlayground::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	
	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//Generate the background
	{
		int xBackgroundDistanceRight = 400;
		int xBackgroundDistanceLeft = -400;

		for (int i = 0; i < 25; i++) {

			backgroundSpawn(xBackgroundDistanceRight);
			xBackgroundDistanceRight += 800;

		}

		for (int i = 0; i < 25; i++) {

			backgroundSpawn(xBackgroundDistanceLeft);
			xBackgroundDistanceLeft -= 800;

		}
	}

	//Setup score board
	{

		//Creates entity
		auto entity = ECS::CreateEntity();

		int width = BackEnd::GetWindowWidth();
		int height = BackEnd::GetWindowHeight();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Hud>(entity);

		//Set up the components
		std::string fileName = "ScoreDisplay.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));
		ECS::GetComponent<Hud>(entity).offset = vec2(130, 65);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
	}

	//Setup static ground
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "CityStreet.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 15000, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), 
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

	}

	//Link entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Player>(entity);
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);
		//ECS::AttachComponent<AnimationController>(entity);

		//Sets up the components
		std::string fileName = "SM-Stand.png";
		//std::string animations = "spidermanAnimations.json";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 15);
		//InitPlayer(fileName, animations, 20, 15, &ECS::GetComponent<Sprite>(entity),
		//	&ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity), true, &ECS::GetComponent<PhysicsBody>(entity));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | HEXAGON, 0.5f, 3.f);
		//std::vector<b2Vec2> points = {b2Vec2(-tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(0.f, tempSpr.GetHeight()/2.f)};
		//tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}
	
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void PhysicsPlayground::Update()
{
	//AnimationUpdate();
	SwingMechanic();
}

void PhysicsPlayground::GUI()
{
	GUIWindowUI();

	if (m_firstWindow)
	{
		GUIWindowOne();
	}

	if (m_secondWindow)
	{
		GUIWindowTwo();
	}
}

void PhysicsPlayground::GUIWindowUI()
{
	ImGui::Begin("Test");

	ImGui::Checkbox("Enable First Window", &m_firstWindow);
	ImGui::SameLine();
	if (ImGui::Button("Toggle Second Window", ImVec2(180.f, 15.f)))
	{
		m_secondWindow = !m_secondWindow;
	}
	
	ImGui::End();
}

void PhysicsPlayground::GUIWindowOne()
{
	//Window begin
	ImGui::Begin("Side Docked Window");
	//is the buffer initialized
	static bool init = false;

	//Buffers
	const unsigned int BUF_LEN = 512;
	static char buf[BUF_LEN];
	//Image load
	static std::string imageLoad = "LinkStandby.png";
	//World gravity slider
	float gravity[2] = { m_physicsWorld->GetGravity().x, m_physicsWorld->GetGravity().y };

	if (!init)
	{
		memset(buf, 0, BUF_LEN);
		init = true;
	}
	m_physicsWorld->SetAllowSleeping(true);

	//ImGui content
	if (ImGui::TreeNode("Vignette Effect"))
	{
		if (EffectManager::GetVignetteHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				VignetteEffect* vig = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
				float innerRad = vig->GetInnerRadius();
				float outerRad = vig->GetOuterRadius();
				float opacity = vig->GetOpacity();

				if (ImGui::SliderFloat("Inner Radius", &innerRad, 0.f, outerRad))
				{
					vig->SetInnerRadius(innerRad);
				}
				if (ImGui::SliderFloat("Outer Radius", &outerRad, innerRad, 1.f))
				{
					vig->SetOuterRadius(outerRad);
				}
				if (ImGui::SliderFloat("Opacity", &opacity, 0.f, 1.f))
				{
					vig->SetOpacity(opacity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetVignetteHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Vignette, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sepia Effect"))
	{
		if (EffectManager::GetSepiaHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				SepiaEffect* sep = (SepiaEffect*)EffectManager::GetEffect(EffectManager::GetSepiaHandle());
				float intensity = sep->GetIntensity();

				if (ImGui::SliderFloat("Inner Radius", &intensity, 0.f, 1.f))
				{
					sep->SetIntensity(intensity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetSepiaHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Sepia, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	ImGui::Separator();
	
	ImGui::Text("Editing World Variables!");
	if (ImGui::SliderFloat2("World Gravity", gravity, -1000.f, 1000.f, "%.2f"))
	{
		m_physicsWorld->SetGravity(b2Vec2(gravity[0], gravity[1]));
		m_physicsWorld->SetAllowSleeping(false);
	}

	ImGui::Separator();

	ImGui::Text("Displaying image dynamically");
	ImGui::InputText("Input filename for image", buf, BUF_LEN, ImGuiInputTextFlags_CharsNoBlank);

	if (ImGui::Button("Confirm File", ImVec2(100.f, 30.f)))
	{
		imageLoad = buf;
	}

	ImGui::Image((void*)(intptr_t)TextureManager::FindTexture(imageLoad)->GetID(), ImVec2(150.f, 150.f), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Separator();
	
	ImGui::Text("Editing Colors");
	ImGui::ColorPicker4("Scene Background Color", &m_clearColor.x);

	ImGui::End();
}

void PhysicsPlayground::GUIWindowTwo()
{
	//Second docked window
	ImGui::Begin("Second Window");
	//Image load
	static std::string imageLoad1 = "FunnyButton.png";
	static std::string imageLoad2 = "LikeButton.png";
	static std::string hahaPressed = "";
	static std::string likePressed = "";

	ImGui::BeginTabBar("Tab Bar Test");

	if (ImGui::BeginTabItem("Tab 1"))
	{
		ImGui::Text("You are within Tab 1");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad1)->GetID(), ImVec2(100.f, 100.f), ImVec2(0,1), ImVec2(1,0)))
		{
			hahaPressed = "You shouldn't have pressed that...";
		}

		ImGui::Text("%s", hahaPressed.c_str());

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Tab 2"))
	{
		ImGui::Text("You are within Tab 2");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad2)->GetID(), ImVec2(100.f, 100.f), ImVec2(0, 1), ImVec2(1, 0)))
		{
			likePressed = "LIKED!!!";
		}

		ImGui::Text("%s", likePressed.c_str());

		ImGui::EndTabItem();
	}
	
	ImGui::EndTabBar();

	ImGui::End();
}



static bool jump = false;
void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	//m_moving = false;
	float speed = 1.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (Input::GetKey(Key::Shift))
	{
		speed *= 2.f;
	}

	if ((Input::GetKey(Key::A)) && (jump == false))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(-120000.f * speed, 0.f), true);
		/*m_facing = LEFT;
		m_moving = true;*/
	}
	if ((Input::GetKey(Key::D)) && (jump == false))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(120000.f * speed, 0.f), true);
		/*m_facing = RIGHT;
		m_moving = true;*/
	} 

	//Change physics body size for circle
	if (Input::GetKey(Key::O))
	{
		player.ScaleBody(1.3 * Timer::deltaTime, 0);
	}
	else if (Input::GetKey(Key::I))
	{
		player.ScaleBody(-1.3 * Timer::deltaTime, 0);
	}
}

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	if (canJump.m_canJump)
	{
		if (Input::GetKeyDown(Key::Space))
		{
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 160000.f), true);
			canJump.m_canJump = false;
		/*	m_moving = false;*/
		}
	}
	if (canJump.m_canJump == false) {
		jump = true;
		//m_moving = true;
	}
	else if ((canJump.m_canJump == true)) {
		jump = false;
		/*m_moving = false;*/

	}

	if (Input::GetKeyDown(Key::Enter))
	{
		MessageBox(NULL, "Placeholder Text", "Game Over", MB_OK | MB_ICONINFORMATION);
	}
}

static float Mx=0.0,My=0.0, Px = 0.0, Py = 0.0;
static 	float score,scoreminus=0;
void PhysicsPlayground::MouseClick(SDL_MouseButtonEvent evnt) {
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	int windowWidth = BackEnd::GetWindowWidth();
	int windowHeight = BackEnd::GetWindowHeight();
	int mainCam = MainEntities::MainCamera();
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		if (swing == false) {
		swing = true;

		vec4 ortho = m_sceneReg->get<Camera>(mainCam).GetOrthoSize();
		vec2 pos = vec2(
			((evnt.x / static_cast<float>(windowHeight) * 2.f * ortho.w) - (ortho.w * static_cast<float>(windowWidth) / static_cast<float>(windowHeight))),
			((-evnt.y / static_cast<float>(windowHeight) * 2.f * ortho.w) + ortho.w));
		pos = pos + vec2(m_sceneReg->get<Camera>(mainCam).GetPositionX(), m_sceneReg->get<Camera>(mainCam).GetPositionY());
		Mx = pos.x;
		My = pos.y;
		Px = m_sceneReg->get<Camera>(mainCam).GetPositionX();
		Py = m_sceneReg->get<Camera>(mainCam).GetPositionY();
		
	}
	}
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		swing = false;
	}
}
void PhysicsPlayground::SwingMechanic()
{

	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	int mainCam = MainEntities::MainCamera();
	Px = m_sceneReg->get<Camera>(mainCam).GetPositionX();
	Py = m_sceneReg->get<Camera>(mainCam).GetPositionY();
	score = m_sceneReg->get<Camera>(mainCam).GetPositionX();
	vec2 diff = vec2(Mx-Px,My-Py);
	float magDiff= sqrt((diff.x * diff.x)+(diff.y*diff.y));
	vec2 Rdiff = diff/magDiff;
	if (swing == true) {
		player.GetBody()->ApplyForceToCenter(b2Vec2(100000.f * Rdiff.x, 100000.f * Rdiff.y), true);
		printf("true");

		/*
		This was used to draw in the webbing for the swing, in place of the animation engine that wouldn't work.
		However a problem occured in which the player vanished the camera stopped working, so it had to be scrapped.
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			int width = BackEnd::GetWindowWidth();
			int height = BackEnd::GetWindowHeight();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<Hud>(entity);

			//Set up the components
			std::string fileName = "Webbing.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, diff.y);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));
			ECS::GetComponent<Hud>(entity).offset = vec2(130, 65);

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;

			b2Body* tempBody;
			b2BodyDef tempDef;
		}*/
	}	
		 if ((canJump.m_canJump == true)) {
			 score = 0.f;
			 scoreminus = m_sceneReg->get<Camera>(mainCam).GetPositionX();
	}
		 if(score / 10 - scoreminus / 10>0){
	printf("( %f)\n", score/10-scoreminus/10 );
		 }
		 else { printf("( %f)\n",0.0); }

}

//void PhysicsPlayground::AnimationUpdate()
//{
//	m_animController = &ECS::GetComponent<AnimationController>(MainEntities::MainPlayer());
//	int activeAnimation = 0;
//
//	if (m_moving)
//	{
//		//Puts it into the WALK category
//		activeAnimation = WALK;
//	}
//	else if (m_attacking)
//	{
//		activeAnimation = ATTACK;
//
//		//Check if the attack animation is done
//		if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
//		{
//			//Will auto set to idle
//			m_locked = false;
//			m_attacking = false;
//			//Resets the attack animation
//			m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();
//
//			activeAnimation = IDLE;
//		}
//	}
//	else
//	{
//		activeAnimation = IDLE;
//	}
//
//	SetActiveAnimation(activeAnimation + (int)m_facing);
//}
//
//void PhysicsPlayground::SetActiveAnimation(int anim)
//{
//	m_animController->SetActiveAnim(anim);
//}

void PhysicsPlayground::KeyboardUp()
{
	

}
