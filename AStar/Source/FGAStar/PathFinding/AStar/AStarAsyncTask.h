#pragma once
class UTile;

class FGASTAR_API AStarAsyncTask : public FNonAbandonableTask
{
// Functions
public:
	AStarAsyncTask(UTile* Start, UTile* Target, TArray<FVector2D>& OutPath, bool const AcceptPartialPath = false);
	void DoWork();
	TStatId GetStatId() const;

// Properties
private:
	UTile* StartingTile = nullptr;
	UTile* TargetTile = nullptr;
	TArray<FVector2D>* Path;
	bool bAcceptPartialPath = false;
};
