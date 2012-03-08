#include "Menu.hpp"

namespace Components
{
	Menu::Menu(ID3D10Device* device, ComponentGroup* ownerGroup, RECT position)
		: Component(ownerGroup, position),
		  mDevice(device)
	{
		//Button::Graphics graphQuestion, graphWarning, graphHint, graphOther;
		//D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/question.png", NULL, NULL, 
		//									   &graphQuestion.textureUp, NULL);
		//D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/questionL.png", NULL, NULL, 
		//									   &graphQuestion.textureDown, NULL);

		//D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/exclamation.png", NULL, NULL, 
		//									   &graphWarning.textureUp, NULL);
		//D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/exclamationL.png", NULL, NULL, 
		//									   &graphWarning.textureDown, NULL);

		//D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/hint.png", NULL, NULL, 
		//									   &graphHint.textureUp, NULL);
		//D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/hintL.png", NULL, NULL, 
		//									   &graphHint.textureDown, NULL);

		//D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/other.png", NULL, NULL, 
		//									   &graphOther.textureUp, NULL);
		//D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/otherL.png", NULL, NULL, 
		//									   &graphOther.textureDown, NULL);

		/*mGraphics.push_back(graphQuestion);
		mGraphics.push_back(graphWarning);
		mGraphics.push_back(graphHint);
		mGraphics.push_back(graphOther);

		UpdateButtonPositions();
		CreateButtons();*/		
	}

	Menu::~Menu() throw()
	{
	}

	void Menu::UpdateButtonPositions()
	{
		mPositions.clear();

		int halfSide = 32;
		RECT posQuestion, posWarning, posHint, posOther;
		posQuestion.left = mPositionRect.right - 32;
		posQuestion.right = mPositionRect.right + 32;
		posQuestion.top = mPositionRect.top - 32;
		posQuestion.bottom = mPositionRect.top + 32;

		posWarning.left = mPositionRect.left - 32;
		posWarning.right = mPositionRect.left + 32;
		posWarning.top = mPositionRect.top - 32;
		posWarning.bottom = mPositionRect.top + 32;

		posHint.left = mPositionRect.left - 32;
		posHint.right = mPositionRect.left + 32;
		posHint.top = mPositionRect.bottom - 32;
		posHint.bottom = mPositionRect.bottom + 32;

		posOther.left = mPositionRect.right - 32;
		posOther.right = mPositionRect.right + 32;
		posOther.top = mPositionRect.bottom - 32;
		posOther.bottom = mPositionRect.bottom + 32;

		mPositions.push_back(posQuestion);
		mPositions.push_back(posWarning);
		mPositions.push_back(posHint);
		mPositions.push_back(posOther);
	}

	void Menu::CreateButtons()
	{
		for(unsigned int i = 0; i < mButtons.size(); ++i)
		{
			delete mButtons[i];
		}
		mButtons.clear();

		for(int i = 0; i < 4; ++i)
		{
			Components::Button* btn = new Components::Button(mOwner, mPositions[i]);
			btn->Initialize(mDevice, mGraphics[i]);
			
			mButtons.push_back(btn);
		}
	}

	void Menu::SetPosition(RECT newPosition)
	{
		mPositionRect = newPosition;

		UpdateButtonPositions();
		CreateButtons();
	}

	void Menu::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		for(unsigned int i = 0; i < mButtons.size(); ++i)
		{
			mButtons[i]->Update(gameTime, currInputState, prevInputState);
		}
	}

	void Menu::Draw()
	{
		for(unsigned int i = 0; i < mButtons.size(); ++i)
		{
			mButtons[i]->Draw();
		}
	}

	void Menu::LostFocus()
	{
	}

	void Menu::GotFocus()
	{
	}

	// DEBUG
	std::string Menu::GetName()
	{
		return "DragonAge Menu";
	}
}