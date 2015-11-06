#include "VoxGame.h"

// Camera controls
void VoxGame::UpdateCamera(float dt)
{
	if (m_cameraMode == CameraMode_FollowPlayer)
	{
		UpdateCameraFollowPlayer(dt);
	}
	else if (m_cameraMode == CameraMode_AutoCamera)
	{
		UpdateCameraAutoCamera(dt);
	}
	else if (m_cameraMode == CameraMode_FirstPerson)
	{
		UpdateCameraFirstPerson(dt);
	}
}

void VoxGame::UpdateCameraFollowPlayer(float dt)
{

}

void VoxGame::UpdateCameraAutoCamera(float dt)
{

}

void VoxGame::UpdateCameraFirstPerson(float dt)
{

}

void VoxGame::UpdateCameraClipping(float dt)
{

}