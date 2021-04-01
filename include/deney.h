#pragma once

#include <vector>
#include <string>

#include "formul.h"
#include "degisken.h"

class Deney {
private:
	Formul* formul = nullptr; 
	std::string DeneyAdi = { "Yeni deney- " };
	int id = 0;
public:

	Deney(int no);
	Deney(Formul&);
	~Deney();
	
	bool FormulAtandi();
	void FormulKullan(Formul& formul);
	void FormulKullan(std::string& formul);

	const int getDegiskenSayisi();
	std::string getDegiskenSembol(int degiskenID);
	std::string getDegiskenIndis(int degiskenID);
	std::string getDegiskenTanim(int degiskenID);
	std::string getDegiskenIsim(int degiskenID);
	float getDegiskenDeger(int degiskenID);
	void setDegiskenSembol(int degiskenID, std::string sembol);
	void setDegiskenIndis(int degiskenID, std::string indis);
	void setDegiskenTanim(int degiskenID, std::string tanim);
	void setDegiskenIsim(int degiskenID, std::string isim);
	void setDegiskenDeger(int degiskenID, float deger);
	void setDegisken(int degiskenID, const char* sembol = nullptr, const char* indis = nullptr,
		const char* tanim = nullptr, const char* isim = nullptr, float* deger = nullptr);
	

	std::string getDeneyAdi();
	void setDeneyAdi(std::string& deneyAdiYeni);

	float getDeneySonucu();
};