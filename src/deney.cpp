#include "..\include\deney.h"


Deney::Deney(int no)
{
	this->id = no;
}

Deney::Deney(Formul& formul) {
	this->formul = new Formul(formul);
}

Deney::~Deney() {
	delete formul;
}

void Deney::setDegisken(int degiskenID, const char* sembol, const char* indis, const char* tanim, const char* isim, float* deger)
{
	if (degiskenID < formul->getDegiskenSayisi()) {
		return;
	}

	Degisken* degisken = formul->getDegisken(degiskenID);
	if (sembol != nullptr) { degisken->Sembol = *sembol; }
	if (indis != nullptr) { degisken->Indis = *indis; }
	if (tanim != nullptr) { degisken->Tanim = *tanim; }
	if (deger != nullptr) { degisken->Deger = *deger; degisken->DegerGirildi = true; }

}

std::string Deney::getDeneyAdi()
{
	return this->DeneyAdi;
}

void Deney::setDeneyAdi(std::string& deneyAdiYeni)
{
	this->DeneyAdi = deneyAdiYeni;
}

float Deney::getDeneySonucu()
{
	return formul->getSonuc();
}

bool Deney::FormulAtandi()
{
	if (formul == nullptr) {
		return 0;
	}
	else
	{
		return 1;
	}
}

void Deney::FormulKullan(Formul& formul)
{
	if (this->formul != nullptr) {
		delete this->formul;
	}
	this->formul = new Formul(formul);
}

void Deney::FormulKullan(std::string& formul)
{
	if (this->formul != nullptr) {
		delete this->formul;
	}
	this->formul = new Formul(formul);
}

const int Deney::getDegiskenSayisi() {
	return formul->getDegiskenSayisi();
}

std::string Deney::getDegiskenSembol(int degiskenID)
{
	if (degiskenID > formul->getDegiskenSayisi()) { std::string str; return str; }
	const Degisken* yeniDegisken = formul->getDegisken(degiskenID);
	return yeniDegisken->Sembol;
}

std::string Deney::getDegiskenIndis(int degiskenID)
{
	if (degiskenID > formul->getDegiskenSayisi()) { std::string str; return str; }
	const Degisken* yeniDegisken = formul->getDegisken(degiskenID);
	return yeniDegisken->Indis;
}

std::string Deney::getDegiskenTanim(int degiskenID)
{
	if (degiskenID > formul->getDegiskenSayisi()) { std::string str; return str; }
	const Degisken* yeniDegisken = formul->getDegisken(degiskenID);
	return yeniDegisken->Tanim;
}

std::string Deney::getDegiskenIsim(int degiskenID)
{
	if (degiskenID > formul->getDegiskenSayisi()) { std::string str; return str; }
	const Degisken* yeniDegisken = formul->getDegisken(degiskenID);
	return yeniDegisken->Isim;
}

float Deney::getDegiskenDeger(int degiskenID)
{
	if (degiskenID > formul->getDegiskenSayisi()) { return -1; }
	const Degisken* yeniDegisken = formul->getDegisken(degiskenID);
	return yeniDegisken->Deger;
}

void Deney::setDegiskenSembol(int degiskenID, std::string sembol)
{
	if (degiskenID > formul->getDegiskenSayisi()) { return; }
	Degisken* degisken = formul->getDegisken(degiskenID);
	degisken->Sembol = sembol;
}

void Deney::setDegiskenIndis(int degiskenID, std::string indis)
{
	if (degiskenID > formul->getDegiskenSayisi()) { return; }
	Degisken* degisken = formul->getDegisken(degiskenID);
	degisken->Indis = indis;

}

void Deney::setDegiskenTanim(int degiskenID, std::string tanim)
{
	if (degiskenID > formul->getDegiskenSayisi()) { return; }
	Degisken* degisken = formul->getDegisken(degiskenID);
	degisken->Tanim = tanim;
}

void Deney::setDegiskenIsim(int degiskenID, std::string isim)
{
	if (degiskenID > formul->getDegiskenSayisi()) { return; }
	Degisken* degisken = formul->getDegisken(degiskenID);
	degisken->Isim = isim;
}

void Deney::setDegiskenDeger(int degiskenID, float deger)
{
	if (degiskenID > formul->getDegiskenSayisi()) { return; }
	Degisken* degisken = formul->getDegisken(degiskenID);
	degisken->Deger = deger;
	degisken->DegerGirildi = true;
}


