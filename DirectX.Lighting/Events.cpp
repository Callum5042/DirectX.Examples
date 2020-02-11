
#include "Events.h"
#include <SDL_events.h>
//#include "InputEvents.h"
#include "WindowEvents.h"
#include "Engine.h"

namespace
{
	void PollSDLEvents()
	{
		auto dispatcher = Engine::GetInstance()->GetEventDispatcher();

		SDL_Event e{};
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				dispatcher->AddEvent(new Events::WindowQuit());
				break;

			/*case SDL_MOUSEBUTTONDOWN:
			{
				auto mouseEvent = new Events::MousePressedEvent();
				mouseEvent->data.button = static_cast<MouseButton>((1 << e.button.button));
				mouseEvent->data.x = e.button.x;
				mouseEvent->data.y = e.button.y;

				dispatcher->AddEvent(mouseEvent);
				break;	
			}

			case SDL_MOUSEBUTTONUP:
			{
				auto mouseEvent = new Events::MouseReleasedEvent();
				mouseEvent->data.button = static_cast<MouseButton>((1 << e.button.button));
				mouseEvent->data.x = e.button.x;
				mouseEvent->data.y = e.button.y;

				dispatcher->AddEvent(mouseEvent);
				break;
			}

			case SDL_MOUSEWHEEL:
			{
				auto mouseEvent = new Rove::Events::MouseWheelEvent();

				if (e.wheel.y > 0)
				{
					mouseEvent->wheel = Rove::MouseWheel::WHEEL_UP;
				}
				else if (e.wheel.y < 0)
				{
					mouseEvent->wheel = Rove::MouseWheel::WHEEL_DOWN;
				}

				dispatcher->AddEvent(mouseEvent);

				break;
			}

			case SDL_MOUSEMOTION:
			{
				auto mouseEvent = new Rove::Events::MouseMotionEvent();
				mouseEvent->data.button = static_cast<Rove::MouseButton>(e.motion.state);
				mouseEvent->data.x = e.motion.x;
				mouseEvent->data.y = e.motion.y;
				mouseEvent->data.xrel = e.motion.xrel;
				mouseEvent->data.yrel = e.motion.yrel;

				dispatcher->AddEvent(mouseEvent);
				break;
			}*/
			}
		}
	}
}

void Events::EventDispatcher::PollEvents()
{
	PollSDLEvents();

	while (!m_Events.empty())
	{
		IEvent* e = m_Events.front();
		e->Handle();

		m_Events.pop();
		delete e;
	}
}

void Events::EventDispatcher::AddEvent(IEvent* e)
{
	m_Events.push(e);
}