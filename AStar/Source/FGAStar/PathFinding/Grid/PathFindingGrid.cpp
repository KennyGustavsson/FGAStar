#include "PathFindingGrid.h"
#include "DrawDebugHelpers.h"
#include "FGAStar/PathFinding/Tile/Tile.h"

float UPathFindingGrid::Sign(FVector2D const A, FVector2D const B, FVector2D const C)
{
	return (A.X - C.X) * (B.Y - C.Y) - (B.X - C.X) * (A.Y - C.Y);
}

bool UPathFindingGrid::PointInTriangle(FVector2D const Point, FTriangle T)
{
	float const D1 = Sign(Point, T.Points[0], T.Points[1]);
	float const D2 = Sign(Point, T.Points[1], T.Points[2]);
	float const D3 = Sign(Point, T.Points[2], T.Points[0]);

	bool const Negative = (D1 < 0) || (D2 < 0) || (D3 < 0);
	bool const Positive = (D1 > 0) || (D2 > 0) || (D3 > 0);

	return !(Negative && Positive);
}

void UPathFindingGrid::Generate(const FVector2DInt GridSize, const int TileScale, const FVector2DInt Offset)
{
	if(TileScale == 0)
	{
		Scale = 1;
	}

	Size = GridSize;
	Scale = TileScale;
	GridOffset = Offset;
	Grid.Empty();

	GenerateGrid(Offset);
	GetObstacleVertices();
	GetObstacleInfosToGrid();

	// Gets Tiles Neighbors
	for (int x = 0; x < Size.X; x++)
	{
		for (int y = 0; y < Size.Y; y++)
		{
			Grid[x][y]->GetNeighbors();
		}
	}

	// TODO Temp, Grid Visuals
	for (int x = 0; x < Size.X; x++)
	{
		for (int y = 0; y < Size.Y; y++)
		{
			FVector Loc = FVector(Grid[x][y]->Position.X, Grid[x][y]->Position.Y, 0);
			FVector Extent = (FVector::OneVector * Scale * 0.5f - 1.0f) * 0.9f;
			Extent.Z = 0;

			FColor Color = Grid[x][y]->Cost > 1 ? FColor::Yellow : FColor::Blue;
			Color = Grid[x][y]->IsWall ? FColor::Red : Color;
				
			DrawDebugBox(Owner->GetWorld(), Loc, Extent, Color, true, -1, 0, 10);
		}
	}
	// --
}

void UPathFindingGrid::GenerateGrid(const FVector2DInt Offset)
{
	// Generates the Grid
	for (int x = 0; x < Size.X; x++)
	{
		// Adding Columns to the Array
		FTileArray ToAdd;
		Grid.Add(ToAdd);

		for (int y = 0; y < Size.Y; y++)
		{
			// Adding Tiles to 2DArray
			UTile* TileToAdd;
			TileToAdd = NewObject<UTile>();
			TileToAdd->InitializeTile(FVector2D(x, y), Offset, 1, false, this);
			
			Grid[x].Tile.Add(TileToAdd);
		}
	}
}

