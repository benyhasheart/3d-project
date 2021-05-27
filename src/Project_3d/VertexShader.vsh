
//struct TransformConstantData
//{
//	dxmath::Matrix matrixWorld;
//	dxmath::Matrix matrixView;
//	dxmath::Matrix matrixProjection;
//};

//cbuffer transformConstantBuffer
//{
//	matrix matrixWorld : packoffset(c0);
//	matrix matrixView : packoffset(c4);
//	matrix matrixProjection : packoffset(c8);
//	float2 x : packoffset(c12);
//};
cbuffer transformConstantBuffer
{
	matrix matrixWorld : packoffset(c0);
};

cbuffer viewPerspectiveConstantBuffer
{
	matrix matrixView : packoffset(c4);
	matrix matrixProjection : packoffset(c8);
};

//cbuffer constnatBuffer0
//{
//	float4 color : packoffset(c0);
//};

struct VS_OUTPUT
{
	float4 position : SV_Position; 
	float2 textureCoord : TEXCOORD0; 
};

VS_OUTPUT main( float4 pos : POSITION, float2 texcood : TEXCOORD )
{
	VS_OUTPUT data;
	data.position = pos;
	data.position = mul(pos, matrixWorld);
	//data.position = mul(data.position, matrixView);
	//data.position = mul(data.position, matrixProjection);
	data.textureCoord = texcood;
	return data;
	//return float4(pos.x,pos.y,pos.z,1.0f);
}