# OpenNiconico Comment Renderer に関して

## このプロジェクトについて
C++ 向けのオープンソースな niconico コメント描画再現プロジェクトです。なお、次のプラットフォームにおけるドライバーが付属しています。

- Windows Desktop (DLL、Windows 8.1/8/7 用、XP/Vista は非サポート[1])
- Windows (コンポーネント、Windows Runtime 1.3 = Windows 8.1/Phone 8.1 向け Runtime 用[2])

##### 注釈
1. Visual Studio 2013 の Visual C++ 再頒布可能パッケージが Windows 7 Service Pack 1 以上のサポート
2. Phone では ＭＳＰゴシック、Gulim、PMingLiU が存在しないので、游ゴシック (デフォルト) および SimSun で代用している。


## 方針
ニコニコ生放送におけるコメントを再現する。

現状、ニコニコ生放送版のアルゴリズムで書かれており、ニコニコ動画版はない。ニコニコ生放送版は中画面 (672x384) を基準に作成されているが、内部処理ではこの場合における px を dip として扱う (1 px = 1 dip)。描画装置側の dpi をいじることで、コメント描画部分のリサイズを可能とする。また、Windows 向けドライバーが同梱されており、描画レイヤーに Direct2D/Write を用いている。


## 未定なこと
基本的に以下のことが未決定。

- ニコニコ動画に対応するか


## ライセンス
二条項BSDライセンスに従う。詳しくは以下のライセンスをご覧ください。

⇒ [OpenNiconico Comment Renderer のライセンス](./license.md)


## 開発する人のための詳細

### コーディングスタイルに関して
- BOM 付き UTF-8
- 最新の　C++ (VC++ でコンパイルが通ること)。
- [mntoneのコーディングスタイル](//gist.github.com/mntone/7513279)

など。

### pull request や issue に関しての注意事項
1. 日本語で記述。
2. コーディングスタイルを合わせる。

### 開発環境
- Visual Studio 2013 + [Visual C++ Compiler November 2013 CTP](http://www.microsoft.com/en-us/download/details.aspx?id=41151)

※ Win32 のデモ アプリ版のみ必要

### コメント分析器に関して

Adobe Flash Player の挙動を再現すべく、内部に [コメント分析器](https://github.com/mntone/OpenNiconicoCommentRenderer/blob/master/nico.renderer/src/comment_analyzer.cpp) が存在する。これに関してまだ完全に規則に載っていない部分が存在する。

なお、このコメント分析器は、最新の OS による最新の Flash に従う。故に現段階では、Windows 8.1 with August Update 上の Flash を基準に実装を行う。

## 関連するプロジェクト
- niconico の API 群を実装するプロジェクト - OpenNiconico2 (C#)
	[https://github.com/mntone/OpenNiconico2](https://github.com/mntone/OpenNiconico2)

##### 2014 年 11 月 19 日 最終更新