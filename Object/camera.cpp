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
		// 前ベクトルを保存
		D3DXVECTOR3 temp_vector = axis_vector[2];

		// 地をはう準備
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ += temp_vector * SPEED;
		this->at_ += temp_vector * SPEED;
	}
	if (GetKeyboardPress(DIK_D))
	{
		// 前ベクトルを保存
		D3DXVECTOR3 temp_vector = axis_vector[2];

		// 地をはう準備
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ -= temp_vector * SPEED;
		this->at_ -= temp_vector * SPEED;
	}
	if (GetKeyboardPress(DIK_S))
	{
		// 前ベクトルを保存
		D3DXVECTOR3 temp_vector = axis_vector[1];

		// 地をはう準備
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ -= temp_vector * SPEED;
		this->at_ -= temp_vector * SPEED;
	}
	if (GetKeyboardPress(DIK_W))
	{
		// 前ベクトルを保存
		D3DXVECTOR3 temp_vector = axis_vector[1];

		// 地をはう準備
		temp_vector.y = 0.0f;

		D3DXVec3Normalize(&temp_vector, &temp_vector);

		this->eye_ += temp_vector * SPEED;
		this->at_ += temp_vector * SPEED;
	}

	// 右周り
	if (GetKeyboardPress(DIK_E))
	{
		// 回転行列
		constexpr float rotation_speed_Y = 0.05f;
		D3DXMatrixRotationY(&RotationY_, rotation_speed_Y);

		// 向きベクトル
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// 向きベクトル及び右ベクトル(上ベクトル):ベクトルを回転させる
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &RotationY_);

		// 右ベクトルの回転
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &RotationY_);

		this->at_ = this->eye_ + orientation_vector;

		// 前ベクトルを作成( 前ベクトル = 視点座標 - カメラ座標 )
		axis_vector[1] = this->at_ - this->eye_;

		// 前ベクトルを単位ベクトルに正規化
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}


	// 左周り
	if (GetKeyboardPress(DIK_Q))
	{
		// 回転行列
		constexpr float rotation_speed_Y = -0.05f;
		D3DXMatrixRotationY(&RotationY_, rotation_speed_Y);

		// 向きベクトル
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// 向きベクトル及び右ベクトル(上ベクトル):ベクトルを回転させる
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &RotationY_);

		// 右ベクトルの回転
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &RotationY_);

		this->at_ = this->eye_ + orientation_vector;

		// 前ベクトルを作成( 前ベクトル = 視点座標 - カメラ座標 )
		axis_vector[1] = this->at_ - this->eye_;

		// 前ベクトルを単位ベクトルに正規化
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}

	// 上周り
	if (GetKeyboardPress(DIK_T))
	{
		// 回転行列
		constexpr float rotation_speed_Right = -0.05f;
		D3DXMatrixRotationAxis(&RotationX_, &axis_vector[2], rotation_speed_Right);

		// 向きベクトル
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// 向きベクトル及び右ベクトル(上ベクトル):ベクトルを回転させる
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &RotationX_);

		// 右ベクトルの回転
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &RotationX_);

		this->at_ = this->eye_ + orientation_vector;
	}

	// 下周り
	if (GetKeyboardPress(DIK_G))
	{
		// 回転行列
		constexpr float rotation_speed_Right = 0.05f;
		D3DXMatrixRotationAxis(&RotationX_, &axis_vector[2], rotation_speed_Right);

		// 向きベクトル
		D3DXVECTOR3 orientation_vector = this->at_ - this->eye_;

		// 向きベクトル及び右ベクトル(上ベクトル):ベクトルを回転させる
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &RotationX_);

		// 右ベクトルの回転
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &RotationX_);

		this->at_ = this->eye_ + orientation_vector;
	}


	// 右周り( 視点中心 )
	if (GetKeyboardPress(DIK_Y))
	{
		// 回転行列
		constexpr float rotation_speed_Y = 0.05f;
		D3DXMatrixRotationY(&RotationY_, rotation_speed_Y);

		// 向きベクトル
		D3DXVECTOR3 orientation_vector = this->eye_ - this->at_;

		// 向きベクトル及び右ベクトル(上ベクトル):ベクトルを回転させる
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &RotationY_);

		// 右ベクトルの回転
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &RotationY_);

		this->eye_ = this->at_ + orientation_vector;

		// 前ベクトルを作成( 前ベクトル = 視点座標 - カメラ座標 )
		axis_vector[1] = this->at_ - this->eye_;

		// 前ベクトルを単位ベクトルに正規化
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}

	// 左周り
	if (GetKeyboardPress(DIK_H))
	{
		// 回転行列
		constexpr float rotation_speed_Y = -0.05f;
		D3DXMatrixRotationY(&RotationY_, rotation_speed_Y);

		// 向きベクトル
		D3DXVECTOR3 orientation_vector = this->eye_ - this->at_;

		// 向きベクトル及び右ベクトル(上ベクトル):ベクトルを回転させる
		D3DXVec3TransformNormal(&orientation_vector, &orientation_vector, &RotationY_);

		// 右ベクトルの回転
		D3DXVec3TransformNormal(&axis_vector[2], &axis_vector[2], &RotationY_);

		this->eye_ = this->at_ + orientation_vector;

		// 前ベクトルを作成( 前ベクトル = 視点座標 - カメラ座標 )
		axis_vector[1] = this->at_ - this->eye_;

		// 前ベクトルを単位ベクトルに正規化
		D3DXVec3Normalize(&axis_vector[1], &axis_vector[1]);
	}

	//ビュー行列の作成
	D3DXMatrixLookAtLH(
		&view_,
		&eye_,
		&at_,
		&up_
	);

	//プロジェクション行列の作成
	D3DXMatrixPerspectiveFovLH(&this->proj_, D3DX_PI / 3 /*D3DXToRadian(60),*/, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);

	//各種類行列の設定
	device->SetTransform(D3DTS_VIEW, &this->view_);
	device->SetTransform(D3DTS_PROJECTION, &this->proj_);
}
