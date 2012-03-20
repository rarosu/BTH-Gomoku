#ifndef PLAYER_LIST_HPP
#define PLAYER_LIST_HPP

#include "Component.hpp"
#include "ClickMenu.hpp"
#include "Sprite.hpp"
#include "Player.hpp"
#include "Session.hpp"
#include <vector>

namespace Components
{
	class PlayerItem : public Button
	{
	public:
		PlayerItem(const Logic::Session* session, Logic::PlayerID playerID, ComponentGroup* ownerGroup, const RECT& position);
		~PlayerItem() throw();

		void Initialize(ID3D10Device* device);
		void Draw();

		std::string GetName();

	protected:
		void Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);

	private:
		const Logic::Session*	mSession;
		Logic::PlayerID			mPlayerID;

		static const D3DXCOLOR	sPlayerColors[];

		GameFont* mFont;
		Sprite* mHighlightBackground;
	};




	class PlayerList : public ComponentGroup
	{
	public:
		PlayerList(const Logic::Session* session, ID3D10Device* device, ComponentGroup* ownerGroup, const RECT& position, int itemWidth, int itemHeight);
		~PlayerList() throw();

		bool GetAndResetClickStatus(Logic::PlayerID playerID);
	private:
		std::vector<PlayerItem*> mPlayerItems;
	};
}

#endif