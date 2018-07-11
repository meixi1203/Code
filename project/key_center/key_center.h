/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef KEY_CENTER_H
#define KEY_CENTER_H

#include <iostream>

namespace walletfront {
class Sql;
class KeyCenter
{
    public:
        KeyCenter();
        ~KeyCenter();

    public:
        bool CheckAppId(const std::string &app_id);
        void KeyCenterInit();

    private:
        KeyCenter(const KeyCenter&);
        KeyCenter& operator=(const KeyCenter&);

    private:
        Sql* m_sql;
};

}
#endif //KEY_CENTER_H