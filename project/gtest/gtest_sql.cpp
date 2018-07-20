#include "gtest/gtest.h"
#define private public
#include "sql.h"

namespace walletfront {

class TestSql:public testing::Test
{
    public:
        TestSql()
        {
            pSql = new Sql();
        }
        ~TestSql()
        {
            if(pSql)
            {
                pSql = NULL;
            }
        }

        virtual void TestBody() {}
        void SetUp()  {}
        void TearDown() {}
    private:
        Sql *pSql;
};

TEST(TestSql, Pointer_TestSql)
{
    TestSql *pTest = new TestSql();
    ASSERT_EQ(true, pTest != NULL);

    TestSql test;
    ASSERT_EQ(true, test.pSql != NULL);
}

TEST(TestSql, Pointer_Sql)
{
    TestSql *pTest = new TestSql();
    ASSERT_EQ(true, pTest != NULL);
    ASSERT_EQ(true, pTest->pSql != NULL);

    TestSql test;
    ASSERT_EQ(true, test.pSql != NULL);
}

TEST(TestSql, Sql_SqlInit)
{
    TestSql *pTest = new TestSql();
    ASSERT_EQ(true, pTest->pSql != NULL);
    pTest->pSql->SqlInit();
}

TEST(TestSql, Sql_select)
{
    TestSql *pTest = new TestSql();
    ASSERT_EQ(true, pTest->pSql != NULL);
    pTest->pSql->SqlInit();

    ASSERT_EQ(true, pTest->pSql->select("select * from appid_record"));
    pTest->pSql->PrintResult();

}
TEST(TestSql, Sql_insert)
{
    TestSql *pTest = new TestSql();
    ASSERT_EQ(true, pTest->pSql != NULL);
    pTest->pSql->SqlInit();
    std::string sql = "insert into appid_record (appid, description, status, create_time, update_time) values ('10001','test appid','1','2018-07-09','2018-07-09')";

    ASSERT_EQ(true, pTest->pSql->insert(sql));
    ASSERT_EQ(true, pTest->pSql->select("select * from appid_record"));
    pTest->pSql->PrintResult();
}

TEST(TestSql, Sql_update)
{
    TestSql *pTest = new TestSql();
    ASSERT_EQ(true, pTest->pSql != NULL);
    pTest->pSql->SqlInit();
    std::string sql = "update appid_record set status='2' where id='1'";

    ASSERT_EQ(true, pTest->pSql->update(sql));
    ASSERT_EQ(true, pTest->pSql->select("select * from appid_record"));
    pTest->pSql->PrintResult();
}

TEST(TestSql, Sql_delete)
{
    TestSql *pTest = new TestSql();
    ASSERT_EQ(true, pTest->pSql != NULL);
    pTest->pSql->SqlInit();

    std::string appid = "10001";
    std::string sql = "select * from appid_record where appid = '"+appid+"'";
    ASSERT_EQ(true, pTest->pSql->select(sql));
    pTest->pSql->PrintResult();

    appid = "06ec8995107752cc9ac0c10914bd6dc2";
    std::string sql_ = "select * from appid_record where appid = '"+appid+"'";
    ASSERT_EQ(true, pTest->pSql->select(sql_));
    pTest->pSql->PrintResult();

    sql = "select * from appid_record where appid = '06ec8995107752cc9ac0c10914bd6dc2'";
    ASSERT_EQ(true, pTest->pSql->select(sql_));
    pTest->pSql->PrintResult();

    sql = "delete from  appid_record where appid ='10001'";
    ASSERT_EQ(true, pTest->pSql->select(sql));
    ASSERT_EQ(true, pTest->pSql->select("select * from appid_record"));
    pTest->pSql->PrintResult();
}

}