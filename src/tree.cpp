//tree.cpp
#include "tree.h"
#include <iostream>

PenTree::NodeElmt::NodeElmt() {
	Left = NULL;
	Right = NULL;
	kar = 0;
	weight = 0;
}

PenTree::NodeElmt::NodeElmt(int k, int w) {
	Left = NULL;
	Right = NULL;
	kar = k;
	weight = w;
}

PenTree::NodeElmt::~NodeElmt() {
	delete Left;
	delete Right;
}

PenTree::~PenTree() {
	delete Node;
	//delete Parent;
}

PenTree::PenTree() {
	Parent = NULL;
	Node = new NodeElmt();
	order = 0;
}

PenTree::PenTree(int k, int w, int o) {
	Parent = NULL;
	Node = new NodeElmt(k, w);
	order = o;
}

int PenTree::GetOrder() {
	return order;
}


void PenTree::SetOrder(int o) {
	order = o;
}

void PenTree::SpawnChild(int kar) {
	Node->kar = -1;
	Node->Right = new PenTree(kar, 1, order - 1);
	Node->Right->Parent = this;
	Node->Left = new PenTree(300, 0, order - 2);
	Node->Left->Parent = this;
}

PenTree* PenTree::SearchNode(int k) {
	if (Node->Left == NULL && Node->Right == NULL) {
		if (Node->kar == k) {
			return this;
		}
		else {
			return NULL;
		}
	}
	else {
		if (Node->Left->SearchNode(k) != NULL) {
			return Node->Left->SearchNode(k);
		}
		else {
			return Node->Right->SearchNode(k);
		}
	}
}

PenTree* ListOrder::SearchList(int k) {
	if (next == NULL) {
		if (item->Node->kar == k) {
			return item;
		}
		else {
			return NULL;
		}
	}
	else {
		if (item->Node->kar == k) {
			return item;
		}
		else {
			return next->SearchList(k);
		}
	}
}



PenTree* PenTree::GetParent() {
	return Parent;
}

void PenTree::SetParent(PenTree* P) {
	Parent = P;
}

void PenTree::PrintTree(int n) {
	for (int i = 0; i<n; i++) {
	}
	
	if (Node->Left == NULL && Node->Right == NULL) {

	}
	else if (Node->Left == NULL && Node->Right != NULL) {
		Node->Right->PrintTree(1 + n);
	}
	else if (Node->Left != NULL && Node->Right == NULL) {
		Node->Left->PrintTree(1 + n);
	}
	else {
		//		cout<<"char : "<<(char) Node->Left->Node->kar<<" order : "<<Node->Left->order<<" weight : "<<Node->Left->Node->weight<<endl;
		//		cout<<"char : "<<(char) Node->Right->Node->kar<<" order : "<<Node->Right->order<<" weight : "<<Node->Right->Node->weight<<endl;
		Node->Left->PrintTree(1 + n);
		Node->Right->PrintTree(1 + n);
	}
}


ListOrder::ListOrder() {
	item = NULL;
	next = NULL;
}

ListOrder::ListOrder(PenTree* Root) {
	item = Root;
	next = NULL;
}

void ListOrder::InsertNode(PenTree* T) {
	if (next == NULL) {
		next = new ListOrder();
		next->item = T;
	}
	else {
		next->InsertNode(T);
	}
}

void ListOrder::PrintList() {
	if (next == NULL) {
		if (item->GetParent() != NULL) {
		}
	}
	else {
		if (item->GetParent() != NULL) {
		}
		next->PrintList();
	}
}

PenTree* ListOrder::SearchHighestOrder(PenTree* T) {
	if (T->Node->weight == item->Node->weight) {
		if (T->GetOrder() >= item->GetOrder()) {
			return NULL;
		}
		else {
			if (item->GetOrder() != T->GetParent()->GetOrder()) {
				return item;
			}
			else {
				return next->SearchHighestOrder(T);
			}
		}
	}
	else {
		if (next == NULL) {
			return NULL;
		}
		else {
			return next->SearchHighestOrder(T);
		}
	}
}

ListOrder::~ListOrder() {
	delete next;
}
