#include "gtest/gtest.h"
#define private public
#include "wallet_front_socket.h"

namespace walletfront {

class TestSocket:public testing::Test
{
    public:
        TestSocket()
        {
            pSocket = new Socket();
        }
        ~TestSocket()
        {
            if(pSocket)
            {
                pSocket = NULL;
            }
        }

        virtual void TestBody() {}
        void SetUp()  {}
        void TearDown() {}
    private:
        Socket *pSocket;
};

TEST(TestSocket, Pointer_TestSocket)
{
    TestSocket *pTest = new TestSocket();
    ASSERT_EQ(true, pTest != NULL);

    TestSocket test;
    ASSERT_EQ(true, test.pSocket != NULL);
}

TEST(TestSocket, Pointer_Sql)
{
    TestSocket *pTest = new TestSocket();
    ASSERT_EQ(true, pTest != NULL);
    ASSERT_EQ(true, pTest->pSocket != NULL);

    TestSocket test;
    ASSERT_EQ(true, test.pSocket != NULL);
}

}