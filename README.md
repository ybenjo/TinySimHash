# TinySimHash  

## 目的

simhashの練習、及び関連検索用

## 環境

gcc 4.2.1、tr1/unordered\_mapに依存  
テストはgoogletestを使用

## データ

LibSVMと同じ形式。1行1データ。特徴量ごとにスペースで区切り、特徴量はカンマ区切り。
	data_id \s feature_id:value \s feature_id:value \s ...

	
## リファレンス
Manku GS, Jain A, Das Sarma A. Detecting near-duplicates for web crawling. Proceedings of the 16th international conference on World Wide Web - WWW'07. 2007:141.
