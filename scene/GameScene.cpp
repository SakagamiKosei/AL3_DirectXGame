#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	// 3Dモデルの生成
	model_ = Model::Create();
	
	// ワールドトランスフォームの縦列の数だけ繰り返す
	for (size_t i = 0; i < _countof(worldTransform_);i++) 
	{
		// ワールドトランスフォームの横列の数だけ繰り返す
		for (size_t j = 0; j < _countof(worldTransform_);j++) 
		{
			// ワールドトランスフォームの大きさを設定
			worldTransform_[i][j].scale_ = {1.0f, 1.0f, 1.0f};
			// ワールドトランスフォームの初期化
			worldTransform_[i][j].Initialize();

			// X,Y,Z　軸回りの平行移動を設定
			worldTransform_[i][j].translation_ = {-15.0f,16.0f,0.0f};
			// ワールドトランスフォームのY座標をずらす
			worldTransform_[i][j].translation_.y -= i * 4.0f;
			// ワールドトランスフォームのX座標をずらす
			worldTransform_[i][j].translation_.x += j * 4.0f;
		}
	
	}
	
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	// ワールドトランスフォームの縦列の数だけ繰り返す
	for (size_t i = 0; i < _countof(worldTransform_); i++) 
	{
		// ワールドトランスフォームの横列の数だけ繰り返す
		for (size_t j = 0; j < _countof(worldTransform_); j++)
		{
			worldTransform_[i][j].UpdateMatrix();
		}
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// ワールドトランスフォームの縦列の数だけ繰り返す
	for (size_t i = 0; i < _countof(worldTransform_); i++) 
	{
		// ワールドトランスフォームの横列の数だけ繰り返す
		for (size_t j = 0; j < _countof(worldTransform_); j++) 
		{
		
			// -------------------------------------------
			// 描画しない条件
			// -------------------------------------------
			// もし縦列が奇数なら
			if ((i & 1) == 1)
			{
				// もし横列が奇数なら
				if ((j & 1) == 1) 
				{
				
				}
				// ------------------------------------------
				// 描画する条件
				// ------------------------------------------
				// もし縦列が奇数で横列が偶数なら
				else
				{
					// 3Dモデル描画
					model_->Draw(worldTransform_[i][j], viewProjection_, textureHandle_);
				}
			} 
			// ------------------------------------------
			// 描画する条件
			// ------------------------------------------
			// もし縦列が偶数なら
			else 
			{
				// 3Dモデル描画
				model_->Draw(worldTransform_[i][j], viewProjection_, textureHandle_);
			}
				
		}	
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
