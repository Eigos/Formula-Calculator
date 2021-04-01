#include "..\include\formul.h"

Formul::Formul(const Formul& formulBase)
{

	this->FormulIsmi = formulBase.FormulIsmi;
	this->FormulYazimi = formulBase.FormulYazimi;
	this->FormulAciklamasi = formulBase.FormulAciklamasi;

	FormulCozumle();

	//formulBase içindeki değişkeni yeni oluşturulan "degisken"e kopyala
	for (int i = 0; i < DegiskenListesi.size(); i++) {
		setDegiskenIndis(i, formulBase.getDegiskenIndis(i));
		setDegiskenIsim(i, formulBase.getDegiskenIsim(i));
		setDegiskenSembol(i, formulBase.getDegiskenSembol(i));
		setDegiskenTanim(i, formulBase.getDegiskenTanim(i));
		setDegiskenDeger(i, formulBase.getDegiskenDeger(i));
	}
	this->sonuc = formulBase.sonuc;

}

Formul::~Formul() {
	for (int i = 0; i < DegiskenListesi.size(); i++) {
		delete DegiskenListesi[i];
	}
	for (int i = 0; i < IslemListesi.size(); i++) {
		delete IslemListesi[i];
	}
}

// test deneme formülü:  (-x*Y^(12*x+y)/z) / _(l^2+12) )
void Formul::FormulCozumle()
{
	std::string yazimGecici = FormulYazimi;
	yazimGecici.insert(0, "(");
	yazimGecici.push_back(')');

	std::vector<char> ozelKarakterler = { '(', ')', '^', '+', '-', '*', '/' };
	std::vector<std::string> ozelIslemler = { "abs", "sin", "cos", "tan", "cot", "sec", "csc", "log", "ln", "!", "mod" };


	//Boşlukları ve kabul edilmeyen karakterleri temizle
	{
		//Ondalık sayıları ifade etmek için kullanılan noktayı silme
		ozelKarakterler.push_back('.');
		for (int i = 0; i < yazimGecici.size(); i++) {
			int harf = static_cast<int>(yazimGecici[i]);
			if (!(BuyukHarfKont(harf) || KucukHarfKont(harf) || RakamKont(harf)))
			{
				bool temizle = true;
				for (int j = 0; j < ozelKarakterler.size(); j++) {
					if (static_cast<char>(harf) == ozelKarakterler[j]) {
						temizle = false;
					}
				}
				if (temizle) {
					yazimGecici.erase(i, 1);
					i--;
				}
			}
		}
		ozelKarakterler.pop_back();
	}

	//Parantezlerin kapandığından emin ol
	//Eşit değil ise parantezleri formul başına veya sonuna ekle
	{
		size_t posA = 0, posB = 0;
		int parantezAc = 0, parantezKapa = 0;
		while ((posA != std::string::npos) || (posB != std::string::npos)) {
			posA = yazimGecici.find("(", posA);
			if (posA != std::string::npos) {
				posA++;
				parantezAc++;
			}
			posB = yazimGecici.find(")", posB);
			if (posB != std::string::npos) {
				posB++;
				parantezKapa++;
			}
		}
		if (parantezAc > parantezKapa) {
			for (int i = 0; i < (parantezAc - parantezKapa); i++) {
				yazimGecici.push_back(')');
			}
		}
		else if (parantezKapa > parantezAc) {
			for (int i = 0; i < (parantezKapa - parantezAc); i++) {
				yazimGecici.insert(0, "(");
			}
		}
	}

	//Değişkenleri yeniden isimlendir örn: d01 d12
	//Yeni değişken oluşturup listeye ekle
	for (int i = 0; i < yazimGecici.size(); i++) {
		int harf = static_cast<int>(yazimGecici[i]);
		if (BuyukHarfKont(harf) || KucukHarfKont(harf)) {
			int posBitis = i;
			const int posBasla = i;
			bool bitisPosBulundu = false;
			while (!bitisPosBulundu) {
				posBitis++;
				char siradakiKarakter = yazimGecici[posBitis];
				if ((false ==
					(BuyukHarfKont(harf) || // Büyük harfler
						KucukHarfKont(harf) || // Küçük harfler
						RakamKont(harf)  // rakamlar
						))) // ondalık sayılar için nokta
				{
					bitisPosBulundu = true;
				}
			}

			std::string degiskenIsim = yazimGecici.substr(i, posBitis - i);
			bool ozelIfade = false;
			//Özel bir işlem olmadığını kontrol et
			for (int k = 0; k < islemTuruTekSayisi; k++) {
				std::string abc(islemTuruTek[k]);
				if (degiskenIsim == islemTuruTek[k]) {
					ozelIfade = true;
					i += degiskenIsim.size() - 1;
					break;
				}
			}
			if (ozelIfade) { continue; }
			//Kayıtlı değişkenlerle karşılaştır

			std::string degiskenNum = "d";

			bool kayitli = false;
			for (int j = 0; j < DegiskenListesi.size(); j++) {

				if (DegiskenListesi[j]->Isim == degiskenIsim.c_str()) {
					degiskenNum.append(std::to_string(j));
					if (degiskenNum.size() == 2) {
						degiskenNum.insert(1, "0");
					}
					kayitli = true;
					break;
				}
			}
			if (!kayitli) {

				degiskenNum.append(std::to_string(DegiskenListesi.size()));
				if (degiskenNum.size() == 2) {
					degiskenNum.insert(1, "0");
				}

				Degisken* degisken = new Degisken;
				degisken->Isim = degiskenIsim;
				degisken->Sembol = degiskenIsim;
				DegiskenListesi.push_back(degisken);
			}
			yazimGecici.erase(posBasla, posBitis - posBasla);
			yazimGecici.insert(posBasla, degiskenNum);
			i += 2; //Sonraki arama için konumu ilerlet
		}
	}

	//Sabit sayıları yeniden isimlendir örn: 12 -> s01(12)
	//Sabit sayılar listesine değerleri ekle
	{
		int say = 0;
		std::string sabit;
		while (say < yazimGecici.size()) {
			int harf = static_cast<int>(yazimGecici[say]);
			size_t posBas = 0, posSon = 0;
			for (int j = 0; j < ozelKarakterler.size(); j++) {
				if (yazimGecici[say] == ozelKarakterler[j]) {
					int baslangic = static_cast<int>(yazimGecici[say + 1]);
					if (RakamKont(baslangic)) {
						posBas = say + 1;
						posSon = posBas + 1;
						break;
					}
					else {
						break;
					}
				}
			}
			if (posBas != 0) {

				bool posSonBulundu = false;
				while (!posSonBulundu) {
					for (int j = 0; j < ozelKarakterler.size(); j++) {
						if (yazimGecici[posSon] == ozelKarakterler[j]) {
							posSonBulundu = true;
							break;
						}
					}
					posSon++;
				}
				posSon--;
				sabit = yazimGecici.substr(posBas, posSon - posBas);

				//Sabitin değeri 
				float sabitf = std::stof(sabit);

				//Formülün tutulduğu stringe yazdırılacak deger örn: s01 s12
				std::string sabitID = "s" + std::to_string(SabitListesi.size());
				if (sabitID.size() == 2) {
					sabitID.insert(1, "0");
				}

				//Sabit değerlerin tutulduğu listeye değeri ekle
				SabitListesi.push_back(sabitf);

				//Formülün tutulduğu stringden sayıları sil
				yazimGecici.erase(posBas, sabit.size());

				//Formülün tutulduğu stringe sabitin bulunduğu numarayı yaz örn: s01 s12
				yazimGecici.insert(posBas, sabitID);
				sabit.erase();

			}
			else {
				say++;
			}
		}
	}

	//İşlemleri ayır / sırala
	{
		int parantezSayisi = 0;
		{
			size_t pos_ = -1;
			while (true) {
				pos_ = yazimGecici.find("(", pos_ + 1);
				if (pos_ != std::string::npos) {
					parantezSayisi++;
				}
				else {
					break;
				}
			}
		}

		//									 posa-------posb
		//									   ^----------^
		//	(s01 + s02 + ( s03 + d01 ^ ( s04 ^ ( s05 + s06) ) ) ) 

		//Parantez içi işlemeri yap
		for (int i = 0; i < parantezSayisi; i++) {
			size_t posa = 0, posb = 0, posGecici = 0;
			posb = yazimGecici.find(")");
			//Parantez içini bul

			posGecici = yazimGecici.find("(", posa + 1);
			while (posb > posGecici) {
				posGecici = yazimGecici.find("(", posGecici + 1);
				posa = yazimGecici.find("(", posa + 1);
			}

			//Negatif ifadeyi ara
			{
				size_t posNegatif = posa;
				while (posNegatif < posb) {
					posNegatif = yazimGecici.find("-", posNegatif);
					if ((posNegatif != std::string::npos) && (posNegatif < posb)) {
						if (!(yazimGecici[posNegatif - 1] >= 48 && yazimGecici[posNegatif - 1] <= 57)) {
							Islem* islem = new Islem;
							islem->islemStr = yazimGecici.substr(posNegatif, 4);
							std::string islemID = "I";

							islemID.append(std::to_string(IslemListesi.size()));
							if (islemID.size() == 2) {
								islemID.insert(1, "0");
							}
							islem->degerIlk = islem->islemStr.substr(1);
							islem->islemTip = IslemTipi::NEG;
							yazimGecici.erase(posNegatif, 4);
							yazimGecici.insert(posNegatif, islemID);
							IslemListesi.push_back(islem);
							posb -= 1; // silinen karakterler için
						}
						else {
							posNegatif++;
						}
					}
				}
			}

			//Pozitif ifadeyi ara
			{
				size_t posPozitif = posa;
				while (posPozitif < posb) {
					posPozitif = yazimGecici.find("+", posPozitif);
					if ((posPozitif != std::string::npos) && (posPozitif < posb)) {
						if (!(yazimGecici[posPozitif - 1] >= 48 && yazimGecici[posPozitif - 1] <= 57)) {

							yazimGecici.erase(posPozitif, 1);
							posb -= 1; // silinen karakterler için
						}
						else {
							posPozitif++;
						}
					}
				}
			}

			//Çift değişkene bağımlı işlemler
			for (int i = 0; i < islemTuruCiftSayisi; i++) {
				IslemAraDegistirCift(islemTuruCift[i], posa, posb, yazimGecici);
			}

			//Tek değişkene bağımlı işlemler
			for (int i = 0; i < islemTuruTekSayisi; i++) {
				IslemAraDegistirTek(islemTuruTek[i], posa, posb, yazimGecici);
			}


			//Parantez içinde işlem kaldı mı
			if ((posb - posa) == 4) {
				yazimGecici.erase(posb, 1);
				yazimGecici.erase(posa, 1);
			}
		}
		//IslemListesi.push_back(yazimGecici);
	}
	//IslemListesi.pop_back();//"son işlemde" de işlem yapılmadığı için çıkarılmalı

}

