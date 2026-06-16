#pragma once

class A;
class B;
class C;

class Base {
	public:
		virtual ~Base();
};

Base* generate(void);
void identify(Base* p);
void identify(Base& p);
