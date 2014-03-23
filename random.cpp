#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

boost::random::mt19937 gen;
int roll_die() {
    boost::random::uniform_int_distribution<> dist(1,6);
    return dist(gen);
}
int main(){
    std::cout << roll_die();
    return 0;
}
