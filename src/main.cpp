//main.cpp

#include "tree.h"
#include <iostream>
#include <fstream>
#include "file.h"
#include <string>
#include "main.h"
#include <msclr\marshal_cppstd.h>
#include "MyForm.h"
#include "MyForm1.h"

void Swap(PenTree* T1, PenTree* T2) {
	PenTree::NodeElmt* temp = T1->Node;
	if (T1->Node->Left != NULL && T1->Node->Right != NULL) {
		T1->Node->Left->SetParent(T2);
		T1->Node->Right->SetParent(T2);
	}
	if (T2->Node->Left != NULL && T2->Node->Right != NULL) {
		T2->Node->Left->SetParent(T1);
		T2->Node->Right->SetParent(T1);
	}
	T1->Node = T2->Node;
	T2->Node = temp;

}

void TraverseTree(PenTree* T, int k, iofile* wr) {
	if (T->Node->Left == NULL && T->Node->Right == NULL) {
		//do nothing
	}
	else {
		if (T->Node->Left->SearchNode(k) != NULL) {
			wr->binwriter(0);
			TraverseTree(T->Node->Left, k, wr);

		}
		else {
			wr->binwriter(1);
			TraverseTree(T->Node->Right, k, wr);
		}
	}
}

void UpdateTree(PenTree* T, ListOrder* first) {
	if (T->GetParent() == NULL) {
		T->Node->weight++;
	}
	else {
		PenTree* High = first->SearchHighestOrder(T);
		if (High != NULL) {
			Swap(T, High);
			T = High;
			T->Node->weight++;
			UpdateTree(T->GetParent(), first);
		}
		else {
			T->Node->weight++;
			UpdateTree(T->GetParent(), first);
		}
	}
}
int getSize(const char* nama) {
	std::ifstream in(nama, std::ifstream::ate | std::ifstream::binary);
	int size = in.tellg();
	in.close();
	return size;
}

void CountingEntropy(PenTree *T, double &x, double frektotal) {
	if (T == NULL) {

	}
	else if (T->Node->Right == NULL && T->Node->Left == NULL && T->Node->kar != 300) {
		double a = (double)T->Node->weight / frektotal;
		x -= (double)a * (log(a) / log(2));
	}
	else {
		CountingEntropy(T->Node->Right, x, frektotal);
		CountingEntropy(T->Node->Left, x, frektotal);
	}
}

void PanjangBitPerSimbol(PenTree *T, int &jumlahsimbol, int &panjangbit, int jumlahbit) {
	if (T == NULL) {

	}
	else if (T->Node->Right == NULL && T->Node->Left == NULL && T->Node->kar != 300) {
		jumlahsimbol++;
		panjangbit += jumlahbit;
	}
	else {
		PanjangBitPerSimbol(T->Node->Left, jumlahsimbol, panjangbit, jumlahbit + 1);
		PanjangBitPerSimbol(T->Node->Right, jumlahsimbol, panjangbit, jumlahbit + 1);
	}
}

void PenDecompress(System::String^ s) {

	std::string file = msclr::interop::marshal_as<std::string>(s);

	iofile* read = new iofile(file.c_str(), true);

	while (!read->fin.eof()) {
		//inisialisasi
		long long int sizecount = 0;
		PenTree* NYT;
		PenTree *Root = new PenTree(300, 0, 512);
		ListOrder *First = new ListOrder(Root);
		//while(!read->fin.eof()){	


		PenTree* mobile = Root;

		//Read header

		unsigned char c_in;
		int b = 1;
		int z;
		long long int counter = 0;

		c_in = read->commonread(); //baca ukuran nama
								   //masih ukuran nama
		while (c_in != (unsigned char)32) {
			if (read->fin.eof()) break;
			c_in = read->commonread();
		}
		if (read->fin.eof()) break;
		c_in = read->commonread(); //baca ukuran nama
								   //masih ukuran nama
		while (c_in != (unsigned char)32) {
			if (read->fin.eof()) break;
			z = (int)c_in;

			z = z - 48;
			z = z*b;
			counter = counter + z;
			b = b * 10;
			c_in = read->commonread();
		}
		if (read->fin.eof()) break;
		//alokasi file name
		char* filename = new char[counter + 1];

		for (int i = 0; i<counter; i++) {
			filename[i] = read->commonread();
		} //dapetin file name

		filename[counter] = '\0';

		//read space
		c_in = read->commonread();

		//read count
		counter = 0;
		b = 1;
		c_in = read->commonread();
		while (c_in != (unsigned char)32) {
			z = (int)c_in;
			z = z - 48;
			z = z*b;
			counter = counter + z;
			b = b * 10;
			c_in = read->commonread();

		}

		int bin;

		std::ofstream fout(filename, std::ios::out | std::ios::binary);
		if (!fout.is_open()) {
			//    abort();
		}

		//inf-loop
		while (counter != sizecount) {

			//check NYT
			if (mobile->Node->kar == 300) {

				unsigned char a = read->getcreader();

				NYT = Root->SearchNode(300);
				fout.write((char *)&a, sizeof(char));
				sizecount++;
				NYT->SpawnChild(a);
				First->InsertNode(NYT->Node->Right);
				First->InsertNode(NYT->Node->Left);
				UpdateTree(NYT, First);
				NYT = Root->SearchNode(300);
				mobile = Root;
			}
			else if (mobile->Node->Left == NULL && mobile->Node->Right == NULL) {
				unsigned char b = ((unsigned char)mobile->Node->kar);

				fout.write((char *)&b, sizeof(char));
				sizecount++;
				UpdateTree(mobile, First);
				mobile = Root;
			}
			else {

				bin = read->binreader();
				if (bin == 1) {
					mobile = mobile->Node->Right;
				}
				else if (bin == 0) {
					mobile = mobile->Node->Left;
				}
				else {
				}
			}
		}

		delete Root;
		delete First;
		delete filename;
		fout.close();
		read->initbin();
		

	}
	System::Windows::Forms::MessageBox::Show("Finished");
	read->fin.close();
}

