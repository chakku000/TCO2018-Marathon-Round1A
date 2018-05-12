# TCO2018-Marathon-Round1A

## 概要

- 地図上に複数の街が存在
- 接続点と道を作成可能
- コストは"建設された道の全長" + "接続点の作成にかかるコスト" * "接続点の数"
- 接続点と街を全て連結にせよ
    - コストが小さいほうがスコアが高い

ただし、接続点の作成は確率で失敗する

## 入力
メソッド

- buildJunction(int S,vector<int> cities,double junctionCost, double failureProbability)
    - プログラム開始時に1度だけ呼ばれる
    - 引数
        - S :
            - 地図のサイズ
            - 街と接続点は[0,S]に存在
        - cities:
            - 街の場所
            - 街iの座標は(cities[2*i],cities[2*i+1])
            - 重複なし
        - junctionCost:
            - 接続点の作成にかかるコスト
            - 接続点の作成に失敗してもかかる
        - failureProbability:
            - 接続点の作成の失敗確率
    - 返り値
        - 作成する接続点のリスト
        - NJ個の接続点を作成するなら2*NJ要素
        - 接続点iの座標は(ret[2*i],ret[2*i+1]) 
        - 全ての接続点は異なる
        - ? 接続点の場所は整数値のみ?

- buildRoad(vector<int> junctionStatus)
    - buildJunctionが呼ばれた後に1度だけよばれる
    - junctionStatus:
        - junctionStatus[i] : buildJunctionで作成するとした接続点の作成の成否
            - 1 なら成功
            - 0 なら失敗
    - 返り値
        - 作成したい道のリスト
        - 道iはret[2*i]とret[2*i+1]を結ぶ
        - 0<=ret[i]<=(NC-1)ならばret[i]は街
        - NC<=ret[i]<=(NC+NJ-1)ならばret[i]-NCが接続点のインデックス
        - 接続点のインデックスにはbuildJunctionで計算されたもので、作成失敗しても除外されない(使用は不可)


## スコア
各テストケースに対しするスコアに対して(best/your)^2の加算に1000000/テストケース数を掛けた値?

## 制約

- 100 <= S <= 1000
- 10 <= NC <= 100
- 0 <= junctionCost <= 10.0
- 0 <= failureProbability <= 40%(0.4)

## Visualizerの使い方

```
java -jar tester.jar -exec "./a.out" -seed <seed>
```
ここで、seedは乱数の種。
ただし、多分seed=1,2に関してはテストケースと一緒になってるはず
