#pragma once
class GameConstants
{
public:
	static const int BOARD_SIZE = 200;

	enum class Layer
	{
		Background,
		Tile,
		Score,

		UNKNOWN
	};

	enum class ShapeTile
	{
		BLANK,

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

