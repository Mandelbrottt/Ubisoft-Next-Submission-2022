#include "pch.h"

#include "Primitive.h"

#include <filesystem>
#include <fstream>

#include "Math/Transformations.h"

#include "NextAPI/SimpleSprite.h"

#include <string>
#include <string_view>

namespace NextCore::Graphics
{
	void
	Primitive::OnUpdate(float a_deltaTime)
	{
		m_sprite.OnUpdate(a_deltaTime);
	}

	static std::unordered_set<std::string> g_processedFiles;
	
	bool
	Primitive::LoadFromTexture(std::string_view a_fileName, unsigned a_nColumns, unsigned a_nRows)
	{
		std::string filename = std::string(a_fileName);
		
		if (g_processedFiles.count(filename) == 0)
		{
			if (std::filesystem::path(a_fileName).extension() == ".bmp")
			{
				ProcessBmp(a_fileName);	
			}
			g_processedFiles.insert(filename);
		}
		
		m_sprite = Sprite(a_fileName.data(), a_nColumns, a_nRows);

		if (!m_sprite.IsValid())
		{
			return false;
		}

		m_sprite.SetFrame(-1);

		return true;
	}

	void
	Primitive::OnRenderPrepare(
		Math::Matrix4 const& a_model,
		Math::Matrix4 const& a_view,
		Math::Matrix4 const& a_projection
	)
	{
		if (!IsValid())
		{
			return;
		}

		using namespace Math;
		
		float runningDepthTotal = 0.0f;

		int numVerts = m_primitiveType == PrimitiveType::Triangle ? 3 : 4;
		
		Math::Vector4 vertexPositions[4];

		for (int i = 0; i < numVerts; i++)
		{
			auto const& [position, uv, normal] = m_vertices[i];

			auto transformedVertex = a_model * Vector4(position, 1.0f);
			vertexPositions[i] = transformedVertex;
		}

		Math::Vector3 normals[2];

		for (int i = 0; i < numVerts - 2; i++)
		{
			Vector3 line1 = Vector3(vertexPositions[i + 1]) - Vector3(vertexPositions[0]);
			Vector3 line2 = Vector3(vertexPositions[i + 2]) - Vector3(vertexPositions[0]);

			normals[i] = Normalize(Cross(line1, line2));
		}

		Vector3 commonPointOnPrimitive = Vector3(vertexPositions[0]);

		if (Dot(normals[0], commonPointOnPrimitive) < 0 || Dot(normals[1], commonPointOnPrimitive) < 0)
		{
			Vector3 commonNormal = normals[0];

			if (normals[1] != Vector3(0))
			{
				commonNormal += normals[1];
				commonNormal = Normalize(commonNormal * 0.5f);
			}
			
			for (int i = 0; i < numVerts; i++)
			{
				Vector3 lightDirection = { -1, 1, 1 };
				lightDirection.Normalize();

				float dotProduct = Dot(commonNormal, -lightDirection);
				dotProduct = std::max(dotProduct, 0.f) + 0.3f;
				
				auto color = Color(dotProduct, dotProduct, dotProduct);
				
				m_sprite.SetColor(color);
				
				auto const& [position, uv, normal] = m_vertices[i];

				auto projectedVertex   = a_projection * vertexPositions[i];

				projectedVertex.x /= projectedVertex.w;
				projectedVertex.y /= projectedVertex.w;
				projectedVertex.z /= projectedVertex.w;

				//projectedVertex.x *= aspect;

				m_sprite.m_simpleSprite->SetVertex(2 * i,     projectedVertex.x);
				m_sprite.m_simpleSprite->SetVertex(2 * i + 1, projectedVertex.y);
				
				m_sprite.m_simpleSprite->SetUv(2 * i,     uv.x);
				m_sprite.m_simpleSprite->SetUv(2 * i + 1, uv.y);

				runningDepthTotal += projectedVertex.z;
			}

			m_depth = runningDepthTotal / numVerts;

			//averageNormal /= numVerts;
			//averageNormal.Normalize();
			
			// If primitive is a triangle, fourth vertex shares values with first
			if (m_primitiveType == PrimitiveType::Triangle)
			{
				auto& s = m_sprite.m_simpleSprite;
				s->SetVertex(6, s->GetVertex(0));
				s->SetVertex(7, s->GetVertex(1));
				s->SetUv(6, s->GetUv(0));
				s->SetUv(7, s->GetUv(1));
			}
		} else
		{
			m_depth = -1;
		}
	}

