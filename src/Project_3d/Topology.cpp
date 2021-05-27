#include "Topology.h"

Topology::Topology(D3D_PRIMITIVE_TOPOLOGY topology)
	: mTopology(topology)
{

}

void Topology::Bind(Graphics& graphics) noexcept
{
	graphics.GetDeviceContext()->IASetPrimitiveTopology(mTopology);
}