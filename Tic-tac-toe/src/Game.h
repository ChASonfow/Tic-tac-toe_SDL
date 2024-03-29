#pragma once
#include "SDL_Handler.h"
#include "RectObject.hpp"

class Game : public SDL_Handler
{
public:
	Game(const std::string& title, const Uint16& width, const Uint16& height);
	virtual ~Game();

	virtual void Run() override;

protected:
	virtual void Clear() override;

	virtual bool Init() override;
	virtual bool InitUI();
	virtual bool InitLabels();

	SDL_Texture* CreateTextTexture(const std::string& text, const SDL_Color& textColor);
	SDL_Texture* CreateTextTexture(const LabelObject& obj, const SDL_Color& textColor);

	virtual void Update();
	virtual void Render();

	virtual void HandleEvents(const SDL_Event& ev);
	virtual void HandleMouseEvents(const SDL_Event& ev);
	virtual void HandleKeyboardEvents(const SDL_Event& ev);

private:
	bool InitPlayfield();
	void InitPlayers();
	
	void ClearPlayers();

	void AIMove();
	void CurrentPlayerMove(FieldObject& tile);
	void ChangeCurrentPlayer();

	void RenderUI();
	void RenderLabels();
	void RenderField();
	
#pragma region Templates
	template<Iterable T>
	int16_t MiniMax(T& pfCopy, bool isMaximazingPlayer, PlayTeam team, Uint8 depth, int16_t alpha = INT16_MIN, int16_t beta = INT16_MAX)
	{
		const GameResult pre_result = CheckWinConditions(pfCopy, team);
		if (!depth || pre_result != GameResult::NONE)
		{
			return (pre_result == GameResult::DRAW || pre_result == GameResult::NONE)
				? 0
				: pre_result == static_cast<GameResult>(m_currentPlayer->GetTeam())
					?  10 + depth
					: -10 - depth;
		}

		const PlayTeam oppositeTeam = team == PlayTeam::SQUARES
			? PlayTeam::CIRCLES
			: PlayTeam::SQUARES;
		int16_t bestEval = isMaximazingPlayer ? INT16_MIN : INT16_MAX;
		for (Uint8 i = 0; i < pfCopy.size(); ++i)
		{
			if (pfCopy.at(i).markedBy == PlayTeam::NONE)
			{
				auto pfDeeperCopy = pfCopy;
				pfDeeperCopy.at(i).markedBy = team;
				const int16_t eval = MiniMax(pfDeeperCopy, !isMaximazingPlayer, oppositeTeam, depth - 1, alpha, beta);
				bestEval = isMaximazingPlayer 
					? std::max(bestEval, eval)
					: std::min(bestEval, eval);
				if (isMaximazingPlayer)
				{
					alpha = std::max(alpha, eval);
				}
				else
				{
					beta = std::min(beta, eval);
				}
				if (beta <= alpha)
					break;
			}
		}
		return bestEval;
	}

	template<Iterable T>
	GameResult CheckWinConditions(T& playfield, PlayTeam team)
	{
		if (m_state == GameState::GAMEPLAY)
		{
			const Uint8 lineSize = static_cast<Uint8>(m_mode) & static_cast<Uint8>(GameMode::SMALL_FIELD) ? 3 : 5;
			const Uint8 fieldSize = playfield.size();
			const PlayTeam oppositeTeam = team == PlayTeam::SQUARES
				? PlayTeam::CIRCLES
				: PlayTeam::SQUARES;

			Uint8 teamHorizontalCount = 0,	teamVerticalCount = 0
				, teamDiagonalCount = 0,	teamAntidiagonalCount = 0
				, emptyTilesCount = 0;
			Uint8 opTeamHorizontalCount = 0,	opTeamVerticalCount = 0
				, opTeamDiagonalCount = 0,		opTeamAntidiagonalCount = 0;
			for (Uint16 i = 0, row = 0; i < fieldSize && row < lineSize; ++row)
			{
				teamHorizontalCount = 0;
				teamVerticalCount = 0;

				opTeamHorizontalCount = 0;
				opTeamVerticalCount = 0;
				for (Uint16 col = 0; col < lineSize; ++i, ++col)
				{
					teamHorizontalCount		+= playfield.at(i).markedBy == team;
					teamVerticalCount		+= playfield.at(row + col * lineSize).markedBy == team;

					opTeamHorizontalCount	+= playfield.at(i).markedBy == oppositeTeam;
					opTeamVerticalCount		+= playfield.at(row + col * lineSize).markedBy == oppositeTeam;

					emptyTilesCount		+= playfield.at(i).markedBy == PlayTeam::NONE;
				}
				teamDiagonalCount		+= playfield.at(row * (lineSize + 1)).markedBy == team;
				teamAntidiagonalCount	+= playfield.at((row + 1) * lineSize - row - 1).markedBy == team;

				opTeamDiagonalCount		+= playfield.at(row * (lineSize + 1)).markedBy == oppositeTeam;
				opTeamAntidiagonalCount += playfield.at((row + 1) * lineSize - row - 1).markedBy == oppositeTeam;

				if (	teamHorizontalCount == lineSize	|| teamVerticalCount == lineSize
					||	teamDiagonalCount == lineSize	|| teamAntidiagonalCount == lineSize)
				{
					return static_cast<GameResult>(team);
				}
				if (	opTeamHorizontalCount == lineSize	|| opTeamVerticalCount == lineSize
					||	opTeamDiagonalCount == lineSize		|| opTeamAntidiagonalCount == lineSize)
				{
					return static_cast<GameResult>(oppositeTeam);
				}
			}
			if (!emptyTilesCount)
			{
				return GameResult::DRAW;
			}
		}

		return GameResult::NONE;
	}

	template<Iterable T>
	T::iterator FindClickedRectObject(T& container, const Sint32& x, const Sint32& y)
	{
		for (auto it = container.begin(); it < container.end(); ++it)
		{
			if ((*it).IsClicked(x, y) && (*it).active)
			{
				return it;
			}
		}
		return container.end();
	}
#pragma endregion

private:
	TTF_Font* m_font;
	SDL_Texture* m_squareTex;
	SDL_Texture* m_circleTex;

	Controller* m_currentPlayer;
	std::array<Controller*, 2> m_players;

	SDL_Rect m_bgRect;
	SDL_Color m_bgColor;

	GameState m_state;
	GameMode m_mode;
	GameResult m_result;

	std::vector<UIObject> m_uiButtons;
	std::vector<LabelObject> m_labels;
	std::vector<FieldObject> m_playfield;
};
