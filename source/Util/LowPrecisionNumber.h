template<typename storage,unsigned int digit>
struct LowPrecisionNumber{
	storage t;
	LowPrecisionNumber operator=(double a){
		t = a*pow(10,digit);
		return *this;
	}
	LowPrecisionNumber operator=(float a){
		t = a*pow(10,digit);
		return *this;
	}
	operator double(){
		return t/pow(10,digit);
	}
};