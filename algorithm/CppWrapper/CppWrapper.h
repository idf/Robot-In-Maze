// CppWrapper.h

#pragma once

#include "robot.h"

using namespace System;

public ref class RobotWrapper
	{
		    Robot* m_nativeClass;

public:
    RobotWrapper::RobotWrapper() { m_nativeClass = new Robot(0, 0, 0); }
    RobotWrapper::~RobotWrapper() { this->!RobotWrapper(); }
    RobotWrapper::!RobotWrapper() { delete m_nativeClass; }
    void RobotWrapper::Method() {
		m_nativeClass->getPosX();
    }
		// TODO: Add your methods for this class here.
	};
