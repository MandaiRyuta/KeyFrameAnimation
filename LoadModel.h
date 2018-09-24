#pragma once
#include "main.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

//�e���ʂ̃t�@�C���p�X
const std::string BODY = "resource/model/body.x";
const std::string HEAD = "resource/model/head.x";
const std::string LEFTARM = "resource/model/arm_l.x";
const std::string LEFTFOREARM = "resource/model/forearm_l.x";
const std::string RIGHTARM = "resource/model/arm_r.x";
const std::string RIGHTFOREARM = "resource/model/forearm_r.x";
const std::string LEFTLEG = "resource/model/leg_l.x";
const std::string LEFTLOWERLEG = "resource/model/lowerleg_l.x";
const std::string RIGHTLEG = "resource/model/leg_r.x";
const std::string RIGHTLOWERLEG = "resource/model/lowerleg_r.x";

//XFile�ǂݍ��ވׂɕK�v�ȍ\����
struct Model
{
	LPD3DXMESH p_mesh;
	D3DMATERIAL9* p_meshmaterial;
	LPDIRECT3DTEXTURE9* p_meshtexture;
	DWORD dw_material;

	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;
	D3DXMATRIX matrix;

	Model* Parent;
};

struct KEY
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Rotation;
	D3DXVECTOR3 Scale;
};

struct KEY_FRAME
{
	int Frame;
	KEY key[10];

	KEY_FRAME()
	{
		Frame = 30;
	}
};

class Player
{
private:
	enum BONENUMBER
	{
		NUMBER_BODY,
		NUMBER_HEAD,
		NUMBER_LEFTARM,
		NUMBER_RIGHTARM,
		NUMBER_LEFTFOREARM,
		NUMBER_RIGHTFOREARM,
		NUMBER_LEFTLEG,
		NUMBER_LEFTLOWERLEG,
		NUMBER_RIGHTLEG,
		NUMBER_RIGHTLOWERLEG
	};
public:
	Player() {
		toolflag = 1;
		InitOn = false;
		ReadOn = false;
		ReadRun = false;
		InitRun = false;
	}
	~Player() {}
public:
	void Init(LPDIRECT3DDEVICE9 device);
	void Uninit();
	void Draw(LPDIRECT3DDEVICE9 device);
	void Update();
private:
	//Update�Ń��[�h���؂�ւ��x�ɏ�����������֐�
	void DataInit();
private:
	//�t�@�C���p�X
	std::string FilePath_;
	//�p�[�c���̕ϐ�
	Model Part[10];
	//DataInit()�֐����̐���p
	bool InitOn;
	bool ReadOn;
	bool ReadRun;
	bool InitRun;
	//csv�ǂݍ��ݖ���csv�������݃��[�h�֐؂�ւ���悤�ɂ���ϐ�
	int toolflag;
	//csv�t�@�C���ɓ����Ă���10�����̕��ʒP�ʂŐ����Ă���ϐ�
	int KeyCount_;
	//Update�ŃL�[�t���[�����Ɍv�Z������ϐ�
	int m_Key_;
	//Update�ŃL�[�t���[�����Ɍv�Z������ϐ�
	int m_Frame_;
	//�L�[�t���[�������Ǘ����Ă���ϐ�
	KEY_FRAME m_KeyFrame_[1000];

	//Draw�֐��ōs��v�Z������p
	D3DXMATRIX mtx_world_;
	D3DXMATRIX mtx_translation_;
	D3DXMATRIX mtx_rotation_;
	D3DXMATRIX mtx_scale_;

	LPDIRECT3DVERTEXBUFFER9 vertexbuffer_;
	LPDIRECT3DINDEXBUFFER9 indexbuffer_;
};