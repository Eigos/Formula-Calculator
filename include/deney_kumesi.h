#pragma once
#include <string>
#include <vector>

#include "deney.h"

class DeneyKumesi {
private:
	std::string KumeIsmi{"Yeni Kume"};
	std::vector<Deney*> DeneylerList;

public:
	
	DeneyKumesi(std::string kumeIsmi) : KumeIsmi(kumeIsmi) {}
	DeneyKumesi(int kumeID) {
		KumeIsmi.append("-ID: ");
		KumeIsmi.append(std::to_string(kumeID));
	}

	~DeneyKumesi() {};

	Deney* operator[](int &id);

	std::string getName();
	Deney* getDeney(int id);
	const int getDeneySayisi();

	void DeneyOlustur();

	void DeneySil(int id);
	void DeneyDuzenle(int id, Deney& deney);
	

};