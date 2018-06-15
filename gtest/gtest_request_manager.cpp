/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "request_manager.h"
#include "gtest/gtest.h"

namespace walletfront {

class TestRequestManager:public testing::Test  
{  
    public:
        TestRequestManager() {}
        ~TestRequestManager() {}

        virtual void TestBody() {}
        void SetUp()  {
            req = new RequestManager();
         }

        void TearDown() {
            delete req;
            req = NULL;
        }
    public:
        walletfront::RequestManager *req;
};

TEST(TestRequestManager,Pointer)
{
    TestRequestManager *test = new TestRequestManager();
    ASSERT_EQ(true, test != NULL);
    ASSERT_EQ(true, test->req != NULL);
}

}