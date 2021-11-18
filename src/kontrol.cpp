#include "kontrol.h"

std::vector<Formul*> Kontrol::formulListesi;

Kontrol::Kontrol()
{
}

const int Kontrol::DeneyYeni()
{
	size_t boyut = deneyListesi.size() + 1;
	Deney* deney = new Deney(boyut);
	deneyListesi.push_back(deney);

	return deneyListesi.size();
}

const int Kontrol::DeneyYeni(Deney* deney)
{
	deneyListesi.push_back(deney);

	return deneyListesi.size();

	return 0;
}

Status Kontrol::DeneySil(uint32_t& deneyID)
{
	if (!ListedeBulunuyor(deneyID, deneyListesi)) {
		return Status::Error;
	}

	deneyListesi.erase(deneyListesi.begin() + deneyID);
	return Status::Done;
}

const char* Kontrol::getDeneyAdi(uint32_t& deneyID)
{
	if (!ListedeBulunuyor(deneyID, deneyListesi)) {
		return nullptr;
	}

	return deneyListesi[deneyID]->DeneyAdi.c_str();

}

Status Kontrol::DeneyDuzenle(int deneyID, std::string yeniDeneyAdi)
{
	if (!ListedeBulunuyor(deneyID, deneyListesi)) {
		return Status::Error;
	}
	deneyListesi[deneyID]->DeneyAdi = yeniDeneyAdi;
	return Status::Done;
}

Status Kontrol::DeneyFormulKullan(const size_t& formulID, Deney* deney)
{
	if (!ListedeBulunuyor(formulID, formulListesi)) {
		return Status::Error;
	}

	const size_t deneySayisi = getDeneySayisi();
	int deneyID = 0;
	for (size_t i = 0; i < deneySayisi; i++) {
		if (deneyListesi[i] == deney) {
			deneyID = i;
			break;
		}
	}

	std::string eskiDeneyAdi = deneyListesi[deneyID]->DeneyAdi.c_str();
	Deney* yeniDeney = new Deney(formulListesi[formulID], eskiDeneyAdi.c_str());
	delete deneyListesi[deneyID];
	deneyListesi.erase(deneyListesi.begin() + deneyID);
	deneyListesi.insert(deneyListesi.begin() + deneyID, yeniDeney);
	
	return Status::Done;
}

Deney* Kontrol::DeneyYeni(const char* deneyAdi) {
	Deney* deney = new Deney(deneyAdi);
	
	return deney;
}

size_t Kontrol::getDeneySayisi()
{
	if (deneyListesi.empty()) {
		return static_cast<size_t>(0);
	}
	return deneyListesi.size();
}


Status Kontrol::FormulYeni(std::string formul, std::string isim, std::string aciklama)
{
	Formul* formulPtr = new Formul(formul, isim, aciklama);
	formulListesi.push_back(formulPtr);

	return Status::Done;
}

Status Kontrol::FormulYeni(Formul* formul)
{
	formulListesi.push_back(formul);

	return Status::Done;
}

Status Kontrol::FormulSil(int& formulID)
{
	if (!ListedeBulunuyor(formulID, formulListesi)) {
		return Status::Error;
	}
	formulListesi.erase(formulListesi.begin() + formulID);

	return Status::Done;
}

bool Kontrol::FormulVar()
{
	if (formulListesi.empty()) {
		return false;
	}

	return true;
}

size_t Kontrol::getFormulSayisi()
{
	if (!FormulVar()) {
		return formulListesi.size();
	}

	return formulListesi.size();
}

const Formul* Kontrol::getFormul(const int& formulID){
	if (!ListedeBulunuyor(formulID, formulListesi)) {
		return nullptr;
	}

	return formulListesi[formulID];
}

void Kontrol::Temizle()
{
	if (!deneyListesi.empty()) {
		for (size_t i = deneyListesi.size() - 1; i > 0; --i) {
			delete deneyListesi[i];
			deneyListesi.pop_back();
		}
	}

	if (!deneyListesi.empty()) {
		for (size_t i = formulListesi.size() - 1; i > 0; --i) {
			delete formulListesi[i];
			formulListesi.pop_back();
		}
	}

}

const Deney* Kontrol::getDeney(int id)
{
	if (!ListedeBulunuyor(id, deneyListesi)) {
		return nullptr;
	}

	return deneyListesi[id];
}

Deney* Kontrol::operator[](int& id)
{
	if (!ListedeBulunuyor(id, deneyListesi)) {
		return nullptr;
	}

	return deneyListesi[id];
}
