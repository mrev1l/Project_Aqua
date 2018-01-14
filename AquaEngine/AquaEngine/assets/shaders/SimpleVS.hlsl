struct VSInput
{
	float4 m_pos : POSITION;
	float4 m_color : COLOR;
};

struct PSInput
{
	float4 m_pos : SV_POSITION;
	float4 m_color : COLOR;
};

PSInput main(VSInput _input)
{
	PSInput output;
	output.m_pos   = _input.m_pos;
	output.m_color = _input.m_color;
	return output;
}