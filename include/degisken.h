#include <string>

#ifndef DEGISKEN_H
#define DEGISKEN_H

class Degisken {
private:
	std::string Sembol = { " " };
	std::string Indis = { " " };
	std::string Tanim = { "Tanismiz" };
	std::string Isim = { "Degisken" };
	float Deger = 0.0f;
	bool DegerGirildi = false;

public :

	Degisken(const Degisken& degisken);
	Degisken();

	std::string getSembol();
	std::string getIndis();
	std::string getTanim();
	std::string getIsim();
	float getDeger();
	bool getDegerAtandi();
	void setSembol(const std::string& sembol);
	void setIndis(const std::string& indis);
	void setTanim(const std::string& tanim);
	void setIsim(const std::string& isim);
	void setDeger(const float& deger);
	void setDegisken(const char* sembol = nullptr, const char* indis = nullptr,
	const char* tanim = nullptr, const char* isim = nullptr, float* deger = nullptr);


};


#endif // !DEGISKEN_H


