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

void Formul::FormulCozumle()
{
	std::string yazimGecici = FormulYazimi;
	yazimGecici.insert(0, "(");
	yazimGecici.push_back(')');

	std::vector<char> ozelKarakterler = { '(', ')', '^', '+', '-', '*', '/', '!'};
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
			const int posBasla = i;
			int posBitis = i;
			bool bitisPosBulundu = false;
			while (!bitisPosBulundu) {
				posBitis++;
				char siradakiKarakter = yazimGecici[posBitis];
				if ((false ==
					(BuyukHarfKont(siradakiKarakter) || // Büyük harfler
						KucukHarfKont(siradakiKarakter) || // Küçük harfler
						RakamKont(siradakiKarakter)  // rakamlar
						))) // ondalık sayılar için nokta
				{
					bitisPosBulundu = true;
				}
			}

			std::string degiskenIsim = yazimGecici.substr(i, posBitis - i);
			bool ozelIfade = false;
			//Özel bir işlem olmadığını kontrol et (cos, sin, tan, ...)
			for (int k = 0; k < islemTuruTekSayisi; k++) {

				if (degiskenIsim == islemTuruTek[k]) {
					ozelIfade = true;
					i += degiskenIsim.size() - 1;
					break;
				}

			}
			for (int k = 0; k < islemTuruCiftSayisi; k++) {

				if (degiskenIsim == islemTuruCift[k]) {
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
					degiskenNum.append(KimlikOlustur(j));

					kayitli = true;
					break;
				}
			}

			if (!kayitli) {

				//degiskenNum.append(std::to_string(DegiskenListesi.size()));
				degiskenNum.append(KimlikOlustur(DegiskenListesi.size()));

				Degisken* degisken = new Degisken;
				degisken->Isim = degiskenIsim;
				degisken->Sembol = degiskenIsim;
				DegiskenListesi.push_back(degisken);
			}
			yazimGecici.erase(posBasla, posBitis - posBasla);
			yazimGecici.insert(posBasla, degiskenNum);
			i += abs(posBitis - posBasla - 1 - haneSayisi); // silinen karakterin uzunluğu - 1(d harfi) - haneSayısı

		}
	}

	//Sabit sayıları yeniden isimlendir örn: 12 -> s01(12)
	//Sabit sayılar listesine değerleri ekle
	{
		int say = 0;
		std::string sabit;
		while (say < yazimGecici.size()) {
			size_t posBas = 0, posSon = 0;
			for (int j = 0; j < ozelKarakterler.size(); j++) {
				if (yazimGecici[say] == ozelKarakterler[j]) {
					int baslangic = static_cast<int>(yazimGecici[say + 1]);
					if (RakamKont(baslangic)) {
						posBas = say + 1;
						posSon = posBas + 1;
					}
					break;
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
				double sabitd = std::stod(sabit);

				//Formülün tutulduğu stringe yazdırılacak deger örn: s01 s12
				std::string sabitID = "s" + (KimlikOlustur(SabitListesi.size()));

				//Sabit değerlerin tutulduğu listeye değeri ekle
				SabitListesi.push_back(sabitd);

				//Formülün tutulduğu stringden sayıları sil
				yazimGecici.erase(posBas, sabit.size());

				//Formülün tutulduğu stringe sabitin bulunduğu numarayı yaz örn: s01 s12
				yazimGecici.insert(posBas, sabitID);
				say = posSon;
				say += degerUzunluk - (static_cast<int>(posSon) - static_cast<int>(posBas)); // silinen karakterin uzunluğu - 1(d harfi) - haneSayısı

				sabit.erase();
			}
			else {
				say++;
			}
		}
	}

	//Çift değişkene bağımlı olan işlemlerin önce yapılması gerekir
	//Tek değişkene bağımlı olan işlemlerin başına ve sonuna parantez koy
	/*
	#Hazırlık
	1.İşleme ait parantezi bul konumunu posBaşlangıç olarak isimlendir
	2.posBaşlangıçGeçici'yi tanımla posBaşlangıç'a eşitle
	3.posBaşlangıçGeçici'den sonraki ilk parantez kapatmayı bul, posSonGeçici'ye eşitle.

	#Döngü
	4.posBaşlangıçGeçici'den sonraki ilk parantez açmanın komunu bul ve posBaşlangıçGeçici'ye eşitle

	#Koşul
	5.posBaşlangıçGeçici'den posSonGeçici'yi Çıkar
	6.ifade negatif ise posSonGeçici'den sonraki ilk parantez kapatmayı bul, posSonGeçici'ye eşitle. Adım dörde dön.

	7.posBaşlangıç - işlemUzunluğu konumuna parantez açma, posSonGeçici + 1 konumuna parantez kapatma işareti koy
	8.Sonraki tek değişkene bağımlı işlemi ara. İşlem bulunursa adım bire dön

	*/

	for (size_t i = 0; i < (islemTuruTekSayisi - 1) ; i++) {
		size_t pos = 0;
		while (yazimGecici.find(islemTuruTek[i], pos) != yazimGecici.npos) {
			//Başlangıç değerleri
			long long int posBaslangic = yazimGecici.find(islemTuruTek[i]);
			long long int posBaslangicGecici = posBaslangic;
			long long int posSonGecici = yazimGecici.find(")", posBaslangicGecici);

			//Doğru parantezi bulmak için gerekli döngü
			while (true) {
				posBaslangicGecici = yazimGecici.find("(", posBaslangicGecici + 1);
				if (( (posSonGecici) - (posBaslangicGecici) ) < 0) {
					posSonGecici = yazimGecici.find(")", posSonGecici + 1);
				}
				else {
					break;
				}
			}

			//Yeni parantezleri ekleme
			yazimGecici.insert(posBaslangic, "(");
			yazimGecici.insert(posSonGecici + 1, ")");

			pos = yazimGecici.find(islemTuruTek[i], pos) + std::strlen(islemTuruTek[i]);
			
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


		//Parantez içi işlemeri yap
		while(yazimGecici.find("(") != yazimGecici.npos) {
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
						if (yazimGecici[posNegatif - 1] == '(') {
							Islem* islem = new Islem;
							const int islemUzunluk = degerUzunluk + 1;
							islem->islemStr = yazimGecici.substr(posNegatif, islemUzunluk);
							std::string islemID = "I";

							islemID.append(KimlikOlustur(IslemListesi.size()));


							islem->degerIlk = islem->islemStr.substr(1);
							islem->islemTip = IslemTipi::NEG;
							yazimGecici.erase(posNegatif, islemUzunluk);
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
						if (yazimGecici[posPozitif - 1] == '(') {

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
			for (int i = 0; i < islemTuruCiftSayisi; i++) { // işlemi algılamıyor
				IslemAraDegistirCift(islemTuruCift[i], posa, posb, yazimGecici);
			}

			//Tek değişkene bağımlı işlemler
			// negatif ifade önceden bulunduğu için "-" yoksayılıyor
			for (int i = 0; i < islemTuruTekSayisi - 1; i++) { 
				IslemAraDegistirTek(islemTuruTek[i], posa, posb, yazimGecici);
			}

			//Parantez içinde işlem kaldı mı
			if ((posb - posa) == degerUzunluk + 1) {
				yazimGecici.erase(posb, 1);
				yazimGecici.erase(posa, 1);
			}
		}

	}

}

//Belirtilen işlemi başlangıç pozisyonunda ve son pozisyon arasında metinde arar. 
//İşlem bulunursa metinde değişken ve sabit yerine I ön ekini ekler ve numaralandırır.
//İki farklı değişken veya sabite ihtiyaç duyulduğu zaman kullanılır. Çift bağımlılık
// örn: d01+s02 -> I01
void Formul::IslemAraDegistirCift(std::string aranacakIslem, size_t& posBas, size_t& posSon, std::string& metin) {
	size_t posIslem = metin.find(aranacakIslem, posBas);
	while (posIslem >= posBas && posIslem <= posSon) {
		posIslem = metin.find(aranacakIslem, posBas);
		if ((posIslem != std::string::npos) && (posIslem < posSon)) {
			Islem* islem = new Islem;

			islem->islemStr = metin.substr(posIslem - degerUzunluk, (degerUzunluk * 2) + aranacakIslem.size());
			islem->degerIlk = islem->islemStr.substr(0, degerUzunluk);
			islem->degerIki = islem->islemStr.substr(degerUzunluk + aranacakIslem.size(),
				islem->islemStr.size() - degerUzunluk - 1);

			std::string islemID = "I";

			islemID.append(KimlikOlustur(IslemListesi.size()));

			metin.erase(posIslem - degerUzunluk, (degerUzunluk * 2) + aranacakIslem.size());
			metin.insert(posIslem - degerUzunluk, islemID);

			islem->islemTip = StringdenIslemTipi(aranacakIslem);

			IslemListesi.push_back(islem);
			posSon -= degerUzunluk + aranacakIslem.size(); // silinen karakterler için

		}
	}
}

//Belirtilen işlemi başlangıç pozisyonunda ve son pozisyon arasında metinde arar. 
//İşlem bulunursa metinde değişken veya sabit yerine I ön ekini ekler ve numaralandırır.
//Tek değişkene / sabite ihtiyaç duyulduğu zaman kullanılır. Tek bağımlılık
// örn: (islem)d01 -> I01
void Formul::IslemAraDegistirTek(std::string aranacakIslem, size_t& posBas, size_t& posSon, std::string& metin) {
	size_t posIslem = 0;
	size_t posBas_ = posBas;
	posIslem = metin.find(aranacakIslem, posBas_ - aranacakIslem.size());
	while (posIslem >= posBas - aranacakIslem.size() && posIslem <= posSon) {
		posBas_ -= aranacakIslem.size();
		posIslem = metin.find(aranacakIslem, posBas_);
		if ((posIslem != std::string::npos) && (posIslem < posSon)) {
			Islem* islem = new Islem;

			//islem->islemStr = metin.substr(posBas + 4, 3); eski kod
			islem->islemStr = metin.substr(posBas_ + aranacakIslem.size() + 1, degerUzunluk);
			islem->degerIlk = islem->islemStr.substr(islem->islemStr.size() - degerUzunluk, islem->islemStr.size() - islem->islemStr.size() - degerUzunluk);
			std::string islemID = "I";

			islemID.append(KimlikOlustur(IslemListesi.size()));


			metin.erase(posIslem, aranacakIslem.size() + degerUzunluk + 2);
			metin.insert(posIslem, islemID);

			islem->islemTip = StringdenIslemTipi(aranacakIslem);

			IslemListesi.push_back(islem);
			//posSon -= aranacakIslem.size() + degerUzunluk; // silinen karakterler için
			posSon -= aranacakIslem.size() + 1; // silinen karakterler için
			posBas -= aranacakIslem.size() + 1;

		}
	}
}

void Formul::IslemUzunlukGuncelle() {

}

std::string Formul::KimlikOlustur(unsigned int numara)
{
	size_t alfabeListeBoyutu = sizeof(alfabeListesi);
	int mevcutHane = haneSayisi;
	std::string yeniNumara;

	double bolen = powf(alfabeListeBoyutu, mevcutHane); // 62^5 => (alfabe uzunluğu)^5


	while (mevcutHane != 1) {

		unsigned int alfabeKarakterID = numara;
		alfabeKarakterID = alfabeKarakterID / bolen;

		yeniNumara.push_back(alfabeListesi[alfabeKarakterID]);

		numara -= alfabeKarakterID * bolen;
		bolen = bolen / alfabeListeBoyutu;
		mevcutHane--; // Sonraki(sağdaki) haneye geç
	}

	yeniNumara.push_back(alfabeListesi[numara]); // Hane 0'a kalan değeri ata

	return yeniNumara;
}

size_t Formul::KimliktenDeger(std::string kimlik)
{
	size_t yeniDeger = 0;
	size_t kimlikUzunluk = kimlik.size();
	size_t strPos = 0;


	while (kimlikUzunluk != 0) {
		yeniDeger = std::pow(search(alfabeListesi, (alfabeListesi + alfaListesiUzunluk), kimlik[strPos]), kimlikUzunluk);
		kimlikUzunluk--;
		strPos++;
	}

	return yeniDeger;
}

const double Formul::IslemleriHesapla() {
	if (!IslemSonucListesi.empty()) { IslemSonucListesi.clear(); }
	for (int i = 0; i < IslemListesi.size(); i++) {
		double deger[2]{ 0,0 }, sonuc = 0;
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
			std::string idStr = degerStr[j].substr(1, haneSayisi);
			int id = KimliktenDeger(idStr);
			switch (karakter)
			{
			case 'I': {
				std::string idStr = degerStr[j].substr(1, degerStr[j].size() - 1);
				int id = std::stoi(idStr);
				deger[j] = IslemListesi[id]->islemSonuc;
				break;
			}

			case 'd': {
				deger[j] = DegiskenListesi[id]->getDeger();
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
				sonuc = log10(deger[0]);
				break;
			}
			case IslemTipi::LN: {
				sonuc = log(deger[0]);
				break;
			}
			case IslemTipi::FAK: {
				int n = deger[0];
				sonuc = deger[0];
				if (n == 0) {
					sonuc = 0;
					break;
				}
				else if(n == 1) {
					sonuc = 1;
					break;
				}
				while (--n > 0) {
					
					sonuc = sonuc * n;
					
				}
				break;
			}
			case IslemTipi::MOD: {
				sonuc = std::fmodf(deger[0], deger[1]);
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

const double Formul::getSonuc()
{
	//if (DegiskenListesi.empty()) {
	//	return -1;
	//}
	for (int i = 0; i < DegiskenListesi.size(); i++) {
		if (!DegiskenListesi[i]->DegerGirildi()) {
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
	deger = degisken.deger;

};

Formul::Degisken::Degisken() {

}

void Formul::Degisken::setDeger(const double& deger_)
{
	deger = deger_; degerGirildi = true;
}

double Formul::Degisken::getDeger()
{
	return deger;
}

bool Formul::Degisken::DegerGirildi()
{
	return degerGirildi;
}

void Formul::setDegisken(const int& id, const char* sembol, const char* indis, const char* tanim, const char* isim, double* deger)
{
	if (sembol != nullptr) { this->DegiskenListesi[id]->Sembol = sembol; }
	if (indis != nullptr) { this->DegiskenListesi[id]->Indis = indis; }
	if (tanim != nullptr) { this->DegiskenListesi[id]->Tanim = tanim; }
	if (deger != nullptr) { this->DegiskenListesi[id]->setDeger(*deger); }


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

const double Formul::getDegiskenDeger(const int& id) const
{
	return this->DegiskenListesi[id]->getDeger();
}

bool Formul::getDegiskenDegerAtandi(const int& id) const {
	return DegiskenListesi[id]->DegerGirildi();
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

void Formul::setDegiskenDeger(const int& id, const double& deger)
{
	this->DegiskenListesi[id]->setDeger(deger);
}

