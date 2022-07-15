#pragma once
#include "GameEnums.hpp"

class Controller
{
public:
	Controller();
	~Controller();

	inline const PlayTeam GetTeam() noexcept { return m_team; }

	virtual bool MakeAMove() = 0;

protected:
	PlayTeam m_team;
};

class Player : public Controller
{
public:
	Player();
	~Player();

	bool MakeAMove() override;
};

class AI : public Controller
{
public:
	AI();
	~AI();

	bool MakeAMove() override;
};