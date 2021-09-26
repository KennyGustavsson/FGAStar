#include "Tile.h"

void UTile::InitializeTile(FVector2D const TileIndex, FVector2DInt const TileOffset, int const TileCost, bool const TileIsWall, UPathFindingGrid* TileGrid)
{
	Index = TileIndex;
	Cost = TileCost;
	IsWall = TileIsWall;
	Grid = TileGrid;

	Position = Index * Grid->Scale;
	Position.X += TileOffset.X;
	Position.Y += TileOffset.Y;
}

void UTile::GetNeighbors()
{
	if(!IsUpLimit())
	{
		Neighbors.Add(Grid->Grid[Index.X][Index.Y + 1]);
	}

	if(!IsDownLimit())
	{
		Neighbors.Add(Grid->Grid[Index.X][Index.Y - 1]);
	}

	if(!IsRightLimit())
	{
		Neighbors.Add(Grid->Grid[Index.X + 1][Index.Y]);
	}

	if(!IsLeftLimit())
	{
		Neighbors.Add(Grid->Grid[Index.X - 1][Index.Y]);
	}
}
