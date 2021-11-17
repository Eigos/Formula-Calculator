

#include <iostream>
#include <string>
#include <vector>
//#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <fstream>
#include "raylib/raylib-cpp.hpp"

#include "formul.h"
#include "kontrol.h"

#define DEBUG 1


Kontrol kontrol;

const char* DosyaVarsYol = { "FormulHesaplama.mBin" };
const static int PrecisionCount = 9;

//void DeneyKumesiMenu();
void DeneylerMenu();
void DeneyMenu(int deneyID);
void AnaMenu();
void FormulMenu();
void AyarlarMenu();

void DosyayaKayit();
void DosyadanOku(const char* dosyaYolu);

void YeniFormulOlusturma();
int KomutKontrol(std::string metin);
std::string KomutGerceklestir(std::string metin);
std::string KomutGerceklestir(std::string metin, int komutID);

inline int GirdiAlInt();
inline double GirdiAlDouble();
inline int GirdiAlIntMenu();
inline std::string GirdiAlStr();

void ClearScreenWin();

template <typename Val>
bool cmp(Val Fval, Val Sval) { return Fval == Sval; }
double setPrecision(const double sayiD, int i = PrecisionCount);

bool TestIslem(std::string formul, double beklenenSonuc) {
	Formul testFormul(formul);
	std::cout << std::endl << formul.c_str() << "\t Test ediliyor...\n";

	for (int i = 0; i < testFormul.getDegiskenSayisi(); i++) {
		std::cout << testFormul.getDegiskenIsim(i) << ": ";

		testFormul.setDegiskenDeger(i, GirdiAlDouble());
		std::cout << std::endl;
	}

	return cmp(setPrecision(testFormul.getSonuc(), PrecisionCount), beklenenSonuc);
}

bool TestIslem(std::string formul, double beklenenSonuc, const std::vector<double>& degerler) {
	Formul testFormul(formul);
	std::cout << std::endl << formul.c_str() << "\t Test ediliyor...\n";

	for (int i = 0; i < degerler.size(); i++) {
		std::cout << testFormul.getDegiskenIsim(i) << ": " << degerler[i] << std::endl;
		testFormul.setDegiskenDeger(i, degerler[i]);
	}

	return cmp(setPrecision(testFormul.getSonuc(), PrecisionCount), beklenenSonuc);
}

double setPrecision(const double sayiD, int i) {
	std::string sayiStr = std::to_string(sayiD).substr(0, i);
	double number = std::stod(sayiStr);
	return number;
}

void sonucYazdir(bool t) {
	if (t) {
		std::cout << "DOGRU\n";
		return;
	}

	std::cout << "YANLIS\n";
}

int main(int argCount, char* argv[])
{
	bool onlyCalculation = false;

	if (argCount > 1) {
		for (int i = 1; i < argCount; i++) {
			if (KomutKontrol(argv[1]) != -1) {
				std::cout <<  KomutGerceklestir(argv[1]).c_str() << std::endl;
				onlyCalculation = true;
			}
		}
	}

	
	bool UI = true;
	while (!onlyCalculation) {
		if (UI) {
			AnaMenu();
		}
		KomutGerceklestir(GirdiAlStr());
	}

	return 0;
}




void AnaMenu() {
	std::cout << "1 Deneyler menu\t2 Kayit \n";
	int girdi = GirdiAlInt();
	if (girdi == 1) {
		DeneylerMenu();
	}
	else if (girdi == 2) {
		AyarlarMenu();
	}
}

