#pragma once
#include "Shape.h"

class Triangle : public Shape
{
	// Inherited via Shape#
	public:
	Triangle() = delete;
	Triangle( float height, float base, DirectX::XMFLOAT3 color );
	[[nodiscard]] const vertex* GetVertices() const override;
	[[nodiscard]] size_t NumberOfVertices() const override;
	void SetPosition(const DirectX::XMFLOAT3 pos ) override;
	[[nodiscard]] DirectX::XMFLOAT3 GetPosition() const override;


	private:
	std::vector<vertex> vertices;
	DirectX::XMFLOAT3 position;
};

