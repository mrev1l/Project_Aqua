struct PSInput
{
	float4 m_pos : SV_POSITION;
	float4 m_color : COLOR;
};

float4 main(PSInput _input) : SV_TARGET
{
	return _input.m_color;
}