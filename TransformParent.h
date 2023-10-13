#pragma once

#include <list>

#include "Vector3.h"

class Transform {
private:

	struct Elm {
		Vector3 m_position;
		float m_rotate;
		Vector3 m_scale;

		Elm() :
			m_position(),
			m_rotate(),
			m_scale()
		{}

		Elm(const Vector3& position, const float rotate, const Vector3& scale) :
			m_position(position),
			m_rotate(rotate),
			m_scale(scale)
		{}
	}m_local, m_global;

	Transform* m_parent = nullptr;
	std::list<Transform*>m_childs;

	void NotifyGlobalUpdate();
	void NotifyUpdate();

	bool AddChild(Transform* const child);
	void RemoveChild(Transform* const child);

public:
	Transform() = default;

	Transform(const Vector3& position)
};