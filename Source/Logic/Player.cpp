#include "Player.hpp"

namespace Logic
{
	Player::Player(const std::string& name, Team team, MarkerType markerType)
		: mName(name)
		, mTeam(team)
		, mMarker(markerType)
	{}

	Player::~Player() throw()
	{}


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