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
		Transform(GameObject* pOwner) :
			m_pOwner{ pOwner },
			m_LocalPosition(0, 0, 0),
			m_WorldPosition(0, 0, 0),
			m_LocalScale(1,1,1)
		{
		};

		~Transform() = default;
		
		void SetDirty() {m_IsDirty = true;};
		void SetChildrenDirty();

		void SetPosition(const glm::vec2& position);
		void SetPosition(const glm::vec3& newPosition);
		void SetPosition(float x, float y, float z);

		void SetRotation(const glm::quat& newRotation);
		void SetScale(const glm::vec3& newScale);

		/*void SetLocalMatrix(const glm::mat4& matrix);
		void DecomposeMatrix(const glm::mat4& transform, glm::vec3& translation, glm::quat& rotation, glm::vec3& scale);*/

		//Getters
		//const glm::mat4& GetLocalMatrix() const;
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
		const glm::quat& GetRotation() const { return m_LocalRotation; }
		const glm::vec3& GetScale() const { return m_LocalScale; }
		//const glm::mat4& GetWorldMatrix() const;
		const glm::vec3& GetWorldPosition();


	private:
		// Local transform components
		glm::vec3 m_LocalPosition;
		glm::quat m_LocalRotation;
		glm::vec3 m_LocalScale;

		mutable glm::vec3 m_WorldPosition;

		// Matrix storage
		//glm::mat4 m_LocalMatrix;
		//mutable glm::mat4 m_WorldMatrix;
		mutable bool m_IsDirty;


		//void UpdateLocalMatrix();
		void UpdateWorldPosition();
		//void UpdateWorldMatrix() const;

	};
}
