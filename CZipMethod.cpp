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
		// DLL ロード
		this->m_LoadDLL();
		// API 関数取得
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
	// ファイルの圧縮
	// 引数リスト：
	// 　LPCSTR lpSrcPath：圧縮対象パス（圧縮対象ファイルまたはフォルダ）
	//　　ワイルドカードは使用不可
	// 　LPCSTR lpDstFileName：圧縮ファイルパス
	// 　※）パスは相対パスも可
	// 処理：
	//　圧縮対象パスを圧縮して圧縮ファイルを作成する。
	// 　出力ファイルは常に上書きされる。
	// 　ZIP DLLのAPIを呼び出す
	// 戻り値：
	//　BOOL TRUE=圧縮が成功、FALSE=圧縮が失敗
	/////////////////////////////////////////////////////////////////////
	BOOL ArchiveMethod::CompressFile(
		LPCSTR lpSrcFileName,
		LPCSTR lpDstFileName
	)
	{
		BOOL retB = TRUE;
		// APIのコマンドライン生成
		CString CmdLine;
		CmdLine.Format(
			"a -tzip \"%s\" \"%s\" -hide -aoa",
			lpDstFileName,
			lpSrcFileName
		);
		// 対象ファイルを圧縮する
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
	// ファイル・フォルダの圧縮
	// 引数リスト：
	// 　LPCSTR lpSrcPath：圧縮対象パス（圧縮対象ファイルまたはフォルダ）
	//　　ワイルドカードは使用不可
	// 　LPCSTR lpDstFileName：圧縮ファイルパス
	// 　※）パスは相対パスも可
	// 処理：
	//　圧縮対象パスと圧縮ファイルの存在チェックをして
	//　CompressFile()を呼び出す
	// 戻り値：
	//　BOOL CompressFile()の戻り値を返す
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
		// 存在チェック
		if (fs::exists(lpSrcPath) == false)
		{
			return retB;
		}
		fs::path fs_SrcPath;
		// ディレクトリでなければそんまま
		if (fs::is_directory(lpSrcPath) == false)
		{
			fs_SrcPath = lpSrcPath;
		}
		// ディレクトリの場合は末尾に\*を付加
		else
		{
			fs_SrcPath = fs::path(lpSrcPath).string() + "\\*";
		}
		// 絶対パス変換
		fs_SrcPath = fs::absolute(fs_SrcPath);
		fs::path fs_DstPath = fs::absolute(lpDstFileName);
		// APIのコマンドライン生成
		// 対象ファイルを圧縮する
		retB = this->CompressFile(
			fs_SrcPath.string().c_str(),
			fs_DstPath.string().c_str()
		);
		return retB;
	}
	/////////////////////////////////////////////////////////////////////
	// ファイルの解凍
	// 引数リスト：
	// 　LPCSTR lpSrcFileName：解凍対象パス（解凍対象ファイル）
	//　　ワイルドカードは使用不可
	// 　LPCSTR lpDstPath：解凍フォルダ
	// 　※）パスは相対パスも可
	// 処理：
	//　解凍対象パス(*.zip)を解凍して、解凍フォルダ配下に展開する
	// 戻り値：
	//　BOOL TRUE=解凍が成功、FALSE=解凍が失敗
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
		// 存在チェック
		if (fs::exists(lpSrcFileName) == false ||
			fs::exists(lpDstPath) == false)
		{
			return retB;
		}
		// 絶対パス変換
		fs::path fs_SrcFile = fs::absolute(lpSrcFileName);
		fs::path fs_DstPath = fs::absolute(lpDstPath);
		// APIのコマンドライン生成
		CString CmdLine;
		CmdLine.Format(
			"x \"%s\" -o\"%s\" -hide -aoa",
			fs_SrcFile.string().c_str(),
			fs_DstPath.string().c_str()
		);
		// 対象ファイルを圧縮する
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
	// ロードライブラリ
	BOOL ArchiveMethod::m_LoadDLL()		// ライブラリのハンドルを取得する
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
			msg.Format("DLLロードエラー：%s", this->m_DLLName.c_str());
			::MessageBox(
				nullptr,
				msg.GetString(),
				"DLLライブラリ",
				MB_ICONERROR
			);
		}
		return retB;
	}
}