void DeneylerMenu() {
	bool MenuKapat = false;
	while (!MenuKapat) {
		//Menü işlemleri
		std::cout << "Deneyler Menu" << std::endl;
		std::cout << "<1>Yeni Deney Ekle <2>Deney Sil <3>Deney Sec <4>Formul Menusu <0>Deney Menusunden Cik\n";
		switch (GirdiAlIntMenu())
		{
			//1-Yeni deney ekle
		case 1: {
			if (kontrol.DeneyYeni() != -1) {
				ClearScreenWin();
				std::cout << "Yeni deney olusturuldu!\n";
			}
			else {
				std::cerr << "BEKLENMEDIK HATA!!!\n";
			}
		}break;

			//2-Deney sil
		case 2: {

			if (kontrol.getDeneySayisi() == 0) {
				ClearScreenWin();
				std::cout << "Kayitli deney bulunamadi!\n";
				break;
			}

			size_t deneySayisi = kontrol.getDeneySayisi();
			std::cout << "Kayitli Deneyler: \n";
			for (int i = 0; i < deneySayisi; i++) {
				std::cout << "<" << i + 1 << ">" << kontrol.getDeneyAdi(i) << std::endl;
			}

			std::cout << "Silemk istediginiz deneyi seciniz: ";
			int deneyNo = GirdiAlInt();
			std::cout << std::endl;

			if ((deneyNo > 0) && (deneyNo <= deneySayisi)) {
				deneyNo--;
				const Deney* deney = kontrol.getDeney(deneyNo);
				if (kontrol.DeneySil(deneyNo) == Status::Done) {
					ClearScreenWin();
					std::string deneyKulForm(deney->getFormulYazim());
					const int deneyGosUzunluk = 35; // Deneyin formülünün gösterilicek uzunluğu

					if (deneyKulForm.size() > deneyGosUzunluk) {
						deneyKulForm.resize(deneyGosUzunluk);
						deneyKulForm += "...";
					}

					std::cout << "'" << deney->DeneyAdi.c_str() << "\t" << deneyKulForm.c_str() << "' Basariyla silinmistir!\n";
				}
				else {
					std::cerr << "BEKLENMEDIK HATA!!!\n";
				}
			}
			else {

				ClearScreenWin();
				std::cout << "Secim isleminiz iptal edilmistir!\n";
				break;
			}
		}break;

			//3-Deney Sec
		case 3: {
			//Deney Seç
			size_t deneySayisi = kontrol.getDeneySayisi();
			if (deneySayisi > 0) {
				std::cout << "Kayitli Deneyler: \n";
				for (int i = 0; i < deneySayisi; i++) {
					const Deney* deney = kontrol.getDeney(i);
					std::string deneyKulForm(deney->getFormulYazim());
					const int deneyGosUzunluk = 35; // Deneyin formülünün gösterilicek uzunluğu

					if (deneyKulForm.size() > deneyGosUzunluk) {
						deneyKulForm.resize(deneyGosUzunluk);
						deneyKulForm += "...";
					}
					std::cout << "<" << i + 1 << ">" << deney->DeneyAdi.c_str() << ":\t"
						<< deneyKulForm.c_str() << std::endl;
				}
				std::cout << "<0> Islemi iptal et\n";
				std::cout << "Secmek istediginiz deneyin numarasi: ";
				int deneyNo = GirdiAlInt();
				if (deneyNo == 0) {
					ClearScreenWin();
					std::cout << "Secim isleminiz iptal edilmistir!\n";
					break;
				}

				deneyNo--;

				if (kontrol.getDeneySayisi() > deneyNo && deneyNo >= 0) {
					DeneyMenu(deneyNo);
				}

			}
			else {
				std::cout << "Kayitli deneyiniz bulunmamakta!\n";
				std::cout << "Yeni bir deney olusturmak ister misiniz?\n";
				std::cout << "<1>Evet <2>Hayir\n";
				if (GirdiAlInt() == 1) {
					kontrol.DeneyYeni();
					DeneyMenu(0);
					ClearScreenWin();
					std::cout << "Yeni deney olusturuldu!\n";
				}
			}

		}break;

			//4-Formül menüsü
		case 4: {
			FormulMenu();
		}break;

			//-2 -Komut girildi
		case -2: {
		}break;

			//0-Deney menüsünden çık
		case 0: {
			ClearScreenWin();
			std::cout << "Deney menusunden cikiliyor...\n";
			MenuKapat = true;

		}break;

		default:
			ClearScreenWin();
			std::cerr << "Hatali girdi!\n";
			break;
		}


	}

}