//Belirtilen işlemi başlangıç pozisyonunda ve son pozisyon arasında metinde arar. 
//İşlem bulunursa metinde değişken ve sabit yerine I ön ekini ekler ve numaralandırır.
//İki farklı değişken veya sabite ihtiyaç duyulduğu zaman kullanılır. Çift bağımlılık
// örn: d01+s02 -> I01
void Formul::IslemAraDegistirCift(std::string aranacakIslem, size_t& posBas, size_t& posSon, std::string& metin) {
	size_t posIslem = 0;
	while (posIslem <= posBas) {
		posIslem = metin.find(aranacakIslem, posBas);
		if ((posIslem != std::string::npos) && (posIslem < posSon)) {
			Islem* islem = new Islem;

			islem->islemStr = metin.substr(posIslem - 3, 6 + aranacakIslem.size());
			islem->degerIlk = islem->islemStr.substr(0, 3);
			islem->degerIki = islem->islemStr.substr(4, islem->islemStr.size() - 4);

			std::string islemID = "I";

			islemID.append(std::to_string(IslemListesi.size()));
			if (IslemListesi.size() < 10) {
				islemID.insert(1, "0");
			}

			metin.erase(posIslem - 3, 6 + aranacakIslem.size());
			metin.insert(posIslem - 3, islemID);

			islem->islemTip = StringdenIslemTipi(aranacakIslem);

			IslemListesi.push_back(islem);
			posSon -= 3 + aranacakIslem.size(); // silinen karakterler için

		}
	}
}

