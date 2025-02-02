#pragma once

class Renderer
{
public:
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual ~Renderer() = 0 {}
};