void DeneyMenu(int deneyID) {

	//değişken düzenleme, formul seçme, isim düzenleme ve menüden çıkma seçenkleri olmalı 

	bool DeneyMenuKapat = false;
	ClearScreenWin();

	while (!DeneyMenuKapat) {
		Deney* deney = kontrol[deneyID];
		std::cout << "'" << deney->DeneyAdi.c_str() << "' isimli deneyin menusu\n";
		std::cout << "<1> Degisken duzenleme <2>Formul secme <3>Deney ismi duzenleme <4>Deney Sonucu <0>Menuden cikis\n";
		switch (GirdiAlIntMenu())
		{
			//Degisken duzenleme
		case 1: {
			if (!deney->getFormulAtandi()) {
				std::cout << "Formul atanmadi. Lutfen once formul atayiniz.\n";
				break;
			}

			int degiskenSayisi = deney->getDegiskenSayisi();

			//Değişken için yeni değer al
			for (int i = 0; i < degiskenSayisi; i++) {
				std::string sembol = deney->getDegiskenIsim(i);
				double degerEski = deney->getDegiskenDeger(i);
				std::cout << "Eski (" << sembol << ")" << "Degisken: " << degerEski << std::endl;
				std::cout << "Yeni (" << sembol << ")" << "Degisken: ";
				std::string degerYeni = GirdiAlStr();
				std::cout << std::endl;
				if (degerYeni.size() != 0) {
					double degerYenid = std::stod(degerYeni);
					deney->setDegiskenDeger(i, degerYenid);
				}
			}

		}break;

			//Deney icin yeni formulu sec
		case 2: {

			if (kontrol.FormulVar() != 0) {
				std::cout << "Deneyiniz icin kullanacaginiz yeni formulu seciniz.\n";

				for (int i = 0; i < kontrol.getFormulSayisi(); i++) {

					const Formul* formul = kontrol.getFormul(i);
					std::cout << "<" << i + 1 << "> " << formul->getFormulIsmi().c_str() << std::endl;
				}


				int secilenFormulID = GirdiAlInt();
				if (secilenFormulID > 0) {
					secilenFormulID--;
					const Formul* secilenFormul = kontrol.getFormul(secilenFormulID);

					std::cout << "'" << secilenFormul->getFormulIsmi().c_str() << "'" << " isimli formul secildi!\n";
					kontrol.DeneyFormulKullan(secilenFormulID, deney);

				}
				else {
					std::cout << "Secim iptal edildi";
				}
			}
			else {
				std::cout << "Kayitli formul bulunamadi.\n";
			}
		}break;

			//Deney ismi duzenleme 
		case 3: {
			ClearScreenWin();
			std::cout << "Degisiklik yapmak istemiyorsaniz enter'a basin\n";
			std::cout << "Deneyin eski adi: " << deney->DeneyAdi.c_str() << std::endl;
			std::cout << "Deneyin yeni adi: ";
			std::string yeniAd = GirdiAlStr();
			if (!yeniAd.empty()) {
				deney->DeneyAdi = yeniAd;
			}

		}break;

			//Deney Sonucunu bul
		case 4: {
			ClearScreenWin();
			std::cout << "Deneyin sonucu : " << deney->getSonuc();
			std::cout << std::endl;
		}break;

			//-2 -Komut girildi
		case -2: {
			break;
		}

			   //Menuden cikis
		case 0: {
			DeneyMenuKapat = true;
		}break;

		default: {
			std::cout << "\nHatali girdi!!!\n";
		}
			   break;
		}

	}

}