	void
	Primitive::OnRender()
	{
		m_sprite.OnRender();
	}
	
	Detail::Vertex
	Primitive::GetVertex(unsigned a_index) const
	{
		return m_vertices[a_index];
	}
	
	void
	Primitive::SetColor(Color a_color)
	{
		m_sprite.SetColor(a_color);
	}
	
	void
	Primitive::SetVertex(unsigned a_index, Detail::Vertex a_value)
	{
		// Don't set the sprite's vertices; We maintain model space
		// vertices that are used to calculate the sprite's vertices per frame
		m_vertices[a_index] = a_value;
	}
	
	bool
	Primitive::IsValid() const
	{
		return m_sprite.IsValid();
	}

	bool
	Primitive::ProcessBmp(std::string_view a_filename)
	{
		std::ifstream is(a_filename, std::ios::binary);

		if (!is)
		{
			return false;
		}

		is.seekg(0, is.end);
		int fileLength = static_cast<int>(is.tellg());
		is.seekg(0, is.beg);

		char header[54];
		
		is.read(header, sizeof(header));

		// Invalid header
		if (is.gcount() < 54)
		{
			return false;
		}

		int16_t bitsPerPixel = *reinterpret_cast<int16_t*>(header + 0x1C);

		if (bitsPerPixel == 32)
		{
			return true;
		} else if (bitsPerPixel != 24)
		{
			return false;
		}

		int32_t dataPos = *reinterpret_cast<int32_t*>(header + 0x0A);
		if (dataPos == 0) dataPos = 54;

		int32_t imageWidth  = *reinterpret_cast<int32_t*>(header + 0x12);
		int32_t imageHeight = *reinterpret_cast<int32_t*>(header + 0x16);

		int imageSize = imageWidth * imageHeight * 3;

		char* fileBuffer = new char[fileLength];
		
		is.seekg(0, is.beg);

		is.read(fileBuffer, fileLength);

		is.close();

		reinterpret_cast<int16_t&>(fileBuffer[0x1C]) = 32;

		std::string tempFileName = std::string(a_filename) + ".tmp";
		
		std::ofstream os(tempFileName, std::ios::binary);

		if (!os)
		{
			return false;
		}
		
		os.write(fileBuffer, dataPos);

		char* newColorBuf = new char[imageWidth * imageHeight * 4];
		char* newColorBufIter = newColorBuf;

		for (int i = dataPos; i < imageSize + dataPos; i += 3)
		{
			char b = fileBuffer[i];
			char g = fileBuffer[i + 1];
			char r = fileBuffer[i + 2];
			char a = static_cast<char>(0xFF);

			newColorBufIter[0] = b;
			newColorBufIter[1] = g;
			newColorBufIter[2] = r;
			newColorBufIter[3] = a;
			newColorBufIter += 4;
		}

		os.write(newColorBuf, imageWidth * imageHeight * 4);

		int postBufferPos = dataPos + imageSize;

		if (fileLength - postBufferPos != 0)
		{
			os.write(fileBuffer + (postBufferPos), fileLength - postBufferPos);
		}
		
		os.close();

		std::filesystem::path oldFile = a_filename;
		std::filesystem::path newFile = tempFileName;

		std::error_code code;
		std::filesystem::rename(newFile, oldFile, code);
		
		return code.value() == 0;
	}
}
