#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/quaternion.hpp>


namespace dae
{
	class GameObject;

	class Transform final
	{
		GameObject* const m_pOwner;

	public:
		explicit Transform(GameObject* pOwner) :
			m_pOwner{ pOwner },
			m_LoacalPosition(glm::vec3(0.0f)),
			m_LocalRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
			m_LocalScale(glm::vec3(1.0f)),
			m_LocalMatrix(1.0f),
			m_WorldMatrix(1.0f),
			m_IsDirty(false) {}
		
		void SetDirty();
		void SetPosition(const glm::vec3& newPosition);
		void SetRotation(const glm::quat& newRotation);
		void SetScale(const glm::vec3& newScale);
		void SetLocalMatrix(const glm::mat4& matrix);
		void DecomposeMatrix(const glm::mat4& transform, glm::vec3& translation, glm::quat& rotation, glm::vec3& scale);

		//Getters
		const glm::mat4& GetLocalMatrix() const { return m_LocalMatrix; }
		const glm::vec3& GetLocalPosition() const { return m_LoacalPosition; }
		const glm::quat& GetRotation() const { return m_LocalRotation; }
		const glm::vec3& GetScale() const { return m_LocalScale; }
		const glm::mat4& GetWorldMatrix() const;


	private:
		// Local transform components
		glm::vec3 m_LoacalPosition;
		glm::quat m_LocalRotation;
		glm::vec3 m_LocalScale;

		// Matrix storage
		glm::mat4 m_LocalMatrix;
		mutable glm::mat4 m_WorldMatrix;
		mutable bool m_IsDirty;


		void UpdateLocalMatrix();
		void UpdateWorldMatrix() const;

	};
}
