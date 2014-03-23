#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <cmath>
using namespace std;
boost::random::mt19937 gen;
boost::ramdon::uniform_int_distrubition<> source(0,15);
boost::random::normal_distribution<long double> noise(0,1);

struct point{
	long double x,y;
	point (){x=y=0};
	point (const point& a){x=a.x; y=a.y;}
	point (long double _x, long double _y){x=_x; y=_y;}
	point operator+(const point& a) const{
		point tmp(*this);
		tmp.x += a.x;
		tmp.y += a.y;
		return tmp;
	}
	point operator-(const point& a) const{
		point tmp(*this);
		tmp.x -= a.x;
		tmp.y -= a.y;
		return tmp;
	}
	point operator*(const double& a) const{
		point tmp(*this);
		tmp.x *= a;
		tmp.y *= a;
		return tmp;
	}
	point& operator~() {
		this->x += noise(gen);
		this->y += noise(gen);
		return *this;
	}
};
ostream& operator<< (ostream& os, point& a){
	os << "("<<a.x<<", "<<a.y<<")";
	return os;
}
long double E_0 = 0;
long double amp = 0;

long double ebToe0(long double eb){
	
}


void showResult();

void simulation();

int main(){
	long double mean = 0, covariance = 0;
	for()
}
