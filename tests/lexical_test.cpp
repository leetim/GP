#include <gtest/gtest.h>
// #include <gmock/gmock.h>
#include <iostream>
#include <lexicalSearch.h>

#define GP_FILE ".gp"
#define GP_DIR "../lexer_examples/"
#define OUT_FILE ".gpout"
#define OUT_DIR "../lexer_examples/"

using namespace std;
vector<string> files = {
	"01source","02source",	"03source",	"04source",	"05source",	"06source",	"07source",	"08source",	"09source", "10source",
	"11source",	"12source",	"13source",	"14source",	"15source",	"16source",	"17source",	"18source",	"19source", "20source",
	"21source",	"22source",	"23source",	"24source",	"25source",	"26source",	"27source",	"28source",	"29source", "30source",
	"31source",	"32source",	"33source",	"34source",	"35source",	"36source",	"37source",	"38source",	"39source", "40source",
	"41source",	"42source",	"43source",	"44source",	"45source",	"46source",	"47source",	"48source",	"49source", "50source"
};

struct TestLexeme{
	TestLexeme();
	TestLexeme(ifstream& input){
		char v[256];
		input >> row >> col >> type;// >> value;
		input.getline(v, 256);
		value = string(v + 1);
		if (value == "NewLine" || type == "COMMENT"){
			value += "\n";
		}
	}
	string type;
	int row;
	int col;
	string value;
};

class LexerTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		s = NULL;
	}

	void TearDown()
	{
		if (s != NULL){
			delete s;
		}
	}

	void comp(Lexeme& l, TestLexeme& lt){
		// l.print();
		// l.print();
		ASSERT_EQ(l.get_value(), lt.value);
		ASSERT_EQ(l.get_type_str(), lt.type);
		ASSERT_EQ(l.get_row() + 1, lt.row);
		ASSERT_EQ(l.get_col() + 1, lt.col);
		// cout << lt.row << endl;
	}

	void preparation(string file){
		if (s != NULL){
			delete s;
			s = NULL;
		}
		s = new Searcher(GP_DIR + file + GP_FILE);
		// cout << 123 << endl;
		// s->learn();
		input.open(OUT_DIR + file + OUT_FILE);
	}

	void testingFile(string file){
		preparation(file);
		while (true){
			TestLexeme lt(input);
			Lexeme l = s->next();
			comp(l, lt);
			if (l.get_type() == LT_EOF){
				break;
			}
		}
	}

	Searcher* s;
	ifstream input;
};

TEST_F(LexerTest, 01_Empty_File){
	testingFile(files[0]);
}

TEST_F(LexerTest, 02_Two_words){
	testingFile(files[1]);
}

TEST_F(LexerTest, 03_Two_vars){
	testingFile(files[2]);
}

TEST_F(LexerTest, 04_Two_vars_with_numbers){
	testingFile(files[3]);
}

TEST_F(LexerTest, 05_Two_arrays){
	testingFile(files[4]);
}

TEST_F(LexerTest, 06_Two_arrays_with_numbers){
	testingFile(files[5]);
}

TEST_F(LexerTest, 07_Operators){
	testingFile(files[6]);
}

TEST_F(LexerTest, 08_Operators){
	testingFile(files[7]);
}

TEST_F(LexerTest, 09_Operators){
	testingFile(files[8]);
}

TEST_F(LexerTest, 10_More_identificators){
	testingFile(files[9]);
}

TEST_F(LexerTest, 11_More_variables){
	testingFile(files[10]);
}

TEST_F(LexerTest, 12_More_arrays){
	testingFile(files[11]);
}

TEST_F(LexerTest, 13_Comment){
	testingFile(files[12]);
}

TEST_F(LexerTest, 14_Separators){
	testingFile(files[13]);
}

TEST_F(LexerTest, 15_Identificators){
	testingFile(files[14]);
}

TEST_F(LexerTest, 16_Integers){
	testingFile(files[15]);
}

TEST_F(LexerTest, 17_Float){
	testingFile(files[16]);
}

TEST_F(LexerTest, 18_Passive_strs){
	testingFile(files[17]);
}

TEST_F(LexerTest, 19_Comments){
	testingFile(files[18]);
}

TEST_F(LexerTest, 20_operators_points1){
	testingFile(files[19]);
}

TEST_F(LexerTest, 21_operators_points2){
	testingFile(files[20]);
}

TEST_F(LexerTest, 22_operators_points_and_int){
	testingFile(files[21]);
}

TEST_F(LexerTest, 23_operators_add_sub_eq){
	testingFile(files[22]);
}

TEST_F(LexerTest, 24_operators_subsubsub){
	testingFile(files[23]);
}

TEST_F(LexerTest, 25_operators_addaddadd){
	testingFile(files[24]);
}

TEST_F(LexerTest, 26_operators_mult_div){
	testingFile(files[25]);
}

TEST_F(LexerTest, 27_operators_bits){
	testingFile(files[26]);
}

TEST_F(LexerTest, 28_operators_comma_and_colon){
	testingFile(files[27]);
}

TEST_F(LexerTest, 29_leterals_float){
	testingFile(files[28]);
}

TEST_F(LexerTest, 30_add_two_integers){
	testingFile(files[29]);
}

TEST_F(LexerTest, 31_bits_or_for_two_vars){
	testingFile(files[30]);
}

TEST_F(LexerTest, 32_operators_points_and_int){
	testingFile(files[31]);
}

TEST_F(LexerTest, 33_operators_points_and_int){
	testingFile(files[32]);
}

TEST_F(LexerTest, 34_operators_points_and_int){
	testingFile(files[33]);
}

TEST_F(LexerTest, 35_operators_points_and_int){
	testingFile(files[34]);
}

TEST_F(LexerTest, 36_operators_points_and_int){
	testingFile(files[35]);
}

TEST_F(LexerTest, 37_operators_points_and_int){
	testingFile(files[36]);
}

TEST_F(LexerTest, 38_operators_points_and_int){
	testingFile(files[37]);
}

TEST_F(LexerTest, 39_operators_points_and_int){
	testingFile(files[38]);
}

TEST_F(LexerTest, 40_operators_points_and_int){
	testingFile(files[39]);
}

TEST_F(LexerTest, 41_operators_points_and_int){
	testingFile(files[40]);
}

TEST_F(LexerTest, 42_operators_points_and_int){
	testingFile(files[41]);
}

TEST_F(LexerTest, 43_operators_points_and_int){
	testingFile(files[42]);
}

TEST_F(LexerTest, 44_operators_points_and_int){
	testingFile(files[43]);
}

TEST_F(LexerTest, 45_operators_points_and_int){
	testingFile(files[44]);
}

TEST_F(LexerTest, 46_operators_points_and_int){
	testingFile(files[45]);
}

TEST_F(LexerTest, 47_operators_points_and_int){
	testingFile(files[46]);
}

TEST_F(LexerTest, 48_operators_points_and_int){
	testingFile(files[47]);
}

TEST_F(LexerTest, 49_operators_points_and_int){
	testingFile(files[48]);
}

TEST_F(LexerTest, 50_operators_points_and_int){
	testingFile(files[49]);
}

int main(int argc, char* argv[]){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
