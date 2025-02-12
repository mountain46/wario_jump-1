#include "DxLib.h"
#include "game.h"
#include "car.h"

namespace
{
	constexpr int kWaitFrameMin = 60;
	constexpr int kWaitFrameMax = 180;

	constexpr float kSpeed = -24.0f;

	//車のジャンプ力
	constexpr float CarJump= -20.0f;
	//車の重力
	constexpr float CarGravity = 0.8f;
	//車の飛ぶ位置
	constexpr int CarJumpPos = 470;
}

Car::Car()
{
	m_handle = -1;
	m_fieldY = 0.0f;
	m_waitFrame = 0;
	m_countTimer = 0;
}

void Car::setGraphic(int handle)
{
	m_handle = handle;
	GetGraphSizeF(m_handle, &m_size.x, &m_size.y);
}

void Car::setup(float fieldY)
{
	m_fieldY = fieldY;
	m_pos.x = Game::kScreenWidth + 16.0f;
	m_pos.y = m_fieldY - m_size.y;

	m_vec.x = kSpeed;
	m_vec.y = 0.0f;

	int randNum = GetRand(99);
	if (randNum < 38 )
	{
		m_moveType = kMoveTypeNormal;
	}
	else if (randNum < 38 + 30)
	{
		m_moveType = kMoveTypeStop;
	}
	else if (randNum < 38 + 30 + 30)
	{
		m_moveType = kMoveTypeJump;
	}
	else
	{
		m_moveType = kMoveTypeReturn;
	}

	//移動し始めるまでの時間を設定	1秒から3秒待つ	60フレームから180フレーム
	m_waitFrame = GetRand(kWaitFrameMax - kWaitFrameMin) + kWaitFrameMin;
}

void Car::update()
{
	if (m_waitFrame > 0)
	{
		m_waitFrame--;
		return;
	}

	switch (m_moveType)
	{
	case kMoveTypeNormal:
		updateNormal();
		break;
	case kMoveTypeStop:
		updateStop();
		break;
	case kMoveTypeJump:
		updateJump();
		break;
	case kMoveTypeReturn:
		updateReturn();
		break;
	default:
		updateNormal();
		break;
		
	}
	
}

void Car::draw()
{
	DrawGraphF(m_pos.x, m_pos.y, m_handle, true);
}

//まっすぐ進む
void  Car::updateNormal()
{
	m_pos += m_vec;
}
//一時停止フェイント
void  Car::updateStop()
{
	m_pos += m_vec;
	if (m_pos.x <= Game::kScreenWidth /2)
	{
		m_vec.x = 0;

		m_countTimer++;
		if (m_countTimer > 20)
		{
			m_vec.x = kSpeed;
		}
	}


}
//ジャンプする
void  Car::updateJump()
{
	m_pos += m_vec;
	//地面との当たり
	bool ifFieldC = false;
	if (m_pos.y > m_fieldY - m_size.y)
	{
		m_pos.y = m_fieldY - m_size.y;
		ifFieldC = true;
	}

	if (m_vec.y < CarJumpPos)
	{


		if (ifFieldC)// 1回しかジャンプできなくする
		{
			m_vec.y = CarJump; //ジャンプ開始

		}
	}
	m_vec.y += CarGravity;
}
//途中で引き返す（必ず成功する）
void  Car::updateReturn()
{
	m_pos += m_vec;
	if (m_pos.x <= Game::kScreenWidth -150)
	{
		m_vec.x++;
	}
}