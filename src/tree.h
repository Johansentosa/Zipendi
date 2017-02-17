#pragma once
//tree.h

class PenTree {


public:


	class NodeElmt {
	public:
		PenTree* Left;
		PenTree* Right;
		int kar;
		long long int weight;
		~NodeElmt();
		NodeElmt();
		NodeElmt(int k, int w);
	};

	NodeElmt* Node;

	PenTree();
	PenTree(int k, int w, int o);
	~PenTree();
	PenTree* GetParent();
	void SetParent(PenTree*);
	NodeElmt* GetNode();
	void SpawnChild(int kar);
	int GetOrder();
	void SetOrder(int);
	PenTree* SearchNode(int kar);
	//void Swap(NodeElmt* T1, NodeElmt* T2);
	void PrintTree(int n);

private:
	PenTree* Parent;
	int order;
};


class ListOrder {
public:
	ListOrder();
	ListOrder(PenTree* Root);
	~ListOrder();
	PenTree* SearchHighestOrder(PenTree* T);
	void InsertNode(PenTree* T);
	void PrintList();
	PenTree* SearchList(int kar);

private:
	PenTree* item;
	ListOrder* next;
};


