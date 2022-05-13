#pragma once

enum {
    TDCR_LEFT = 0x1,
    TDCR_RIGHT = 0x2,
    TDCR_UP = 0x4,
    TDCR_DOWN = 0x8
};

enum fixtureUserDataTypeR {
    FUDR_GROUND_AREA,
    FUDR_CAR_TIRE,
    FUDR_TRACK_WALL,
    FUDR_BARREL
};

//a class to allow subclassing of different fixture user data
class FixtureUserDataR {
    fixtureUserDataTypeR m_type;
protected:
    FixtureUserDataR(fixtureUserDataTypeR type) : m_type(type) {}
public:
    virtual fixtureUserDataTypeR getType() { return m_type; }
    virtual ~FixtureUserDataR() {}

    bool operator < (const FixtureUserDataR& other) { return true; }
};

class GroundAreaFUDR : public FixtureUserDataR {
public:
    float frictionModifier = 0;
    float dragModifier = 0;

    GroundAreaFUDR(float fm, float dm) : FixtureUserDataR(FUDR_GROUND_AREA) {
        frictionModifier = fm;
        dragModifier = dm;
    }
};