#pragma once
#include <string>
#include <atlstr.h>
#define D_DLLNAME "7-zip64.dll"
#define D_APINAME "SevenZip"
/*
 【ライセンス】
　7-zip64.dllは GNU Lesser General Public License (LGPL)の元で配布されて
　います。
　ソフトの改変、再配布等については LGPL に従ってください。
　http://www.gnu.org/copyleft/lesser.ja.html

  7-zip64.dllの入手先：
  ver 9.22.00.02 私家版 【作者】秋田 稔氏
　http://ayakawa.o.oo7.jp/bin/7z922002_64.zip

 【サポート】
  Web サイト：
　http://akky.xrea.jp/
  メール：
  Email:support@akky.xrea.jp<秋田 稔>
*/
// API関数型宣言 API関数名：SevenZip
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
		BOOL Compress(
			LPCSTR lpSrcPath,
			LPCSTR lpDstFileName
		);
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
		BOOL Extract(
			LPCSTR lpSrcFileName,
			LPCSTR lpDstPath
		);
	private:
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
		BOOL CompressFile(
			LPCSTR lpSrcPath,
			LPCSTR lpDstFileName
		);
		/////////////////////////////////////////////////////////////////////
		// ロードライブラリ
		// ライブラリのハンドルを取得する
		BOOL m_LoadDLL(void);
		/////////////////////////////////////////////////////////////////////
		// DLLライブラリのハンドル
		HMODULE hModule;
		/////////////////////////////////////////////////////////////////////
		// DLL API関数ポインタ
		T_LPZIP lpSevenZip;
		/////////////////////////////////////////////////////////////////////
		// API Name
		std::string m_ApiName;
		/////////////////////////////////////////////////////////////////////
		// DLL Name
		std::string m_DLLName;
		/////////////////////////////////////////////////////////////////////
		// API関数取得
		// 引数リスト：
		//　const char* ApiName：API関数名
		// 処理：
		//　hModule（DLLライブラリ）を元に該当する関数ポインタを取得
		// 戻り値：
		//　該当するAPI関数ポインタを返す。（!=NULL）
		//　該当関数がなかった時はNULLを返す。
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
				msg.Format("API関数取得エラー：%s", ApiName);
				::MessageBox(
					nullptr,
					msg.GetString(),
					"DLL関数",
					MB_ICONERROR
				);
			}
			return retB;
		}
	};
}