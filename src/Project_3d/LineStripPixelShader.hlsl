struct PS_INPUT
{
	float4 position : SV_Position;
	float4 color : COLOR;
};

float4 main(PS_INPUT data) : SV_TARGET
{
	return data.color;
}