void UPathFindingGrid::GetObstacleVertices()
{
	for (FObstacleInfo Obstacle : Obstacles)
	{
		FObstacleQuad Quad;
		Quad.ObstacleInfo = Obstacle;
		
		// Triangulate Extent
		FTriangle Tri1;
		Tri1.Points.Add(FVector2D(Obstacle.Extent.X, Obstacle.Extent.Y));
		Tri1.Points.Add(FVector2D(-Obstacle.Extent.X, Obstacle.Extent.Y));
		Tri1.Points.Add(FVector2D(Obstacle.Extent.X, -Obstacle.Extent.Y));

		FTriangle Tri2;
		Tri2.Points.Add(FVector2D(-Obstacle.Extent.X, Obstacle.Extent.Y));
		Tri2.Points.Add(FVector2D(-Obstacle.Extent.X, -Obstacle.Extent.Y));
		Tri2.Points.Add(FVector2D(Obstacle.Extent.X, -Obstacle.Extent.Y));
		
		Quad.Tris.Add(Tri1);
		Quad.Tris.Add(Tri2);

		// Transform Triangles
		FObstacleQuad Transformed;
		float const RotationRad = FMath::DegreesToRadians(Obstacle.Rotation.Yaw);

		// Rotation
		for (int i = 0; i < Quad.Tris.Num(); i++)
		{
			Transformed.Tris.Add(Quad.Tris[i]);
			
			for (int j = 0; j < 3; j++)
			{
				Transformed.Tris[i].Points[j].X = Quad.Tris[i].Points[j].X * FMath::Cos(RotationRad) - Quad.Tris[i].Points[j].Y * FMath::Sin(RotationRad);
				Transformed.Tris[i].Points[j].Y = Quad.Tris[i].Points[j].X * FMath::Sin(RotationRad) + Quad.Tris[i].Points[j].Y * FMath::Cos(RotationRad);
			}
		}

		// Translate
		for (int i = 0; i < Transformed.Tris.Num(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Transformed.Tris[i].Points[j].X += Obstacle.Location.X;
				Transformed.Tris[i].Points[j].Y += Obstacle.Location.Y;
			}
		}

		// TODO Temp, for visualizing obstacles
		FColor ObstacleColor = FColor::Orange;
		ObstacleColor.A *= 0.5f;
		
		for (int i = 0; i < Quad.Tris.Num(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int const EndIndex = j == 2 ? 0 : j + 1;

				FVector BeginPoint = FVector(Transformed.Tris[i].Points[j].X, Transformed.Tris[i].Points[j].Y, 10);
				FVector EndPoint = FVector(Transformed.Tris[i].Points[EndIndex].X, Transformed.Tris[i].Points[EndIndex].Y, 10);
				
				DrawDebugLine(Owner->GetWorld(), BeginPoint, EndPoint, ObstacleColor, true, 10, -1, 4);
			}
		}
		// --

		Transformed.ObstacleInfo = Obstacle;
		ObstacleQuads.Add(Transformed);
	}
}

void UPathFindingGrid::GetObstacleInfosToGrid()
{
	for (auto Quads : ObstacleQuads)
	{
		for (auto t : Quads.Tris)
		{
			int const MaxX = FMath::Max(t.Points[0].X, FMath::Max(t.Points[1].X, t.Points[2].X));
			int const MinX = FMath::Min(t.Points[0].X, FMath::Min(t.Points[1].X, t.Points[2].X));
			
			int const MaxY = FMath::Max(t.Points[0].Y, FMath::Max(t.Points[1].Y, t.Points[2].Y));
			int const MinY = FMath::Min(t.Points[0].Y, FMath::Min(t.Points[1].Y, t.Points[2].Y));

			for (int y = MinY; y < MaxY + 1; y++)
			{
				for (int x = MinX; x < MaxX + 1; x++)
				{
					FVector2D const Point = FVector2D(x, y);
					if (PointInTriangle(Point, t))
					{
						UTile* Tile = GetTileFromWorldLocation(FVector(x, y, 0));

						if(Tile)
						{
							Tile->Cost = Quads.ObstacleInfo.Cost;
							Tile->IsWall = Quads.ObstacleInfo.bIsWall;
						}
					}
				}
			}
		}
	}
}

UTile* UPathFindingGrid::GetNearestTileFromWorldLocation(const FVector Location)
{
	if(Size.X == 0 && Size.Y == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to get tile from grid, but grid has not been generated."))
		return nullptr;
	}
	
	const int x = FMath::Clamp(FGenericPlatformMath::RoundToInt((Location.X - GridOffset.X) / Scale), 0, Size.X - 1);
	const int y = FMath::Clamp(FGenericPlatformMath::RoundToInt((Location.Y - GridOffset.Y) / Scale), 0, Size.Y - 1);
	
	return Grid[x][y];
}

UTile* UPathFindingGrid::GetTileFromWorldLocation(const FVector Location)
{
	if(Size.X == 0 && Size.Y == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Trying to get tile from grid, but grid has not been generated."))
		return nullptr;
	}
	
	const int x = FGenericPlatformMath::RoundToInt((Location.X - GridOffset.X) / Scale);
	const int y = FGenericPlatformMath::RoundToInt((Location.Y - GridOffset.Y) / Scale);

	if(x > Size.X - 1 || x < 0 || y > Size.Y - 1 || y < 0)
	{
		return nullptr;
	}
	
	return Grid[x][y];
}

void UPathFindingGrid::AddGridObstacle(FObstacleInfo const ObstacleInfo)
{
	Obstacles.Add(ObstacleInfo);
}
