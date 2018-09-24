#include "grit.h"
constexpr float height = -4.0f;
void Grit::Update()
{
	D3DXMATRIX Identity(world_);
	D3DXMatrixScaling(&world_,1, 1, 1);
	float LINE_X = MINUS_LENGTH;
	float LINE_Z = PLUS_LENGTH;

	for (int i = 0; i < MAXGRITNUM / 2; i += 2)
	{
		if (i != MAXGRITNUM / 2 + 2)
		{
			v[i] = { D3DXVECTOR3(MINUS_LENGTH, height, LINE_Z), D3DCOLOR_RGBA(0, 255, 255, 255) };
			v[i + 1] = { D3DXVECTOR3(PLUS_LENGTH, height, LINE_Z), D3DCOLOR_RGBA(0, 255, 255, 255) };
		}
		else
		{
			v[i] = { D3DXVECTOR3(MINUS_LENGTH, height, LINE_Z), D3DCOLOR_RGBA(0, 255, 255, 255) };
			v[i + 1] = { D3DXVECTOR3(PLUS_LENGTH, height, LINE_Z), D3DCOLOR_RGBA(0, 255, 255, 255) };
		}

		LINE_Z -= 10.0f;
	}
	for (int p = MAXGRITNUM / 2; p < MAXGRITNUM; p += 2)
	{
		if (p != MAXGRITNUM / 3 + 2)
		{
			v[p] = { D3DXVECTOR3(LINE_X, height, PLUS_LENGTH), D3DCOLOR_RGBA(0, 255, 255, 255) };
			v[p + 1] = { D3DXVECTOR3(LINE_X, height, MINUS_LENGTH), D3DCOLOR_RGBA(0, 255, 255, 255) };
		}
		else
		{
			v[p] = { D3DXVECTOR3(LINE_X, height,PLUS_LENGTH), D3DCOLOR_RGBA(0, 255, 255, 255) };
			v[p + 1] = { D3DXVECTOR3(LINE_X, height, MINUS_LENGTH), D3DCOLOR_RGBA(0, 255, 255 , 255) };
		}
		LINE_X += 10.0f;
	}
}

void Grit::Draw()
{
	LPDIRECT3DDEVICE9 device = GetDevice();

	device->SetFVF(FVF_GRIT);

	// ƒ‰ƒCƒg‚ÌŒvŽZˆ—‚ð‚µ‚È‚¢
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->SetTransform(D3DTS_WORLD, &world_);
	
	device->DrawPrimitiveUP(
		D3DPT_LINELIST, MAXGRITNUM / 2, &v[0], sizeof(GRITINFO_3D)
	);
}
