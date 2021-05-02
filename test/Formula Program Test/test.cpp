#include "pch.h"
#include "..//..//include/formul.h"
#include <string>

class TestObj{
private:
	Formul* testFormul;


public:
	
	TestObj(std::string formulStr){
	testFormul = new Formul(formulStr);

	}

	Formul* getFormul() {
		if (!testFormul) {
			return testFormul;
		}
	}

	~TestObj(){
		delete testFormul;
	}

	
};

TEST(Deneme_Test, Random_Input) {
	std::string FormulStr = "10+20+30+40+50+60+70+20+5+10+20+20+60";
	
	TestObj testObj(FormulStr);
	
	Formul* formul = testObj.getFormul();

	

	EXPECT_EQ(315, formul->getSonuc());

}


int main(int argc, char *argv []) {
	

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}