void AyarlarMenu() {
	std::cout << "<1> Formul ve Deneyi kaydet <2> Kayitli dosyadan Formul ve Deneyleri al (!Mevcut deney ve formuller SILINIR!) <3> Ayarlar Menusunden Cik \n";

	switch (GirdiAlIntMenu()) {

	case 1: {

		DosyayaKayit();
		break;
	}

	case 2: {
		DosyadanOku(DosyaVarsYol);
		break;
	}

	case 3: {

		break;
	}

		  //-2 -Komut girildi
	case -2: {
		break;
	}

	default: {

		std::cout << "\nHatali girdi";
		break;
	}
	}

	ClearScreenWin();
}


// DENEY
/*
// 1 - Deney Sayısı
// 2 - Deney Adı
// 3 - Formül ismi
// 4 - Formul Yazımı
// 5 - Formul Açıklama
// 6 - Değişken Sayısı
// 7 - Değişken isim
// 8 - Değişken indis
// 9 - Değişken sembol
// 10- Değişken tanım
// 11- Değişken değer
// 12- Değişken Değer atandı
*/
// FORMUL
/*
// 1 - Formul Sayısı
// 2 - Formul ismi
// 3 - Formül Yazımı
// 4 - Formül açıklaması
// 5 - Değişken sayısı
// 6 - Değişken isim
// 7 - Değişken indis
// 8 - Değişken sembol
// 9 - Değişken tanım
// 10- Değişken değer
// 11- Değişken değer atandı
*/
void DosyayaKayit() {
	std::fstream outFile;

	size_t bufferSize = 1024 * 100;
	std::string buffer;

	//Deney veya formul yoksa çık
	if (!(kontrol.getDeneySayisi() || kontrol.getFormulSayisi())) {
		std::cerr << "Dosyaya yazilacak deney veya formul bulunamadi!" << std::endl;
		return;
	}

	buffer += std::to_string(kontrol.getDeneySayisi());
	buffer += "\n";
	for (int i = 0; i < kontrol.getDeneySayisi(); i++) {
		Deney* deney = kontrol[i];
		buffer += deney->DeneyAdi + "\n" + deney->getFormulIsmi() + "\n" + deney->getFormulYazim() + "\n" + deney->getFormulAciklama() + "\n";
		buffer += std::to_string(deney->getDegiskenSayisi());
		buffer += "\n";

		for (int j = 0; j < deney->getDegiskenSayisi(); j++) {
			buffer += deney->getDegiskenIsim(j) + "\n" + deney->getDegiskenIndis(j) + "\n" + deney->getDegiskenSembol(j) +
				"\n" + deney->getDegiskenTanim(j) + "\n";
			buffer += std::to_string(deney->getDegiskenDeger(j)) + "\n" + std::to_string(deney->getDegiskenDegerAtandi(j)) + "\n";
		}
	}

	buffer += std::to_string(kontrol.getFormulSayisi());
	buffer += "\n";
	for (int i = 0; i < kontrol.getFormulSayisi(); i++) {
		const Formul* formul = kontrol.getFormul(i);

		buffer += formul->getFormulIsmi() + "\n" + formul->getFormulYazim() + "\n" + formul->getFormulAciklama() + "\n";
		buffer += std::to_string(formul->getDegiskenSayisi());
		buffer += "\n";
		for (int j = 0; j < formul->getDegiskenSayisi(); j++) {
			buffer += formul->getDegiskenIsim(j) + "\n" + formul->getDegiskenIndis(j) + "\n" + formul->getDegiskenSembol(j) +
				"\n" + formul->getDegiskenTanim(j) + "\n";
			buffer += std::to_string(formul->getDegiskenDeger(j)) + "\n" + std::to_string(formul->getDegiskenDegerAtandi(j)) + "\n";
		}
	}

	outFile.open("FormulHesaplama.mBin", std::ios::out | std::ios::trunc);

	if (outFile.is_open())
	{
		outFile.write(buffer.c_str(), buffer.size());

	}
	else {
		std::cout << "Dosya acilamadi!\n";
		return;
	}

	if (outFile.is_open()) {
		outFile.close();
	}
}

