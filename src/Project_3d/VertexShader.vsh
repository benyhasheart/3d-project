
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

//struct VS_OUTPUT
//{
//	float4 position : SV_Position; 
//	float4 textureCoord : TEXCOORD0; 
//};
struct VS_input
{
	float4 position :POSITION;
	float4 color : COLOR;
	float4 normal : NORMAL;
	float4 texcoord : TEXTURECOORD;
};

struct VS_output
{
	float4 position :SV_POSITION;
	float4 color : COLOR;
	float4 normal : NORMAL;
	float4 texcoord : TEXCOORD0;
};

VS_output main( VS_input input )
{
	VS_output outputData;
	outputData = input;
	outputData.position = mul(input.position, matrixWorld);
	return outputData;
}