#pragma once

#include "Events.h"
#include <vector>

namespace Events
{
	class WindowQuit : public IEvent
	{
	public:
		WindowQuit();
		virtual void Handle() override;
	};

	class WindowListener
	{
	public:
		WindowListener();
		virtual ~WindowListener() = default;

		virtual void OnQuit() { }

	private:
		static std::vector<WindowListener*> m_WindowListeners;

		friend class WindowQuit;
	};
}