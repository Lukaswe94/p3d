#pragma once
#include "Shape.h"

class Square : public Shape	
{
public:
	Square() = delete;

	Square( float height, float base, DirectX::XMFLOAT3 color );
	[[nodiscard]] const vertex* GetVertices() const override;
	[[nodiscard]] size_t NumberOfVertices() const override;
	void SetPosition(const DirectX::XMFLOAT3 pos) override;
	[[nodiscard]] DirectX::XMFLOAT3 GetPosition() const override;
};

