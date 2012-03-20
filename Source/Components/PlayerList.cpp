#include "PlayerList.hpp"


namespace Components
{
	PlayerItem::PlayerItem(const Logic::Session* session, Logic::PlayerID playerID, ComponentGroup* ownerGroup, const RECT& position)
		: Button(ownerGroup, position)
		, mSession(session)
		, mPlayerID(playerID)
		, mFont(NULL)
		, mHighlightBackground(NULL)
	{
	
	}
	
	PlayerItem::~PlayerItem() throw()
	{
		SafeDelete(mHighlightBackground);
		SafeDelete(mFont);
	}
	

	void PlayerItem::Initialize(ID3D10Device* device)
	{
		Graphics buttonGraphics;
		buttonGraphics.activeColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		buttonGraphics.idleColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		buttonGraphics.hoverColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
		buttonGraphics.textureUp = new Sprite(device, sViewport, "buttonBase.png", GetWidth(), GetHeight());

		Button::Initialize(device, buttonGraphics);
		mHighlightBackground = new Sprite(device, sViewport, "whitePixel.png", GetWidth(), GetHeight());

		int textSize = GetHeight() / 2;
		mFont = new GameFont(device, "Jing Jing", textSize, false, true);
	}
	
	void PlayerItem::Draw()
	{
		if(!IsVisible())
			return;

		// Render background
		if (mSession->GetCurrentPlayer() == mPlayerID &&
			(!mSession->HasOpenSlot() || mSession->GetWinner() != Logic::C_PLAYER_NONE) )
		{
			mHighlightBackground->Draw(GetPosition(), D3DXCOLOR(0.3f, 0.0f, 0.3f, 0.5f));
		}
		else
		{
			Button::Draw();
		}

		// Render text
		std::string caption = mSession->GetPlayerName(mPlayerID);
		D3DXVECTOR2 pos = GetPosition();
		RECT r = { pos.x, pos.y, pos.x + GetWidth(), pos.y + GetHeight() };
		if(IsEnabled())
		{
			if(IsHovered())
				mFont->WriteText(caption, &r, D3DXCOLOR(1.0, 1.0, 0.0, 1.0), GameFont::Center, GameFont::Middle);
			else
				mFont->WriteText(caption, &r, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), GameFont::Center, GameFont::Middle);
		}
		else
			mFont->WriteText(caption, &r, D3DXCOLOR(0.5, 0.5, 0.5, 1.0), GameFont::Center, GameFont::Middle);
	}
	

	std::string PlayerItem::GetName()
	{
		return "Player: " + mSession->GetPlayerName(mPlayerID);
	}

	void PlayerItem::Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{

	}
	





	PlayerList::PlayerList(const Logic::Session* session, ID3D10Device* device, ComponentGroup* ownerGroup, const RECT& position, int itemWidth, int itemHeight)
		: ComponentGroup(ownerGroup, "PlayerList", position)
	{
		for (unsigned int i = 0; i < session->GetPlayerCount(); ++i)
		{
			RECT r;
			r.left = 0;
			r.right = r.left + itemWidth;
			r.top = itemHeight * i;
			r.bottom = r.top + itemHeight;

			PlayerItem* item = new PlayerItem(session, static_cast<Logic::PlayerID>(i), this, r);
			item->Initialize(device);

			//mComponents.push_back(item);
			mPlayerItems.push_back(item);
		}
	}

	PlayerList::~PlayerList() throw()
	{

	}

	bool PlayerList::GetAndResetClickStatus(Logic::PlayerID playerID)
	{
		return mPlayerItems[playerID]->GetAndResetClickStatus();
	}

}