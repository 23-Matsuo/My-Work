//--------------------------------------------------------------------------------------
// File: DisplayCollision.cpp
//
// �R���W�����\���N���X�i�f�o�b�O�p�j
//
// Date: 2023.3.8
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "DisplayCollision.h"

#ifdef _COLLISION_LINE_ON
#include "DebugDraw.h"
#endif

using namespace DirectX;
using namespace Imase;

// �R���X�g���N�^
DisplayCollision::DisplayCollision(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	bool modelActive,
	bool lineActive,
	uint32_t collisionMax)
	: m_modelActive(modelActive),
	m_lineActive(lineActive),
	m_collisionMax(collisionMax)
{
	// ���f���̍쐬�i���j
	m_modelSphere = GeometricPrimitive::CreateSphere(context, 2.0f, 8);

	// ���f���̍쐬�i�{�b�N�X�j
	m_modelBox = GeometricPrimitive::CreateCube(context);

	// ----- �G�t�F�N�g ----- //

	// �G�t�F�N�g�̍쐬�i���f���p�j
	m_modelEffect = std::make_unique<NormalMapEffect>(device);
	m_modelEffect->SetVertexColorEnabled(false);
	m_modelEffect->SetBiasedVertexNormals(false);
	m_modelEffect->SetInstancingEnabled(true);
	m_modelEffect->SetFogEnabled(false);
	m_modelEffect->SetTexture(nullptr);
	m_modelEffect->DisableSpecular();
	m_modelEffect->EnableDefaultLighting();
	m_modelEffect->SetWorld(SimpleMath::Matrix::Identity);

	// �G�t�F�N�g�̍쐬�i���b�V���p�j
	m_meshEffect = std::make_unique<BasicEffect>(device);
	m_meshEffect->SetVertexColorEnabled(true);
	m_meshEffect->SetTextureEnabled(false);
	m_meshEffect->SetLightingEnabled(true);
	m_meshEffect->EnableDefaultLighting();
	m_meshEffect->SetWorld(SimpleMath::Matrix::Identity);

	// �G�t�F�N�g�̍쐬�i���C���p�j
	m_lineEffect = std::make_unique<BasicEffect>(device);
	m_lineEffect->SetVertexColorEnabled(true);
	m_lineEffect->SetTextureEnabled(false);
	m_lineEffect->SetLightingEnabled(false);
	m_lineEffect->SetWorld(SimpleMath::Matrix::Identity);

	// ----- ���̓��C�A�E�g ----- //

	// ���̓��C�A�E�g�̍쐬�i���f���p�j
	const D3D11_INPUT_ELEMENT_DESC c_InputElements[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
		{ "InstMatrix",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "InstMatrix",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "InstMatrix",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect(device, m_modelEffect.get(),
			c_InputElements, std::size(c_InputElements),
			m_modelInputLayout.ReleaseAndGetAddressOf())
	);

	// ----- �萔�o�b�t�@ ----- //

	// �C���X�^���V���O�p�̃��[���h�s��i�[�p�萔�o�b�t�@���쐬
	auto desc = CD3D11_BUFFER_DESC(
		static_cast<UINT>(DISPLAY_COLLISION_MAX * sizeof(XMFLOAT3X4)),
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_USAGE_DYNAMIC,
		D3D11_CPU_ACCESS_WRITE);
	DX::ThrowIfFailed(
		device->CreateBuffer(&desc, nullptr,
			m_instancedVB.ReleaseAndGetAddressOf())
	);

	// �v���~�e�B�u�o�b�`�̍쐬�i���b�V���p�j
	m_meshBatch = std::make_unique<PrimitiveBatch<VertexPositionNormalColor>>(context);

	// ���̓��C�A�E�g�̍쐬�i���b�V���p�j
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionNormalColor>(device, m_meshEffect.get(),
			m_meshInputLayout.ReleaseAndGetAddressOf())
	);

#ifdef _COLLISION_LINE_ON
	// �v���~�e�B�u�o�b�`�̍쐬
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

	// ���̓��C�A�E�g�̍쐬
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(device, m_lineEffect.get(),
			m_lineInputLayout.ReleaseAndGetAddressOf())
	);
#endif

}

// �o�^���ꂽ�R���W�����̕`��֐�
void DisplayCollision::DrawCollision(
	ID3D11DeviceContext* context,
	CommonStates* states,
	const SimpleMath::Matrix& view,
	const SimpleMath::Matrix& proj,
	FXMVECTOR baseColor,
	FXMVECTOR lineColor,
	float alpha)
{
	// �F�{�A���t�@�l
	SimpleMath::Color color = baseColor;
	color.w = alpha;

	// �R���W�������f���̕`��
	if (m_modelActive) DrawCollisionModel(context, states, view, proj, color);

#ifdef _COLLISION_LINE_ON
	// ���C���̐F���w�肵�Ă���ꍇ
	SimpleMath::Color c = lineColor;
	if (c.w != 0.0f) color = lineColor;

	// �R���W�������C���̕`��
	if (m_lineActive) DrawCollisionLine(context, states, view, proj, color);
#else
	UNREFERENCED_PARAMETER(lineColor);
#endif

	// �o�^���ꂽ�\�������N���A����
	m_spheres.clear();
	m_boxes.clear();
	m_meshes.clear();
	m_lineSegments.clear();
}

