#include <Windows.h>
#include "CZipMethod.h"
#include <filesystem>
namespace fs = std::filesystem;
namespace ZIP
{
	ArchiveMethod::ArchiveMethod() :
		m_ApiName(D_APINAME),
		m_DLLName(D_DLLNAME),
		hModule{},
		lpSevenZip{}
	{
		// DLL ���[�h
		this->m_LoadDLL();
		// API �֐��擾
		this->lpSevenZip = this->Tmpl_GetFunction<T_LPZIP>(this->m_ApiName.c_str());
	}
	ArchiveMethod::~ArchiveMethod()
	{
		if (this->lpSevenZip != nullptr)
		{
			::FreeLibrary(this->hModule);
		}
	}
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
	BOOL ArchiveMethod::CompressFile(
		LPCSTR lpSrcFileName,
		LPCSTR lpDstFileName
	)
	{
		BOOL retB = TRUE;
		// API�̃R�}���h���C������
		CString CmdLine;
		CmdLine.Format(
			"a -tzip \"%s\" \"%s\" -hide -aoa",
			lpDstFileName,
			lpSrcFileName
		);
		// �Ώۃt�@�C�������k����
		auto retv = this->lpSevenZip(
			nullptr,
			CmdLine.GetString(),
			nullptr,
			NULL
		);
		if (retv != 0)
		{
			retB = FALSE;
		}
		return retB;
	}
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
	BOOL ArchiveMethod::Compress(
		LPCSTR lpSrcPath,
		LPCSTR lpDstFileName
	)
	{
		BOOL retB = FALSE;
		if (this->lpSevenZip == nullptr)
		{
			return retB;
		}
		// ���݃`�F�b�N
		if (fs::exists(lpSrcPath) == false)
		{
			return retB;
		}
		fs::path fs_SrcPath;
		// �f�B���N�g���łȂ���΂���܂�
		if (fs::is_directory(lpSrcPath) == false)
		{
			fs_SrcPath = lpSrcPath;
		}
		// �f�B���N�g���̏ꍇ�͖�����\*��t��
		else
		{
			fs_SrcPath = fs::path(lpSrcPath).string() + "\\*";
		}
		// ��΃p�X�ϊ�
		fs_SrcPath = fs::absolute(fs_SrcPath);
		fs::path fs_DstPath = fs::absolute(lpDstFileName);
		// API�̃R�}���h���C������
		// �Ώۃt�@�C�������k����
		retB = this->CompressFile(
			fs_SrcPath.string().c_str(),
			fs_DstPath.string().c_str()
		);
		return retB;
	}
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
	BOOL ArchiveMethod::Extract(
		LPCSTR lpSrcFileName,
		LPCSTR lpDstPath
	)
	{
		BOOL retB = FALSE;
		if (this->lpSevenZip == nullptr)
		{
			return retB;
		}
		// ���݃`�F�b�N
		if (fs::exists(lpSrcFileName) == false ||
			fs::exists(lpDstPath) == false)
		{
			return retB;
		}
		// ��΃p�X�ϊ�
		fs::path fs_SrcFile = fs::absolute(lpSrcFileName);
		fs::path fs_DstPath = fs::absolute(lpDstPath);
		// API�̃R�}���h���C������
		CString CmdLine;
		CmdLine.Format(
			"x \"%s\" -o\"%s\" -hide -aoa",
			fs_SrcFile.string().c_str(),
			fs_DstPath.string().c_str()
		);
		// �Ώۃt�@�C�������k����
		auto retv = this->lpSevenZip(
			nullptr,
			CmdLine.GetString(),
			nullptr,
			NULL
		);
		if (retv == 0)
		{
			retB = TRUE;
		}
		return retB;
	}
	// ���[�h���C�u����
	BOOL ArchiveMethod::m_LoadDLL()		// ���C�u�����̃n���h�����擾����
	{
		BOOL retB = TRUE;
		HMODULE hModLib = ::LoadLibrary(this->m_DLLName.c_str());
		if (hModLib != nullptr)
		{
			this->hModule = hModLib;
		}
		else
		{
			retB = FALSE;
			CString msg;
			msg.Format("DLL���[�h�G���[�F%s", this->m_DLLName.c_str());
			::MessageBox(
				nullptr,
				msg.GetString(),
				"DLL���C�u����",
				MB_ICONERROR
			);
		}
		return retB;
	}
}