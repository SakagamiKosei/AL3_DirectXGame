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
	//// X,Y,Z 方向のスケーリングを設定
	// worldTransform_.scale_ = {5.0f, 1.0f, 1.0f};
	//// X,Y,Z 軸周りの回転角を設定
	// worldTransform_.rotation_ = {0.0f,XM_PI/4.0f,0.0f};
	
	for (int i = 0; i < 20; i++) 
	{
		////  X,Y,Z 軸回りの回転角を設定
		//worldTransform_[i].rotation_ = {0.0f, XMConvertToRadians(45.0f), 0.0f};
		//// X,Y,Z 軸回りの平行移動を設定
		// worldTransform_.translation_ = {0.0f, 10.0f, 0.0f};
		//  X,Y,Z 方向のスケーリングを設定
		worldTransform_[i].scale_ = {5.0f, 5.0f, 5.0f};
		//// X,Y,Z　軸回りの回転角を設定
		//worldTransform_[i].rotation_ = {XM_PI / 4.0f, XM_PI / 4.0f, 0.0f};
		//// X,Y,Z 軸回りの平行移動を設定
		//worldTransform_[i].translation_ = {10.0f, 10.0f, 10.0f};
		// ワールドトランスフォームの初期化
		worldTransform_[i].Initialize();
		// X,Y,Z 軸回りの平行移動を設定
		
		// 下
		if (9 < i) {
			// X,Y,Z　軸回りの平行移動を設定
			worldTransform_[i].translation_ = {-40, -20, 10.0f};
			// X座標をすらす
			worldTransform_[i].translation_.x += (i - 10) * 10;
		} 
		// 上
		else{
			// X,Y,Z 軸回りの平行移動
			worldTransform_[i].translation_ = {-50, 20, 10.0f};
			// X座標をすらす
			worldTransform_[i].translation_.x += i * 10;
		}
	}
	
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	// スプライトの今の座標を取得
	XMFLOAT2 position = sprite_->GetPosition();
	// 座標を{2,0}移動
	position.x += 2.0f;
	position.y += 1.0f;
	// 移動した座標をスプライトに反映
	sprite_->SetPosition(position);

	for (int i = 0; i < 20; i++) {
		worldTransform_[i].UpdateMatrix();
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
	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		// 3Dモデル描画
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
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
	/*sprite_->Draw();*/
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
