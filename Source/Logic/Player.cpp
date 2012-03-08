#include "Player.hpp"

namespace Logic
{
	Player::Player()
		: mTeam(0)
		, mMarker(0)
	{

	}

	Player::~Player() throw()
	{
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
}