//Belirtilen işlemi başlangıç pozisyonunda ve son pozisyon arasında metinde arar. 
//İşlem bulunursa metinde değişken veya sabit yerine I ön ekini ekler ve numaralandırır.
//Tek değişkene / sabite ihtiyaç duyulduğu zaman kullanılır. Tek bağımlılık
// örn: (islem)d01 -> I01
void Formul::IslemAraDegistirTek(std::string aranacakIslem, size_t posBas, size_t& posSon, std::string& metin) {
	size_t posIslem = 0;
	while (posIslem <= posBas) {
		posBas -= aranacakIslem.size();
		posIslem = metin.find(aranacakIslem, posBas);
		if ((posIslem != std::string::npos) && (posIslem < posSon)) {
			Islem* islem = new Islem;

			//islem->islemStr = metin.substr(posBas + 4, 3); eski kod
			islem->islemStr = metin.substr(posBas + aranacakIslem.size() + 1, 3);
			islem->degerIlk = islem->islemStr.substr(islem->islemStr.size() - 3, islem->islemStr.size() - islem->islemStr.size() - 3);
			std::string islemID = "I";

			islemID.append(std::to_string(IslemListesi.size()));
			if (IslemListesi.size() < 10) {
				islemID.insert(1, "0");
			}

			metin.erase(posIslem, aranacakIslem.size() + 5);
			metin.insert(posIslem, islemID);

			islem->islemTip = StringdenIslemTipi(aranacakIslem);

			IslemListesi.push_back(islem);
			posSon -= aranacakIslem.size() + 3; // silinen karakterler için

		}
	}
}

