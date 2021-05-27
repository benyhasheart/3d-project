cbuffer TranseForm
{
	matrix matrixWorld : packoffset(c0);
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float4 color : COLOR;
};

VS_OUTPUT main( float4 pos : POSITION, float4 color : COLOR )
{
	VS_OUTPUT outData;

	outData.position = pos;
	outData.position = mul(pos, matrixWorld);
	outData.color = color;
	return outData;
}