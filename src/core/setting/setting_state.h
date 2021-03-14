#pragma once

class SettingState {
public:
    bool enabled = false;
    double value = 0.0;

    friend bool operator==(const SettingState& l, const SettingState& r) = default;
};
