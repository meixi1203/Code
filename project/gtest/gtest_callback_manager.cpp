#include "gtest/gtest.h"
#include "data_manager.h"
#include "wallet_front_base.h"
#include "log.h"

#define private public
#include "callback_manager.h"

namespace walletfront {

class TestCallbackManager:public testing::Test
{
    public:
        TestCallbackManager()
        {
            pCbManager = new CallBackManager();
        }
        ~TestCallbackManager()
        {
            if(pCbManager)
            {
                pCbManager = NULL;
            }
        }

        virtual void TestBody() {}
        void SetUp()  {}
        void TearDown() {}
    private:
        CallBackManager *pCbManager;
};

TEST(TestCallbackManager, Pointer_TestCallbackManager)
{
    TestCallbackManager *ptest = new TestCallbackManager();
    ASSERT_EQ(true, ptest != NULL);

    TestCallbackManager test;
    ASSERT_EQ(true, test.pCbManager != NULL);
}

TEST(TestCallbackManager, Pointer_pCbManager)
{
    TestCallbackManager *ptest = new TestCallbackManager();
    ASSERT_EQ(true, ptest != NULL);
    ASSERT_EQ(true, ptest->pCbManager != NULL);

    TestCallbackManager test;
    ASSERT_EQ(true, test.pCbManager != NULL);
}

TEST(TestCallbackManager, CreatAccountCallBack)
{
    TestCallbackManager test;
    ASSERT_EQ(true, test.pCbManager != NULL);

    FrontEngine::CallBackMessage cb_message;

    cb_message.set_client_id("CreatAccountCallBack");
    cb_message.set_request_id("cb_message.set_request_id");
    cb_message.set_front_id("cb_message.set_front_id");
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_CreateAccountCallBack);

    FrontEngine::CreateAccountCallBack* pCreatAccount = cb_message.mutable_account();
    FrontEngine::UserInfo* pUserInfo = pCreatAccount->mutable_account();

    if(pCreatAccount)
    {
        pCreatAccount->set_request_id("pCreatAccount->set_request_id");
        pCreatAccount->set_errorcode(1001);
        pCreatAccount->set_error("pCreatAccount->set_error");
        if(pUserInfo)
        {
            pUserInfo->set_type(FrontEngine::enums_CoinType::enums_CoinType_BTC);
            pUserInfo->set_username("pUserInfo->set_username");
            pUserInfo->set_password("pUserInfo->set_password");
            pUserInfo->set_address("pUserInfo->set_address");
        }
        
    }
    test.pCbManager->SerializeToArray(cb_message);

    pCreatAccount->release_account();
    pUserInfo = NULL;
    cb_message.release_account();
    pCreatAccount = NULL;

    char message[MESSAGE_BODY_SIZE] = {'\0'};

    DataManager::GetInstance()->CallBackQueueBack("CreatAccountCallBack", message);
    test.pCbManager->OnResponse(message);

    memset(message, 0, MESSAGE_BODY_SIZE);
    DataManager::GetInstance()->CallBackQueueBack("CreatAccountCallBack", message);
    test.pCbManager->OnResponse(message);
}

TEST(TestCallbackManager, QueryBalanceCallBack)
{
    TestCallbackManager test;
    ASSERT_EQ(true, test.pCbManager != NULL);

    FrontEngine::CallBackMessage cb_message;

    cb_message.set_client_id("QueryBalanceCallBack");
    cb_message.set_request_id("cb_message.set_request_id");
    cb_message.set_front_id("cb_message.set_front_id");
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_QueryBalanceCallBack);

    FrontEngine::QueryBalanceCallBack* pQueryBalance = cb_message.mutable_balance();
    FrontEngine::CoinCapital* pCoinCapital = pQueryBalance->mutable_capital();

    if(pQueryBalance)
    {
        pQueryBalance->set_request_id("QueryBalance->request_id");
        pQueryBalance->set_errorcode(1002);
        pQueryBalance->set_error("pQueryBalance->error");
        if(pCoinCapital)
        {
           pCoinCapital->set_address("pCoinCapital->address");
           pCoinCapital->set_type(FrontEngine::enums_CoinType::enums_CoinType_ETH);
           pCoinCapital->set_amount("pCoinCapital->amount");
           pCoinCapital->set_frozen_amount("pCoinCapital->frozen_amount");
        }
    }
    test.pCbManager->SerializeToArray(cb_message);

    pQueryBalance->release_capital();
    pCoinCapital = NULL;
    cb_message.release_balance();
    pQueryBalance = NULL;

    char message[MESSAGE_BODY_SIZE] = {'\0'};

    DataManager::GetInstance()->CallBackQueueBack("QueryBalanceCallBack", message);
    test.pCbManager->OnResponse(message);

    memset(message, 0, MESSAGE_BODY_SIZE);
    DataManager::GetInstance()->CallBackQueueBack("QueryBalanceCallBack", message);
    test.pCbManager->OnResponse(message);
}

