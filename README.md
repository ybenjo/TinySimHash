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

* ハッシュファイルの圧縮。及びRead/Writeの高速化→boostでシリアライズ
* テストの充実
* 引数チェック
* ハッシュテーブルファイルを生成せずに検索のみを行うモード

# リファレンス
Ravichandran D, Pantel P, Hovy E.
Randomized algorithms and nlp: using locality sensitive hash function for high speed noun clustering.
In: Proceedings of the 43rd Annual Meeting on Association for Computational Linguistics. Association for Computational Linguistics;
2005:622-629.

