#include "gtest/gtest.h"
#define private public
#include "data_manager.h"

namespace walletfront {

class TestDataManager:public testing::Test
{
    public:
        TestDataManager()
        {
            pDataManager = DataManager::GetInstance();
        }
        ~TestDataManager()
        {
            if(pDataManager)
            {
                pDataManager = NULL;
            }
        }

        virtual void TestBody() {}
        void SetUp()  {}
        void TearDown() {}
    private:
        DataManager *pDataManager;
};

TEST(TestDataManager, Pointer_TestDataManager)
{
    TestDataManager *pTest = new TestDataManager();
    ASSERT_EQ(true, pTest != NULL);
    ASSERT_EQ(true, pTest->pDataManager != NULL);
    TestDataManager test;
    ASSERT_EQ(true, test.pDataManager != NULL);
}
}