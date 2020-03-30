#pragma once

#include "InputEvents.h"

class Model : Events::InputListener
{
public:

	bool Init();
	void Update();
	void Render();

	float X;
	float Y;
	float Z;

	void OnMouseDown(MouseData&& data) override;

private:

};