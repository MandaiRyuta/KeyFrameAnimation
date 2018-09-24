#include "camera.h"
#include "../Input/input.h"
#include "../main.h"

D3DXMATRIX Camera::view_;
D3DXMATRIX Camera::proj_;

constexpr float SPEED = 1.5f;

Camera::~Camera()
{
}

void Camera::Update()
{
	LPDIRECT3DDEVICE9 device = GetDevice();

	if (GetKeyboardPress(DIK_A))
	{
		// �O�x�N�g����ۑ�
		D3DXVECTOR3 temp_vector = axis_vector[2];

		// �n���͂�����
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ += temp_vector * SPEED;
		this->at_ += temp_vector * SPEED;
	}
	if (GetKeyboardPress(DIK_D))
	{
		// �O�x�N�g����ۑ�
		D3DXVECTOR3 temp_vector = axis_vector[2];

		// �n���͂�����
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ -= temp_vector * SPEED;
		this->at_ -= temp_vector * SPEED;
	}
	if (GetKeyboardPress(DIK_S))
	{
		// �O�x�N�g����ۑ�
		D3DXVECTOR3 temp_vector = axis_vector[1];

		// �n���͂�����
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ -= temp_vector * SPEED;
		this->at_ -= temp_vector * SPEED;
	}
	if (GetKeyboardPress(DIK_W))
	{
		// �O�x�N�g����ۑ�
		D3DXVECTOR3 temp_vector = axis_vector[1];

		// �n���͂�����
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ += temp_vector * SPEED;
		this->at_ += temp_vector * SPEED;
	}

	// �E����
	if (GetKeyboardPress(DIK_E))
	{
		// ��]�s��
		constexpr float rotation_speed_Y = 0.05f;
		D3DXMatrixRotationY(&RotationY_, rotation_speed_Y);

		// �����x�N�g��
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// �����x�N�g���y�щE�x�N�g��(��x�N�g��):�x�N�g������]������
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &RotationY_);

		// �E�x�N�g���̉�]
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &RotationY_);

		this->at_ = this->eye_ + orientation_vector;

		// �O�x�N�g�����쐬( �O�x�N�g�� = ���_���W - �J�������W )
		axis_vector[1] = this->at_ - this->eye_;

		// �O�x�N�g����P�ʃx�N�g���ɐ��K��
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}


	// ������
	if (GetKeyboardPress(DIK_Q))
	{
		// ��]�s��
		constexpr float rotation_speed_Y = -0.05f;
		D3DXMatrixRotationY(&RotationY_, rotation_speed_Y);

		// �����x�N�g��
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// �����x�N�g���y�щE�x�N�g��(��x�N�g��):�x�N�g������]������
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &RotationY_);

		// �E�x�N�g���̉�]
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &RotationY_);

		this->at_ = this->eye_ + orientation_vector;

		// �O�x�N�g�����쐬( �O�x�N�g�� = ���_���W - �J�������W )
		axis_vector[1] = this->at_ - this->eye_;

		// �O�x�N�g����P�ʃx�N�g���ɐ��K��
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}

	// �����
	if (GetKeyboardPress(DIK_T))
	{
		// ��]�s��
		constexpr float rotation_speed_Right = -0.05f;
		D3DXMatrixRotationAxis(&RotationX_, &axis_vector[2], rotation_speed_Right);

		// �����x�N�g��
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// �����x�N�g���y�щE�x�N�g��(��x�N�g��):�x�N�g������]������
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &RotationX_);

		// �E�x�N�g���̉�]
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &RotationX_);

		this->at_ = this->eye_ + orientation_vector;
	}

	// ������
	if (GetKeyboardPress(DIK_G))
	{
		// ��]�s��
		constexpr float rotation_speed_Right = 0.05f;
		D3DXMatrixRotationAxis(&RotationX_, &axis_vector[2], rotation_speed_Right);

		// �����x�N�g��
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// �����x�N�g���y�щE�x�N�g��(��x�N�g��):�x�N�g������]������
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &RotationX_);

		// �E�x�N�g���̉�]
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &RotationX_);

		this->at_ = this->eye_ + orientation_vector;
	}


	// �E����( ���_���S )
	if (GetKeyboardPress(DIK_Y))
	{
		// ��]�s��
		constexpr float rotation_speed_Y = 0.05f;
		D3DXMatrixRotationY(&RotationY_, rotation_speed_Y);

		// �����x�N�g��
		D3DXVECTOR3 orientation_vector = this->eye_ - this->at_;

		// �����x�N�g���y�щE�x�N�g��(��x�N�g��):�x�N�g������]������
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &RotationY_);

		// �E�x�N�g���̉�]
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &RotationY_);

		this->eye_ = this->at_ + orientation_vector;

		// �O�x�N�g�����쐬( �O�x�N�g�� = ���_���W - �J�������W )
		axis_vector[1] = this->at_ - this->eye_;

		// �O�x�N�g����P�ʃx�N�g���ɐ��K��
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}

	// ������
	if (GetKeyboardPress(DIK_H))
	{
		// ��]�s��
		constexpr float rotation_speed_Y = -0.05f;
		D3DXMatrixRotationY(&RotationY_, rotation_speed_Y);

		// �����x�N�g��
		D3DXVECTOR3 orientation_vector = this->eye_ - this->at_;

		// �����x�N�g���y�щE�x�N�g��(��x�N�g��):�x�N�g������]������
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &RotationY_);

		// �E�x�N�g���̉�]
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &RotationY_);

		this->eye_ = this->at_ + orientation_vector;

		// �O�x�N�g�����쐬( �O�x�N�g�� = ���_���W - �J�������W )
		axis_vector[1] = this->at_ - this->eye_;

		// �O�x�N�g����P�ʃx�N�g���ɐ��K��
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}

	//�r���[�s��̍쐬
	D3DXMatrixLookAtLH(
		&view_,
		&eye_,
		&at_,
		&up_
	);

	//�v���W�F�N�V�����s��̍쐬
	D3DXMatrixPerspectiveFovLH(&this->proj_, D3DX_PI / 3 /*D3DXToRadian(60),*/, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);

	//�e��ލs��̐ݒ�
	device->SetTransform(D3DTS_VIEW, &this->view_);
	device->SetTransform(D3DTS_PROJECTION, &this->proj_);
}
