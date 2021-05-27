cbuffer transformConstantBuffer
{
	matrix matrixWorld : packoffset(c0);
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float4 color : COLOR;
};

VS_OUTPUT main( float4 pos : POSITION, float4 color : COLOR)
{
	VS_OUTPUT data;
	data.position = pos;
	data.position = mul(pos,matrixWorld);
	data.color = color;
	return data;
}