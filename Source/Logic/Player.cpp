#include "Player.hpp"

namespace Logic
{
	Player::Player(const std::string& name, Team team)
		: mName(name)
		, mTeam(team)
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
}