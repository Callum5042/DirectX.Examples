#pragma once

#include "Events.h"
#include "Input.h"

namespace Events
{
	class MousePressedEvent : public IEvent
	{
	public:
		MousePressedEvent();
		void Handle() override;

		MouseData data;
	};

	class MouseReleasedEvent : public IEvent
	{
	public:
		MouseReleasedEvent();
		void Handle() override;

		MouseData data;
	};

	class MouseMotionEvent : public IEvent
	{
	public:
		MouseMotionEvent();
		void Handle() override;

		MouseData data;
	};

	class MouseWheelEvent : public IEvent
	{
	public:
		MouseWheelEvent();
		void Handle() override;

		MouseWheel wheel;
	};

	class InputListener
	{
	public:
		InputListener();
		virtual ~InputListener() = default;

		virtual void OnMouseDown(MouseData&& data) { };
		virtual void OnMouseReleased(MouseData&& data) { };
		virtual void OnMouseMotion(MouseData&& data) { };
		virtual void OnMouseWheel(MouseWheelEvent* e) { };

	private:
		static std::vector<InputListener*> m_InputListeners;
		friend class MousePressedEvent;
		friend class MouseReleasedEvent;
		friend class MouseMotionEvent;
		friend class MouseWheelEvent;
	};
}