// Dosyadan yükle ve yer değiştir
void DosyadanOku(const char* dosyaYolu) {
	std::ifstream dosya;

	std::string temp;

	dosya.open(dosyaYolu);

	if (!dosya.is_open()) {
		std::cerr << "\nDosya okunamadi\nDevam etmek icin herhangi bir tusa basin..." << std::endl;
		ClearScreenWin();
		return;
	}

	char backSpace = 8;
	size_t deneySayisi = 0;

	std::getline(dosya, temp);
	if (!temp.empty()) {
		deneySayisi = std::stoull(temp);
		kontrol.Temizle();
	}

	for (size_t i = 0; i < deneySayisi; i++) {

		Deney* yeniDeney;
		std::string deneyAdi;
		std::string formulIsmi;
		std::string formulYazimi;
		std::string formulAciklama;


		std::getline(dosya, deneyAdi);
		std::getline(dosya, formulIsmi);
		std::getline(dosya, formulYazimi);
		std::getline(dosya, formulAciklama);

		yeniDeney = new Deney(deneyAdi.c_str());

		yeniDeney->setFormulIsmi(formulIsmi);
		yeniDeney->setFormulYazim(formulYazimi);
		yeniDeney->setFormulAciklama(formulAciklama);

		std::getline(dosya, temp);

		size_t degiskenSayisi = 0;
		degiskenSayisi = std::stoull(temp);

		for (size_t j = 0; j < degiskenSayisi; j++) {

			std::string degiskenIsmi;
			std::string degiskenIndis;
			std::string degiskenSembol;
			std::string degiskenTanim;
			std::string degiskenDegerS;
			std::string degiskenDegerAtandi;

			double degiskenDegerD = 0;


			std::getline(dosya, degiskenIsmi);
			std::getline(dosya, degiskenIndis);
			std::getline(dosya, degiskenSembol);
			std::getline(dosya, degiskenTanim);
			std::getline(dosya, degiskenDegerS);
			std::getline(dosya, degiskenDegerAtandi);


			degiskenDegerD = std::stod(degiskenDegerS);

			yeniDeney->setDegisken(j, degiskenSembol.c_str(), degiskenIndis.c_str(), degiskenTanim.c_str(), degiskenIsmi.c_str(), &degiskenDegerD);
		}

		kontrol.DeneyYeni(yeniDeney);


	}

	size_t formulSayisi = 0;

	std::getline(dosya, temp);
	if (!temp.empty()) {
		formulSayisi = std::stoull(temp);
		kontrol.Temizle();
	}

	for (size_t i = 0; i < formulSayisi; i++) {

		Formul* yeniFormul;
		std::string formulIsmi;
		std::string formulYazimi;
		std::string formulAciklama;


		std::getline(dosya, formulIsmi);
		std::getline(dosya, formulYazimi);
		std::getline(dosya, formulAciklama);

		yeniFormul = new Formul(formulYazimi, formulIsmi, formulAciklama);

		yeniFormul->setFormulIsmi(formulIsmi);
		yeniFormul->setFormulYazim(formulYazimi);
		yeniFormul->setFormulAciklama(formulAciklama);

		std::getline(dosya, temp);

		size_t degiskenSayisi = 0;
		degiskenSayisi = std::stoull(temp);

		for (size_t j = 0; j < degiskenSayisi; j++) {

			std::string degiskenIsmi;
			std::string degiskenIndis;
			std::string degiskenSembol;
			std::string degiskenTanim;
			std::string degiskenDegerS;
			std::string degiskenDegerAtandi;

			double degiskenDegerD = 0;


			std::getline(dosya, degiskenIsmi);
			std::getline(dosya, degiskenIndis);
			std::getline(dosya, degiskenSembol);
			std::getline(dosya, degiskenTanim);
			std::getline(dosya, degiskenDegerS);
			std::getline(dosya, degiskenDegerAtandi);


			degiskenDegerD = std::stod(degiskenDegerS);

			yeniFormul->setDegisken(j, degiskenSembol.c_str(), degiskenIndis.c_str(), degiskenTanim.c_str(), degiskenIsmi.c_str(), &degiskenDegerD);
		}

		kontrol.FormulYeni(yeniFormul);

	}

	dosya.close();

}

