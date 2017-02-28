#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <lexicalSearch.h>

#define GP_FILE ".gp"
#define GP_DIR "../examples/"
#define OUT_FILE ".outgp"
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
		// char v[256];
		input >> type >> row >> col >> value;
		// input.getline(v, 256);
		// value = string(v);
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
		EXPECT_EQ(l.get_value(), lt.value);
		EXPECT_EQ(l.get_type_str(), lt.type);
		EXPECT_EQ(l.get_row() + 1, lt.row);
		EXPECT_EQ(l.get_col() + 1, lt.col);
	}

	void preparation(string file){
		if (s != NULL){
			delete s;
			s = NULL;
		}
		s = new Searcher(GP_DIR + file + GP_FILE);
		s->learn();
		input.open(OUT_DIR + file + OUT_FILE);
	}

	void testingFile(string file){
		preparation(file);
		while (true){
			TestLexeme lt(input);
			if (lt.type == "EOF"){
				try{
					Lexeme l = s->next();
				}
				catch(Lexeme e){
					comp(e, lt);
				}
				break;
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

// TEST_F(LexerTest, 06_Test){
// 	testingFile(files[5]);
// }

TEST_F(LexerTest, Exception_test){
	ASSERT_ANY_THROW(testingFile(files[5]));
}

int main(int argc, char* argv[]){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
