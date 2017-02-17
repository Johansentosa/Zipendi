#pragma once
#include "tree.h"
#include "file.h"

void Swap(PenTree* T1, PenTree* T2);

void TraverseTree(PenTree* T, int k, iofile* wr);

void UpdateTree(PenTree* T, ListOrder* first);
int getSize(const char* nama);

void CountingEntropy(PenTree *T, double &x, double frektotal);

void PanjangBitPerSimbol(PenTree *T, int &jumlahsimbol, int &panjangbit, int jumlahbit);

void PenDecompress(System::String^ s);

void PenCompress(array <System::String^>^ Fpath, System::String^ output, System::Windows::Forms::ListView^ p);