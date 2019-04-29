#pragma once

class ITest {
public:
    virtual ~ITest() = default;

    virtual void Run() = 0;
};