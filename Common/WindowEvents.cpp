
#include "WindowEvents.h"
using Events::EventType;

std::vector<Events::WindowListener*> Events::WindowListener::m_WindowListeners;

Events::WindowListener::WindowListener()
{
	m_WindowListeners.push_back(this);
}

Events::WindowQuit::WindowQuit()
{
	m_EventType = EventType::WINDOW_QUIT;
}

void Events::WindowQuit::Handle()
{
	for (auto& listener : WindowListener::m_WindowListeners)
	{
		listener->OnQuit();
	}
}