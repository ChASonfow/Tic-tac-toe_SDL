#pragma once
#include "pch.h"
#include "GameEnums.hpp"

extern class FieldObject;

class Controller
{
public:
	Controller(PlayTeam team)
		: m_team(team), m_isAllowedToMove(false)
	{}
	virtual ~Controller() {};

	inline PlayTeam GetTeam() const noexcept { return m_team; }
	inline void SetAllowanceToMove(bool allowance) noexcept { m_isAllowedToMove = allowance; }
	inline bool IsAllowedToMove() const noexcept { return m_isAllowedToMove; }

protected:
	PlayTeam m_team;
	bool m_isAllowedToMove;
};

class Player : public Controller
{
public:
	Player(PlayTeam team)
		: Controller(team)
	{}
};

class AI : public Controller
{
public:
	AI(PlayTeam team)
		: Controller(team)
	{}
};