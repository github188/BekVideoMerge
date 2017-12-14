#pragma once

#ifndef __BEK_HIK_UTIL_H__
#define __BEK_HIK_UTIL_H__



#ifndef DLL_HIKUTIL_API
#define DLL_HIKUTIL_API __declspec(dllexport)
#endif

/**
* @class CBekHikUtil
* @brief �����豸������װ��
*
* ���ڲ��������豸������������������NVR��DVR �ȣ�
*/

class CBekHikUtil
{
public:
	
	/** @brief ���� sdk ��ʼ��
	*	@return  ���ز������
	*	 - false ��ʼ��ʧ��
	*	 - true	��ʼ���ɹ�
	*/
	static DLL_HIKUTIL_API bool WINAPI InitSDK();

	/** @brief ��¼�����豸
	*	@param[in]	ip		�豸IP
	*	@param[in]	username 	�û���
	*	@param[in]	password 	����
	*	@param[in]	port	 	�˿�
	*	@param[out] userId	�����¼id
	*	@return  ���ز������
	*	 - false ��¼ʧ��
	*	 - true	��¼�ɹ�
	*/
	static DLL_HIKUTIL_API bool WINAPI LoginDevice(wstring ip, wstring username, wstring password, int port, int &userId);

	/** @brief ��ȡ�豸������
	*	@param[in]	userId		��¼id
	*	@param[out] struDecAbility	�豸�������ṹ��
	*	@return  ���ز������
	*	 - false ����ʧ��
	*	 - true	�����ɹ�
	*/
	static DLL_HIKUTIL_API bool WINAPI  GetDeviceAbility(int userId, NET_DVR_MATRIX_ABILITY_V41 &struDecAbility);

	/** @brief �����豸��ʱ����
	*	@param[in]	userId		��¼id
	*	@param[in]	byDate		���ڼ���1-7��������һ��������
	*	@param[in]	byHour		ʱ
	*	@param[in]	byMinute		��	
	*	@return  ���ز������
	*	 - false ����ʧ��
	*	 - true	�����ɹ�
	*/
	static DLL_HIKUTIL_API bool WINAPI  SetAutoReboot(int userId, int byDate, int byHour, int byMinute);
};


#endif