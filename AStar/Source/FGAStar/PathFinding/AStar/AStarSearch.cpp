#include "AStarSearch.h"
#include "FGAStar/PathFinding/Tile/Tile.h"
#include "Algo/Reverse.h"
#include "FGAStar/PathFinding/PriorityQueue/TPriorityQueue.h"

void AStarSearch::AStar(UTile* Start, UTile* Target, TArray<FVector2D>& OutPath, bool const AcceptPartialPath)
{
	TPriorityQueue<UTile*> Frontier;
	TMap<UTile*, UTile*> CameFrom;
	TMap< UTile*, float> CostSoFar;

	OutPath.Empty();
	
	Frontier.Push(Start, 0);
	CameFrom.Add(Start, nullptr);
	CostSoFar.Add(Start, 0);

	while(!Frontier.IsEmpty())
	{
		UTile* Current = Frontier.Pop();

		if(Current == Target)
		{
			break;
		}

		for (UTile* Next : Current->Neighbors)
		{
			float NewCost = CostSoFar[Current] + Next->Cost;

			if(!CostSoFar.Contains(Next) || NewCost < CostSoFar[Next])
			{
				if(Next->IsWall) // Not Walkable tile
				{
					continue;
				}

				// Cost
				if(CostSoFar.Contains(Next))
				{
					CostSoFar[Next] = NewCost;
				}
				else
				{
					CostSoFar.Add(Next, NewCost);
				}

				// Priority
				float const Priority = NewCost + Heuristic(Next->Position, Target->Position);
				Frontier.Push(Next, Priority);

				// Came from so we know where we came from, Breadcrumb method
				if(CameFrom.Contains(Next))
				{
					CameFrom[Next] = Current;
				}
				else
				{
					CameFrom.Add(Next, Current);
				}
			}
		}
	}

	// Breadcrumb method, adding it to the path
	UTile* PathCurrent = Target;
	TArray<FVector2D> Path;
	if(!CameFrom.Contains(PathCurrent) && !AcceptPartialPath) // If path isn't complete and not accepting partial paths
	{
		UE_LOG(LogTemp, Log, TEXT("No Valid Path"))
	}
	else
	{
		while(PathCurrent != Start)
		{
			Path.Add(PathCurrent->Position);
			PathCurrent = CameFrom[PathCurrent];
		}
	}

	// Reverse the path so the closest point will be next to the start location
	Algo::Reverse(Path);
	OutPath = Path;
}
