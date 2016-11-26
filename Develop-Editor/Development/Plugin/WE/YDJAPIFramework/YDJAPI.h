#ifndef YDJAPI_H_INCLUDED
#define YDJAPI_H_INCLUDED

#if defined(_MSC_VER) && _MSC_VER > 1000
#pragma once
#endif

#if !defined(_MSC_VER) || _MSC_VER >= 1600
#include <stdint.h>
#else
typedef int int32_t;
#endif
#include <windows.h>

/** \brief ��ʼ��JAPIģ��
 *
 * ��ʼ��JAPIģ�顣�����game.dll����JAPI��ص�һЩ����������hook��
 *
 * \param gameDllModule - game.dll��HMODULE������ַ��
 * \return �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
BOOL InitJAPIFramework(HMODULE gameDllModule);

/** \brief ע�� ע��JAPI��Callback
 *
 * �ȽϹ���ĺ����������Ǳ������������������������JAPIϵͳ�ṩһ���ص���������ħ�޳�ʼ��common.j��ʱ�������
 * �����������á���Ӧ�����������ص�������ʹ��<code>RegisterJAPINative</code>ע��JAPI����
 *
 * \param pfCallback - �ص�����
 * \see RegisterJAPINative
 */
void RegisterJAPICallback(void (*pfCallback)());

/** \brief ע��JAPI����
 *
 * �����������ע��JAPI������
 *
 * \param pfNativeFunction - ������ַ
 * \param nativeName - ��������Jass����������Ƶ��ú���
 * \param nativeSignature - ����ǩ��
 * \see RegisterJAPICallback
 */
void RegisterJAPINative(void *pfNativeFunction, const char *nativeName, const char *nativeSignature);

/** \brief ���ַ��������н��C�ַ���
 *
 * Jass���ַ�������JAPI�����л���һ��ָ�룬ָ��ĳ�ַ������еĶ�������������ַ��������а�C����ַ����������ʹ�á�
 *
 * \param stringObject - �ַ�������ʵ������ָ��
 * \return �����C�ַ���
 *
 * \note �ַ�����UTF-8�����
 */
const char *GetStringFromNativeParameter(int32_t stringObject);

/** \brief ��C�ַ���ӳ��Ϊħ���ڲ��Ķ���
 *
 * �������������һ���������෴����C�ַ���ӳ��Ϊħ���ڲ�������������������Jass����string
 *
 * \param string - ��Ҫ���غ�ӳ���C�ַ���
 * \return ӳ����ڲ�����ָ��
 *
 * \note �ַ�����UTF-8�����
 */
int32_t MapStringToInternalObject(const char *string);

/// ���ټ���
#define YDJAPI_LOG_TRACE 0
/// ���Լ���
#define YDJAPI_LOG_DEBUG 1
/// ��Ϣ����
#define YDJAPI_LOG_INFO 2
/// ���漶�𣨳�����С�����ǲ�Ӱ�칦�ܣ�
#define YDJAPI_LOG_WARNING 3
/// ���󼶱𣨳�����Ӱ�칦�ܵĴ���
#define YDJAPI_LOG_ERROR 4
/// ���ؼ��𣨼����������󣬵���ϵͳ�޷����У�
#define YDJAPI_LOG_FATAL 5

/** \brief ��־����
 *
 * �����־����־������󳤶�8191�ֽ�
 *
 * \param level - ��־�ȼ�
 * \param format - ��ʽ����
 * \return ����ֽ���
 */
int JAPILog(int level, const char *format, ...);

/** \brief ��װinline����
 *
 * ��װinline���ӡ�����������Զ�����trampoline���޸�ԭ����ָ��
 *
 * \param [inout] ppOriginalFunction - ��ָ���ָ�룩����֮ǰ��ʾ��hook�ĺ���������֮����޸�Ϊtrampoline��ַ
 * \param pSubstitutionFunction - �������
 *
 * \return TRUE ��ʾ�ɹ���FALSE��ʾʧ��
 */
BOOL InstallInlineHook(void **ppOriginalFunction, void *pSubstitutionFunction);

/** \brief ж��inline����
 *
 * ж��inline���ӡ�
 *
 * \param [inout] ppOriginalFunction - ��ָ���ָ�룩����֮ǰ��ʾtrampoline��ַ������֮����޸�Ϊ��hook�ĺ���
 * \param pSubstitutionFunction - �������
 *
 * \return TRUE ��ʾ�ɹ���FALSE��ʾʧ��
 */
BOOL UninstallInlineHook(void **ppOriginalFunction, void *pSubstitutionFunction);

#endif // YDJAPI_H_INCLUDED
