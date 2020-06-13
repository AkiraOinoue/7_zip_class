#pragma once
#include <string>
#include <atlstr.h>
#define D_DLLNAME "7-zip64.dll"
#define D_APINAME "SevenZip"
/*
 �y���C�Z���X�z
�@7-zip64.dll�� GNU Lesser General Public License (LGPL)�̌��Ŕz�z�����
�@���܂��B
�@�\�t�g�̉��ρA�Ĕz�z���ɂ��Ă� LGPL �ɏ]���Ă��������B
�@http://www.gnu.org/copyleft/lesser.ja.html

  7-zip64.dll�̓����F
  ver 9.22.00.02 ���Ɣ� �y��ҁz�H�c ����
�@http://ayakawa.o.oo7.jp/bin/7z922002_64.zip

 �y�T�|�[�g�z
  Web �T�C�g�F
�@http://akky.xrea.jp/
  ���[���F
  Email:support@akky.xrea.jp<�H�c ��>
*/
// API�֐��^�錾 API�֐����FSevenZip
typedef int(WINAPI* T_LPZIP)(
	const HWND _hwnd,
	LPCSTR _szCmdLine,
	LPSTR _szOutput,
	const DWORD _dwSize
);
namespace ZIP
{
	class ArchiveMethod
	{
	public:
		ArchiveMethod(void);
		~ArchiveMethod(void);
	public:
		/////////////////////////////////////////////////////////////////////
		// �t�@�C���E�t�H���_�̈��k
		// �������X�g�F
		// �@LPCSTR lpSrcPath�F���k�Ώۃp�X�i���k�Ώۃt�@�C���܂��̓t�H���_�j
		//�@�@���C���h�J�[�h�͎g�p�s��
		// �@LPCSTR lpDstFileName�F���k�t�@�C���p�X
		// �@���j�p�X�͑��΃p�X����
		// �����F
		//�@���k�Ώۃp�X�ƈ��k�t�@�C���̑��݃`�F�b�N������
		//�@CompressFile()���Ăяo��
		// �߂�l�F
		//�@BOOL CompressFile()�̖߂�l��Ԃ�
		/////////////////////////////////////////////////////////////////////
		BOOL Compress(
			LPCSTR lpSrcPath,
			LPCSTR lpDstFileName
		);
		/////////////////////////////////////////////////////////////////////
		// �t�@�C���̉�
		// �������X�g�F
		// �@LPCSTR lpSrcFileName�F�𓀑Ώۃp�X�i�𓀑Ώۃt�@�C���j
		//�@�@���C���h�J�[�h�͎g�p�s��
		// �@LPCSTR lpDstPath�F�𓀃t�H���_
		// �@���j�p�X�͑��΃p�X����
		// �����F
		//�@�𓀑Ώۃp�X(*.zip)���𓀂��āA�𓀃t�H���_�z���ɓW�J����
		// �߂�l�F
		//�@BOOL TRUE=�𓀂������AFALSE=�𓀂����s
		/////////////////////////////////////////////////////////////////////
		BOOL Extract(
			LPCSTR lpSrcFileName,
			LPCSTR lpDstPath
		);
	private:
		/////////////////////////////////////////////////////////////////////
		// �t�@�C���̈��k
		// �������X�g�F
		// �@LPCSTR lpSrcPath�F���k�Ώۃp�X�i���k�Ώۃt�@�C���܂��̓t�H���_�j
		//�@�@���C���h�J�[�h�͎g�p�s��
		// �@LPCSTR lpDstFileName�F���k�t�@�C���p�X
		// �@���j�p�X�͑��΃p�X����
		// �����F
		//�@���k�Ώۃp�X�����k���Ĉ��k�t�@�C�����쐬����B
		// �@�o�̓t�@�C���͏�ɏ㏑�������B
		// �@ZIP DLL��API���Ăяo��
		// �߂�l�F
		//�@BOOL TRUE=���k�������AFALSE=���k�����s
		/////////////////////////////////////////////////////////////////////
		BOOL CompressFile(
			LPCSTR lpSrcPath,
			LPCSTR lpDstFileName
		);
		/////////////////////////////////////////////////////////////////////
		// ���[�h���C�u����
		// ���C�u�����̃n���h�����擾����
		BOOL m_LoadDLL(void);
		/////////////////////////////////////////////////////////////////////
		// DLL���C�u�����̃n���h��
		HMODULE hModule;
		/////////////////////////////////////////////////////////////////////
		// DLL API�֐��|�C���^
		T_LPZIP lpSevenZip;
		/////////////////////////////////////////////////////////////////////
		// API Name
		std::string m_ApiName;
		/////////////////////////////////////////////////////////////////////
		// DLL Name
		std::string m_DLLName;
		/////////////////////////////////////////////////////////////////////
		// API�֐��擾
		// �������X�g�F
		//�@const char* ApiName�FAPI�֐���
		// �����F
		//�@hModule�iDLL���C�u�����j�����ɊY������֐��|�C���^���擾
		// �߂�l�F
		//�@�Y������API�֐��|�C���^��Ԃ��B�i!=NULL�j
		//�@�Y���֐����Ȃ���������NULL��Ԃ��B
		/////////////////////////////////////////////////////////////////////
		template <typename _T_FuncType>
		_T_FuncType Tmpl_GetFunction(const char* ApiName)
		{
			_T_FuncType retB = nullptr;
			if (this->hModule == nullptr)
			{
				return retB;
			}
			retB = (_T_FuncType)::GetProcAddress(
				this->hModule,
				ApiName
			);
			if (retB == nullptr)
			{
				CString msg;
				msg.Format("API�֐��擾�G���[�F%s", ApiName);
				::MessageBox(
					nullptr,
					msg.GetString(),
					"DLL�֐�",
					MB_ICONERROR
				);
			}
			return retB;
		}
	};
}