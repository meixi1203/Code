#include "gtest/gtest.h"
#define private public
#include "key_center.h"

namespace walletfront {

class TestKeyCenter:public testing::Test  
{  
    public:
        TestKeyCenter()
        {
            pkeyCenter = new KeyCenter();
        }
        ~TestKeyCenter()
        {
            if(pkeyCenter)
            {
                pkeyCenter = NULL;
            }
        }

        virtual void TestBody() {}
        void SetUp()  {}
        void TearDown() {}
    private:
        KeyCenter *pkeyCenter;
};

TEST(TestKeyCenter, Pointer_TestKeyCenter)
{
    TestKeyCenter *pTest = new TestKeyCenter();
    ASSERT_EQ(true, pTest != NULL);

    TestKeyCenter test_keycenter;
    ASSERT_EQ(true, test_keycenter.pkeyCenter != NULL);
}

TEST(TestKeyCenter, Pointer_KeyCenter)
{
    TestKeyCenter *pTest = new TestKeyCenter();
    ASSERT_EQ(true, pTest != NULL);
    ASSERT_EQ(true, pTest->pkeyCenter != NULL);

    TestKeyCenter test_keycenter;
    ASSERT_EQ(true, test_keycenter.pkeyCenter != NULL);
}

TEST(TestKeyCenter, KeyCenter_CheckAppId_NO)
{
    TestKeyCenter *pTest = new TestKeyCenter();
    ASSERT_EQ(true, pTest != NULL);
    ASSERT_EQ(true, pTest->pkeyCenter != NULL);
    pTest->pkeyCenter->KeyCenterInit();
    ASSERT_EQ(0, pTest->pkeyCenter->CheckAppId("123456"));
}

TEST(TestKeyCenter, KeyCenter_CheckAppId_Yes)
{
    TestKeyCenter *pTest = new TestKeyCenter();
    ASSERT_EQ(true, pTest != NULL);
    ASSERT_EQ(true, pTest->pkeyCenter != NULL);
    pTest->pkeyCenter->KeyCenterInit();
    ASSERT_EQ(true, pTest->pkeyCenter->CheckAppId("06ec8995107752cc9ac0c10914bd6dc2"));
}

}