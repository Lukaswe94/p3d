#pragma once
#include "Vertex.h"
#include <vector>
class Shape
{
	public:
	[[nodiscard]] virtual const vertex* GetVertices() const = 0;
	[[nodiscard]] virtual size_t NumberOfVertices() const = 0;
	virtual void SetPosition( const DirectX::XMFLOAT3 pos ) = 0;
	[[nodiscard]] virtual DirectX::XMFLOAT3 GetPosition() const = 0;
};

