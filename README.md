# MyApp by katahiromz

## これは何？

katahiromz（片山博文MZ）氏が作ったWin32アプリのテンプレートです。

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
RosBEでのビルド方法は次の通りです。

```cmd
cd (MyAppのフォルダへのパス)
cmake -G Ninja .
ninja
```

`MSYS2`を使う場合は、必要なパッケージを`MSYS2`内部の`pacman`でインストールした後で、
`cd`コマンド、`cmake -G "MinGW Makefiles" .`、そして`mingw32-make.exe` でできます。
`pacman`によるパッケージのインストール方法はここでは説明しません。検索エンジンや人工知能にでもお伺い下さい。

Visual C++を使うのでしたら、まずはC++開発ができるように開発環境を整備し、Hello worldから始めてください。
それが終わったら、スタンドアロンのCMake をインストールして、CMake GUI または CMake コマンドを使って
CMakeからビルドしてください。Visual C++でのビルドはいろんなやり方があるので、ここではあまり説明しません。
CMakeを使ったビルドは、ソリューションファイルを使う方法よりも遠回りですが、
ソリューションファイルは人間がマネージできるファイルではないので、後々が厳しいです。

## 使用許諾と著作権

- License: MIT
- Copyright (C) 2024 katahiromz <katayama.hirofumi.mz@gmail.com>

## 参照

- Win32 Programmer's Reference (ご自分で自己責任でお探しください)
- C++日本語リファレンス ― https://cpprefjp.github.io/
- UPX ― https://upx.github.io/
