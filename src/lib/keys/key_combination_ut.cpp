#include <gtest/gtest.h>

#include <lib/serialization/serialization.h>
#include <lib/test/print_functions.h>

#include "key_combination.h"
#include "mouse_keys.h"

TEST(KeyCombination, TestSerialization) {
    EXPECT_EQ(lit("X"), serialized(Qt::Key_X));
    EXPECT_EQ(lit("Mouse1"), serialized(Qt::Key_Mouse1));
    EXPECT_EQ(lit("Ctrl+0"), serialized(Qt::Key_0 | Qt::ControlModifier));
    EXPECT_EQ(lit("Ctrl+X"), serialized(Qt::Key_X | Qt::ControlModifier));
    EXPECT_EQ(lit("Ctrl+Mouse1"), serialized(Qt::Key_Mouse1 | Qt::ControlModifier));
    EXPECT_EQ(lit("Ctrl+Mouse2"), serialized(Qt::Key_Mouse2 | Qt::ControlModifier));
    EXPECT_EQ(lit("Ctrl+Mouse3"), serialized(Qt::Key_Mouse3 | Qt::ControlModifier));
    EXPECT_EQ(lit("Alt+MouseWheelUp"), serialized(Qt::Key_WheelUp | Qt::AltModifier));
    EXPECT_EQ(lit("Shift+MouseWheelDown"), serialized(Qt::Key_WheelDown | Qt::ShiftModifier));
    EXPECT_EQ(lit("Ctrl+MouseWheelLeft"), serialized(Qt::Key_WheelLeft | Qt::ControlModifier));
    EXPECT_EQ(lit("MouseWheelRight"), serialized(Qt::Key_WheelRight));
}

TEST(KeyCombination, TestDeserialization) {
    EXPECT_EQ(deserialized<Qt::Key>(lit("X")), Qt::Key_X);
    EXPECT_EQ(deserialized<QKeyCombination>(lit("Ctrl+0")), Qt::Key_0 | Qt::ControlModifier);
    EXPECT_EQ(deserialized<QKeyCombination>(lit("Ctrl+X")), Qt::Key_X | Qt::ControlModifier);
    EXPECT_EQ(deserialized<QKeyCombination>(lit("Ctrl++")), Qt::Key_Plus | Qt::ControlModifier);
    EXPECT_EQ(deserialized<QKeyCombination>(lit("Mouse1")), Qt::Key_Mouse1);
    EXPECT_EQ(deserialized<QKeyCombination>(lit("Ctrl+Mouse1")), Qt::Key_Mouse1 | Qt::ControlModifier);
    EXPECT_EQ(deserialized<QKeyCombination>(lit("Ctrl+Mouse2")), Qt::Key_Mouse2 | Qt::ControlModifier);
    EXPECT_EQ(deserialized<QKeyCombination>(lit("Alt+MouseWheelUp")), Qt::Key_WheelUp | Qt::AltModifier);
    EXPECT_EQ(deserialized<QKeyCombination>(lit("Ctrl+Shift+MouseWheelDown")), Qt::Key_WheelDown | (Qt::ShiftModifier | Qt::ControlModifier));
}