float Formul::IslemleriHesapla() {
	if (!IslemSonucListesi.empty()) { IslemSonucListesi.clear(); }
	for (int i = 0; i < IslemListesi.size(); i++) {
		float deger[2]{ 0,0 }, sonuc = 0;
		std::vector<std::string> degerStr;
		size_t pos = 0;
		if (!IslemListesi[i]->degerIki.empty()) {
			degerStr.push_back(IslemListesi[i]->degerIlk);
			degerStr.push_back(IslemListesi[i]->degerIki);
		}
		else {
			degerStr.push_back(IslemListesi[i]->degerIlk);
		}
		//Değerleri elde et
		for (int j = 0; j < degerStr.size(); j++) {
			//değer türü (değişken veya sabit)
			char karakter = degerStr[j][0];

			//değer indisi
			std::string idStr = degerStr[j].substr(1, 2);
			int id = std::stoi(idStr);
			switch (karakter)
			{
			case 'I': {
				deger[j] = IslemListesi[id]->islemSonuc;
				break;
			}

			case 'd': {
				deger[j] = DegiskenListesi[id]->Deger;
				break;
			}

			case 's': {
				deger[j] = SabitListesi[id];
			}

			default:
				break;
			}

		}
		try {
			switch (IslemListesi[i]->islemTip) {

			case IslemTipi::UST: {
				sonuc = std::powf(deger[0], deger[1]);
				break;
			}
			case IslemTipi::TOP: {
				sonuc = deger[0] + deger[1];
				break;
			}
			case IslemTipi::CIK: {
				sonuc = deger[0] - deger[1];
				break;
			}
			case IslemTipi::CARP: {
				sonuc = deger[0] * deger[1];
				break;
			}
			case IslemTipi::BOL: {
				try {
					sonuc = deger[0] / deger[1];
				}
				catch (std::exception& e) {
					//e.what();  // error code
				}
				break;
			}
			case IslemTipi::ABS: {
				sonuc = abs(deger[0]);
				break;
			}
			case IslemTipi::SIN: {
				sonuc = sin(deger[0]);
				break;
			}
			case IslemTipi::COS: {
				sonuc = cos(deger[0]);
				break;
			}
			case IslemTipi::TAN: {
				sonuc = tan(deger[0]);
				break;
			}
			case IslemTipi::COT: {
				sonuc = 1 / tan(deger[0]);
				break;
			}
			case IslemTipi::SEC: {
				sonuc = 1 / cos(deger[0]);
				break;
			}
			case IslemTipi::CSC: {
				sonuc = 1 / sin(deger[0]);
				break;
			}
			case IslemTipi::LOG: {
				sonuc = log10(deger[10]);
				break;
			}
			case IslemTipi::LN: {
				sonuc = log(deger[0]);
				break;
			}
			case IslemTipi::FAK: {
				int n = deger[0];
				sonuc = deger[0];
				while (n > 0) {
					sonuc = sonuc * n;
					n--;
				}
				break;
			}
			case IslemTipi::MOD: {
				sonuc = static_cast<unsigned int>(deger[0]) % static_cast<unsigned int>(deger[1]);
				break;
			}
			case IslemTipi::NEG: {
				sonuc = deger[0] * (-1);
				break;
			}

			default: {
				break;
			}

			}

			IslemListesi[i]->islemSonuc = sonuc;
		}
		catch (std::exception& e) {
			//e.what();
		}
	}
	this->sonuc = IslemListesi.back()->islemSonuc;
	return sonuc;
}

