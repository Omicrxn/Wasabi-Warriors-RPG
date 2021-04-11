#include "Notifier.h"

Notifier* Notifier::instance = nullptr;

Notifier* Notifier::GetInstance()
{
	if (instance == nullptr) instance = new Notifier();
	return instance;
}