#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& graphics) noexcept
{
	UINT offset = 0u;
	graphics.GetDeviceContext()->IASetVertexBuffers(0, 1u, mVertexBuffer.GetAddressOf(), &mStride, &offset);
}

