#include "degisken.h"

Degisken::Degisken(const Degisken& degisken) {
	Sembol = degisken.Sembol;
	Indis  = degisken.Indis;
	Tanim  = degisken.Tanim;
	Isim   = degisken.Isim;
	Deger  = degisken.Deger;

};

Degisken::Degisken() {

};

void Degisken::setDegisken(const char* sembol, const char* indis, const char* tanim, const char* isim, float* deger)
{
	if (sembol != nullptr) { this->Sembol = *sembol; }
	if (indis != nullptr) { this->Indis = *indis; }
	if (tanim != nullptr) { this->Tanim = *tanim; }
	if (deger != nullptr) { this->Deger = *deger; this->DegerGirildi = true; }

}

std::string Degisken::getSembol()
{
	return this->Sembol;
}

std::string Degisken::getIndis()
{
	return this->Indis;
}

std::string Degisken::getTanim()
{
	return this->Tanim;
}

std::string Degisken::getIsim()
{
	return this->Isim;
}

float Degisken::getDeger()
{
	return this->Deger;
}

bool Degisken::getDegerAtandi() {
	return DegerGirildi;
}

void Degisken::setSembol(const std::string& sembol)
{
	this->Sembol = sembol;
}

void Degisken::setIndis(const std::string& indis)
{
	this->Indis = indis;
}

void Degisken::setTanim(const std::string& tanim)
{
	this->Tanim = tanim;
}

void Degisken::setIsim(const std::string& isim)
{
	this->Isim = isim;
}

void Degisken::setDeger(const float& deger)
{
	this->Deger = deger;
	this->DegerGirildi = true;
}
