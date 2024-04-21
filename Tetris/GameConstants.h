#pragma once

namespace GameConstants
{
	constexpr int BOARD_SIZE = 264;
	constexpr int BOARD_ROW = 22;
	constexpr int BOARD_COL = 12;
	constexpr float TILE_DOWN_TIME = 1.0f;
	constexpr float TILE_SIZE = 32.0f;
	constexpr float START_TIME_MOVE_TILE = 0.7f;
	constexpr float KEY_PRESSED_INTERVAL_TIME = 0.6f;
	constexpr float CAN_MOVE_TIME = 0.7f;
	constexpr float PI = M_PI;	

	enum class Layer
	{
		Background,
		Tile,
		Score,
		Board,

		UNKNOWN
	};

	enum class ShapeTile
	{
		BLANK,
		WALL,
		I_SHAPE,
		J_SHAPE,
		L_SHAPE,
		O_SHAPE,
		S_SHAPE,
		T_SHAPE,
		Z_SHAPE,

		UNKNOWN
	};
};