TEST(TestCallbackManager, QueryFeeCallBack)
{
    TestCallbackManager test;
    ASSERT_EQ(true, test.pCbManager != NULL);

    FrontEngine::CallBackMessage cb_message;

    cb_message.set_client_id("QueryFeeCallBack");
    cb_message.set_request_id("cb_message.set_request_id");
    cb_message.set_front_id("cb_message.set_front_id");
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_QueryFeeCallBack);

    FrontEngine::QueryFeeCallBack* pQueryFee = cb_message.mutable_fee();

    if(pQueryFee)
    {
        pQueryFee->set_request_id("pQueryFee->request_id");
        pQueryFee->set_coin_type(FrontEngine::enums_CoinType::enums_CoinType_ETH);
        pQueryFee->set_fee("pQueryFee->fee");
        pQueryFee->set_errorcode(1003); 
        pQueryFee->set_error("pQueryFee->error"); 
    }
    test.pCbManager->SerializeToArray(cb_message);

    cb_message.release_fee();
    pQueryFee = NULL;

    char message[MESSAGE_BODY_SIZE] = {'\0'};
    DataManager::GetInstance()->CallBackQueueBack("QueryFeeCallBack", message);
    test.pCbManager->OnResponse(message);

    memset(message, 0, MESSAGE_BODY_SIZE);
    DataManager::GetInstance()->CallBackQueueBack("QueryFeeCallBack", message);
    test.pCbManager->OnResponse(message);
}

TEST(TestCallbackManager, QueryOrderCallBack)
{
    TestCallbackManager test;
    ASSERT_EQ(true, test.pCbManager != NULL);

    FrontEngine::CallBackMessage cb_message;

    cb_message.set_client_id("QueryOrderCallBack");
    cb_message.set_request_id("cb_message.set_request_id");
    cb_message.set_front_id("cb_message.set_front_id");
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_QueryOrderCallBack);

    FrontEngine::QueryOrderCallBack* pQueryOrder = cb_message.mutable_order();
    FrontEngine::Order* pOrder = pQueryOrder->mutable_order();

    if(pQueryOrder)
    {
        pQueryOrder->set_request_id("pQueryOrder->request_id");
        pQueryOrder->set_errorcode(1004);
        pQueryOrder->set_error("pQueryOrder->error");
        if(pOrder)
        {
            pOrder->set_order_id("pOrder->order_id");
            pOrder->set_type(FrontEngine::enums_CoinType::enums_CoinType_ETH);
            pOrder->set_from_address("pOrder->from_address");
            pOrder->set_to_address("pOrder->to_address");
            pOrder->set_description("pOrder->description");
            pOrder->set_amount("pOrder->amount");
            pOrder->set_fee("pOrder->fee");
            pOrder->set_order_date(20170713);
            pOrder->set_order_time(20170713);
            pOrder->set_status(FrontEngine::enums_OrderStatus::enums_OrderStatus_TO_BE_REPORTED);
        }
    }
    test.pCbManager->SerializeToArray(cb_message);

    pQueryOrder->release_order();
    pOrder = NULL;
    cb_message.release_order();
    pQueryOrder = NULL;

    char message[MESSAGE_BODY_SIZE] = {'\0'};
    DataManager::GetInstance()->CallBackQueueBack("QueryOrderCallBack", message);
    test.pCbManager->OnResponse(message);

    memset(message, 0, MESSAGE_BODY_SIZE);
    DataManager::GetInstance()->CallBackQueueBack("QueryOrderCallBack", message);
    test.pCbManager->OnResponse(message);
}

