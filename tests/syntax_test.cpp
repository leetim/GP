#include <gtest/gtest.h>
// #include <gmock/gmock.h>
#include <iostream>
#include <lexicalSearch.h>
#include <syntaxis_analyser.h>

#define GP_FILE ".gp"
#define GP_DIR "../syntax_examples/"
#define OUT_FILE ".gpout"
#define OUT_DIR "../syntax_examples/"

using namespace std;
vector<string> files = {
	"01source","02source",	"03source",	"04source",	"05source",	"06source",	"07source",	"08source",	"09source", "10source",
	"11source",	"12source",	"13source",	"14source",	"15source",	"16source",	"17source",	"18source",	"19source", "20source",
	"21source",	"22source",	"23source",	"24source",	"25source",	"26source",	"27source",	"28source",	"29source", "30source",
	"31source",	"32source",	"33source",	"34source",	"35source",	"36source",	"37source",	"38source",	"39source", "40source",
	"41source",	"42source",	"43source",	"44source",	"45source",	"46source",	"47source",	"48source",	"49source", "50source"
};


class SyntaxisTest : public ::testing::Test
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

	void preparation(string file){
		if (s != NULL){
			delete s;
			s = NULL;
		}
		// cout << "123 " << GP_DIR + file + GP_FILE << endl;
		s = new SyntaxisAnalyser(GP_DIR + file + GP_FILE);
	}

	string get_tree_str(string file){
		ifstream input(GP_DIR + file + OUT_FILE, ifstream::binary);
    input.seekg (0, input.end);
    int length = input.tellg();
    input.seekg (0, input.beg);
		char* str = new char [length+1];
		input.read(str, length);
		string res(str, length);
		delete[] str;
		return res;
	}

	void testingFile(string file){
		preparation(file);
		PNocle pn = s->get_tree();
		ASSERT_EQ(pn->get_str(), get_tree_str(file));
	}

	void testingExeption(string file){
		preparation(file);
		try{
			PNocle pn = s->get_tree();
		}
		catch(Errors::Unknown_lexeme e){
			ASSERT_EQ(e.get_error_str(), get_tree_str(file));
		}
		catch(Errors::Illegal_expression e){
			ASSERT_EQ(e.get_error_str(), get_tree_str(file));
		}
		catch(Errors::ClosingParenthesisNotFound e){
			ASSERT_EQ(e.get_error_str(), get_tree_str(file));
		}
	}

	SyntaxisAnalyser* s;
};

TEST_F(SyntaxisTest, 01_Empty_File){
	testingExeption(files[0]);
}

TEST_F(SyntaxisTest, 02_Simple_integer){
	testingFile(files[1]);
}

TEST_F(SyntaxisTest, 03_Simple_float){
	testingFile(files[2]);
}

TEST_F(SyntaxisTest, 04_Simple_var){
	testingFile(files[3]);
}

TEST_F(SyntaxisTest, 05_Simple_func_name){
	testingFile(files[4]);
}

TEST_F(SyntaxisTest, 06_Symple_type_name){
	testingFile(files[5]);
}

TEST_F(SyntaxisTest, 07_operation_add_with_integers){
	testingFile(files[6]);
}

TEST_F(SyntaxisTest, 08_operation_add_with_identificators){
	testingFile(files[7]);
}

TEST_F(SyntaxisTest, 09_operation_two_sub){
	testingFile(files[8]);
}

TEST_F(SyntaxisTest, 10_array_at){
	testingFile(files[9]);
}

TEST_F(SyntaxisTest, 11_array_at_twice){
	testingFile(files[10]);
}

TEST_F(SyntaxisTest, 12_function_one_argument){
	testingFile(files[11]);
}

TEST_F(SyntaxisTest, 13_function_two_arguments){
	testingFile(files[12]);
}

TEST_F(SyntaxisTest, 14_function_two_arguments_with_expr){
	testingFile(files[13]);
}

TEST_F(SyntaxisTest, 15_function_one_argument_without_separators){
	testingFile(files[14]);
}

TEST_F(SyntaxisTest, 16_function_two_argument_without_separators){
	testingFile(files[15]);
}

TEST_F(SyntaxisTest, 17_function_two_argument_without_separators_with_expr){
	testingFile(files[16]);
}

TEST_F(SyntaxisTest, 18_superposition_functions){
	testingFile(files[17]);
}

TEST_F(SyntaxisTest, 19_adding_functions){
	testingFile(files[18]);
}

TEST_F(SyntaxisTest, 20_mult_field_of_record_and_method){
	testingFile(files[19]);
}

TEST_F(SyntaxisTest, 21_classic_example_with_priority){
	testingFile(files[20]);
}

TEST_F(SyntaxisTest, 22_logic_and_binary_operation_priority){
	testingFile(files[21]);
}

int main(int argc, char* argv[]){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
