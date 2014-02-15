# OpenNiconico Comment Renderer に関して

## このプロジェクトについて
C++ 向けのオープンソースな niconico コメント描画再現プロジェクトです。なお、次のプラットフォームにおけるドライバーが付属しています。

- Windows Desktop (DLL、Windows 8.1/8/7 用、XP/Vista は非サポート[1])
- Windows Runtime (DLL、ストア向けビルド、Windows Runtime 1.3 = Windows 8.1 向け Runtime 用[2])

##### 注釈
1. Visual Studio 2013 の Visual C++ 再頒布可能パッケージが Windows 7 Service Pack 1 以上のサポート
2. 今後追加


## 方針
ニコニコ生放送におけるコメントを再現する。

現状、ニコニコ生放送版のアルゴリズムで書かれており、ニコニコ動画版はない。ニコニコ生放送版は中画面 (672x384) を基準に作成されているが、内部処理ではこの場合における px を dip として扱う (1 px = 1 dip)。描画装置側の dpi をいじることで、コメント描画部分のリサイズを可能とする。また、Windows 向けドライバーが同梱されており、描画レイヤーに Direct2D/Write を用いている。


## 現状
現段階のバージョンではバグがたくさんあるので、まずそこの解決が必要となっている。バグ解決後、互換性を高めていく作業を行う。

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
3. 新しい API を追加する場合、それに関しての内容を記述する。

### 開発環境
- Visual Studio 2013 + [Visual C++ Compiler November 2013 CTP](http://www.microsoft.com/en-us/download/details.aspx?id=41151)


## 関連するプロジェクト
- niconico の API 群を実装するプロジェクト - OpenNiconico
	[https://github.com/mntone/OpenNiconico](https://github.com/mntone/OpenNiconico)

##### 2014 年 2 月 15 日 最終更新