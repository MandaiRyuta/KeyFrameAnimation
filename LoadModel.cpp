#include "LoadModel.h"
#include "main.h"
#include "Object\camera.h"

#pragma warning (disable:4996)

//���[�V�����̃X�g�b�N���@�t�@�C���̍s���ŃX�g�b�N�������m
//FILE_DATA�̓��f���̈ʒu�A��]�A�g��k���̏�������������
//����̓q�g�^�Ł@10���� * 3�̏��(�ʒu�A��]�A�g��k��) * 2�t���[���� 60�͕K���m�ۂ��Ă������ق����ǂ��B
constexpr int FILE_DATA = 900;

void Player::Init(LPDIRECT3DDEVICE9 device)
{
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;
	//�L�[���t���[���̏�����
	m_Key_ = 0;
	m_Frame_ = 0;

	for (int i = 0; i < 10; i++)
	{
		//for���[�v�ɉ�����std::string FilePath_�̕������ς���
		switch (i)
		{
		case NUMBER_BODY:
			FilePath_ = BODY;
			break;
		case NUMBER_HEAD:
			FilePath_ = HEAD;
			break;
		case NUMBER_LEFTARM:
			FilePath_ = LEFTARM;
			break;
		case NUMBER_RIGHTARM:
			FilePath_ = RIGHTARM;
			break;
		case NUMBER_LEFTFOREARM:
			FilePath_ = LEFTFOREARM;
			break;
		case NUMBER_RIGHTFOREARM:
			FilePath_ = RIGHTFOREARM;
			break;
		case NUMBER_LEFTLEG:
			FilePath_ = LEFTLEG;
			break;
		case NUMBER_LEFTLOWERLEG:
			FilePath_ = LEFTLOWERLEG;
			break;
		case NUMBER_RIGHTLEG:
			FilePath_ = RIGHTLEG;
			break;
		case NUMBER_RIGHTLOWERLEG:
			FilePath_ = RIGHTLOWERLEG;
			break;

		}

		if (FAILED(D3DXLoadMeshFromX(FilePath_.c_str(), D3DXMESH_SYSTEMMEM, device, NULL, &pD3DXMtrlBuffer, NULL, &Part[i].dw_material, &Part[i].p_mesh)))
		{
			MessageBox(NULL, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", FilePath_.c_str(), MB_OK);
			PostQuitMessage(0);
		}

		D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
		Part[i].p_meshmaterial = new D3DMATERIAL9[Part[i].dw_material];
		Part[i].p_meshtexture = new LPDIRECT3DTEXTURE9[Part[i].dw_material];

		for (DWORD p = 0; p < Part[i].dw_material; p++)
		{
			Part[i].p_meshmaterial[p] = d3dxMaterials[p].MatD3D;
			Part[i].p_meshmaterial[p].Ambient = Part[i].p_meshmaterial->Diffuse;
			Part[i].p_meshtexture[p] = NULL;

			if (d3dxMaterials[p].pTextureFilename != NULL &&strlen(d3dxMaterials[p].pTextureFilename) > 0)
			{
				if (FAILED(D3DXCreateTextureFromFile(device, d3dxMaterials[p].pTextureFilename, &Part[p].p_meshtexture[p])))
				{
					MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
					PostQuitMessage(0);
				}
			}
		}
	}

	pD3DXMtrlBuffer->Release();

	//�����̐e�͒N�Ȃ̂���ݒ肵�Ă���B�i�e�̃|�C���^�[�ݒ�j
	Part[NUMBER_BODY].Parent = NULL;							//���� �@= NULL
	Part[NUMBER_HEAD].Parent = &Part[NUMBER_BODY];				//���� �@= ����
	Part[NUMBER_LEFTARM].Parent = &Part[NUMBER_BODY];			//����r = ����
	Part[NUMBER_LEFTFOREARM].Parent = &Part[NUMBER_LEFTARM];	//���O�r = ����r
	Part[NUMBER_RIGHTARM].Parent = &Part[NUMBER_BODY];			//�E��r = ����
	Part[NUMBER_RIGHTFOREARM].Parent = &Part[NUMBER_RIGHTARM];	//�E�O�r = �E��r
	Part[NUMBER_LEFTLEG].Parent = &Part[NUMBER_BODY];			//����� = ����
	Part[NUMBER_LEFTLOWERLEG].Parent = &Part[NUMBER_LEFTLEG];	//������ = �����
	Part[NUMBER_RIGHTLEG].Parent = &Part[NUMBER_BODY];			//�E��� = ����
	Part[NUMBER_RIGHTLOWERLEG].Parent = &Part[NUMBER_RIGHTLEG]; //�E���� = �E���
}

void Player::DataInit()
{
	//�L�[�t���[���̔z��p�̐��𐔂���ϐ�
	//�ʒu�������͉�]�������̓X�P�[���̃t�@�C���ǂݍ��݂̂ǂ����ŃJ�E���g������B
	int BoneData = 0;
	int p = 0;
	//toolflag == -1�@:�@csv�t�@�C���ɕۑ����Ă���f�[�^��1�񂾂��ǂݍ���
	if (toolflag == -1)
	{
		if (!ReadOn)
		{
			std::ifstream readfile("ModelAnimation.csv");

			if (!readfile)
			{
				//�G���[����
			}

			std::string FileData[FILE_DATA][3];
			std::string str = "";

			KeyCount_ = 0;
			int DataBlock = 0;
			int DataCount = 0;
			while (std::getline(readfile, str))
			{
				std::string tmp = "";
				std::istringstream stream(str);
				
				while (std::getline(stream, tmp,','))
				{
					FileData[DataBlock][DataCount] = tmp.c_str();
					DataCount++;
				}

				DataCount = 0;
				DataBlock++;
			}
			int count = 0;
			char* endptr = {};
			for (int p = 0; p < DataBlock - 1; p+=3)
			{
				if (count >= 10)
				{
					KeyCount_++;
					count = 0;
				}


				m_KeyFrame_[KeyCount_].key[count].Position.x = strtof(FileData[p][0].c_str(),&endptr);
				m_KeyFrame_[KeyCount_].key[count].Position.y = strtof(FileData[p][1].c_str(),&endptr);
				m_KeyFrame_[KeyCount_].key[count].Position.z = strtof(FileData[p][2].c_str(),&endptr);

				m_KeyFrame_[KeyCount_].key[count].Rotation.x = strtof(FileData[p + 1][0].c_str(),&endptr);
				m_KeyFrame_[KeyCount_].key[count].Rotation.y = strtof(FileData[p + 1][1].c_str(),&endptr);
				m_KeyFrame_[KeyCount_].key[count].Rotation.z = strtof(FileData[p+1][2].c_str(),&endptr);

				m_KeyFrame_[KeyCount_].key[count].Scale.x = strtof(FileData[p + 2][0].c_str(),&endptr);
				m_KeyFrame_[KeyCount_].key[count].Scale.y = strtof(FileData[p + 2][1].c_str(),&endptr);
				m_KeyFrame_[KeyCount_].key[count].Scale.z = strtof(FileData[p+2][2].c_str(),&endptr);

				count++;
			}
			
			ReadOn = true;
			ReadRun = true;
			InitRun = false;
			InitOn = false;
		}
	}
	//toolflag == 1   :  T�{�[���̏�Ԃ�1�x�����Z�b�g����B
	else if (toolflag == 1)
	{
		if (!InitOn)
		{
			//����
			Part[NUMBER_BODY].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Part[NUMBER_BODY].rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Part[NUMBER_BODY].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			//����
			//*�e�̃|�C���^�[���ݒ肳��Ă�����̑S��*�@�e����̑��΍��W��ݒ肷��@��
			Part[NUMBER_HEAD].position = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
			Part[NUMBER_HEAD].rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Part[NUMBER_HEAD].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			//����r
			Part[NUMBER_LEFTARM].position = D3DXVECTOR3(0.2f, 1.0f, 0.0f);
			Part[NUMBER_LEFTARM].rotation = D3DXVECTOR3(0.0f, D3DXToRadian(90.0f), 0.0f);
			Part[NUMBER_LEFTARM].scale = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
			//���O�r
			Part[NUMBER_LEFTFOREARM].position = D3DXVECTOR3(0.0f, 0.0f, 5.0f);
			Part[NUMBER_LEFTFOREARM].rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Part[NUMBER_LEFTFOREARM].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			//�E��r
			Part[NUMBER_RIGHTARM].position = D3DXVECTOR3(-0.2f, 1.0f, 0.0f);
			Part[NUMBER_RIGHTARM].rotation = D3DXVECTOR3(0.0f, D3DXToRadian(90.0f), 0.0f);
			Part[NUMBER_RIGHTARM].scale = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
			//�E�O�r
			Part[NUMBER_RIGHTFOREARM].position = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
			Part[NUMBER_RIGHTFOREARM].rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Part[NUMBER_RIGHTFOREARM].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			//�����
			Part[NUMBER_LEFTLEG].position = D3DXVECTOR3(0.5f, -1.5f, 0.0f);
			Part[NUMBER_LEFTLEG].rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Part[NUMBER_LEFTLEG].scale = D3DXVECTOR3(0.6f, 0.6f, 0.6f);
			//������
			Part[NUMBER_LEFTLOWERLEG].position = D3DXVECTOR3(0.0f, -3.0f, 0.0f);
			Part[NUMBER_LEFTLOWERLEG].rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Part[NUMBER_LEFTLOWERLEG].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			//�E���
			Part[NUMBER_RIGHTLEG].position = D3DXVECTOR3(-0.5f, -1.5f, 0.0f);
			Part[NUMBER_RIGHTLEG].rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Part[NUMBER_RIGHTLEG].scale = D3DXVECTOR3(0.6f, 0.6f, 0.6f);
			//�E����
			Part[NUMBER_RIGHTLOWERLEG].position = D3DXVECTOR3(0.0f, -3.0f, 0.0f);
			Part[NUMBER_RIGHTLOWERLEG].rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Part[NUMBER_RIGHTLOWERLEG].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			//�������܂�


			//ImGui���ɏ������p�̏���n��
			for (int i = 0; i < 10; i++)
			{
				ImGui::InitModelData(i, Part[i].position, Part[i].rotation, Part[i].scale);
			}

			InitOn = true;
			InitRun = true;
			ReadRun = false;
			ReadOn = false;
		}
	}
}

void Player::Uninit()
{
	for (int i = 0; i < 10; i++)
	{
		if (Part[i].p_mesh != NULL)
		{
			Part[i].p_mesh->Release();
			Part[i].p_mesh = NULL;
		}
		delete Part[i].p_meshtexture;
		delete Part[i].p_meshmaterial;
	}
}

void Player::Draw(LPDIRECT3DDEVICE9 device)
{
	//for(int t = 0; t < 10; t++ )�e�p�[�c�̏�� �𖈉�`�悳�����

	//Update()�Ŋe���ʂŃL�[�t���[�����Ɍv�Z���ꂽ�l���s��Ɍv�Z�����ā@
	//world = scale * rotation * translation;�@�Ŋ|�����킹�ăf�o�C�X�֏���n���Ă���
	for (int t = 0; t < 10; t++)
	{	
		D3DXMatrixIdentity(&mtx_world_);
		D3DXMatrixScaling(&mtx_scale_, Part[t].scale.x, Part[t].scale.y, Part[t].scale.z);
		D3DXMatrixMultiply(&mtx_world_, &mtx_world_, &mtx_scale_);
		
		D3DXMatrixRotationYawPitchRoll(&mtx_rotation_, Part[t].rotation.y, Part[t].rotation.x, Part[t].rotation.z);
		D3DXMatrixMultiply(&mtx_world_, &mtx_world_, &mtx_rotation_);

		D3DXMatrixTranslation(&mtx_translation_, Part[t].position.x, Part[t].position.y, Part[t].position.z);
		D3DXMatrixMultiply(&mtx_world_, &mtx_world_, &mtx_translation_);

		if (Part[t].Parent != NULL)
		{
			D3DXMatrixMultiply(&mtx_world_, &mtx_world_, &Part[t].Parent->matrix);
		}
		//mtx_world_ = mtx_world_ * Camera::SetView() * Camera::SetProj();
		Part[t].matrix = mtx_world_;

		//���C�g�𖳌��ɂ���
		device->LightEnable(0, FALSE);
		// ���C�g�̌v�Z���������Ȃ�
		device->SetRenderState(D3DRS_LIGHTING, FALSE);
		//�}�e���A���̐ݒ�

		device->SetTransform(D3DTS_WORLD, &mtx_world_);

		for (DWORD i = 0; i < Part[t].dw_material; i++)
		{
			Part[t].p_mesh->DrawSubset(i);
		}
	}
}

void Player::Update()
{
	//ImGui���Ō�X
	if (ImGui::Button("ToolButton"))
	{
		ImGui::ToolFlag(toolflag);
	}

	//���[�h�ؑւ̎���1��Ă΂��֐�
	DataInit();

	//�t�@�C���ǂݍ��݂̏ꍇ�Ə�������Ԃ̏ꍇ�ɕ����Ă���B
	if (ReadRun)
	{	//�A�j���[�V�����f�[�^�̌v�Z
		//m_Key�̏���m_Key + 1�̏����擾���A�t���[���P�ʂňړ��������v�Z���APart[]�֏���n���Ă���B
		for (int i = 0; i < 10; i++)
		{
			//csv�t�@�C���̃L�[�̌��𒴂������ɂȂ����ꍇ�Ƀ��Z�b�g������
			if (m_Key_ >= KeyCount_)
			{
				m_Key_ = 0;
			}
			//�t���[�����[�g�̌v�Z
			//m_Frame��Init�łO�ɏ���������Ă���
			//m_KeyFrame[m_Key].Frame�͍\���̂̃R���X�g���N�^�[��30�ɐݒ肵�Ă���
			float rate = (float)m_Frame_ / m_KeyFrame_[m_Key_].Frame;

			//Part[].position,Part[].rotation,Part[].scale��3�����Ɍv�Z���đ�����Ă���ϐ�
			//�����̃R�����g�͒��ڌv�Z�������Ă����
			//D3DXVECTOR3 curPos = m_KeyFrame_[m_Key_].key[i].Position;
			//D3DXVECTOR3 curRot = m_KeyFrame_[m_Key_].key[i].Rotation;
			//D3DXVECTOR3 curScale = m_KeyFrame_[m_Key_].key[i].Scale;
			//
			//D3DXVECTOR3 nextPos = m_KeyFrame_[m_Key_ + 1].key[i].Position;
			//D3DXVECTOR3 nextRot = m_KeyFrame_[m_Key_ + 1].key[i].Rotation;
			//D3DXVECTOR3 nextScale = m_KeyFrame_[m_Key_+1].key[i].Scale;

			Part[i].position = m_KeyFrame_[m_Key_].key[i].Position * (1.0f - rate) + m_KeyFrame_[m_Key_ + 1].key[i].Position * rate;
			Part[i].rotation = m_KeyFrame_[m_Key_].key[i].Rotation * (1.0f - rate) + m_KeyFrame_[m_Key_ + 1].key[i].Rotation * rate;
			Part[i].scale = m_KeyFrame_[m_Key_].key[i].Scale * (1.0f - rate) + m_KeyFrame_[m_Key_ + 1].key[i].Scale * rate;

			//ImGui���ŕ��ʂ��Ƃ̏���\��������悤�ɂ��Ă���B
			ImGui::GetPositionInfomation("position", Part[i].position);
			ImGui::GetScaleInfomation("scale", Part[i].scale);
			ImGui::GetRotationInfomation("rotation", Part[i].rotation);
		}
		m_Frame_++;

		if (m_Frame_ >= m_KeyFrame_[m_Key_].Frame)
		{
			m_Key_++;
			m_Frame_ = 0;
		}
	}
	if (InitRun)
	{	//�e����ImGui���ŎQ�Ƃ����Ĉ����̒l���������悤�ɂ��Ă���B
		for (int i = 0; i < 10; i++)
		{
			ImGui::SetModelInfo(i,"Setting", Part[i].position, Part[i].rotation, Part[i].scale);
		}
	}
}
