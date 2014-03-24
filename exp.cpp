#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <cmath>
#include <stdlib.h>
using namespace std;
boost::random::mt19937 gen;
boost::random::uniform_int_distribution<> source(0,15);
boost::random::normal_distribution<long double> noise(0,1);

struct point{
	long double x,y;
	point (){x=y=0;};
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
		x += noise(gen);
		y += noise(gen);
		return *this;
	}
	long double distanceSquare(point& a){
		return pow(x-a.x,2) + pow(y-a.y,2);
	}
	friend ostream& operator<< (ostream&, point&);
};
ostream& operator<< (ostream& os, point& a) {
	os << "("<<a.x<<", "<<a.y<<")";
	return os;
}
long double E_0 = 0;
long double amp = 0;

point gray_coding[]{
	{-3,-3},
	{-3,-1},
	{-3, 3},
	{-3, 1},
	{-1,-3},
	{-1,-1},
	{-1, 3},
	{-1, 1},
	{ 3,-3},
	{ 3,-1},
	{ 3, 3},
	{ 3, 1},
	{ 1,-3},
	{ 1,-1},
	{ 1, 3},
	{ 1, 1}
};

point encoding(int n){
	return gray_coding[n];
}

void ebToe0(long double eb){
	E_0 = 4.0*eb/10.0;
	amp = sqrt(E_0);
}

int min_dist(point& a, point* con, int num){
	long double min_d = a.distanceSquare(con[0]);
	int idx = 0;
	for(int i=1;i<num;i++){
		long double dd = a.distanceSquare(con[i]);
		if(dd < min_d){
			min_d = dd;
			idx = i;
		}
	}
	return idx;
}

long double serThe(){
	long double pe = 3.0/4.0*erfc(sqrt(E_0/2));
	return 1-(1-pe)*(1-pe);
}

void showResult(long double SNR, int symbol_error, long long duration,long long bit_all, long long bit0, long long bit1, long long bit2, long long bit3){
	cout<< SNR << "\t" << (long double) symbol_error/duration << "\t" << serThe()<<"\t";
	cout << (long double) bit_all / (4.0 * duration) << "\t";
	cout << (long double) bit0 / (duration) << "\t";
	cout << (long double) bit1 / (duration) << "\t";
	cout << (long double) bit2 / (duration) << "\t";
	cout << (long double) bit3 / (duration) << "\t";
	cout << E_0 <<"\t"<<amp<<endl;
}


int bit_cnt(int src, int rev,int mask){
	int a = src ^ rev;
	//cout << a << endl;
	a &= mask;
	int cnt = 0;
	while(a > 0){
		cnt += a & 1;
		a >>= 1;
	}
	return cnt;
}

void simulation(long double SNR, int symbol_error) {
	long double eb = 2*pow(10.0,SNR/10.0);
	int sending, receiving;
	point snd,rev;
	long long error_cnt = 0, duration = 0,bit_all = 0, bit0 = 0, bit1 = 0, bit2 = 0, bit3 = 0;
	ebToe0(eb);
	point constallation[16];
	for(int i=0;i<16;i++){
		constallation[i] = encoding(i) * amp;
	}
	while(error_cnt < symbol_error){
		duration++;
		sending = source(gen);
		snd = encoding(sending) * amp;
		rev = ~snd;
		receiving = min_dist(rev, constallation, 16);
		if(receiving != sending){
			error_cnt++;
			bit_all += bit_cnt(sending,receiving,15);
			bit0 += bit_cnt(sending,receiving,1);
			bit1 += bit_cnt(sending,receiving,2);
			bit2 += bit_cnt(sending,receiving,4);
			bit3 += bit_cnt(sending,receiving,8);
		}
	}
	showResult(SNR, symbol_error, duration,bit_all,bit0,bit1,bit2,bit3);
}

int main(int argc, char* argv[]){
	gen.seed((unsigned int) time(0));
	/*cout << bit_cnt(0,1,1) <<endl;
	cout << bit_cnt(4,9,7) << endl;
	cout << bit_cnt(3,5,2) << endl;
	for(int i=0;i<16;i++,cout<<endl)
		for(int j=0; j<16; j++)
			cout << bit_cnt(i,j,1) << " ";
	
	for(int i=0;i<16;i++,cout<<endl)
		for(int j=0; j<16; j++)
			cout << bit_cnt(i,j,2) << " ";
	
	for(int i=0;i<16;i++,cout<<endl)
		for(int j=0; j<16; j++)
			cout << bit_cnt(i,j,4) << " ";
	
	for(int i=0;i<16;i++,cout<<endl)
		for(int j=0; j<16; j++)
			cout << bit_cnt(i,j,8) << " ";
	
	for(int i=0;i<16;i++,cout<<endl)
		for(int j=0; j<16; j++)
			cout << bit_cnt(i,j,15) << " ";
	*/
	long double mean = 0, covariance = 0;
	int times = 10000000;
	for(int i=0;i<times;i++){
		long double x = noise(gen);
		mean += x;
		covariance += x*x;
	}
	for(int i=0;i<2000;i++){
		point a = encoding(source(gen)) * 2;
		//cout << a <<endl;
	}
	mean /= times;
	covariance /= times;
	int etime = atoi(argv[1]);
	for(double i = 0;i<=15;i+=0.5){
		simulation(i, etime);
	}

	cout << "noise test: mean "<< mean/times <<", covariance" << covariance / times << endl;
	return 0;
}
