#pragma once
#include "Event.h"

namespace dae
{
  class GameObject;

  class Observer
  {
	  //friend class Subject;

  public:

	  //Observer() : m_pNext(nullptr) {};
	  virtual ~Observer() {};
	  virtual void OnNotify(const GameObject& entity, const Event& event) = 0;

  private:
	 // Observer* m_pNext;
  };
  
};