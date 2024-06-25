//------------------------------------------
// バイナリファイルを扱うクラス
//------------------------------------------
#pragma once

#ifndef BINARYFILE_DEFINED
#define BINARYFILE_DEFINED

#include <memory>
class BinaryFile
	{
	protected:

		// データ
		std::unique_ptr<char[]> m_data;

		// サイズ
		unsigned int m_size;

	public:

		// ファイル名を指定してロード
		static BinaryFile LoadFile(const wchar_t* fileName);

		BinaryFile();

		// ムーブコンストラクタ
		BinaryFile(BinaryFile&& in);

		// アクセサ
		char* GetData() { return m_data.get(); }
		unsigned int GetSize() { return m_size; }
};

#endif // BINARYFILE_DEFINED