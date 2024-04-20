#pragma once
class GameConstants
{
public:
	static const int BOARD_SIZE = 231;
	static const int BOARD_ROW = 21;
	static const int BOARD_COL = 11;

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

