#pragma once

#include <string>
#include <vector>
#include <cmath>

enum class IslemTipi : int {
	DEF = -1,
	UST = 0,
	CARP,
	BOL,
	TOP,
	CIK,
	MOD,
	ABS,
	SIN,
	COS,
	TAN,
	COT,
	SEC,
	CSC,
	LOG,
	LN,
	FAK,
	NEG
};

//Tek ve çift değişkene veya sabite bağımlı olan islemler
static const int islemTuruCiftSayisi = 6;
static const int islemTuruTekSayisi = 11;
static const char* islemTuruCift[] = { "^", "*", "/", "+", "-", "mod" };
static const char* islemTuruTek[] = { "abs", "sin", "cos", "tan", "cot", "sec", "csc", "log", "ln", "!", "-" };


class Formul {
private:
	struct Islem;
	struct Degisken;

	std::string FormulYazimi;
	std::string FormulIsmi;
	std::string FormulAciklamasi;
	std::vector<Degisken*> DegiskenListesi;

	std::vector<Islem*> IslemListesi;
	std::vector<float> IslemSonucListesi;
	std::vector<float> SabitListesi;
	float sonuc = 0;

private:

	//std::vector<std::string > islemTuru = { "+", "-", "*", "/", "^", "abs", "sin", "cos", "tan", "cot", "sec", "csc", "log", "ln", "!", "mod" };
	void IslemAraDegistirCift(std::string aranacakIslem, size_t& posBas, size_t& posSon, std::string& metin);
	void IslemAraDegistirTek(std::string aranacakIslem, size_t posBas, size_t& posSon, std::string& metin);


	void FormulCozumle();
	float IslemleriHesapla();
	IslemTipi StringdenIslemTipi(std::string tip);

private:

	struct Islem {
		Islem() {};
		std::string islemStr;
		IslemTipi islemTip = IslemTipi::DEF;
		std::string degerIlk;
		std::string degerIki;
		float islemSonuc = 0;
	};

	struct Degisken {
		Degisken(const Degisken& degisken);
		Degisken();
		std::string Sembol = { " " };
		std::string Indis = { " " };
		std::string Tanim = { "Tanismiz" };
		std::string Isim = { "Degisken" };
		float Deger = 0.0f;
		bool DegerGirildi = false;
	};


public:
	//örn (-a ((M*R^2*t^2)/(l*2*x)) - (M*R^2)  )
	Formul();
	Formul(std::string& formul) :FormulYazimi(formul) { FormulCozumle(); }
	Formul(std::string& formul, std::string& isim, std::string& aciklama) :
		FormulYazimi(formul), FormulIsmi(isim), FormulAciklamasi(aciklama)
	{
		FormulCozumle();
	}
	Formul(const Formul& formul);
	~Formul();


	const int getDegiskenSayisi()const;

	float getSonuc();


	const std::string getFormulIsmi()const;
	const std::string getFormulAciklama()const;
	const std::string getFormulYazim()const;

	void setFormulIsmi(std::string isim);
	void setFormulAciklama(std::string aciklama);
	void setFormulYazim(std::string yazim);
	bool getFormulAtandi();


	inline const static bool BuyukHarfKont(const char karakter);
	inline const static bool KucukHarfKont(const char karakter);
	inline const static bool RakamKont(const char karakter);


	const std::string getDegiskenSembol(const int& id)const;
	const std::string getDegiskenIndis(const int& id)const;
	const std::string getDegiskenTanim(const int& id)const;
	const std::string getDegiskenIsim(const int& id)const;
	const float getDegiskenDeger(const int& id) const;
	bool getDegiskenDegerAtandi(const int& id)const;
	void setDegiskenSembol(const int& id, const std::string& sembol);
	void setDegiskenIndis(const int& id, const std::string& indis);
	void setDegiskenTanim(const int& id, const std::string& tanim);
	void setDegiskenIsim(const int& id, const std::string& isim);
	void setDegiskenDeger(const int& id, const float& deger);
	void setDegisken(const int& id, const char* sembol = nullptr, const char* indis = nullptr,
		const char* tanim = nullptr, const char* isim = nullptr, float* deger = nullptr);
};

inline const bool Formul::BuyukHarfKont(const char karakter)
{
	return karakter >= 65 ? karakter <= 90 ? true : false : false;
}

inline const bool Formul::KucukHarfKont(const char karakter)
{
	return karakter >= 97 ? karakter <= 122 ? true : false : false;
}

inline const bool Formul::RakamKont(const char karakter)
{
	return karakter >= 48 ? karakter <= 57 ? true : false : false;
}
