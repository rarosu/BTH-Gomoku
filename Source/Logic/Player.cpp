#include "Player.hpp"

namespace Logic
{
	Player::Player()
		: mTeam(0)
		, mMarker(0)
		, mController(NULL)
	{

	}

	Player::~Player() throw()
	{
		SafeDelete(mController);
	}


	void Player::SetName(const std::string& name)
	{
		mName = name;
	}

	void Player::SetTeam(Team team)
	{
		mTeam = team;
	}

	void Player::SetMarkerType(MarkerType markerType)
	{
		mMarker = markerType;
	}


	void Player::SetController(PlayerController* controller)
	{
		SafeDelete(mController);
		mController = controller;
	}

	bool Player::MakeMove(Cell& cell) const
	{
		cell.x = 0;
		cell.y = 0;

		if (mController != NULL)
			return mController->MakeMove(cell);

		// If we don't have a controller, we'll never make a move, so just return false.
		return false;
	}

}