TEST(TestCallbackManager, QueryOrdersCallBack)
{
    TestCallbackManager test;
    ASSERT_EQ(true, test.pCbManager != NULL);

    FrontEngine::CallBackMessage cb_message;

    cb_message.set_client_id("QueryOrdersCallBack");
    cb_message.set_request_id("cb_message.set_request_id");
    cb_message.set_front_id("cb_message.set_front_id");
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_QueryOrdersCallBack);

    FrontEngine::QueryOrdersCallBack* pQueryOrders = cb_message.mutable_orders();
    FrontEngine::Order* pOrder = NULL;
    if(pQueryOrders)
    {
        pQueryOrders->set_request_id("pQueryOrders->request_id");
        pQueryOrders->set_errorcode(1005);
        pQueryOrders->set_error("pQueryOrders->error");

        for(int index = 0; index < 5; index++)
        {
            pOrder = pQueryOrders->add_order();
            if(pOrder)
            {
                pOrder->set_order_id("pOrder->order_id");
                pOrder->set_type(FrontEngine::enums_CoinType::enums_CoinType_ETH);
                pOrder->set_from_address("pOrder->from_address");
                pOrder->set_to_address("pOrder->to_address");
                pOrder->set_description("pOrder->description");
                pOrder->set_amount("pOrder->amount");
                pOrder->set_fee("pOrder->fee");
                pOrder->set_order_date(20170713);
                pOrder->set_order_time(20170713);
                pOrder->set_status(FrontEngine::enums_OrderStatus::enums_OrderStatus_TO_BE_REPORTED);
            }
        }
    }

    test.pCbManager->SerializeToArray(cb_message);

    cb_message.release_orders();
    pQueryOrders = NULL;
    pOrder = NULL;

    char message[MESSAGE_BODY_SIZE] = {'\0'};

    DataManager::GetInstance()->CallBackQueueBack("QueryOrdersCallBack", message);
    test.pCbManager->OnResponse(message);

    memset(message, 0, MESSAGE_BODY_SIZE);
    DataManager::GetInstance()->CallBackQueueBack("QueryOrdersCallBack", message);
    test.pCbManager->OnResponse(message);
}

TEST(TestCallbackManager, TradedCallBack)
{
    TestCallbackManager test;
    ASSERT_EQ(true, test.pCbManager != NULL);

    FrontEngine::CallBackMessage cb_message;

    cb_message.set_client_id("TradedCallBack");
    cb_message.set_request_id("cb_message.set_request_id");
    cb_message.set_front_id("cb_message.set_front_id");
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_TradedCallBack);

    FrontEngine::TradedCallBack* pTraded = cb_message.mutable_traded_info();
    FrontEngine::Order* pOrder = pTraded->mutable_order();

    if(pTraded)
    {
        pTraded->set_request_id("pTraded->set_request_id");
        pTraded->set_errorcode(1006);
        pTraded->set_error("pTraded->set_error");
        if(pOrder)
        {
            pOrder->set_order_id("pOrder->order_id");
            pOrder->set_type(FrontEngine::enums_CoinType::enums_CoinType_ETH);
            pOrder->set_from_address("pOrder->from_address");
            pOrder->set_to_address("pOrder->to_address");
            pOrder->set_description("pOrder->description");
            pOrder->set_amount("pOrder->amount");
            pOrder->set_fee("pOrder->fee");
            pOrder->set_order_date(20170713);
            pOrder->set_order_time(20170713);
            pOrder->set_status(FrontEngine::enums_OrderStatus::enums_OrderStatus_TO_BE_REPORTED);
        }
    }
    test.pCbManager->SerializeToArray(cb_message);

    pTraded->release_order();
    pOrder = NULL;
    cb_message.release_traded_info();
    pTraded = NULL;

    char message[MESSAGE_BODY_SIZE] = {'\0'};
    DataManager::GetInstance()->CallBackQueueBack("TradedCallBack", message);
    test.pCbManager->OnResponse(message);

    memset(message, 0, MESSAGE_BODY_SIZE);
    DataManager::GetInstance()->CallBackQueueBack("TradedCallBack", message);
    test.pCbManager->OnResponse(message);
}

