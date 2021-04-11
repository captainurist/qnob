#pragma once

#include <cassert>

#include "command_line_option.h"
#include "option_parsers.h"

class CommandLineOptionBuilder {
public:
    CommandLineOptionBuilder(CommandLineOption* option) :
        m_option(option)
    {}

    /* No name / help setters here, they are in parser. */

    CommandLineOptionBuilder& required() {
        m_option->required = true;
        return *this;
    }

    CommandLineOptionBuilder& hidden() {
        m_option->hidden = true;
        return *this;
    }

    template<class T>
    CommandLineOptionBuilder& argument(const QString& name, T* target) {
        assert(!name.isEmpty());

        m_option->valueName = name;
        m_option->handler = [=](QStringView value) {
            parseOptionValue(value, target);
        };
        return *this;
    }

    template<class T>
    CommandLineOptionBuilder& argument(const char* name, T* target) {
        return argument(QString::fromUtf8(name), target);
    }

    CommandLineOptionBuilder& flag(bool* target) {
        assert(m_option->valueName.isEmpty());

        m_option->handler = [=](QStringView) {
            *target = true;
        };
        return *this;
    }

    CommandLineOptionBuilder& defaultValue(const QString &value) {
        m_option->defaultValue = value;
        return *this;
    }

private:
    CommandLineOption* m_option;
};
