// obtaining file size
#include <iostream>
#include <fstream>
#include "file.h"


	//ctor
	iofile::iofile() {
		count = 0;
		pcount = 0;
	}

	iofile::iofile(const char* namafile, bool isinput) {
		if (isinput) {
			fin.open(namafile, std::ios::binary);
			if (!fin.is_open()) {
				//    abort();
			}
		}
		else {
			fout.open(namafile, std::ios::out | std::ios::binary);
			if (!fout.is_open()) {
				//    abort();
			}
		}
		count = 0;
		pcount = 0;
	}

	void iofile::binwriter(int bin) {//int yang dimasukkan harus angka 1 atau 0;
		if (count == 0) {
			c = bin;
			count = 1;
		}
		else {
			c = c << 1;
			c += bin;
			count++;
		}

		if (count == 8) {
			fout.write((char *)&c, sizeof(char));
			count = 0;
		}
	}

	void iofile::binwriter(unsigned char x) {//int yang dimasukkan harus angka 1 atau 0;
		unsigned char temp = (unsigned char)x;
		if (count == 0) {
			c = x;
			pending = 0;
			pcount = 0;
			count = 8;
		}
		else {
			temp = temp >> count;
			c = c << (8 - count);
			c += temp;
			pending = (unsigned char)x << (8 - count); //char dimasukkan 9 bit untuk membuat pseudo EOF
			pending = pending >> (8 - count);
			pcount = count;
			count = 8;
		}

		if (count >= 8) {
			fout.write((char *)&c, sizeof(char));
			if (pcount>0) {
				c = pending;
				count = pcount;
				pcount = 0;
				pending = 0;
			}
			else {
				c = 0;
				count = 0;
			}
		}
	}

	void iofile::binwriterpending() {
		if (count > 0) {
			c <<= (8 - count);
			fout.write((char *)&c, sizeof(char));
			count = 0;
			c = 0;
			pending = 0;
			pcount = 0;
		}
	}

	void iofile::initbin() {
		count = 0;
		c = 0;
		pending = 0;
		pcount = 0;
	}

	void iofile::binwriterEOF() {
		int neof = 9;
		c = c << (8 - count);
		neof = 9 - count;
		fout.write((char *)&c, sizeof(char));
		c = 0;
		if (neof == 9) {
			fout.write((char *)&c, sizeof(char));
		}
		fout.write((char *)&c, sizeof(char));
	}


	int iofile::binreader() { //mengembalikan nilai 0 atau 1
		int bin = 0;
		char c_in;
		if (count == 0) {
			fin.read((char *)&c_in, sizeof(char));
			if (fin.eof()) {
				eofinput = true;
			}
			c = (unsigned char)c_in;
			count = 8;
		}
		if ((c / 128) == 1) {
			bin = 1;
		}
		count--;
		c = c << 1;

		return bin;
	}
	unsigned char iofile::commonread() {
		unsigned char c_in;
		fin.read((char *)&c_in, sizeof(char));
		return c_in;
	}

	unsigned char iofile::getcreader() {
		unsigned char c_in, chasil;
		unsigned char cx;

		if (count == 0) {
			fin.read((char *)&c_in, sizeof(char));
			if (fin.eof()) {
				eofinput = true;
			}
			c = (unsigned char)c_in;
			count = 8;
		}


		if (count == 8) {
			chasil = (char)c;
			count = 0;
			c = 0;
		}
		else {
			fin.read((char *)&c_in, sizeof(char));
			if (fin.eof()) {
				eofinput = true;
			}
			cx = (unsigned char)c_in;

			cx = cx >> count;

			chasil = c + cx;

			c = (unsigned char)c_in << (8 - count);
		}
		return chasil;
	}


	bool iofile::isEOF() {
		return eofinput;
	}

