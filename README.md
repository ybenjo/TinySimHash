# TinySimHash  

## 目的

simhashの練習、及び関連検索用

## 環境

gcc 4.2.1、tr1/unordered\_mapに依存  
テストはgoogletestを使用

## 特徴ファイル

LibSVMと同じ形式。1行1データ。特徴量ごとにスペースで区切り、特徴量はカンマ区切り。
	data_id \s feature_id:value \s feature_id:value \s ...

## クエリ

1行のみ。特徴量ごとにスペースで区切り、特徴量はカンマ区切り。
	feature_id:value \s feature_id:value \s ...

## パラメータ

* -f, --feature:入力する特徴ファイル  
* -h, --hash:既に生成したハッシュテーブルファイル   
* -q, --query:クエリ
* --make:ハッシュテーブルファイル生成モード  
* -b:検索モードにおいて前後b個のデータと比較。デフォルトは10。  

## 使い方

まずmake
	make simhash
ハッシュテーブルファイルを生成。ハッシュテーブルファイルは/path/to/feature.hashに生成される。
	./simhash --feature /path/to/feature.txt --make
次に検索
	./simhash --feature /path/to/feature.txt --hash /path/to/feature.hash --query /path/to/query.txt -b [num]
検索結果は/path/to/query.txt.outに出力される。形式は以下の通りで降順に出力される。
	data_id,cosine_similarity
	data_id,cosine_similarity
	data_id,cosine_similarity

# ToDo

* 元論文に書かれているハッシュの圧縮。及びRead/Writeの高速化
* テストの充実
* 引数チェック
* ハッシュテーブルファイルを生成せずに検索のみを行うモード

# リファレンス
Manku GS, Jain A, Das Sarma A.
Detecting near-duplicates for web crawling.
Proceedings of the 16th international conference on World Wide Web - WWW '07. 2007:141.
次元が高い場合に関してのsimhashの計算 - tsubosakaの日記 - http://d.hatena.ne.jp/tsubosaka/20090928/1254147181
