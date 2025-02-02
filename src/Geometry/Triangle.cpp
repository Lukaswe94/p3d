#include "Triangle.h"

Triangle::Triangle( float height, float base, DirectX::XMFLOAT3 color )
{
	//vertices = {
	//	{DirectX::XMFLOAT3( 0, 0.5 * height,  0 ), color},
	//	{DirectX::XMFLOAT3( 0.5 * base, -0.5 * height,  0), color},
	//	{DirectX::XMFLOAT3( -0.5 * base, -0.5 * height,  0), color}
//		};
	vertices = {
			{DirectX::XMFLOAT3( 0,  height,  0 ), color},
			{DirectX::XMFLOAT3( base,0,  0 ), color},
			{DirectX::XMFLOAT3( 0, 0,  0 ), color}
	};
}

const vertex* Triangle::GetVertices() const
{
	return vertices.data();
}

size_t Triangle::NumberOfVertices() const
{
	return vertices.size();
}

void Triangle::SetPosition(const DirectX::XMFLOAT3 pos)
{
	position = pos;
}

DirectX::XMFLOAT3 Triangle::GetPosition() const
{
	return position;
}
