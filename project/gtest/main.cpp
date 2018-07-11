#include "gtest/gtest.h"

static void SetCurrentDir()
{
    if(chdir("../../") < 0)
    {
        std::cout << "SetCurrentDir error " << std::endl;
        return;
    }
    char current_pwd[100] = {'\0'};
    getcwd(current_pwd, sizeof(current_pwd));

    printf("current dir :%s   \n" , current_pwd);
}

int main(int argc, char **argv)
{   
    SetCurrentDir();
    testing::GTEST_FLAG(output) = "xml:";
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}