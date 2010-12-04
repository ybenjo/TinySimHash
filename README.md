# TinySimHash  

## 目的

simhashの練習、及び関連検索用

## 環境

gcc 4.2.1、tr1/unordered\_mapに依存  
テストはgoogletestを使用

## データ

LibSVMと同じ形式。1行1データ。特徴量ごとにスペースで区切り、特徴量はカンマ区切り。
	data_id \s feature_id:value \s feature_id:value \s ...

## クエリ

1行のみ。特徴量ごとにスペースで区切り、特徴量はカンマ区切り。
	feature_id:value \s feature_id:value \s ...

	
## リファレンス
Ravichandran D, Pantel P, Hovy E.
Randomized algorithms and nlp: using locality sensitive hash function for high speed noun clustering.
In: Proceedings of the 43rd Annual Meeting on Association for Computational Linguistics. Association for Computational Linguistics;
2005:622-629.

