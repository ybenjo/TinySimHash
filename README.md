# TinySimHash  

## 目的

simhashの練習、及び関連検索用

## 内容
simhash。データを32bitのハッシュ値で表現して近傍探索します。

## 環境

gcc 4.2.1、tr1/unordered\_mapに依存  
テストはgoogletestを使用
また、データ収納についてはTokyo Tyrant 1.1.41を使用

## 注意
ソースからTokyo Tyrantを導入した場合、Makefileの書き換えが必要

## 特徴ファイル

LibSVMと同じ形式。1行1データ。特徴量ごとにスペースで区切り、特徴量はカンマ区切り。
	data_id \s feature_id:value \s feature_id:value \s ...

## クエリ

1行のみ。特徴量ごとにスペースで区切り、特徴量はカンマ区切り。
	feature_id:value \s feature_id:value \s ...

## パラメータ

* -f, --feature:入力する特徴ファイル  
* -h, --hash:既に生成したハッシュテーブルファイル   
* -q, --query:クエリファイル
* --make:ハッシュテーブルファイル生成モード  
* -b:検索時において前後b個のデータと比較。デフォルトは10。  
* -l, --limit:検索結果出力時の件数。デフォルトは10。
* -i, --iteration:検索時にハッシュのビットシャッフル & ソートを繰り返す回数。デフォルトでは繰り返しなし(=1)。  
* --fserver [string]:特徴をtokyotyrantへ格納。検索時に指定することにより、近傍の特徴のみをここから取得するので速度が著しく向上する。
* --hserver [string]:生成したハッシュテーブルをtokyotyrantへ格納する。検索時に指定することにより、ファイルからではなくこのデータベースから全件読み込む。
* --fast:ハッシュテーブルを32bitのうち、32/2 + 16/2の24bitごとに保持して保存及びマッチングを行う。

## 使い方

まずmake
	make simhash
ハッシュテーブルファイルを生成。ハッシュテーブルファイルは/path/to/feature.hashに生成される。
	./simhash --feature /path/to/feature.txt --make
次に検索
	./simhash --feature /path/to/feature.txt --hash /path/to/feature.hash --query /path/to/query.txt -b [num] -t [num] -l [num]
検索結果は/path/to/query.txt.outに出力される。形式は以下の通りで降順に出力される。
	data_id,cosine_similarity
	data_id,cosine_similarity
	data_id,cosine_similarity

tokyotyrantを用いる場合次のようにする。まずTokyoTyrantのサーバを起動させる。  
その際、サンプルのttservctlを2つコピーし、内部のpidfile、dbnameを変更しておく。  
また、portをそれぞれユニークなものに書き換える。ここでは例として1つをarticle\_bowとしportを1001、もう一つをarticle\_hashとしportを1002とする。  
	sudo /path/to/script/article_bow start
	sudo /path/to/script/article_hash start
次にハッシュテーブルを格納する。この時--fserverを指定するとファイル書き出しを行わなくなる。  
	./simhash --feature /path/to/feature.txt --fserver localhost:1001 --hserver localhost:1002 --make
その後検索を行う。この時に--fserver及び--hserverを指定する.
	./simhash --fserver localhost:1001 --hserver localhost:1002 --query /path/to/query.txt -b [num] -t [num] -l [num]
--fastを有効にする場合、--fastを付けずに作成したハッシュテーブルデータベースを一度削除し再構築する必要がある。
	./simhash --fserver localhost:1001 --hserver localhost:1002 --make --fast
検索の際、-b, -tの引数は無視される。
	./simhash --fserver localhost:1001 --hserver localhost:1002 --query /path/to/query.txt -l [num] --fast
	

# ToDo

* 元論文に書かれているハッシュの圧縮
* テストの充実
* 引数チェック
* ハッシュテーブルファイルを生成せずに検索のみを行うモード

# リファレンス
Manku GS, Jain A, Das Sarma A.
Detecting near-duplicates for web crawling.
Proceedings of the 16th international conference on World Wide Web - WWW '07. 2007:141.  
次元が高い場合に関してのsimhashの計算 - tsubosakaの日記 - http://d.hatena.ne.jp/tsubosaka/20090928/1254147181
