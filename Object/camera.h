#pragma once
#include "../main.h"

class Camera
{
private:
	D3DXVECTOR3 eye_;
	D3DXVECTOR3 at_;
	D3DXVECTOR3 up_;

	D3DXMATRIX RotationY_;
	D3DXMATRIX RotationX_;

	// カメラの前に物を動かすのに便利
	D3DXVECTOR3 axis_vector[3] = {
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),	// 上ベクトル
		D3DXVECTOR3(0.0f, 0.0f, 1.0f),	// 前ベクトル
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),	// 右ベクトル
	};

	static D3DXMATRIX view_;
	static D3DXMATRIX proj_;
public:
	Camera(D3DXVECTOR3 at, D3DXVECTOR3 eye , D3DXVECTOR3 up)
		: at_(at), eye_(eye), up_(up)
	{

	}
	~Camera();
public:
	void Update();

	D3DXVECTOR3 SetAt() { return at_; }
	D3DXVECTOR3 SetEye() { return eye_; }
	D3DXVECTOR3 SetUp() { return up_; }

	static D3DXMATRIX SetView() { return view_; }
	static D3DXMATRIX SetProj() { return proj_; }
};

