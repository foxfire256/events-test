
#include <cstdio>
#include <string>
#include <iostream>

#include "events/manager.hpp"
#include "events/message_base.hpp"
#include "events/observer.hpp"

class A : public events::observer
{
public:
	A() = delete;
	A(events::manager_interface *emi) : events::observer(emi) {}
	virtual ~A() = default;
	void subscribe(std::string name) { emi->subscribe(name, this); }
	void process_messages(events::message_base *e) override
	{
		printf("Class A, addr=%llX, message=%s\n", (unsigned long long)this, e->name.c_str());
	}
};

class B : public events::observer
{
public:
	B() = delete;
	B(events::manager_interface *emi) : events::observer(emi) {}
	void subscribe(std::string name) { emi->subscribe(name, this); }
	void process_messages(events::message_base *e) override
	{
		printf("Class B, addr=%llX, message=%s\n", (unsigned long long)this, e->name.c_str());
	}
};

int main(int argc, char **argv)
{
	events::manager *em = new events::manager();
	
	A *a0, *a1, *a2, *a3;

	a0 = new A(em);
	a1 = new A(em);
	a2 = new A(em);
	a3 = new A(em);
	
	a0->subscribe("e1");
	a0->subscribe("e2");
	a1->subscribe("e1");
	a1->subscribe("e1");
	a2->subscribe("e1");
	a2->subscribe("e2");
	a2->subscribe("e3");

	em->post_event(new events::message_base("e2"));
	em->pump_events();

	em->post_event(new events::message_base("e1"));
	em->pump_events();

	delete a1;
	a1 = nullptr;

	em->post_event(new events::message_base("e1"));
	em->pump_events();

	delete a0;
	delete a1;
	delete a2;
	delete a3;

	delete em;

#ifdef _MSC_VER
	system("PAUSE");
#endif
	
	return 0;
}