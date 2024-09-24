# MyApp by katahiromz

## これは何？

katahiromzが作ったWin32アプリのテンプレートです。

## ファイル群

- `.gitignore` ― Gitが無視すべきファイル群を記載
- `AntiRE.h` ― 逆工学対策
- `CMakeLists.txt` ― ビルドツール CMake用の設定ファイル
- `LICENSE.txt` ― ライセンス（使用許諾）情報
- `MCenterWindow.h` ― ウィンドウやメッセージボックスを中央寄せする
- `mdbg.h` ― デバッグ時出力
- `mt19937ar.c` と `mt19937ar.h` ― メルセンヌツイスター乱数生成
- `MyApp.cpp` ― 私のアプリのソース
- `MyApp.h` ― 私のアプリのヘッダ
- `MyApp_res.rc` ― 私のアプリのリソースデータ
- `README.md` ― このファイル
- `resource.h` ― リソースID群
- `targetver.h` ― ターゲットOSバージョン

## ビルド方法

普通に CMake と C++11/Win32 コンパイラを使ってください。
C++11/Win32 コンパイラで迷ったら、WinXP互換の RosBE (ReactOS Build Environment) でも使ってやってください。

```cmd
cd (MyAppのフォルダへのパス)
cmake -G Ninja .
ninja
```

## 使用許諾と著作権

- License: MIT
- Copyright (C) 2024 katahiromz <katayama.hirofumi.mz@gmail.com>

## 参照

- UPX https://upx.github.io/
