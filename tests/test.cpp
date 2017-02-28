#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <lexicalSearch.h>

#define GP_FILE ".gp"
#define GP_DIR "../examples/"
#define OUT_FILE ".out"
#define OUT_DIR "../lexer_results/"

using namespace std;

vector<string> files = {
	"01source",
	"02source",
	"03source",
	"04source",
	"05source",
	"06source",
	"07source",
	"08source",
	"09source"
};

struct TestLexeme{
	TestLexeme();
	TestLexeme(ifstream& input){
		input >> type >> row >> col >> value;
		if (value == "NewLine"){
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
		ASSERT_EQ(l.get_value(), lt.value);
		ASSERT_EQ(l.get_type_str(), lt.type);
		ASSERT_EQ(l.get_row() + 1, lt.row);
		ASSERT_EQ(l.get_col() + 1, lt.col);
	}

	void preparation(string file){
		if (s != NULL){
			delete s;
			s = NULL;
		}
		s = new Searcher(GP_DIR + files[0] + GP_FILE);
		s->learn();
		input.open(OUT_DIR + files[0] + OUT_FILE);
	}

	void testingFile(string file){
		preparation(files[0]);
		while (true){
			TestLexeme lt(input);
			if (lt.type == "EOF"){
				try{
					s->next();
				}
				catch(Lexeme e){
					comp(e, lt);
					break;
				}
			}
			Lexeme l = s->next();
			comp(l, lt);
		}
	}

	Searcher* s;
	ifstream input;
};

TEST_F(LexerTest, 01_Test){
	testingFile(files[0]);
}

TEST_F(LexerTest, 02_Test){
	testingFile(files[1]);
}

TEST_F(LexerTest, 03_Test){
	testingFile(files[2]);
}

TEST_F(LexerTest, 04_Test){
	testingFile(files[3]);
}

TEST_F(LexerTest, 05_Test){
	testingFile(files[4]);
}

TEST_F(LexerTest, 06_Test){
	testingFile(files[5]);
}

// TEST_F(LexerTest, 02_Test){
// 	testingFile(files[1]);
// }

int main(int argc, char* argv[]){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
