#include <gtest/gtest.h>

#include "../controller/controller.h"
#include "../model/model.h"

struct PNTest : public testing::Test {
  s21::PolishNotation pn;
  std::string input;
  std::string x;
};

TEST_F(PNTest, SimpleOperations) {
  input = "103.25 + 67.008";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), 103.25 + 67.008);

  input = "103.25 - 67.008";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), 103.25 - 67.008);

  input = "103.25 * 67.008";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), 103.25 * 67.008);

  input = "103.25 / 67.008";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), 103.25 / 67.008);

  input = "103.25 mod 6";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), fmod(103.25, 6));

  input = "103.25 ^ 2.1";
  pn.Calculate(input, x);
  EXPECT_NEAR(pn.GetAnswer(), pow(103.25, 2.1), 1e-7);
}

TEST_F(PNTest, SimpleFunctions) {
  input = "cos(10.023)";
  pn.Calculate(input, x);
  EXPECT_NEAR(pn.GetAnswer(), cos(10.023), 1e-7);

  input = "cos(0)";
  pn.Calculate(input, x);
  EXPECT_NEAR(pn.GetAnswer(), cos(0), 1e-7);

  input = "sin(10.023)";
  pn.Calculate(input, x);
  EXPECT_NEAR(pn.GetAnswer(), sin(10.023), 1e-7);

  input = "sin(0)";
  pn.Calculate(input, x);
  EXPECT_NEAR(pn.GetAnswer(), sin(0), 1e-7);

  input = "tan(10.023)";
  pn.Calculate(input, x);
  EXPECT_NEAR(pn.GetAnswer(), tan(10.023), 1e-7);

  input = "tan(0)";
  pn.Calculate(input, x);
  EXPECT_NEAR(pn.GetAnswer(), tan(0), 1e-7);

  input = "acos(0.456)";
  pn.Calculate(input, x);
  EXPECT_NEAR(pn.GetAnswer(), acos(0.456), 1e-7);

  input = "asin(-0.456)";
  pn.Calculate(input, x);
  EXPECT_NEAR(pn.GetAnswer(), asin(-0.456), 1e-7);

  input = "atan(0.3)";
  pn.Calculate(input, x);
  EXPECT_NEAR(pn.GetAnswer(), atan(0.3), 1e-7);

  input = "ln(3.409)";
  pn.Calculate(input, x);
  EXPECT_NEAR(pn.GetAnswer(), log(3.409), 1e-7);

  input = "log(3.409)";
  pn.Calculate(input, x);
  EXPECT_NEAR(pn.GetAnswer(), log10(3.409), 1e-7);

  input = "sqrt(9)";
  pn.Calculate(input, x);
  EXPECT_NEAR(pn.GetAnswer(), 3, 1e-7);
}

TEST_F(PNTest, CompoundOperations) {
  input = "10.48209 - cos(3^4.3 - (8 + 2)) - 10";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), (10.48209 - cos(pow(3, 4.3) - (8 + 2)) - 10));

  input = "-(67 + 3 * 0.144) * ln(8 + (5 * 0.001))";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), (-(67 + 3 * 0.144) * log(8 + (5 * 0.001))));

  input = "1000000000 - 8^ (90 + 3 + 4 / 109.6) - sqrt(5 mod 3 * 5)";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(),
            (1000000000 - pow(8, (90 + 3 + 4 / 109.6)) - sqrt(fmod(5, 3) * 5)));

  input = "-(+(-sin(tan(4 - 3.8 * 1.9) * 2) / 1.1))";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), (-(+(-sin(tan(4 - 3.8 * 1.9) * 2) / 1.1))));
}

TEST_F(PNTest, BracketErrors) {
  // утечкааа
  input = "(1 + 3";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "(1 + 3))";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "((1 + 3)";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = ")1 + 3";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "(1 + 3)(";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "(1 + 3 + (4^ 7)";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "1 + 3)";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "1 + (3 - 2";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);
}

TEST_F(PNTest, WrongOrderOfLexemas) {
  input = "mod 2";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "/ 2";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "^ 2";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "2 + - 3";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "sin 4";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "cos + 2";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "(1 + tan) + 2";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "2 sin";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "- * 3";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "2 +";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "2 *";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "2 + tan";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "2 + (* 4)";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "2 + ()";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);
}

TEST_F(PNTest, NumbersWithE) {
  input = "1.09 + 2e4";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), 1.09 + 2e4);

  input = "1.09 + 2e+4";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), 1.09 + 2e4);

  input = "1.09 + 2e-4";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), 1.09 + 2e-4);

  input = "1.09 + 2e-4-5";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), 1.09 + 2e-4 - 5);

  input = "1.09 + 2ee";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "1.09 + 2e5e";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "1.09 + 2e+";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "1.09 + 2e--";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "1.09 + e4";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);
}

TEST_F(PNTest, XValue) {
  input = "3 ^ x - 6";
  x = "5.99";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), pow(3, 5.99) - 6);

  input = "3 ^ X - 6";
  x = "1e-4";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), pow(3, 1e-4) - 6);

  x = "-9";
  pn.Calculate(input, x);
  EXPECT_EQ(pn.GetAnswer(), pow(3, -9) - 6);

  x = "";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  x = "123j";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  x = "asdfg";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  x = "1..";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  x = "--1";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  x = "-a";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);
}

TEST_F(PNTest, UncorrectFunctions) {
  input = "2 - cof(3)";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "2 - asinm(0.3)";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "asdfghj(7)";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "7 - 4 & 5";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "3 : 2";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "2 ! 4";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);
}

TEST_F(PNTest, UncorrectNumbers) {
  input = "2 - 3.4.";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "2 - 3.4.";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "2 - 3.4.5";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "2 - 2.";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);

  input = "2 - .2";
  EXPECT_THROW(pn.Calculate(input, x), std::invalid_argument);
}

TEST_F(PNTest, GraphTest1) {
  input = "x";
  std::vector<std::pair<double, double>> pairs;

  pairs = pn.GetGraph(input, -10, 10);
  EXPECT_EQ(pairs.size(), 500);
  for (size_t i = 0; i < 500; i += 10) {
    EXPECT_EQ(pairs[i].first, pairs[i].second);
  }
}

TEST_F(PNTest, GraphTest2) {
  input = "x^2";
  std::vector<std::pair<double, double>> pairs;

  pairs = pn.GetGraph(input, -10, 10);
  EXPECT_EQ(pairs.size(), 500);
  for (size_t i = 0; i < 500; i += 10) {
    EXPECT_EQ(pairs[i].second, pow(pairs[i].first, 2));
  }
}

TEST_F(PNTest, GraphTestExceptions) {
  // утечка!!
  input = "x^";
  EXPECT_THROW(pn.GetGraph(input, -10, 10), std::invalid_argument);

  input = "x+1";
  EXPECT_THROW(pn.GetGraph(input, -10, -12), std::invalid_argument);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}