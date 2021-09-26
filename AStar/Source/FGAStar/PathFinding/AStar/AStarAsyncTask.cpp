#include "AStarAsyncTask.h"
#include "AStarSearch.h"

AStarAsyncTask::AStarAsyncTask(UTile* Start, UTile* Target, TArray<FVector2D>& OutPath, bool const AcceptPartialPath)
{
	StartingTile = Start;
	TargetTile = Target;
	Path = &OutPath;
	bAcceptPartialPath = AcceptPartialPath;
}

void AStarAsyncTask::DoWork()
{
	AStarSearch::AStar(StartingTile, TargetTile, *Path, bAcceptPartialPath);
}

TStatId AStarAsyncTask::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(PrimeCalculationAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
}