void PenCompress(array <System::String^>^ Fpath, System::String^ output, System::Windows::Forms::ListView^ p) {
	double sizeawal = 0;
	double sizeakhir, P;
	double entropi = 0;
	double frektotal;
	double JumlahBitPerSimbol;

	Zipendi::MyForm1^ progress = gcnew Zipendi::MyForm1();
	System::Windows::Forms::Label^ lab2 = progress->getLabel2();
	System::Windows::Forms::Label^ lab4 = progress->getLabel4();
	double sum = 0;
	clock_t start = clock();
	System::Windows::Forms::ProgressBar^ bar = progress->GetProgressBar();
	System::Threading::Thread^ newThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(progress,&Zipendi::MyForm1::showdial));
	newThread->Start();
	bar->Minimum = 0;
	bar->Maximum = 100;
	bar->Visible = true;
	lab4->Text = System::Convert::ToString(Fpath->Length);

	std::string file1 = msclr::interop::marshal_as<std::string>(output);
	file1 = file1 + ".pendi";
	iofile* write = new iofile(file1.c_str(), false);
	

	for (int i = 0; i<Fpath->Length; i++) {
		
		lab2->Text = System::Convert::ToString(i);
		System::String^ s = Fpath[i];
		std::string file = msclr::interop::marshal_as<std::string>(s);
		//inisialisasi
		PenTree* NYT;
		PenTree* Search;
		PenTree *Root = new PenTree(300, 0, 512);
		ListOrder *First = new ListOrder(Root);
		char mod;
		//Create header
		mod = (char)255;
		write->fout.write((char*)&mod, sizeof(char));
		mod = (char)32;
		write->fout.write((char*)&mod, sizeof(char));
		long long int b = file.size();
		do {
			mod = (char)(b % 10) + '0';
			write->fout.write((char*)&mod, sizeof(char));
			b = b / 10;
		} while (b != 0);

		//tulis spasi
		unsigned char a = (unsigned char)32;
		write->fout.write((char*)&a, sizeof(char));

		//tulis nama string
		write->fout << file;
		//cout<<"filename:"<<file[i]<<endl;

		//tulis spasi
		write->fout.write((char*)&a, sizeof(char));
		//hitung file size
		b = getSize(file.c_str());
		sizeawal = sizeawal + (long double)b;
		frektotal = (double)b;

		do { //simpen file size secara terbalik
			mod = (char)(b % 10) + '0';
			write->fout.write((char*)&mod, sizeof(char));
			b = b / 10;
		} while (b != 0);

		//tulis spasi
		write->fout.write((char*)&a, sizeof(char));


		//buka file
		std::ifstream myfile(file.c_str(), std::ios::binary);
		if (!myfile.is_open()) {
		}

		//baca isi file sekaligus buat tree dan tulis ke compress
		unsigned char c_in;
		int i_in;
		while (true) {
			myfile.read((char *)&c_in, sizeof(char));
			if (myfile.eof()) break;
			i_in = c_in;


			//Tree operation
			Search = First->SearchList(i_in);
			if (Search == NULL) {
				//NYT dibikin
				NYT = Root->SearchNode(300);
				TraverseTree(Root, 300, write);
				write->binwriter(c_in);
				NYT->SpawnChild(i_in);
				First->InsertNode(NYT->Node->Right);
				First->InsertNode(NYT->Node->Left);
				UpdateTree(NYT, First);
				NYT = Root->SearchNode(300);
			}
			else {
				//karakter ada
				TraverseTree(Root, i_in, write);
				UpdateTree(Search, First);
			}
		}

		CountingEntropy(Root, entropi, frektotal);

		int panjangbit = 0;
		int jumlahsimbol = 0;
		PanjangBitPerSimbol(Root, jumlahsimbol, panjangbit, 0);
		JumlahBitPerSimbol = ((double)panjangbit) / ((double)jumlahsimbol);

		delete Root;
		delete First;
		myfile.close();
		write->binwriterpending();

		sizeakhir = (long double)getSize(file1.c_str());
		P = sizeakhir / sizeawal;

		bar->Value = (i+1) *(100 / (Fpath->Length));
		bar->Refresh();
		if (i == Fpath->Length-1) {
			//progress->Close();
			bar->Value = 100;
			System::Windows::Forms::MessageBox::Show("Compress Finished");
			System::Windows::Forms::ListView^ lv = p;
			lv->Clear();
			
		}
	}
	lab2->Text = System::Convert::ToString(Fpath->Length);
	clock_t end = clock();
	sum = ((double)(end - start) / CLOCKS_PER_SEC);
	progress->label11->Text = System::Convert::ToString(sum);
	progress->label12->Text = System::Convert::ToString(JumlahBitPerSimbol);
	progress->label13->Text = System::Convert::ToString(entropi/Fpath->Length);
	progress->label16->Text = System::Convert::ToString(P*100);
	write->fout.close();
}


