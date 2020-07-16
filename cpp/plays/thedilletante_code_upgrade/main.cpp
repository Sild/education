#include <iostream>
#include <vector>
class Person {
public:
    virtual ~Person() = default;
    virtual void kick() const = 0;
};

void bit(const std::vector<Person*> &persons) {
    for(const auto p: persons) {
        p->kick();
    }
}

class Player: public Person {
public:
    Player(int i) {
        init(i);
    }
    virtual void kick() const override {
        std::cout << "player was kicked!" << std::endl;
    }

private:
    int number;
    void init(int num) {
        number = num;
        std::cout << "person №" << number << " created" << std::endl;
    }
};



int main() {
    int player_count = 4;
    std::vector<Person*> players;
    for(int i = 0; i < player_count; i++) {
        players.push_back(new Player(i));
    }
    bit(players);
}