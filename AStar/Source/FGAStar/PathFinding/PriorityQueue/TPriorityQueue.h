#pragma once

template <typename InElementType>
struct TPriorityQueueNode
{
// Properties
	InElementType Element;
	float Priority = 0;

// Functions
	TPriorityQueueNode(){}
	TPriorityQueueNode(InElementType InElement, float InPriority) { Element = InElement; Priority = InPriority; }

// Operator Overloads
	bool operator<(const TPriorityQueueNode<InElementType> Other) const { return Priority < Other.Priority; }
};

template <typename InElementType>
class TPriorityQueue
{
// Functions
public:
	TPriorityQueue()
	{
		Array.Heapify();
	}
	
	InElementType Pop()
	{
		TPriorityQueueNode<InElementType> Node;
		Array.HeapPop(Node);
		return Node.Element;
	}

	TPriorityQueueNode<InElementType> PopNode()
	{
		TPriorityQueueNode<InElementType> Node;
		Array.HeapPop(Node);
		return Node;
	}

	void Push(InElementType Element, float Priority)
	{
		Array.HeapPush(TPriorityQueueNode<InElementType>(Element, Priority));
	}

	bool IsEmpty() const
	{
		return Array.Num() == 0;
	}
	
	
// Properties
private:
	TArray<TPriorityQueueNode<InElementType>> Array;
};
