#include <gtest/gtest.h>

#include "BigNumber.h"

TEST(BigNumber, PositiveInput) {
	BigNumber A{124}, B{456}, C{0};

	A.multiply(B, C);
	EXPECT_EQ(A.number(), 124);
	EXPECT_EQ(B.number(), 456);
	EXPECT_EQ(C.number(), 124 * 456);
}

TEST(BigNumber, AxB_EQ_BxA) {
	BigNumber A{9}, B{10}, C1{0}, C2{0};

	A.multiply(B, C1);
	B.multiply(A, C2);
	EXPECT_EQ(C1.number(), C2.number());
}

TEST(BigNumber, NegativeInput) {
	BigNumber A{246}, B{-166}, C{0};

	A.multiply(B, C);
	EXPECT_EQ(C.number(), 246 * -166);
}

TEST(BigNumber, AxB_EQ_BxA_Negative) {
	BigNumber A{177}, B{-279}, C1{0}, C2{0};

	A.multiply(B, C1);
	B.multiply(A, C2);
	EXPECT_EQ(C1.number(), C2.number());
}

TEST(BigNumber, HandleZeroCase) {
	BigNumber A{1980}, B{0}, C{20};

	EXPECT_EQ(C.number(), 20);
	A.multiply(B, C);
	EXPECT_EQ(C.number(), 0);
}