void FormulMenu() {
	//yeni formül - formül sil - formuül düzenle
	ClearScreenWin();
	bool FormulMenuKapat = false;

	while (!FormulMenuKapat) {

		std::cout << "Formul Menusu!\n";
		std::cout << "<1> Yeni Formul <2>Formul Sil <3>Formul Duzenle <0>Menuden Cikis\n";

		switch (GirdiAlIntMenu())
		{
			//Yeni Formül oluşturma
		case 1: {
			ClearScreenWin();
			YeniFormulOlusturma();
			ClearScreenWin();
			std::cout << "\nFormulunuz basariyla olusturuldu!\n";
		}break;

			//Formül silme
		case 2: {

			if (!kontrol.FormulVar()) {
				std::cout << "\nKayitli formul bulunamadi. Yeni formul olusturmak ister misiniz?\n";
				std::cout << "<1>Evet\t<2>Hayir";
				if (GirdiAlInt() == 1) {
					ClearScreenWin();
					YeniFormulOlusturma();
				}
				break;
			}

			for (int i = 0; i < kontrol.getFormulSayisi(); i++) {
				std::cout << "<" << i + 1 << ">" << kontrol.getFormul(i)->getFormulIsmi().c_str();
			}
			std::cout << "Islemi iptal etmek icin <0>\n";
			std::cout << "Silmek istediginiz formulu seciniz: ";
			int silinecekFormulID = GirdiAlInt();
			std::cout << std::endl;

			ClearScreenWin();
			if (silinecekFormulID < kontrol.getFormulSayisi()) {
				kontrol.FormulSil(silinecekFormulID);
			}
			else {
				std::cout << "Hatali girdi!!!";
			}


		}break;

			//Formül düzenleme
		case 3: {
			ClearScreenWin();
			std::string yazim;
			std::string aciklama;
			std::string isim;

			if (!kontrol.FormulVar()) {
				std::cout << "Kayitli formul bulunamadi!\n";
				break;
			}
			for (int i = 0; i < kontrol.getFormulSayisi(); i++) {
				std::cout << "<" << i + 1 << ">" << kontrol.getFormul(i)->getFormulIsmi().c_str() << std::endl;
			}
			std::cout << "Duzenlemek istediginiz formulu secini: ";
			int secim = GirdiAlInt();
			secim--;
			if (secim < 0 || secim >= kontrol.getFormulSayisi()) {
				break;
			}

			std::string eskiYazim = kontrol.getFormul(secim)->getFormulYazim().c_str();
			std::cout << "Eski formulunuz: ";
			std::cout << eskiYazim.c_str() << std::endl;
			std::cout << "Formul: ";
			yazim = GirdiAlStr();
			if (yazim.empty()) {
				yazim = eskiYazim.c_str();
			}

			std::string eskiIsim = kontrol.getFormul(secim)->getFormulIsmi().c_str();
			std::cout << "\nEski ismi: ";
			std::cout << kontrol.getFormul(secim)->getFormulIsmi().c_str() << std::endl;
			std::cout << "\nYeni Isim: ";
			isim = GirdiAlStr();
			if (isim.empty()) {
				isim = eskiIsim.c_str();
			}

			std::string eskiAciklama = kontrol.getFormul(secim)->getFormulAciklama().c_str();
			std::cout << "\nEski aciklama: ";
			std::cout << kontrol.getFormul(secim)->getFormulAciklama().c_str() << std::endl;
			std::cout << "\nYeni aciklama: ";
			aciklama = GirdiAlStr();
			if (aciklama.empty()) {
				aciklama = eskiIsim.c_str();
			}

			kontrol.FormulSil(secim);
			kontrol.FormulYeni(yazim, isim, aciklama);

			ClearScreenWin();
			std::cout << "\nDegisiklikler basariyla kaydedildi!\n";

		}break;


		case 0: {
			FormulMenuKapat = true;
		}break;

		default:
			break;
		}

	}

}

