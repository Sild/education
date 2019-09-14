#include "print_lib.h"
#include <list>
#include <vector>
#include <map>
//Печать адреса как char(-1)
//Печать адреса как short(0)
//Печать адреса как int(2130706433)
//Печать адреса как long(8875824491850138409)
//Печать адреса как std::string
//Печать адреса как std::vector
//Печать адреса как std::list
//Опционально. Печать адреса как std::tuple

int main() {
    print_ip(char(-1));
    print_ip(short(0));
    print_ip(int(2130706433));
    print_ip(long(8875824491850138409));

    print_ip(std::string("192.168.1.2"));
    print_ip(std::vector<int>{172, 17, 0, 0, 1});
    print_ip(std::list<int>{172, 17, 0, 0, 2});

    print_ip(std::tuple<int, int, int, int, int>{172, 17, 0, 0, 3});
    return 0;
}
