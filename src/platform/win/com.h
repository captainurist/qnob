#pragma once

class Com {
public:
    Com();
    ~Com();

    bool initialized() const {
        return initialized_;
    }

private:
    bool initialized_ = false;
};
