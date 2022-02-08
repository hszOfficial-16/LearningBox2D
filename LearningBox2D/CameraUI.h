#ifndef CAMERA_H_
#define CAMERA_H_

// Draw the scene
#include "character.h"

// SDL Renderer
#include "graphic.h"

class UICamera
{
public:

private:

public:
	~UICamera() = default;
	UICamera(const UICamera&) = delete;
	UICamera& operator=(const UICamera&) = delete;
	static UICamera& GetInstance()
	{
		static UICamera instance;
		return instance;
	}
private:
	UICamera();
};

#endif