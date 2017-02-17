#pragma once
// obtaining file size
#include <iostream>
#include <fstream>

class iofile {
public:
	//ctor
	iofile();

	iofile(const char* namafile, bool isinput);

	void binwriter(int bin);

	void binwriter(unsigned char x);

	void binwriterpending();

	void initbin();

	void binwriterEOF();


	int binreader();
	unsigned char commonread();

	unsigned char getcreader();


	bool isEOF();
	std::ifstream fin;
	std::ofstream fout;

private:
	unsigned char c;
	int count;
	int pcount; //menghitung karakter yang pending
	unsigned char pending;
	bool eofinput;
};