void YeniFormulOlusturma() {
	std::string yazimi;
	std::string aciklama;
	std::string isim;


	std::cout << "Formulunuzu yaziniz\n";
	std::cout << "Formul: ";
	yazimi = GirdiAlStr();

	std::cout << "\nFormulunuze isim verin\n";
	std::cout << "Isim: ";
	isim = GirdiAlStr();

	std::cout << "\nFormulunuze aciklama ekleyin\n";
	std::cout << "Aciklama: ";
	aciklama = GirdiAlStr();

	kontrol.FormulYeni(yazimi, isim, aciklama);

	std::cout << "\nFormulunuz basariyla olusturuldu!\n";
}

inline int GirdiAlInt() {
	std::string girdi;

	int girdi_ = 0;
	try
	{
		std::getline(std::cin, girdi);

		if (girdi.empty()) {
			return -1;
		}

		girdi_ = std::stoi(girdi);
	}
	catch (const std::invalid_argument& e)
	{
#ifdef DEBUG
		std::cout << e.what() << std::endl;
#endif // !DEBUG


		ClearScreenWin();
		std::cout << "\nLutfen sayi giriniz!\n";
		girdi_ = -1;
	}
	catch (const std::out_of_range& e) {
#ifdef DEBUG
		std::cerr << e.what() << std::endl;
#endif
		ClearScreenWin();
		std::cerr << "\nGirilen ifade fazla buyuk\n";
		girdi_ = -1;
	}

	return girdi_;
}

inline double GirdiAlDouble() {
	std::string girdi;

	double girdi_ = 0;
	try
	{
		std::getline(std::cin, girdi);

		if (girdi.empty()) {
			return -1;
		}

		girdi_ = std::stod(girdi);
	}
	catch (const std::invalid_argument& e)
	{
#ifdef DEBUG
		std::cout << e.what() << std::endl;
#endif // !DEBUG


		ClearScreenWin();
		std::cout << "\nLutfen sayi giriniz!\n";
		girdi_ = -1;
	}
	catch (const std::out_of_range& e) {
#ifdef DEBUG
		std::cerr << e.what() << std::endl;
#endif
		ClearScreenWin();
		std::cerr << "\nGirilen ifade fazla buyuk\n";
		girdi_ = -1;
	}

	return girdi_;
}

inline int GirdiAlIntMenu() {
	std::string girdi;

	int girdi_ = 0;
	try
	{
		std::getline(std::cin, girdi);

		//Girdi boş ise
		if (girdi.empty()) {
			return -1;
		}

		//Komut girildi ise
		if (KomutKontrol(girdi)) {
			std::string sonuc = KomutGerceklestir(girdi);
			std::cout << std::endl << sonuc;
			return -2;
		}

		girdi_ = std::stoi(girdi);
	}
	catch (const std::invalid_argument& e)
	{
#ifdef DEBUG
		std::cout << e.what() << std::endl;
#endif // !DEBUG

		ClearScreenWin();
		std::cout << "\nLutfen sayi giriniz!\n";
		girdi_ = -1;
	}
	catch (const std::out_of_range& e) {
#ifdef DEBUG
		std::cerr << e.what() << std::endl;
#endif
		ClearScreenWin();
		std::cerr << "\nGirilen ifade fazla buyuk\n";
		girdi_ = -1;
	}

	return girdi_;
}

int KomutKontrol(std::string metin) {
	if (metin.empty()) {
		return 0;
	}
	if (metin[0] != '-') {
		return 0;
	}

	std::string komut = metin.substr(1, metin.find(" ", 0) - 1);

	static std::vector<const char*> KomutListesi = { "calc", "help", "cls" };

	int komutID = -1;
	for (int i = 0; i < KomutListesi.size(); i++) {

		if (komut == KomutListesi[i]) {
			komutID = i;
			break;
		}

	}
	return komutID;
}

