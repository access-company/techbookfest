# ビルド方法

1. [docker](https://www.docker.com/)のインストールと初回起動
2. teckbookfest12ディレクトリで`./makepdf.sh`を実行

# 補足

このリポジトリは、[Re:VIEW Template](https://github.com/TechBooster/ReVIEW-Template)から派生しています。

Markdown(.md)からReview形式(.re)への変換は[pandocが推奨](https://github.com/TechBooster/ReVIEW-Template/blob/master/articles/markdown.md)されていますが、macOSで`'spawn': No such file or directory - uplatex (Errno::ENOENT)`というエラーが解決できなかったので、自前のシェルを使っています。
