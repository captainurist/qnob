#include "meta_closure_ut.h"

#include <gtest/gtest.h>

#include <memory>

#include <QtCore/QVariantAnimation>

#include "meta_closure.h"
#include "meta_call_exception.h"

TEST(MetaClosure, TestInvokeReturnVoid) {
    std::unique_ptr<MetaClosureTest> object = std::make_unique<MetaClosureTest>(nullptr);
    object->setValue(100);

    QVariant result = MetaClosure(object.get(), "setValue", {999}).invoke();
    EXPECT_EQ(object->value(), 999);
    EXPECT_TRUE(!result.isValid());

    result = MetaClosure(object.get(), "setValue", { lit("111") }).invoke();
    EXPECT_EQ(object->value(), 111);
    EXPECT_TRUE(!result.isValid());
}

TEST(MetaClosure, TestInvokeReturnNonVoid) {
    std::unique_ptr<MetaClosureTest> object = std::make_unique<MetaClosureTest>(nullptr);
    object->setValue(100);

    QVariant result = MetaClosure(object.get(), "value", {}).invoke();

    EXPECT_EQ(result.typeId(), QMetaType::Int);
    EXPECT_EQ(result.toInt(), 100);
}

TEST(MetaClosure, TestInvokeReturnCustom) {
    std::unique_ptr<MetaClosureTest> object = std::make_unique<MetaClosureTest>(nullptr);
    object->setData({100});

    QVariant result = MetaClosure(object.get(), "data", {}).invoke();

    EXPECT_EQ(result.typeId(), qMetaTypeId<std::vector<int>>());
    EXPECT_EQ(result.value<std::vector<int>>(), std::vector<int>({100}));
}

TEST(MetaClosure, TestInvokePassCustom) {
    std::unique_ptr<MetaClosureTest> object = std::make_unique<MetaClosureTest>(nullptr);

    QVariant result = MetaClosure(object.get(), "setData", { QVariant::fromValue(std::vector<int>({100})) }).invoke();

    EXPECT_EQ(object->data(), std::vector<int>({100}));
    EXPECT_TRUE(!result.isValid());
}

TEST(MetaClosure, TestExceptions) {
    std::unique_ptr<MetaClosureTest> object = std::make_unique<MetaClosureTest>(nullptr);

    EXPECT_THROW(MetaClosure(object.get(), "Lol", {}).invoke(), MetaCallException);
    EXPECT_THROW(MetaClosure(object.get(), "setData", {1}).invoke(), MetaCallException);
    EXPECT_THROW(MetaClosure(object.get(), "setData", {1, 2}).invoke(), MetaCallException);
}
