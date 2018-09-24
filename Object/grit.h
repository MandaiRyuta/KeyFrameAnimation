#pragma once

#include "../main.h"

constexpr int MAXGRITNUM = 10000;
constexpr float PLUS_LENGTH = 10000.0f;
constexpr float MINUS_LENGTH = -10000.0f;

#define FVF_GRIT (D3DFVF_XYZ | D3DFVF_DIFFUSE)

typedef struct
{
	D3DXVECTOR3 pos;
	D3DCOLOR color;
}GRITINFO_3D;

class Grit
{
public:
	Grit() {}
	~Grit() {}

	void Update();
	void Draw();
private:
	GRITINFO_3D v[MAXGRITNUM];

	D3DXMATRIX world_;

	float gritx_;
	float gritz_;
};