std::string KomutGerceklestir(std::string metin) {
	std::string komut = metin.substr(1, metin.find(" ", 0) - 1);

	static std::vector<const char*> KomutListesi = { "calc", "help", "cls" };

	int komutID = -1;
	for (int i = 0; i < KomutListesi.size(); i++) {

		if (komut == KomutListesi[i]) {
			komutID = i;
			break;
		}

	}

	switch (komutID)
	{
		//Hesaplama
		// -calc = <formula>
	case 0: {
		std::string islem = metin.substr(metin.find("=") + 1);
		Formul hesapla(islem);
		for (size_t i = 0; i < hesapla.getDegiskenSayisi(); i++) {
			std::cout << "\"" << hesapla.getDegiskenIsim(i).c_str() << "\" icin deger = ";

			std::string degiskenDeger = GirdiAlStr();
			std::cout << std::endl;
			if (degiskenDeger.size() != 0) {
				double degiskenDegerd = std::stod(degiskenDeger);
				hesapla.setDegiskenDeger(i, degiskenDegerd);
			}
		}

		return std::to_string(hesapla.getSonuc());
		break;
	}

		  //help
	case 1: {
		return std::string("Komut bolumu yardim kismi");
		break;
	}

		  //cls
	case 2: {
		ClearScreenWin();
		return "0";
		break;
	}

		  //Hatalı komut
	case -1: {
		return std::string("Hatali komut girildi. Komutlar icin -help yaziniz.");
		break;
	}

	default: {

		break;
	}

	}
	return "1";
}

std::string KomutGerceklestir(std::string metin, int komutID) {
	std::string komut = metin.substr(1, metin.find(" ", 0) - 1);

	switch (komutID)
	{
		//Hesaplama
		// -calc = <formula>
	case 0: {
		std::string islem = metin.substr(metin.find("=") + 1);
		Formul hesapla(islem);
		for (size_t i = 0; i < hesapla.getDegiskenSayisi(); i++) {
			std::cout << "\"" << hesapla.getDegiskenIsim(i).c_str() << "\" icin deger = ";

			std::string degiskenDeger = GirdiAlStr();
			std::cout << std::endl;
			if (degiskenDeger.size() != 0) {
				double degiskenDegerd = std::stod(degiskenDeger);
				hesapla.setDegiskenDeger(i, degiskenDegerd);
			}
		}

		return std::to_string(hesapla.getSonuc());
		break;
	}

		  //help
	case 1: {
		return std::string("Komut bolumu yardim kismi\nHesaplama yapmak icin ''-calc = <islem>'' ifade tirnak icine alinmali");
		break;
	}

		  //cls
	case 2: {
		ClearScreenWin();
		return std::string();
		break;
	}

		  //Hatalı komut
	case -1: {
		return std::string("Hatali komut girildi. Komutlar icin -help yaziniz.");
		break;
	}

	default: {

		break;
	}

	}
	return std::string();
}

inline std::string GirdiAlStr() {
	std::string girdi;
	try {
		std::getline(std::cin, girdi);
		ClearScreenWin();
	}
	catch (std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
	}
	return girdi.c_str();
}

void ClearScreenWin()
{
	system("CLS");
	/*
		HANDLE                     hStdOut;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD                      count;
		DWORD                      cellCount;
		COORD                      homeCoords = { 0, 0 };

		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hStdOut == INVALID_HANDLE_VALUE) return;

		// Get the number of cells in the current buffer
		if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
		cellCount = csbi.dwSize.X * csbi.dwSize.Y;

		// Fill the entire buffer with spaces
		if (!FillConsoleOutputCharacter(
			hStdOut,
			(TCHAR)' ',
			cellCount,
			homeCoords,
			&count
		)) return;

		// Fill the entire buffer with the current colors and attributes
		if (!FillConsoleOutputAttribute(
			hStdOut,
			csbi.wAttributes,
			cellCount,
			homeCoords,
			&count
		)) return;

		// Move the cursor home
		SetConsoleCursorPosition(hStdOut, homeCoords);
		*/
}
