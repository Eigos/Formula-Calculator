#pragma once

#include <string>
#include <vector>
#include <type_traits>

#include "formul.h"

enum class Status {

	Error,
	Done
};

struct Deney : public Formul
{
	std::string DeneyAdi;
	
	Deney(const int& id) {
		DeneyAdi = { "Deney- " + std::to_string(id) };

	}

	Deney(const size_t& id) {
		DeneyAdi = { "Deney- " + std::to_string(id) };
	}

	Deney(const Formul* yeniFormul, std::string deneyAdi = "Deney- Yeni") : Formul(*yeniFormul) {
		DeneyAdi = deneyAdi;
	};

	Deney() {
		DeneyAdi = { "Deney- Yeni" };
	}

	Deney(const char* deneyAdi) {
		DeneyAdi = { deneyAdi};
	}

};


class Kontrol {
private:
	std::vector<Deney*> deneyListesi;
	static std::vector<Formul*> formulListesi;
	
	template<typename ListType>
	inline bool ListedeBulunuyor(const size_t& id,const std::vector<ListType*>& liste);

private:
	Deney* DeneyYeni(const char*);


public:
	Kontrol();
	const int DeneyYeni();
	const int DeneyYeni(Deney* deney);
	Status DeneySil(uint32_t& deneyID);
	const char* getDeneyAdi(uint32_t& deneyID);
	Status DeneyDuzenle(int deneyID, std::string yeniDeneyAdi);
	Status DeneyFormulKullan(const size_t& formulID, Deney* deney);
	size_t getDeneySayisi();
	Deney* operator[](int& id);
	const Deney* getDeney(int id);
	
	Status FormulYeni(std::string formul = "", std::string isim = {"FormulNo-" + std::to_string(formulListesi.size())}, std::string aciklama = "Yeni Formul");
	Status FormulYeni(Formul* formul);
	Status FormulSil(int& formulID);
	bool FormulVar();
	size_t getFormulSayisi();
	const Formul* getFormul(const int& formulID);
	
	void Temizle();

};

template<typename ListType>
inline bool Kontrol::ListedeBulunuyor(const size_t& id, const std::vector<ListType*>& liste)
{
	if (!liste.empty()) {
		if (id < liste.size()) {
			if (id >= 0) {
				return true;
			}
		}
	}
	return false;
}
