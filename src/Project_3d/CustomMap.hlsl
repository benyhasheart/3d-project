Texture2D texture0 : register (t0); // texture 
SamplerState sampler0 : register (s0); // sampler 

cbuffer transformConstantBuffer
{
	matrix transform : packoffset(c0);
};

struct VS_input
{
	float4 position :POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR;
	float4 texcoord : TEXTURECOORD;
};

struct VS_output
{
	float4 position :SV_POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR;
	float4 texcoord : TEXCOORD0;
};

struct PS_input
{
	float4 position :POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR;
	float4 texcoord : TEXCOORD0;
};

VS_output vertexShaderMain(VS_input input)
{
	input.position = mul(input.position, transform);
	return input;
}

float4 pixelShaderMain(PS_input input) : SV_TARGET
{
	//return input.color;
	return texture0.Sample(sampler0, input.texcoord);
}