TEST(TestCallbackManager, CanceledCallBack)
{
    TestCallbackManager test;
    ASSERT_EQ(true, test.pCbManager != NULL);

    FrontEngine::CallBackMessage cb_message;

    cb_message.set_client_id("CanceledCallBack");
    cb_message.set_request_id("cb_message.set_request_id");
    cb_message.set_front_id("cb_message.set_front_id");
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_CanceledCallBack);

    FrontEngine::CanceledCallBack* pCanceled = cb_message.mutable_canceled_info();
    FrontEngine::Order* pOrder = pCanceled->mutable_order();

    if(pCanceled)
    {
        pCanceled->set_request_id("pCanceled->set_request_id");
        pCanceled->set_errorcode(1007);
        pCanceled->set_error("pCanceled->set_error");
        if(pOrder)
        {
            pOrder->set_order_id("pOrder->order_id");
            pOrder->set_type(FrontEngine::enums_CoinType::enums_CoinType_ETH);
            pOrder->set_from_address("pOrder->from_address");
            pOrder->set_to_address("pOrder->to_address");
            pOrder->set_description("pOrder->description");
            pOrder->set_amount("pOrder->amount");
            pOrder->set_fee("pOrder->fee");
            pOrder->set_order_date(20170713);
            pOrder->set_order_time(20170713);
            pOrder->set_status(FrontEngine::enums_OrderStatus::enums_OrderStatus_TO_BE_REPORTED);
        }
    }
    test.pCbManager->SerializeToArray(cb_message);

    pCanceled->release_order();
    pOrder = NULL;
    cb_message.release_canceled_info();
    pCanceled = NULL;

    char message[MESSAGE_BODY_SIZE] = {'\0'};

    DataManager::GetInstance()->CallBackQueueBack("CanceledCallBack", message);
    test.pCbManager->OnResponse(message);

    memset(message, 0, MESSAGE_BODY_SIZE);
    DataManager::GetInstance()->CallBackQueueBack("CanceledCallBack", message);
    test.pCbManager->OnResponse(message);
}

TEST(TestCallbackManager, RejectedCallBack)
{
    TestCallbackManager test;
    ASSERT_EQ(true, test.pCbManager != NULL);

    FrontEngine::CallBackMessage cb_message;

    cb_message.set_client_id("RejectedCallBack");
    cb_message.set_request_id("cb_message.set_request_id");
    cb_message.set_front_id("cb_message.set_front_id");
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_RejectedCallBack);

    FrontEngine::RejectedCallBack* pRejected = cb_message.mutable_rejected_info();
    FrontEngine::Order* pOrder = pRejected->mutable_order();

    if(pRejected)
    {
        pRejected->set_request_id("pRejected->set_request_id");
        pRejected->set_errorcode(1007);
        pRejected->set_error("pRejected->set_error");
        if(pOrder)
        {
            pOrder->set_order_id("pOrder->order_id");
            pOrder->set_type(FrontEngine::enums_CoinType::enums_CoinType_ETH);
            pOrder->set_from_address("pOrder->from_address");
            pOrder->set_to_address("pOrder->to_address");
            pOrder->set_description("pOrder->description");
            pOrder->set_amount("pOrder->amount");
            pOrder->set_fee("pOrder->fee");
            pOrder->set_order_date(20170713);
            pOrder->set_order_time(20170713);
            pOrder->set_status(FrontEngine::enums_OrderStatus::enums_OrderStatus_TO_BE_REPORTED);
        }
    }
    test.pCbManager->SerializeToArray(cb_message);

    pRejected->release_order();
    pOrder = NULL;
    cb_message.release_rejected_info();
    pRejected = NULL;

    char message[MESSAGE_BODY_SIZE] = {'\0'};

    DataManager::GetInstance()->CallBackQueueBack("RejectedCallBack", message);
    test.pCbManager->OnResponse(message);

    memset(message, 0, MESSAGE_BODY_SIZE);
    DataManager::GetInstance()->CallBackQueueBack("RejectedCallBack", message);
    test.pCbManager->OnResponse(message);
}

}