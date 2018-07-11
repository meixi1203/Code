#include "gtest/gtest.h"
#define private public
#include "key_center.h"

namespace walletfront {

class TestKeyCenter:public testing::Test  
{  
    public:
        TestKeyCenter()
        {
            pkey_center = new KeyCenter();
        }
        ~TestKeyCenter()
        {
            if(pkey_center)
            {
                pkey_center = NULL;
            }
        }

        virtual void TestBody() {}
        void SetUp()  {}
        void TearDown() {}
    private:
        KeyCenter *pkey_center;
};

TEST(TestKeyCenter, Pointer_TestKeyCenter)
{
    TestKeyCenter *ptest_keycenter = new TestKeyCenter();
    ASSERT_EQ(true, ptest_keycenter != NULL);
    TestKeyCenter test_keycenter;
    ASSERT_EQ(true, test_keycenter.pkey_center != NULL);
}

TEST(TestKeyCenter, Pointer_KeyCenter)
{
    TestKeyCenter *ptest_keycenter = new TestKeyCenter();
    ASSERT_EQ(true, ptest_keycenter != NULL);
    ASSERT_EQ(true, ptest_keycenter->pkey_center != NULL);
    TestKeyCenter test_keycenter;
    ASSERT_EQ(true, test_keycenter.pkey_center != NULL);
}

TEST(TestKeyCenter, KeyCenter_CheckAppId_NO)
{
    TestKeyCenter *ptest_keycenter = new TestKeyCenter();
    ASSERT_EQ(true, ptest_keycenter != NULL);
    ASSERT_EQ(true, ptest_keycenter->pkey_center != NULL);
    ptest_keycenter->pkey_center->KeyCenterInit();
    ASSERT_EQ(0, ptest_keycenter->pkey_center->CheckAppId("123456"));
}

TEST(TestKeyCenter, KeyCenter_CheckAppId_Yes)
{
    TestKeyCenter *ptest_keycenter = new TestKeyCenter();
    ASSERT_EQ(true, ptest_keycenter != NULL);
    ASSERT_EQ(true, ptest_keycenter->pkey_center != NULL);
    ptest_keycenter->pkey_center->KeyCenterInit();
    ASSERT_EQ(true, ptest_keycenter->pkey_center->CheckAppId("06ec8995107752cc9ac0c10914bd6dc2"));
}

}