IslemTipi Formul::StringdenIslemTipi(std::string islem)
{
	for (int i = 0; i < islemTuruCiftSayisi; i++) {
		if (islemTuruCift[i] == islem) {
			return IslemTipi(i);
		}
	}

	for (int i = 0; i < islemTuruTekSayisi; i++) {
		if (islemTuruTek[i] == islem) {
			return IslemTipi(i + islemTuruCiftSayisi);
		}
	}
	return IslemTipi::DEF;
}

Formul::Formul() {
	const int reserveAmount = 5;
	DegiskenListesi.reserve(reserveAmount);
	IslemSonucListesi.reserve(reserveAmount);
	IslemListesi.reserve(reserveAmount);
	SabitListesi.reserve(reserveAmount);
}

const int Formul::getDegiskenSayisi()const
{
	return DegiskenListesi.size();
}

float Formul::getSonuc()
{
	//if (DegiskenListesi.empty()) {
	//	return -1;
	//}
	for (int i = 0; i < DegiskenListesi.size(); i++) {
		if (!DegiskenListesi[i]->DegerGirildi) {
			return -1;
		}
	}
	sonuc = IslemleriHesapla();
	return sonuc;
}

const std::string Formul::getFormulIsmi() const
{
	return this->FormulIsmi;
}

const std::string Formul::getFormulAciklama()const
{
	return this->FormulAciklamasi;
}

const std::string Formul::getFormulYazim()const
{
	return this->FormulYazimi;
}

void Formul::setFormulIsmi(std::string isim)
{
	this->FormulIsmi = isim;
}

void Formul::setFormulAciklama(std::string aciklama)
{
	this->FormulAciklamasi = aciklama;
}

void Formul::setFormulYazim(std::string yazim)
{
	this->FormulYazimi = yazim;
}

bool Formul::getFormulAtandi()
{
	if (IslemListesi.empty()) {
		return false;
	}
	return true;
}

Formul::Degisken::Degisken(const Degisken& degisken) {
	Sembol = degisken.Sembol;
	Indis = degisken.Indis;
	Tanim = degisken.Tanim;
	Isim = degisken.Isim;
	Deger = degisken.Deger;

};

Formul::Degisken::Degisken() {

}

void Formul::setDegisken(const int& id, const char* sembol, const char* indis, const char* tanim, const char* isim, float* deger)
{
	if (sembol != nullptr) { this->DegiskenListesi[id]->Sembol = sembol; }
	if (indis != nullptr) { this->DegiskenListesi[id]->Indis = indis; }
	if (tanim != nullptr) { this->DegiskenListesi[id]->Tanim = tanim; }
	if (deger != nullptr) { this->DegiskenListesi[id]->Deger = *deger; this->DegiskenListesi[id]->DegerGirildi = true; }

}

const std::string Formul::getDegiskenSembol(const int& id) const
{
	return this->DegiskenListesi[id]->Sembol;
}
const std::string Formul::getDegiskenIndis(const int& id)const
{
	return this->DegiskenListesi[id]->Indis;
}
const std::string Formul::getDegiskenTanim(const int& id)const
{
	return this->DegiskenListesi[id]->Tanim;
}
const std::string Formul::getDegiskenIsim(const int& id)const
{
	return this->DegiskenListesi[id]->Isim;
}
const float Formul::getDegiskenDeger(const int& id) const
{
	return this->DegiskenListesi[id]->Deger;
}

bool Formul::getDegiskenDegerAtandi(const int& id) const {
	return DegiskenListesi[id]->DegerGirildi;
}

void Formul::setDegiskenSembol(const int& id, const std::string& sembol)
{
	this->DegiskenListesi[id]->Sembol = sembol;
}

void Formul::setDegiskenIndis(const int& id, const std::string& indis)
{
	this->DegiskenListesi[id]->Indis = indis;
}

void Formul::setDegiskenTanim(const int& id, const std::string& tanim)
{
	this->DegiskenListesi[id]->Tanim = tanim;
}

void Formul::setDegiskenIsim(const int& id, const std::string& isim)
{
	this->DegiskenListesi[id]->Isim = isim;
}

void Formul::setDegiskenDeger(const int& id, const float& deger)
{
	this->DegiskenListesi[id]->Deger = deger;
	this->DegiskenListesi[id]->DegerGirildi = true;
}

