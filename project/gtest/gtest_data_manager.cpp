#include "gtest/gtest.h"
#define private public
#include "data_manager.h"

namespace walletfront {

class TestDataManager:public testing::Test
{
    public:
        TestDataManager()
        {
            pdata_manager = DataManager::GetInstance();
        }
        ~TestDataManager()
        {
            if(pdata_manager)
            {
                pdata_manager = NULL;
            }
        }

        virtual void TestBody() {}
        void SetUp()  {}
        void TearDown() {}
    private:
        DataManager *pdata_manager;
};

TEST(TestDataManager, Pointer_TestDataManager)
{
    TestDataManager *ptest = new TestDataManager();
    ASSERT_EQ(true, ptest != NULL);
    ASSERT_EQ(true, ptest->pdata_manager != NULL);
    TestDataManager test;
    ASSERT_EQ(true, test.pdata_manager != NULL);
}
}