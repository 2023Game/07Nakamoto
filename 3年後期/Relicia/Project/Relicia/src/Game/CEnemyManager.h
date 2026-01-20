#pragma once
#include "CTask.h"

class CEnemy;

class CEnemyManager : public CTask
{
public:
    // インスタンスのポインタの取得
    static CEnemyManager* Instance();

    // コンストラクタ
    CEnemyManager();
    // デストラクタ
    ~CEnemyManager();
    // 敵をリストに追加
    void AddEnemy(CEnemy* enemy);
    // 敵をリストから削除
    void RemoveEnemy(CEnemy* enemy);

    // 全ての敵をリストから削除
    void AllClear();

private:
    // 敵の管理クラスのインスタンス
    static CEnemyManager* spInstance;
    // 敵のリスト
    std::vector<CEnemy*> mEnemys;
};