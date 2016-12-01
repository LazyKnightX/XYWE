#ifndef __UICOMBOBOX_H__
#define __UICOMBOBOX_H__

#pragma once

#ifdef DUI_USE_COMBOBOX

namespace DuiLib
{
	/// ��չ�����б��
	/// ����arrowimage����,һ��ͼƬƽ���ֳ�5��,Normal/Hot/Pushed/Focused/Disabled(������source����)
	/// <Default name="ComboBox" value="arrowimage=&quot;file='sys_combo_btn.png' source='0,0,16,16'&quot; "/>
	class UILIB_API CComboBoxUI : public CComboUI
	{
	public:
		CComboBoxUI();
		const wchar_t* GetClass() const;

		void SetAttribute(const wchar_t* pstrName, const wchar_t* pstrValue);

		void PaintText(HDC hDC);
		void PaintStatusImage(HDC hDC);

	protected:
#pragma warning(push)
#pragma warning(disable:4251)
		std::unique_ptr<CImage> m_sArrowImage;
#pragma warning(pop)
		int        m_nArrowWidth;
	};
}
#endif

#endif // __UICOMBOBOX_H__
