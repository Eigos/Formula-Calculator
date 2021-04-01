#include "..\include\deney_kumesi.h"

std::string DeneyKumesi::getName()
{
	return KumeIsmi;
}

void DeneyKumesi::DeneyOlustur()
{
	Deney* deneyYeni = new Deney(DeneylerList.size());
	DeneylerList.push_back(deneyYeni);
}

void DeneyKumesi::DeneySil(int id) {
	if (id < DeneylerList.size()) {
		if (id >= 0) {
			delete DeneylerList[id];
			DeneylerList.erase(DeneylerList.begin() + id);
		}
	}
}

Deney* DeneyKumesi::getDeney(int id) {
	if (id < DeneylerList.size()) {
		if (id >= 0) {
			return DeneylerList[id];
		}
	}
	return nullptr;
}

const int DeneyKumesi::getDeneySayisi()
{
	return this->DeneylerList.size();
}

void DeneyKumesi::DeneyDuzenle(int id, Deney& deney) {
	if (id < DeneylerList.size()) {
		if (id >= 0) {
			//DeneylerList[id];
		}
	}
}

Deney* DeneyKumesi::operator[](int& id) {
	if (id > DeneylerList.size()) {
		if (id < 0) {
			return nullptr;
		}
	}

	return DeneylerList[id];
}