// �R���W�������f���̕`��
void DisplayCollision::DrawCollisionModel(ID3D11DeviceContext* context, DirectX::CommonStates* states, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, FXMVECTOR color)
{
	// �o�^�����ő�\�����𒴂��Ă��Ȃ����H
	assert(static_cast<uint32_t>(m_spheres.size()) < m_collisionMax);
	assert(static_cast<uint32_t>(m_boxes.size()) < m_collisionMax);

	// ----- ���̃��f����`�� ----- //

	// �G�t�F�N�g��K������
	m_modelEffect->SetColorAndAlpha(color);
	m_modelEffect->SetView(view);
	m_modelEffect->SetProjection(proj);
	m_modelEffect->Apply(context);

	{
		// ���[���h�s���ݒ�
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		context->Map(m_instancedVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		XMFLOAT3X4* p = static_cast<XMFLOAT3X4*>(mappedResource.pData);
		ZeroMemory(p, sizeof(XMFLOAT3X4) * m_spheres.size());
		for (int i = 0; i < m_spheres.size(); i++)
		{
			// �g��k��
			p[i]._11 = p[i]._22 = p[i]._33 = m_spheres[i].radius;
			// �ʒu
			p[i]._14 = m_spheres[i].center.x;
			p[i]._24 = m_spheres[i].center.y;
			p[i]._34 = m_spheres[i].center.z;
		}
		context->Unmap(m_instancedVB.Get(), 0);
	}

	// �����C���X�^���X���`�悵�Ă���i�������̂��߁j
	m_modelSphere->DrawInstanced(m_modelEffect.get(), m_modelInputLayout.Get(), static_cast<uint32_t>(m_spheres.size()), true, false, 0, [=]()
		{
			UINT stride = sizeof(XMFLOAT3X4);
			UINT offset = 0;
			context->OMSetDepthStencilState(states->DepthRead(), 0);
			context->IASetVertexBuffers(1, 1, m_instancedVB.GetAddressOf(), &stride, &offset);
		}
	);

	// ----- �{�b�N�X�̃��f����`�� ----- //

	{
		// ���[���h�s���ݒ�
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		context->Map(m_instancedVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		XMFLOAT3X4* p = static_cast<XMFLOAT3X4*>(mappedResource.pData);
		ZeroMemory(p, sizeof(XMFLOAT3X4) * m_boxes.size());
		for (int i = 0; i < m_boxes.size(); i++)
		{
			SimpleMath::Matrix s = SimpleMath::Matrix::CreateScale(m_boxes[i].extents * 2.0f);
			SimpleMath::Matrix r = SimpleMath::Matrix::CreateFromQuaternion(m_boxes[i].rotate);
			SimpleMath::Matrix c = SimpleMath::Matrix::CreateTranslation(m_boxes[i].center);
			SimpleMath::Matrix m = s * r * c;
			p[i]._11 = m._11;  p[i]._12 = m._21; p[i]._13 = m._31; p[i]._14 = m._41;
			p[i]._21 = m._12;  p[i]._22 = m._22; p[i]._23 = m._32; p[i]._24 = m._42;
			p[i]._31 = m._13;  p[i]._32 = m._23; p[i]._33 = m._33; p[i]._34 = m._43;
		}
		context->Unmap(m_instancedVB.Get(), 0);
	}

	// �{�b�N�X���C���X�^���X���`�悵�Ă���i�������̂��߁j
	m_modelBox->DrawInstanced(m_modelEffect.get(), m_modelInputLayout.Get(), static_cast<uint32_t>(m_boxes.size()), true, false, 0, [=]()
		{
			UINT stride = sizeof(XMFLOAT3X4);
			UINT offset = 0;
			context->OMSetDepthStencilState(states->DepthRead(), 0);
			context->IASetVertexBuffers(1, 1, m_instancedVB.GetAddressOf(), &stride, &offset);
		}
	);

	// ----- ���b�V���̃��f����`�� ----- //
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFFFF);
	context->RSSetState(states->CullCounterClockwise());
	
	// �G�t�F�N�g��K������
	m_meshEffect->SetColorAndAlpha(color);
	m_meshEffect->SetView(view);
	m_meshEffect->SetProjection(proj);
	m_meshEffect->Apply(context);

	// ���̓��C�A�E�g��ݒ肷��
	context->IASetInputLayout(m_meshInputLayout.Get());

	m_meshBatch->Begin();

	// ----- ���b�V���̕`�� ----- //
	for (int i = 0; i < m_meshes.size(); i++)
	{
		std::vector<SimpleMath::Vector3> vertexes;

		// ���_�����W�ϊ�
		for (size_t j = 0; j < m_meshes[i].vertexes.size(); j++)
		{
			SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateFromQuaternion(m_meshes[i].rotate);
			SimpleMath::Matrix trans = SimpleMath::Matrix::CreateTranslation(m_meshes[i].position);
			SimpleMath::Matrix world = rotate * trans;
			vertexes.push_back(SimpleMath::Vector3::Transform(m_meshes[i].vertexes[j], world));
		}

		// �O�p�`�̕`��
		for (size_t j = 0; j < m_meshes[i].indexes.size() / 3; j++)
		{
			// �e�ʂ̖@���x�N�g�����v�Z
			SimpleMath::Vector3 ba = vertexes[m_meshes[i].indexes[j * 3 + 1]] - vertexes[m_meshes[i].indexes[j * 3]];
			SimpleMath::Vector3 ca = vertexes[m_meshes[i].indexes[j * 3 + 2]] - vertexes[m_meshes[i].indexes[j * 3]];
			SimpleMath::Vector3 normals = ca.Cross(ba);
			normals.Normalize();
			VertexPositionNormalColor v[3] = {
				{ vertexes[m_meshes[i].indexes[j * 3 + 0]], normals, Colors::White },
				{ vertexes[m_meshes[i].indexes[j * 3 + 1]], normals, Colors::White },
				{ vertexes[m_meshes[i].indexes[j * 3 + 2]], normals, Colors::White },
			};
			m_meshBatch->DrawTriangle(v[0], v[1], v[2]);
		}
	}

	m_meshBatch->End();

}

#ifdef _COLLISION_LINE_ON
// �R���W�������C���̕`��
void DisplayCollision::DrawCollisionLine(ID3D11DeviceContext* context, DirectX::CommonStates* states, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, FXMVECTOR color)
{
	// ----- ���C����`�� ----- //

	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());

	// �G�t�F�N�g��K������
	m_lineEffect->SetView(view);
	m_lineEffect->SetProjection(proj);
	m_lineEffect->Apply(context);

	// ���̓��C�A�E�g��ݒ肷��
	context->IASetInputLayout(m_lineInputLayout.Get());

	m_primitiveBatch->Begin();

	// ----- ���̃��C����`�� ----- //
	for (int i = 0; i < m_spheres.size(); i++)
	{
		DirectX::BoundingSphere shpere(m_spheres[i].center, m_spheres[i].radius);
		SimpleMath::Color lineColor = color;
		if (m_spheres[i].lineColor.w != 0.0f) lineColor = m_spheres[i].lineColor;
		DX::Draw(m_primitiveBatch.get(), shpere, lineColor);
	}

	// ----- �{�b�N�X�̃��C����`�� ----- //
	for (int i = 0; i < m_boxes.size(); i++)
	{
		DirectX::BoundingOrientedBox box(m_boxes[i].center, m_boxes[i].extents, m_boxes[i].rotate);
		SimpleMath::Color lineColor = color;
		if (m_boxes[i].lineColor.w != 0.0f) lineColor = m_boxes[i].lineColor;
		DX::Draw(m_primitiveBatch.get(), box, lineColor);
	}

	// ----- ���b�V���̃��C����`�� ----- //
	for (int i = 0; i < m_meshes.size(); i++)
	{
		std::vector<SimpleMath::Vector3> vertexes;

		// ���_�����W�ϊ�
		for (size_t j = 0; j < m_meshes[i].vertexes.size(); j++)
		{
			SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateFromQuaternion(m_meshes[i].rotate);
			SimpleMath::Matrix trans = SimpleMath::Matrix::CreateTranslation(m_meshes[i].position);
			SimpleMath::Matrix world = rotate * trans;
			vertexes.push_back(SimpleMath::Vector3::Transform(m_meshes[i].vertexes[j], world));
		}

		SimpleMath::Color lineColor = color;
		if (m_meshes[i].lineColor.w != 0.0f) lineColor = m_meshes[i].lineColor;

		// �O�p�`�̕`��
		for (size_t j = 0; j < m_meshes[i].indexes.size() / 3; j++)
		{
			DX::DrawTriangle(m_primitiveBatch.get(),
				vertexes[m_meshes[i].indexes[j * 3]], vertexes[m_meshes[i].indexes[j * 3 + 1]], vertexes[m_meshes[i].indexes[j * 3 + 2]],
				lineColor
			);
		}
	}

	// ----- ������`�� ----- //
	for (int i = 0; i < m_lineSegments.size(); i++)
	{
		SimpleMath::Color lineColor = color;
		if (m_lineSegments[i].lineColor.w != 0.0f) lineColor = m_lineSegments[i].lineColor;

		// �����̕`��
		VertexPositionColor verts[2] = {};
		XMStoreFloat3(&verts[0].position, m_lineSegments[i].a);
		XMStoreFloat3(&verts[1].position, m_lineSegments[i].b);
		XMStoreFloat4(&verts[0].color, lineColor);
		XMStoreFloat4(&verts[1].color, lineColor);
		m_primitiveBatch->Draw(D3D_PRIMITIVE_TOPOLOGY_LINELIST, verts, 2);
	}

	m_primitiveBatch->End();
}
#endif
