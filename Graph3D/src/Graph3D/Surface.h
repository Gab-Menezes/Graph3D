#pragma once

class Surface
{
public:
	Surface() {};
	~Surface() {};

	virtual void Draw() const = 0; //Draws the Graph3D
	virtual void ImGuiDraw() = 0;
	virtual bool IsSolid() const = 0;
};
